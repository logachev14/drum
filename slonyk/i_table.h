#pragma once
#include "i_register.h"

class ITable
{
    public:
        virtual void getRegs(IRegister * regs, uint32_t * regsNum) = 0;
        virtual void writeReg(uint32_t regAddr, uint8_t * data, uint32_t dataLen) = 0;
        virtual IRegister *  getReg(uint32_t regAddr) = 0;
        virtual ~ITable(){};
};
