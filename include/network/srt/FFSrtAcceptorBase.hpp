#pragma once

#include <string>
#include <memory>
#include <thread>
#include <stop_token>

extern "C" {
#include "srt/srt.h"
}

namespace ff {
	class FFSrtRelayServer;

	class FFSrtAcceptorBase {
	public:
		explicit FFSrtAcceptorBase(FFSrtRelayServer& relayServer);
		explicit FFSrtAcceptorBase(FFSrtRelayServer& relayServer, const std::string& bindIpAddr, int port);
		virtual ~FFSrtAcceptorBase();

	public:
		void start();
		void stop();

		void setBindIpAddr(const std::string& bindIpAddr) { this->bindIpAddr = bindIpAddr; }
		void setPort(int port) { this->bindPort = port; }

	protected:
		FFSrtRelayServer& getRelayServer() { return this->relayServer; }

		virtual void onAccept(SRTSOCKET newSocket) = 0;

	private:
		void acceptLoop(std::stop_token stopToken);

	private:
		FFSrtRelayServer& relayServer;

		SRTSOCKET srtSocket;

		std::string bindIpAddr;
		int bindPort;

		std::shared_ptr<std::jthread> acceptThread;
	};
};