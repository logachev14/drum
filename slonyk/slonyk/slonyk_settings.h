#pragma once

#define SL_ACK_REQUEST 0
#define SL_ACK_RESPONSE 1

#define SL_MESS_BROADCAST 0
#define SL_MESS_WRITE 1
#define SL_MESS_READ 2

#define SL_REG_ADDR_LEN 2

#define SL_BROADCAST_ADDRESS 65535
#define SL_DATA_START_POS 4
// в байтах
#define SL_HEADER_LEN 4
// 1 байт - кол-во регистров в сегменте
#define SL_MAX_DATA_LEN 199
#define SL_MAX_PACKET_LEN 204

#define SL_ACK_POS 0
#define SL_MTYPE_POS 1
#define SL_ADDR_POS 2
#define SL_REGS_ADDR_START_POS 5

enum SlMessageType
{
        SL_BROADCAST = 0,
        SL_WRITE = 1,
        SL_READ = 2
};

enum SlAcknowledge
{
        SL_REQUEST = 0,
        SL_RESPONSE = 1
};

