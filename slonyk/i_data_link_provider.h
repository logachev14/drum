#pragma once
#include "eng_settings.h"
#include "callbacks.h"
#include "i_network_provider.h"


class IDataLinkProvider
{
    public:
        virtual void sendData(uint8_t * data, uint32_t len) = 0;
        virtual ~IDataLinkProvider(){};
};


