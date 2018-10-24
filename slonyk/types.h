#pragma once
#include "eng_settings.h"
#include "i_register.h"

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
        virtual ~IRawDataProvider(){};
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

        virtual ~RawData(){};
    private:
        uint8_t m_rawData[maxLen];
};

class IPacket
{
    public:
        IPacket(IRawDataProvider & rawDataProvider) : m_rawDataProvider(rawDataProvider)
        {
        }

        void setRawData(uint8_t * buf, uint32_t len)
        {
            m_rawDataProvider.setData(buf, len);
        }
        virtual void getHeader(uint8_t * buf, uint32_t * len) = 0;
        virtual void getData(uint8_t * buf, uint32_t * len) = 0;

        virtual ~IPacket(){};
    protected:
        IRawDataProvider & m_rawDataProvider;

};

class Packet : public IPacket
{
    public:
        Packet(IRawDataProvider & rawDataProvider, uint32_t headerLen, uint32_t dataLen) : IPacket(rawDataProvider), m_dataLen(dataLen), m_headerLen(headerLen)
        {

        }
        void getHeader(uint8_t * buf, uint32_t * len)
        {
            if(m_rawDataProvider.getLen() < 0)
            {
                return;
            }
            buf = m_rawDataProvider.getDataPtr();
            *len = m_headerLen;
        }
        void getData(uint8_t * buf, uint32_t * len)
        {
            if(m_rawDataProvider.getLen() < 0)
            {
                return;
            }
            buf = &m_rawDataProvider[m_headerLen];
            *len = m_rawDataProvider.getLen();
        }

    private:
    uint32_t m_dataLen = 0;
    uint32_t m_headerLen = 0;
};



class ISegment
{
    public :
        ISegment(IRawDataProvider & rawDataProvider) : m_rawDataProvider(rawDataProvider)
        {
        }
        // для формирования сегмента, необходимо указать каким образом будет происходить добавление
        // добавление регистра с данными, или добавление просто адреса ргеистра
        // true - регистр добален в сегмент
        // false - соответственно, добален небыл.
        virtual bool addReg(IRegister & reg) = 0;
        virtual bool addReg(uint32_t regAddr, uint32_t regLen) = 0;
        virtual ~ISegment(){};
    protected:
            IRawDataProvider & m_rawDataProvider;
};

//каждый segment содержит в себе Ack, MessageType, devAddr и набор полу-сырых байт
template<class AckType, class MessageType, class DeviceAddrType>
class SegmentBase : public ISegment
{
    public:
        SegmentBase(IRawDataProvider & rawDataProvider): ISegment(rawDataProvider)
        {

        }
        virtual AckType & getAck() = 0;
        virtual MessageType & getMessageType() = 0;
        virtual DeviceAddrType & getDevAddr() = 0;
        virtual ~SegmentBase(){};
};






