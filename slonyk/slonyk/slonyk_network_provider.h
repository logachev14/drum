#pragma once
#include <i_data_link_provider.h>
#include "slonyk_settings.h"
#include "i_network_provider.h"
#include "slonyk_packet.h"

template<uint32_t ProvidersNum>
class SlNetworkProvider : public INetworkProvider
{
    public:
        void send(IPacket & packet)
        {
            uint8_t * dataPtr = 0;
            uint32_t * len = 0;
            //packet.getData(dataPtr, len);
            sendData(dataPtr, *len);
        }

        SlNetworkProvider() : m_totalProviders(0)
        {

        }
        void AddProvider(ITransportProvider * provider)
        {
            if(m_totalProviders == ProvidersNum)
            {
                ENG_ASSERT();
            }
            m_providers[m_totalProviders] = provider;
            m_providers[m_totalProviders]->sendPacket = CALLBACK_BIND_MEMBER(*this, SlNetworkProvider::send);
        }

        void dataReceived(uint8_t * data, uint32_t len)
        {
            m_slutPacket.setHeader(data, SL_HEADER_LEN);
            m_slutPacket.setData(data + SL_DATA_START_POS, len - SL_HEADER_LEN);
            for(uint8_t i = 0; i < m_totalProviders; i++ )
            {
                m_providers[i]->packetReceived(m_slutPacket);
            }
        }

    protected:
        ITransportProvider * m_providers[ProvidersNum];
        uint32_t m_totalProviders;
        SlPacket m_slutPacket;
};
