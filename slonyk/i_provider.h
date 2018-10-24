#pragma once
#include "eng_settings.h"




//template<uint32_t ProvidersNum>
//class SlDataLinkProvider : public IDataLinkProvider
//{
//    public:
//        SlDataLinkProvider(): m_totalProviders(0)
//        {
//
//        }
//        void addProvider(INetworkProvider * provider)
//        {
//            if(m_totalProviders == ProvidersNum)
//            {
//                ENG_ASSERT();
//            }
//            m_providers[m_totalProviders] = provider;
//            m_providers[m_totalProviders]->sendData = CALLBACK_BIND_MEMBER(*this, SlDataLinkProvider::sendData);
//        }
//    protected:
//        virtual void sendData(uint8_t * data, uint32_t len) = 0;
//        uint32_t m_totalProviders;
//        INetworkProvider * m_providers[ProvidersNum];
//};



template<uint32_t ProvidersNum, class StageTopType, class StageBottomType, class StageTopArgs, class StageBottomArgs>
class ProviderBase
{
	void addProvider(INetworkProvider * provider)
	{
//		if(m_totalProviders == ProvidersNum)
//		{
//			ENG_ASSERT();
//		}
//		m_providers[m_totalProviders] = provider;
//		m_providers[m_totalProviders]->sendData = CALLBACK_BIND_MEMBER(*this, SlDataLinkProvider::sendData);
	}
	virtual ~ProviderBase(){};
protected:
	uint32_t m_totalProviders;
	INetworkProvider * m_providers[ProvidersNum];
};

//class ITransportProvider
//{
//    public:
//        // для того, что стоит выше
//        virtual void send(ISegment & segment) = 0;
//
//        // для Network provider'a
//        virtual void packetReceived(IPacket & packet) = 0;
//        callback::Callback<void (IPacket & packet)> sendPacket;
//        virtual ~ITransportProvider(){};
//};

//// Владеет значнием того, как преобразовать сырую дату в пакет и сегмент в пакет
//class INetworkProvider
//{
//    public:
//        // для Transport provider'a
//        virtual void send(IPacket & packet) = 0;
//
//        // для DataLinkProvider'a (как бэ вниз)
//        virtual void dataReceived(uint8_t * data, uint32_t len) = 0;
//        callback::Callback<void (uint8_t * data, uint32_t len)> sendData;
//        virtual ~INetworkProvider(){};
//};

//class IDataLinkProvider
//{
//    public:
//        virtual void sendData(uint8_t * data, uint32_t len) = 0;
//        virtual ~IDataLinkProvider(){};
//};
