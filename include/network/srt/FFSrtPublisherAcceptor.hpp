#pragma once

#include "network/srt/FFSrtAcceptorBase.hpp"

namespace ff {
	class FFSrtPublisherAcceptor : public FFSrtAcceptorBase {
	public:
		explicit FFSrtPublisherAcceptor(FFSrtRelayServer& relayServer);
		explicit FFSrtPublisherAcceptor(FFSrtRelayServer& relayServer, const std::string& bindIpAddr, int port);
		virtual ~FFSrtPublisherAcceptor();

	protected:
		virtual void onAccept(SRTSOCKET newSocket) override;
	};
};