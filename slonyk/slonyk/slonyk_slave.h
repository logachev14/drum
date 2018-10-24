#pragma once
#include "eng_settings.h"
#include "slonyk_table.h"
#include "sl_session.h"
#include "i_transport_provider.h"



/**
 *
 * @Имя класса : ISlave
 * @Описание :
 *
*/

class ISlave
{
public:
	virtual void getRoRegs(IRegister * reg, uint32_t * regsNum) = 0;
	virtual void getRegsToUpdate(IRegister * reg, uint32_t * regsNum) = 0;
	virtual uint32_t getAddr() = 0;
	virtual uint32_t getTimeout() = 0;
	virtual ITransportProvider & getProvider() = 0;
	virtual void connectionStateChanged(bool state) = 0;
	virtual ~ISlave(){};
};

class SlSlaveBase : public ISlave
{
public :
	callback::Callback<void (SlSegment & segment)> segmentReceivedCallback;
};

class SimpleSlave : public SlSlaveBase
{
public:
	void getRoRegs(IRegister * reg, uint32_t * regsNum){};
	void getRegsToUpdate(IRegister * reg, uint32_t * regsNum){};
	uint32_t getAddr(){return 0;};
	uint32_t getTimeout(){return 0;};
	ITransportProvider & getProvider(){};
	void connectionStateChanged(bool state){};
};

template<class TableType>
class SlSlave : public SlSlaveBase
{
    public:
        SlSlave(uint32_t addr, TableType & table, ITransportProvider & provider): m_addr(addr), m_table(table), m_provider(provider)
        {

        }
        uint32_t getAddr()
        {
            return m_addr;
        }
        TableType & getTable()
        {
            return m_table;
        }
        virtual void onSegmentReceived(SlSegment & segment)
        {
        	if(segment.getAck() == SlAcknowledge::SL_REQUEST && segment.getDevAddr() == m_addr)
        	{
        		switch(segment.getMessageType())
        		{
					case SlMessageType::SL_READ :
					{
						// нужно собрать ответ
						composeAnswer(segment);
						m_provider.send(m_responseSegment);
						break;
					}
					case SlMessageType::SL_WRITE :
					{
						composeAnswer(segment);
						m_provider.send(m_responseSegment);
						break;
					}
					case SlMessageType::SL_BROADCAST :
					{
						break;
					}

        		}
        	}

        }
        void getRoRegs(IRegister * reg, uint32_t * regsNum){};
        void getRegsToUpdate(IRegister * reg, uint32_t * regsNum){};
        uint32_t getTimeout(){return 0;};
        ITransportProvider & getProvider(){return m_provider;};
        void connectionStateChanged(bool state){};
    private:
    void composeAnswer(SlSegment & incomeSegment)
    {
    	//TODO заполнить m_responseSegment, получить/отдать данные
//    	// предполагается, что ответить необходимо, поэтому собираем ответ
//    	SlAcknowledge ack = SlAcknowledge::SL_RESPONSE;
//    	m_responseSegment.setAck(ack);
//    	m_responseSegment.setMessageType(segment.getMessageType());
//    	m_responseSegment.setAddr(m_addr);
//
//    	uint8_t * responseDataPtr = 0;
//    	uint32_t * len;
//    	m_responseSegment.getData(responseDataPtr, len);
//
//
//    	uint8_t * data = 0;
//    	//
//    	uint32_t * len = 0;
//    	segment.getData(data, len);
//    	if(*len <= 0 )
//    	{
//    		ENG_ASSERT();
//    	}
//    	uint8_t numOfRegs = *data;
//    	*responseDataPtr = numOfRegs;
//    	// ���������� �� �������
//		uint8_t regDataStartPos = SL_REGS_ADDR_START_POS + (numOfRegs * 2);
//		for(uint8_t i = 0; i < numOfRegs; i++)
//		{
//			// ����
//			memcpy((responseDataPtr + 1),(data + (SL_REGS_ADDR_START_POS) * (i + 1)), 2 );
//			uint16_t addr = 0;
//			addr |= *(data + (SL_REGS_ADDR_START_POS) * (i + 1))  << 0;
//			addr |= *(data + (SL_REGS_ADDR_START_POS + 1) * (i + 1)) << 8;
//
//			IRegister * reg = m_table.getReg(addr);
//			uint8_t regDataLen = reg->getLen();
//			if(segment.getMessageType() == SlMessageType::SL_WRITE)
//			{
//				reg->setBytes((data + regDataStartPos), regDataLen);
//			}
//			else
//			{
//
//
//			}
//
//			regDataStartPos += regDataLen;
//		}
    }
    SlSegment m_responseSegment;
    uint32_t m_addr;
    ITable & m_table;
    ITransportProvider & m_provider;
};
