#pragma once
#include "project_config.h"
#include <QObject>
#include <QUdpSocket>
#include <QVector>
#include <QTimer>
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>
#include "slonyk/slonyk_master.h"
#include "slonyk/slonyk_slave.h"
#include "slonyk/slonyk_table.h"
#include "slonyk/slonyk_transport_provider.h"

class Reg_proto : public QObject
{
    Q_OBJECT
public:
    Reg_proto(): m_master(&m_motherSlave), m_motherSlave(SlSlave(1, m_slaveTable, m_transportProvider))
    {

    }
    void init();
private:
    SimpleTable m_slaveTable;
    SlMaster<1> m_master;
    SlSlave m_motherSlave;
    SlTransportProvider m_transportProvider;
    QTimer m_senderTimer{this};
    void timerTickEvent();
    void work();
};

