#pragma once
#include "i_data_link_provider.h"
#include "eng_settings.h"
#include "i_network_provider.h"

template<uint32_t ProvidersNum>
class SlDataLinkProvider : public IDataLinkProvider
{
    public:
        SlDataLinkProvider(): m_totalProviders(0)
        {

        }
        void addProvider(INetworkProvider * provider)
        {
            if(m_totalProviders == ProvidersNum)
            {
                ENG_ASSERT();
            }
            m_providers[m_totalProviders] = provider;
            m_providers[m_totalProviders]->sendData = CALLBACK_BIND_MEMBER(*this, SlDataLinkProvider::sendData);
        }
    protected:
        virtual void sendData(uint8_t * data, uint32_t len) = 0;
        uint32_t m_totalProviders;
        INetworkProvider * m_providers[ProvidersNum];
};
