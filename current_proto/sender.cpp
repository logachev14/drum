#include "sender.h"


Sender::Sender(QObject *parent)
{


    //

}
void Sender::send()
{
    static uint8_t buf[100];
    static uint8_t dataLen;
    switch(m_sendState)
    {
        case SendState::IDLE:
        {

            if(m_queue.getUsedElemNum()>0)
            {
                m_sendState = SendState::SEND_NEW_MOTIONS;
            }
            break;
        }
        case SendState::SEND_NEW_MOTIONS:
        {
            if(m_queue.isEmpty() == true)
            {
                m_sendState = SendState::IDLE;
                break;
            }
            m_numOfLastMotions = prepareCommands();
            if(m_numOfLastMotions == 0)
            {
                return;
            }
            m_totalFrameCounter++;
            getFrameData(m_lastMotions, m_numOfLastMotions, buf, &dataLen);
            m_udpSocket->writeDatagram((const char*)buf, dataLen, m_destAdr, m_destPort);
            qDebug()<<"sended!";
            m_sendState = SendState::WAIT_ANSWER;
            m_frameState = ReceivedFrame::WAIT;

            clearTimeout();
            break;
        }
        case SendState::WAIT_ANSWER:
        {
            switch(checkValidResponse())
            {
                case ReceivedFrame::TIMEOUT :
                {
                    m_sendState = SendState::REPEAT_LAST_PACKET;
                    break;
                }
                case ReceivedFrame::NOT_WRITTEN :
                {
                    m_sendState = SendState::WAIT_MOTHER_FREE_QUEUE;
                    break;
                }
                case ReceivedFrame::WRITTEN:
                {
                    m_sendState = SendState::SEND_NEW_MOTIONS;
                    break;
                }
                case ReceivedFrame::WAIT:
                {
                    break;
                }
                default:
                {
                    UMBA_ASSERT_FAIL();
                }
            }
            break;
        }
        case SendState::REPEAT_LAST_PACKET:
        {
            m_udpSocket->writeDatagram((const char*)buf, dataLen, m_destAdr, m_destPort);
            m_sendState = SendState::WAIT_ANSWER;
            clearTimeout();
            break;
        }
        case SendState::WAIT_MOTHER_FREE_QUEUE:
        {
            if(m_motherFreeElements >= 10)
            {
                m_sendState = SendState::SEND_NEW_MOTIONS;
            }
            break;
        }
        default:
        {
            UMBA_ASSERT_FAIL();
        }
    }
}
ReceivedFrame Sender::checkValidResponse()
{
    if(isTimeout())
    {
        return ReceivedFrame::TIMEOUT;
    }
    return m_frameState;
}
bool Sender::isTimeout()
{
    return false;
}
void Sender::clearTimeout()
{

}
uint8_t Sender::prepareCommands()
{
    uint8_t numOfMotions = 0;
    uint32_t freeElemNum = m_queue.getUsedElemNum();
    if(freeElemNum >= 4)
    {
        for(; numOfMotions < 4; numOfMotions++)
        {
            m_lastMotions[numOfMotions] = m_queue.readTail();
        }
    }
    else
    {
        for(; numOfMotions < freeElemNum; numOfMotions++)
        {
            m_lastMotions[numOfMotions] = m_queue.readTail();
        }
    }
    return numOfMotions;
}
void Sender::init()
{
    m_udpSocket = new QUdpSocket(this);
    m_udpSocket->bind(m_localAdr, m_localPort);
    connect(m_udpSocket, SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));
    const uint32_t m_sendInterval = 1;
    connect( &m_senderTimer, &QTimer::timeout, this, &Sender::send );
    m_senderTimer.start(m_sendInterval);
    m_sendState = SendState::IDLE;
}

void Sender::readPendingDatagrams()
{
    QByteArray datagram;
    qDebug()<<"income";
    do
    {
        datagram.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(datagram.data(), datagram.size());
    }
    while (m_udpSocket->hasPendingDatagrams());
    uint8_t array[20];
    uint8_t * data = reinterpret_cast<uint8_t *> (datagram.data());
    memcpy(array, data, 20);
    auto len = datagram.length();
    auto tmp = data + QUEUE_FREE_ELEMENTS_POS;
    m_motherFreeElements = common_functions::bytesToUint32(*(tmp), *(tmp + 1), *(tmp + 2), *(tmp + 3));
    if(*(data + WAS_WRITTEN_FLAG_POS) == 1)
    {
        m_frameState = ReceivedFrame::WRITTEN;
    }
    else if(*(data + WAS_WRITTEN_FLAG_POS) == 0)
    {
        m_frameState = ReceivedFrame::NOT_WRITTEN;
    }

}

void Sender::getFrameData(Motion * motion, uint8_t numOfMotions, uint8_t * buffer, uint8_t * len)
{
    //проверка на null
    if(motion == NULL || buffer == NULL) {UMBA_ASSERT_FAIL();}
    (*len) = 0;
    Motion * motionPtr = motion;
    uint8_t * bufPtr = buffer;
    uint8_t head[] = {0xAB, 0xAB, motion->command, numOfMotions};

    // забиваем голову
    for(uint8_t i = 0; i < 4; i++, bufPtr++, (*len)++)
    {
        *bufPtr = head[i];
    }
    // забиваем кол-во тотальных фреймов
    for(uint8_t i=0; i < 4; i++, bufPtr++, (*len)++)
    {
        *bufPtr = (m_totalFrameCounter >> (i*8))  & 0xFF;
    }
    // 21 байт занимает структура с перемещением:
    uint8_t data[21];
    for(uint8_t i = 0; i < numOfMotions; i++, motionPtr++, bufPtr+=21 )
    {
        getMotionData(*motionPtr, data);
        memcpy(bufPtr, data, 21);
        (*len) = ((*len) + 21);
    }
}
void Sender::getMotionData(Motion motion, uint8_t * buffer)
{
    uint8_t * bufPtr = buffer;
    float values[] = {motion.x, motion.y, motion.z, motion.e, motion.f};
    uint32_t fourBytes = 0;

    for(uint8_t i=0; i < 5; i++, bufPtr+=4)
    {
        memcpy(&fourBytes, &values[i], 4);
        *(uint32_t *)bufPtr = fourBytes;
    }
    *(bufPtr) = motion.command;
}
void Sender::releaseSemaphore()
{
    m_semaphore.release();
}
void Sender::addNewMotion(Motion motion)
{
    m_queue.writeHead(motion);
    m_cmdNum++;
    emit onCommandAdded(m_cmdNum);
}


