#pragma once
#include "i_table.h"

class SlTable : public  ITable
{
    public :
        void getRoRegs(IRegister * regs, uint32_t regsNum)
                {

                }
    private:
        uint32_t m_someVal = 555;
};


enum class RoUint32RegNames
{
    x = 0,
    y = 1,
    z = 2,
    FUCK = 3,
};

enum class RoInt8RegNames
{
    tmp1 = 4,
    tmp2 = 5,
};

enum class WoUint32RegNames
{
    x = 6,
    y = 7,
    z = 8,
};

enum class WoInt8RegNames
{
    tmp1 = 9,
    tmp2 = 10,
};


class SimpleTable : public ITable
{
    public:
        void getRegs(IRegister * regs, uint32_t * regsNum)
        {
        	regs = *m_totalRegs;
        	*regsNum = m_totalRegsNum;
        }
        void writeReg(uint32_t regAddr, uint8_t * data, uint32_t dataLen)
        {
        	if(m_totalRegs[regAddr]->getAccessType() == RegAccessType::WO)
        	{
        		ENG_ASSERT();
        	}
        	m_totalRegs[regAddr]->setBytes(data, dataLen);
        }
        IRegister * getReg(uint32_t regAddr)
        {
        	return m_totalRegs[regAddr];
        }
        Uint32Reg & getReg(RoUint32RegNames addr)
        {
            return m_rouint32regnames_gen[static_cast<uint32_t>(addr) - 0];
        }
        Uint8Reg & getReg(RoInt8RegNames addr)
        {
            return m_roint8regnames_gen[static_cast<uint32_t>(addr) - 4];
        }
        Uint32Reg & getReg(WoUint32RegNames addr)
        {
            return m_wouint32regnames_gen[static_cast<uint32_t>(addr) - 6];
        }
        Uint8Reg & getReg(WoInt8RegNames addr)
        {
            return m_woint8regnames_gen[static_cast<uint32_t>(addr) - 9];
        }
        template<class NewValType>
        void writeReg(WoUint32RegNames addr, NewValType newVal)
        {
                m_wouint32regnames_gen[static_cast<uint32_t>(addr) - 6].writeReg(newVal);
        }



    private:
        Uint32Reg m_rouint32regnames_gen[4] = { Uint32Reg(0, RegAccessType::RO),
                                                Uint32Reg(1, RegAccessType::RO),
                                                Uint32Reg(2, RegAccessType::RO),
                                                Uint32Reg(3, RegAccessType::RO) };

        Uint8Reg m_roint8regnames_gen[2] = { Uint8Reg(4, RegAccessType::RO),
                                             Uint8Reg(5, RegAccessType::RO) };

        Uint32Reg m_wouint32regnames_gen[3] = { Uint32Reg(6, RegAccessType::WO),
                                                Uint32Reg(7, RegAccessType::WO),
                                                Uint32Reg(8, RegAccessType::WO) };

        Uint8Reg m_woint8regnames_gen[2] = { Uint8Reg(9, RegAccessType::WO),
                                             Uint8Reg(10, RegAccessType::WO) };
        IRegister * m_totalRegs[4] = {&m_rouint32regnames_gen[0], &m_rouint32regnames_gen[1], &m_rouint32regnames_gen[2], &m_rouint32regnames_gen[3] };
        uint32_t m_totalRegsNum = 4;
};
