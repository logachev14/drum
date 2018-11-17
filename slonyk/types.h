#pragma once
#include "eng_settings.h"
#include "i_register.h"
#include "slonyk/slonyk_settings.h"

class IRawDataProvider
{
    public:
        IRawDataProvider():  m_rawDataPtr(0), m_currDataLen(0)
        {

        }
        virtual void setData(uint8_t * newData, uint32_t len) = 0;
        uint32_t getLen()
        {
            return m_currDataLen;
        }
        uint8_t & operator[](uint32_t n)
        {
            if(n > m_currDataLen - 1 )
            {
                ENG_ASSERT();
            }
            return *(m_rawDataPtr + n);
        }
        uint8_t * getDataPtr()
        {
            return m_rawDataPtr;
        }
        virtual ~IRawDataProvider(){}
    protected:
        uint8_t * m_rawDataPtr;
        uint32_t m_currDataLen;

};

template<uint32_t maxLen>
class RawData : public IRawDataProvider
{
    public:
        RawData()
        {
            m_rawDataPtr = m_rawData;
        }
        void setData(uint8_t * newData, uint32_t len)
        {
            memcpy(m_rawData, newData, len);
            m_currDataLen = len;
        }

        virtual ~RawData(){}
    private:
        uint8_t m_rawData[maxLen];
};

class ISegment
{
    public :
        ISegment()
        {
        }
        // для формирования сегмента, необходимо указать каким образом будет происходить добавление.
        // добавление регистра с данными, или добавление просто адреса ргеистра
        // true - регистр добален в сегмент
        // false - соответственно, добален небыл.
        virtual bool addReg(IRegister & reg) = 0;
        virtual bool addReg(uint32_t regAddr, uint32_t regLen) = 0;
        virtual void setRawData(IRawDataProvider & rawDataProvider) = 0;
        virtual ~ISegment(){}

    protected:
        IRawDataProvider * m_rawDataProviderPtr;
};

//каждый segment содержит в себе Ack, MessageType, devAddr и набор полу-сырых байт
template<class AckType, class MessageType, class DeviceAddrType>
class SegmentBase : public ISegment
{
    public:
        SegmentBase()
        {

        }
        virtual AckType & getAck() = 0;
        virtual MessageType & getMessageType() = 0;
        virtual DeviceAddrType & getDevAddr() = 0;
        virtual ~SegmentBase(){}
};






