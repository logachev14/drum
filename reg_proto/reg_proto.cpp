#include "reg_proto.h"

void Reg_proto::init()
{
    const uint32_t m_sendInterval = 1;
    connect( &m_senderTimer, &QTimer::timeout, this, &Reg_proto::work );
    m_senderTimer.start(m_sendInterval);
}

void Reg_proto::timerTickEvent()
{
}

void Reg_proto::work()
{

}
