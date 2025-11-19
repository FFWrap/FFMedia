#include "network/srt/FFSrtSubscriberAcceptor.hpp"
#include "network/srt/FFSrtRelayServer.hpp"

namespace ff {
	FFSrtSubscriberAcceptor::FFSrtSubscriberAcceptor(FFSrtRelayServer& relayServer) 
	: FFSrtAcceptorBase(relayServer) 
	{

	}

	FFSrtSubscriberAcceptor::FFSrtSubscriberAcceptor(FFSrtRelayServer& relayServer, const std::string& bindIpAddr, int port)
	: FFSrtAcceptorBase(relayServer, bindIpAddr, port){

	}

	FFSrtSubscriberAcceptor::~FFSrtSubscriberAcceptor() {

	}

	void FFSrtSubscriberAcceptor::onAccept(SRTSOCKET newSocket) {
		this->getRelayServer().onSubscriberAccepted(newSocket);
	}
}