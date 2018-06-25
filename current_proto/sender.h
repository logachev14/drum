#pragma once
#include "motion/motion.h"
#include "project_config.h"
#include <QObject>
#include <QUdpSocket>
#include <QVector>
#include <QTimer>
#include <QSemaphore>
#include <QMutex>
#include <QMutexLocker>

#define WAS_WRITTEN_FLAG_POS 2
#define QUEUE_FREE_ELEMENTS_POS 4
STRONG_ENUM(SendState,
            IDLE,
            SEND_NEW_MOTIONS,
            REPEAT_LAST_PACKET,
            WAIT_ANSWER,
            WAIT_MOTHER_FREE_QUEUE);
STRONG_ENUM(ReceivedFrame,
            TIMEOUT,
            WRITTEN,
            NOT_WRITTEN,
            WAIT);

class Sender : public QObject
{
    Q_OBJECT

signals:
    void onCommandAdded(uint32_t cmd_num);
public:
    explicit Sender(QObject *parent = nullptr);
    void init();
private:
    SendState m_sendState = SendState::IDLE;
    QHostAddress m_destAdr = QHostAddress("240.0.0.2");
    QHostAddress m_localAdr = QHostAddress("240.0.0.1");
    uint32_t m_localPort = 10000;
    uint32_t m_destPort = 63000;
    uint32_t m_totalFrameCounter = 1;
    QTimer m_senderTimer{this};
    void storeMotion(Motion motion);
    Motion m_lastMotions[4];
    uint8_t m_numOfLastMotions;
    CircularBuffer<Motion, 1000000> m_queue;
    QUdpSocket * m_udpSocket;
    ReceivedFrame checkValidResponse();
    ReceivedFrame m_frameState = ReceivedFrame::WAIT;
    uint8_t prepareCommands();
    void releaseSemaphore();
    void getFrameData(Motion * motion, uint8_t m_numOfMotions, uint8_t * buffer, uint8_t * len);
    void getMotionData(Motion motion, uint8_t * buffer);
    bool isTimeout();
    void clearTimeout();
    QSemaphore m_semaphore;
    uint32_t m_motherFreeElements;
    uint32_t m_cmdNum;
protected slots:
    void send();
public slots:
    void readPendingDatagrams();
    void addNewMotion(Motion motion);
};
