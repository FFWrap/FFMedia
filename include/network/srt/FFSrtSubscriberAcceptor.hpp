#pragma once

#include "network/srt/FFSrtAcceptorBase.hpp"

namespace ff {
	class FFSrtSubscriberAcceptor : public FFSrtAcceptorBase {
	public:
		explicit FFSrtSubscriberAcceptor(FFSrtRelayServer& relayServer);
		explicit FFSrtSubscriberAcceptor(FFSrtRelayServer& relayServer, const std::string& bindIpAddr, int port);
		virtual ~FFSrtSubscriberAcceptor();

	protected:
		virtual void onAccept(SRTSOCKET newSocket) override;
	};
};