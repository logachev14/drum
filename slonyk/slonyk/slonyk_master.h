#pragma once
#include "eng_settings.h"
#include "slonyk_slave.h"
#include "i_register.h"
#include "slonyk_segment.h"

enum UpdateState
{
	GET_NEXT_SLAVE = 0,
	READ_SLAVE_REGS = 1,
	COMPOSE_SEGMENT_TO_SEND = 2,
	SEND_SEGMENT = 3,
	WAIT_ANSWER = 4,
	SOME_4_STATE = 5,

};

template<uint32_t slavesNum>
class SlMaster : public SlSession
{
    public:
        template <class... SlavesArg>
        SlMaster(SlavesArg*... slavesArg) : m_updateRegsNum(NULL),
											m_slaveToUpdate(NULL),
											m_regsToUpdate(NULL),
											m_isCorrectAnswerReceived(false)

        {
            addSlave(slavesArg...);
        }

        /**
         *
         * @Имя метода: updateRoRegs
         * @Описание : Реализация конечного автомата для обновления ro регистров слейвов
         * метод предполагает вызов в бесконечном цикле
         * GET_NEXT_SLAVE - берем следующего слейва из массива @m_slaves, который был создан при инстанцировании класса
         * READ_SLAVE_REGS - заполняем указатели @m_regsToupdate и @m_updateRegsNum регистрами, которые нам даст слейв
         * COMPOSE_SEGMENT_TO_SEND - заполняем @m_segToSend регистрами для отправки
         * SEND_SEGMENT - берем у слейва его провайдера и засылаем ему @m_segTosend
         * WAIT_ANSWER - ждем ответа, или заданного слейву таймаута
         * @ Входные параметры :
         * uint32_t currTime - текущее время вызова метода
         * @ Выходные параметры : нет
         *
        */

        void updateRoRegs(uint32_t currTime)
        {
            switch(m_state)
            {
                case UpdateState::GET_NEXT_SLAVE:
                {
                    // обновление ro регистров каждого слейва начинается отсюда :
                    m_slaveToUpdate = m_slaves[m_slavesCounter];
                    m_slavesCounter %= slavesNum;
                    m_slavesCounter++;
                    m_state = UpdateState::READ_SLAVE_REGS;
                    break;
                }
                case UpdateState::READ_SLAVE_REGS:
                {
                    // спрашиваем регистры для обновления
                    m_regsToUpdate = NULL;
                    *m_updateRegsNum = 0;
                    m_slaveToUpdate->getRegsToUpdate(m_regsToUpdate, m_updateRegsNum);

                    // если таких нет, берем следующего слейва
                    if(m_regsToUpdate == NULL || *m_updateRegsNum <= 0)
                    {
                        m_state = UpdateState::GET_NEXT_SLAVE;
                    }
                    m_segToSend.setAck(SlAcknowledge::SL_REQUEST);
                    m_segToSend.setAddr(m_slaveToUpdate->getAddr());
                    m_segToSend.setMessageType(SlMessageType::SL_READ);
                    m_state = UpdateState::COMPOSE_SEGMENT_TO_SEND;
                    break;
                }
                case UpdateState::COMPOSE_SEGMENT_TO_SEND:
                {
                    // условие того, что регистры для обновления закончились
                    if(*m_updateRegsNum == 0)
                    {
                        m_state = UpdateState::GET_NEXT_SLAVE;
                        break;
                    }
                    // запихиваем в сегмент по-максимуму
                    while(m_segToSend.addReg(m_regsToUpdate->getRegAddr(), m_regsToUpdate->getLen()) ||
                         *m_updateRegsNum > 0 )
                    {
                        m_regsToUpdate++;
                        *m_updateRegsNum--;
                    }
                    m_state = UpdateState::SEND_SEGMENT;
                    break;
                }
                case UpdateState::SEND_SEGMENT:
                {
                    // выплевываем сегмент провайдеру слейва
        //			m_slaveToUpdate->getProvider().send(m_segToSend);
                    m_startTime = currTime;
                    m_state = UpdateState::WAIT_ANSWER;
                    break;
                }
                case UpdateState::WAIT_ANSWER:
                {
                    CRITICAL_SECTION_START();
                    if(m_isCorrectAnswerReceived)
                    {
                        m_isCorrectAnswerReceived = false;
                        m_slaveToUpdate->connectionStateChanged(true);
                        m_state = UpdateState::COMPOSE_SEGMENT_TO_SEND;
                    }
                    // условие таймаута
                    if(m_isCorrectAnswerReceived == false && m_slaveToUpdate->getTimeout() < currTime - m_startTime)
                    {
                        m_slaveToUpdate->connectionStateChanged(false);
                        m_state = UpdateState::GET_NEXT_SLAVE;
                    }
                    CRITICAL_SECTION_END();
                    break;
                }
            }
        }


        /**
         *
         * @Имя метода: setUpdateInterval
         * @Описание : установка периода обновления для RO регистров
         * @ Входные параметры :
         * uint32_t interval - период обновления
         * @ Выходные параметры : нет
         *
        */

        void setUpdatePeriod(uint32_t interval)
        {
            m_updatePeriod = interval;
        }

    private:
        template <class... AddArg>
        void addSlave(SlSlaveBase * slave, AddArg*... slavesArg)
        {
            m_slaves[m_slavesItr] = slave;
            m_slavesItr++;
            #warning //slave->segmentReceivedCallback = CALLBACK_BIND_MEMBER(*this, SlMaster::onSegmentReceived);
            addSlave(slavesArg...);
        }
        void addSlave(){}


        /**
         *
         * @Имя метода: onSegmentReceived
         * @Описание : коллбэк, который дергает слейв, которому пришел сегмент, к нему относящийся
         * для начала, сбрасываем флаг @m_isCorrectAnswerReceived
         * поскольку предполагается что мы ожидаем только ответов на наши запросы
         * то проверяем на совпадение аки и месстайпы сегментов @m_segToSend и @recSegment
         * так же проверям совдение адресов слейвов
         * если это оказался сегмент, который мы ожидали, устанавливаем @m_isCorrectAnswerReceived в true
         * @ Входные параметры :
         * ISegment & recSegment - сегмент, который был получен слейвом
         * @ Выходные параметры : нет
         *
        */

        void onSegmentReceived(SlSegment & recSegment)
        {
            m_isCorrectAnswerReceived = false;
            if(m_segToSend.getDevAddr() != recSegment.getDevAddr())
            {
                // не совпадают адреса
                ENG_ASSERT();
            }
            //if(m_segToSend.getMessageType != recSegment.getMessageType())
            {
                // не совпадают типы сообщений
                ENG_ASSERT();
            }
            if(recSegment.getAck() != SlAcknowledge::SL_RESPONSE)
            {
                // не ответ на запрос
                ENG_ASSERT();
            }
            // во всех остальных случаях читаем/проверяем что пришло что нужно
            m_isCorrectAnswerReceived = true;
        }

        uint32_t m_slavesItr = 0;
        SlSlaveBase * m_slaves[slavesNum];

        // Все для обновления:
        UpdateState m_state = UpdateState::GET_NEXT_SLAVE;
        uint32_t m_slavesCounter = 0;
        ISlave * m_slaveToUpdate;
        IRegister * m_regsToUpdate;
        uint32_t * m_updateRegsNum;
        uint32_t m_regsCounter = 0;
        uint32_t m_startTime = 0;
        bool m_isCorrectAnswerReceived;

        SlSegment m_segToSend;

        uint32_t m_updatePeriod = 0;
};
