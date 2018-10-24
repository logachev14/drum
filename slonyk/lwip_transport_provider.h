#pragma once
#include "eng_settings.h"
#include "i_transport_provider.h"
#include "i_network_provider.h"

#include "lwip/src/include/lwip/udp.h"
#include "lwip/src/include/lwip/timeouts.h"
#include "lwip/src/include/lwip/init.h"
#include "lwip/src/include/lwip/netif.h"
#include "lwip/src/include/lwip/etharp.h"
#include "lwip/src/include/netif/ethernet.h"
#include "lwip/src/include/lwip/ip_addr.h"
#include "lwip_port/netif/ethernetif.h"

template<uint32_t NetworkProvidersNum>
class LwIpTransportProvider : public ITransportProvider
{
    public:
        void send(IPacket & packet)
        {

        }
        void addNetworkProvider(INetworkProvider & provider)
        {

        }
    private:
        uint32_t m_nums = NetworkProvidersNum;
        uint32_t m_currNetworksNum = 0;
        INetworkProvider * m_providers[NetworkProvidersNum];

        static void receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, const ip_addr_t *addr, uint16_t port);
        struct netif m_gnetif;
        struct udp_pcb * m_udpServer;
        ip4_addr_t m_ipaddr;
        ip4_addr_t m_pcAddr;
        ip4_addr_t m_netmask;
        ip4_addr_t m_gw;
};
