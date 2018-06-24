#pragma once
#include "i_register.h"
#define U_8_REGS_NUM 10
#define U_32_REGS_NUM 10
class Table
{
public:
    Uint8Reg & u_8Regs[U_8_REGS_NUM];
    Uint32Reg & u_32Regs[U_32_REGS_NUM];
};
