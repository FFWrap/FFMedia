#include "network/srt/FFSrtPublisherAcceptor.hpp"
#include "network/srt/FFSrtRelayServer.hpp"

namespace ff {
	FFSrtPublisherAcceptor::FFSrtPublisherAcceptor(FFSrtRelayServer& relayServer)
	: FFSrtAcceptorBase(relayServer) 
	{

	}

	FFSrtPublisherAcceptor::FFSrtPublisherAcceptor(FFSrtRelayServer& relayServer, const std::string& bindIpAddr, int port)
	: FFSrtAcceptorBase(relayServer, bindIpAddr, port) 
	{

	}

	FFSrtPublisherAcceptor::~FFSrtPublisherAcceptor() {

	}

	void FFSrtPublisherAcceptor::onAccept(SRTSOCKET newSocket) {
		this->getRelayServer().onPublisherAccepted(newSocket);
	}
}