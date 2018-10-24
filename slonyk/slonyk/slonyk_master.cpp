#include "slonyk_master.h"

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
template<uint32_t slavesNum>
void SlMaster<slavesNum>::updateRoRegs(uint32_t currTime)
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
			m_slaveToUpdate->getProvider().send(m_segToSend);
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
//template <uint32_t slavesNum>
//void SlMaster<slavesNum>::onSegmentReceived(SlSegment & recSegment)
//{
//	m_isCorrectAnswerReceived = false;
//	if(m_segToSend.getDevAddr() != recSegment.getDevAddr())
//	{
//		// не совпадают адреса
//		ENG_ASSERT();
//	}
//	//if(m_segToSend.getMessageType != recSegment.getMessageType())
//	{
//		// не совпадают типы сообщений
//		ENG_ASSERT();
//	}
//	if(recSegment.getAck() != SlAcknowledge::SL_RESPONSE)
//	{
//		// не ответ на запрос
//		ENG_ASSERT();
//	}
//	// во всех остальных случаях читаем/проверяем что пришло что нужно
//	m_isCorrectAnswerReceived = true;
//}

