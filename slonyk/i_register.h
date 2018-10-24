#pragma once
#include "eng_settings.h"


enum class RegAccessType
{
        RO = 0,
        WO = 1
};

struct IRegister
{
    public:
        virtual void getBytes(uint8_t * buf, uint32_t * len) = 0;
        virtual void setBytes(uint8_t * buf, uint32_t len) = 0;
        virtual uint32_t getLen() = 0;
        virtual uint32_t getRegAddr() = 0;
        virtual RegAccessType getAccessType() = 0;
        virtual ~IRegister(){};
};

template<class T>
class Register : public IRegister
{
public:
    Register(uint32_t addr, RegAccessType accType): m_size(sizeof(T)),
                                                    m_accType(accType),
                                                    m_addr(addr)
    {

    }
    T getRegVal()
    {
        return m_value;
    }

    virtual void getBytes(uint8_t * buf, uint32_t * len) = 0;
    virtual void setBytes(uint8_t * buf, uint32_t len) = 0;
    virtual uint32_t getLen() = 0;
    uint32_t getRegAddr()
    {
            return m_addr;
    }
    RegAccessType getAccessType()
    {
        return m_accType;
    }
    void writeReg(T newVal)
    {
        m_value = newVal;
    }
protected:
    T m_value;
    size_t m_size;
private:
    RegAccessType m_accType;
    uint32_t m_addr;
};

//template<class T>
//void Register<uint32_t>::getBytes(uint8_t * buf, uint32_t * len)
//{
//    *buf = 0;
//    *(buf + 1) = 1;
//    *(buf + 2) = 2;
//    *(buf + 3) = 3;
//    *len = 4;
//}


class Uint32Reg : public Register<uint32_t>
{
    public:
        Uint32Reg(uint32_t addr, RegAccessType accType) : Register<uint32_t>(addr, accType)
        {

        }
        virtual void getBytes(uint8_t * buf, uint32_t * len)
        {
            *buf = 0;
            *(buf + 1) = 1;
            *(buf + 2) = 2;
            *(buf + 3) = 3;
            *len = 4;
        }
        virtual void setBytes(uint8_t * buf, uint32_t len)
        {

        }
        virtual uint32_t getLen()
        {
            return m_size;
        }
};

class Uint8Reg : public Register<uint8_t>
{
    public:
        Uint8Reg(uint32_t addr, RegAccessType accType) : Register<uint8_t>(addr, accType)
        {

        }
        virtual void getBytes(uint8_t * buf, uint32_t * len)
        {
            *buf = 0;
            *(buf + 1) = 1;
            *(buf + 2) = 2;
            *(buf + 3) = 3;
            *len = 4;
        }
        virtual void setBytes(uint8_t * buf, uint32_t len)
        {

        }
        virtual uint32_t getLen()
        {
            return m_size;
        }
};













