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
        void updateRoRegs(uint32_t currTime);

    private:
        template <class... AddArg>
        void addSlave(SlSlaveBase * slave, AddArg*... slavesArg)
        {
            m_slaves[m_slavesItr] = slave;
            m_slavesItr++;
            slave->segmentReceivedCallback = CALLBACK_BIND_MEMBER(*this, SlMaster::onSegmentReceived);
            addSlave(slavesArg...);
        }
        void addSlave(){};


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
};
