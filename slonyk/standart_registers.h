#pragma once
#include "i_register.h"

//class Uint32Reg : public IRegister<uint32_t>
//{
//    public:
//
//        virtual void getBytes(uint8_t * buf, uint32_t * len)
//        {
//            *buf = 0;
//            *(buf + 1) = 1;
//            *(buf + 2) = 2;
//            *(buf + 3) = 3;
//            *len = 4;
//        }
//        virtual void setBytes(uint8_t * buf, uint32_t len)
//        {
//
//        }
//        virtual uint32_t getLen()
//        {
//            return m_size;
//        }
//        virtual uint32_t getRegAddr()
//        {
//            return 0;
//        }
//
//        uint32_t getRegVal()
//        {
//            return m_value;
//        }
//    protected :
//        uint32_t m_value;
//        size_t m_size;
//};
