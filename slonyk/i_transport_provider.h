#pragma once
#include "eng_settings.h"
#include "types.h"
#include "i_network_provider.h"

class ITransportProvider
{
    public:
        // для того, что стоит выше
        virtual void send(ISegment & segment) = 0;

        // для Network provider'a
        virtual void packetReceived(IRawDataProvider & raw) = 0;
        callback::Callback<void (IRawDataProvider & raw)> sendPacket;
        virtual ~ITransportProvider(){}
};
