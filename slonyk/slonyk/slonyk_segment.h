#pragma once
#include "eng_settings.h"
#include "types.h"
#include "slonyk_settings.h"


class SlSegment : public SegmentBase<SlAcknowledge, SlMessageType, uint16_t>
{
    public:
        SlSegment(IRawDataProvider & rawDataProvider) : SegmentBase<SlAcknowledge, SlMessageType, uint16_t>(rawDataProvider),
                                                        m_currAck(SlAcknowledge::SL_REQUEST),
                                                        m_currMessType(SlMessageType::SL_BROADCAST),
                                                        m_currAddr(0),
                                                        m_totalDataLen(0),
                                                        m_startRegsDataPos(m_totalData + 1),
                                                        m_endRegsAddrPos(reinterpret_cast<uint16_t *>(m_totalData + 1)),
                                                        m_endRegsDataPos(m_totalData + 1)
        {
        }
        SlAcknowledge & getAck()
        {
            return m_currAck;
        }
        void setAck(SlAcknowledge ack)
        {
        	m_currAck = ack;
        }
        SlMessageType & getMessageType()
        {
            return m_currMessType;
        }
        void setMessageType(SlMessageType messType)
        {
        	m_currMessType = messType;
        }
        uint16_t & getDevAddr()
        {
            return m_currAddr;
        }
        void setAddr(uint16_t addr)
        {
        	m_currAddr = addr;
        }
        bool addReg(IRegister & reg)
        {
        	bool result = false;
        	uint8_t * regData = NULL;
        	uint32_t * regLen = NULL;
        	reg.getBytes(regData, regLen);

        	// длина в байтах данных и + длина адреса
        	if(hasFreeSpace(*regLen + SL_REG_ADDR_LEN))
        	{
        		// необходима некоторая магия по перебрасываю байтов данных и адресов туда-сюда
        		// 1. сдвигаем текущую дату вправо, что бы освободить место под новый адрес
        		// и итерируем адреса данных, начальный и конечный
#warning не знаю что произойдет, если будет перекрытие адресов данных для memcpy, поэтому делаю пока так
        		for(uint8_t i = 0; i < *m_startRegsDataPos - * m_endRegsDataPos + 1; i++ )
        		{
        			*(m_endRegsDataPos + sizeof(uint16_t) - i) = *(m_endRegsDataPos - i);
        		}
        		m_endRegsDataPos+=2;
        		m_startRegsDataPos+=2;
        		// 2. добавляем адрес регистра и итерируем
        		*m_endRegsAddrPos = reg.getRegAddr();
        		m_endRegsAddrPos++;
        		// 3. добавляем дату в массив, изменяя endDataPos
        		memcpy(m_endRegsDataPos+1, regData, *regLen);
        		m_endRegsDataPos += *regLen;
        		// 4. увеличиваем тотальную длину на величину данных и длину адреса
        		m_totalDataLen += (*regLen + SL_REG_ADDR_LEN);
        		result = true;
        	}
        	return result;
        }
        bool addReg(uint32_t regAddr, uint32_t regLen)
        {
        	bool result = false;
        	if(hasFreeSpace(regLen + SL_REG_ADDR_LEN))
        	{
        		*m_endRegsAddrPos = regAddr;
        		m_endRegsAddrPos++;
        		result = true;
        	}
        	return result;
        }

//        void getData(uint8_t * data, uint32_t * len)
//        {
//            data = m_rawData;
//            *len = m_dataLen;
//        }
//        void setData(uint8_t * newData, uint32_t len)
//        {
//        	if(len > SL_MAX_DATA_LEN)
//        	{
//        		ENG_ASSERT();
//        	}
//        	memcpy(m_rawData, newData, len);
//        	m_dataLen = len;
//        }

//
//        void fromPacket(IPacket & packet)
//        {
//            uint8_t * headerPtr;
//            uint32_t * headerLen;
//            uint8_t * dataPtr;
//            uint32_t * dataLen;
//            packet.getHeader(headerPtr, headerLen);
//            parseHeader(headerPtr, *headerLen);
//            packet.getData(dataPtr, dataLen);
//            memcpy(m_rawData, dataPtr, *dataLen);
//        }

    private:
        SlAcknowledge m_currAck;
        SlMessageType m_currMessType;
        uint16_t m_currAddr;
        uint8_t m_totalData[SL_MAX_DATA_LEN];
        uint32_t m_totalDataLen;

        // адреса регистров - 2 байта
        // дата - сырая, 1 байт, потому такие итераторы
        uint8_t * m_startRegsDataPos;
        uint16_t * m_endRegsAddrPos;
        uint8_t * m_endRegsDataPos;
        bool hasFreeSpace(uint32_t newData)
        {
        	if(SL_MAX_DATA_LEN - m_totalDataLen >= newData)
        	{
        		return true;
        	}
        	return false;
        }
        void parseHeader(uint8_t * header, uint32_t len)
        {
            if(len < SL_HEADER_LEN)
            {
                ENG_ASSERT();
            }
            uint8_t ack = *(header + SL_ACK_POS);
            uint8_t mType = *(header + SL_MTYPE_POS);
            uint16_t addr = 0;
            addr |= *(header + SL_ADDR_POS)  << 0;
            addr |= *(header + SL_ADDR_POS + 1) << 8;

            switch(ack)
            {
                case SL_ACK_REQUEST:
                {
                    m_currAck = SlAcknowledge::SL_REQUEST;
                    break;
                }
                case SL_ACK_RESPONSE:
                {
                    m_currAck = SlAcknowledge::SL_RESPONSE;
                    break;
                }
                default:
                {
                    ENG_ASSERT();
                }
            }


            switch(mType)
            {
                case SL_MESS_BROADCAST:
                {
                    m_currMessType = SlMessageType::SL_BROADCAST;
                    break;
                }
                case SL_MESS_WRITE:
                {
                	m_currMessType = SlMessageType::SL_WRITE;
                    break;
                }
                case SL_MESS_READ:
                {
                	m_currMessType = SlMessageType::SL_READ;
                    break;
                }
                default:
                {
                    ENG_ASSERT();
                }
            }
            m_currAddr = addr;
        }
};
