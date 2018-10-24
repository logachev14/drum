#pragma once
#include "eng_settings.h"
#include "types.h"



// Владеет значнием того, как преобразовать сырую дату в пакет и сегмент в пакет
class INetworkProvider
{
    public:
        // для Transport provider'a
        virtual void send(IPacket & packet) = 0;

        // для DataLinkProvider'a (как бэ вниз)
        virtual void dataReceived(uint8_t * data, uint32_t len) = 0;
        callback::Callback<void (uint8_t * data, uint32_t len)> sendData;
        virtual ~INetworkProvider(){};
};
