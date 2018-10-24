#pragma once
//#include "i_register.h"
//#define U_8_REGS_NUM 10
//#define U_32_REGS_NUM 10

//#define ADD_REGS(REGS_TYPE, ...)
//template <typename ... T>
//class Table
//{
//public:
//    Uint8Reg u_8Regs[U_8_REGS_NUM];
//    Uint32Reg u_32Regs[U_32_REGS_NUM];
//};

#include <iostream>

using namespace std;

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
    Register(){}
    T getRegVal()
    {
        return m_value;
    }

    virtual void getBytes(uint8_t * buf, uint32_t * len)
    {}
    virtual void setBytes(uint8_t * buf, uint32_t len)
    {}
    virtual uint32_t getLen()
    {
        return 42;
    }
    virtual uint32_t getRegAddr()
    {
        return 42;
    }
protected :
    T m_value;
    size_t m_size;
};

class Uint32Reg : public Register<uint32_t>
{
public:

    Uint32Reg(){}
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


#define CREATE(Name, ...)  \
enum class Name { __VA_ARGS__, TOTAL};

template<class TType, class TNames, class ... TOthers>
struct length
{
    static const uint32_t value = (uint32_t)TNames::TOTAL + length<TOthers ...>::value;
};

template<class TType, class TNames>
struct length<TType, TNames>
{
    static const uint32_t value =(uint32_t) TNames::TOTAL;
};


template<class TMyName, class TType, class TNames, class ... TOthers>
struct value_type
{
    using type = typename value_type<TMyName, TOthers ...>::type;
};
template<class TType, class TNames, class ... TOthers>
struct value_type<TNames, TType, TNames, TOthers ...>
{
    using type = TType;
};


template <class ... T>
class Table
{
public:
    Table() : m_holder( m_regs[0] )
    {}

    template<class TNames>
    typename value_type<TNames, T...>::type getValue( TNames name )
    {
        return m_holder.getValue(name);
    }

    template<class TType, class TNames>
    void setValue( TNames name, TType val )
    {
        m_holder.setValue(name, val);
    }

private:

    template<class TType, class TNames, class ... TOthers>
    class Holder : public Holder<T ...>
    {
    public:
        using Holder<TOthers...>::getValue;
        using Holder<TOthers...>::setValue;

        TType getValue( TNames name )
        {
            return m_values[(uint32_t)name];
        }
        void setValue( TNames name, TType val )
        {
            m_values[(uint32_t)name] = val;
        }

        Holder( IRegister * iRegs ) : Holder<TOthers...>(iRegs+(uint32_t)TNames::TOTAL)
        {
            for( uint32_t i = 0 ; i < (uint32_t)TNames::TOTAL; i++ )
            {
                iRegs = &m_values[i];
                iRegs++;
            }
        }
    private:
        TType m_values[(uint32_t)TNames::TOTAL];
    };
    template<class TType, class TNames>
    class Holder<TType, TNames>
    {
    public:

        TType getValue( TNames name )
        {
            return m_values[(uint32_t)name];
        }
        void setValue( TNames name, TType val )
        {
            m_values[(uint32_t)name] = val;
        }

        Holder( IRegister * iRegs )
        {
            for( uint32_t i = 0 ; i < (uint32_t)TNames::TOTAL; i++ )
            {
                iRegs = &m_values[i];
                iRegs++;
            }
        }
    private:
        TType m_values[(uint32_t)TNames::TOTAL];
    };

    Holder<T ...> m_holder;

    IRegister * m_regs[length<T ...>::value];
};

CREATE(uint32RegNames, motionX, motionY, motionZ, motionE);

void foo( uint32RegNames as )
{
}

int main() {
    Table<Uint32Reg, uint32RegNames> table;


    int x=10;
    int y=25;
    int z=x+y;

    foo( uint32RegNames::motionX );

    cout<<"Sum of x+y = " << table.getValue(uint32RegNames::motionX).getLen();
    table.setValue(uint32RegNames::motionY, table.getValue(uint32RegNames::motionX));
}
