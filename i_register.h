#pragma once
struct IRegister
{
    virtual void getBytes(uint8_t * buf, uint32_t * len) = 0;
    virtual void setBytes(uint8_t * buf, uint32_t len) = 0;
    virtual uint32_t getLen() = 0;
    virtual uint32_t getRegAddr() = 0;
};

template<class T>
class Register : public IRegister
{
public:
    Register(T & value) : m_value(value), m_size(sizeof(value))
    {

    }
    T getRegVal()
    {
        return m_value;
    }
protected :
    T & m_value;
    size_t m_size;
};

class Uint32Reg : public Register<uint32_t>
{
public:
    Uint32Reg(uint32_t value) : Register<uint32_t>(value)
    {

    }
    void getBytes(uint8_t * buf, uint32_t * len)
    {
        *buf = 0;
        *(buf + 1) = 1;
        *(buf + 2) = 2;
        *(buf + 3) = 3;
        *len = 4;
    }
    void setBytes(uint8_t * buf, uint32_t len)
    {

    }
    uint32_t getLen()
    {
        return m_size;
    }
    uint32_t getRegAddr()
    {
        return 0;
    }
};

class Uint8Reg : public Register<uint8_t>
{
public:
    Uint32Reg(uint8_t value) : Register<uint8_t>(value)
    {

    }
    void getBytes(uint8_t * buf, uint32_t * len)
    {
        *buf = 0;
        *len = 1;
    }
    void setBytes(uint8_t * buf, uint32_t len)
    {

    }
    uint32_t getLen()
    {
        return m_size;
    }
    uint32_t getRegAddr()
    {
        return 0;
    }
};
