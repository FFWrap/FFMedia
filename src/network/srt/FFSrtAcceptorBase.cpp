#include "network/srt/FFSrtAcceptorBase.hpp"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif

namespace ff {
	FFSrtAcceptorBase::FFSrtAcceptorBase(FFSrtRelayServer& relayServer)
	: relayServer(relayServer)
	, bindIpAddr("0.0.0.0")
	, bindPort(10000) 
	, srtSocket(SRT_INVALID_SOCK)
	{

	}
	FFSrtAcceptorBase::FFSrtAcceptorBase(FFSrtRelayServer& relayServer, const std::string& bindIpAddr, int port)
	: relayServer(relayServer)
	, bindIpAddr(bindIpAddr)
	, bindPort(port) 
	, srtSocket(SRT_INVALID_SOCK)
	{

	}

	FFSrtAcceptorBase::~FFSrtAcceptorBase() {

	}

	void FFSrtAcceptorBase::start() {
		if (this->srtSocket != SRT_INVALID_SOCK) {
			return;
		}

		this->srtSocket = srt_create_socket();
		if (this->srtSocket == SRT_INVALID_SOCK) {
			std::cout << "Create ListenSocket Error: " << srt_getlasterror_str() << std::endl;
			return;
		}

		struct sockaddr_in sa;
		memset(&sa, 0x00, sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_port = htons(this->bindPort);
		sa.sin_addr.s_addr = htonl(INADDR_ANY);
		if (srt_bind(this->srtSocket, (struct sockaddr*)&sa, sizeof(sa)) == SRT_ERROR) {
			std::cout << "Listen Socket Bind Error: " << srt_getlasterror_str() << std::endl;
			srt_close(this->srtSocket);
			return;
		}

		if (srt_listen(this->srtSocket, 5) == SRT_ERROR) {
			std::cout << "Listen Error: " << srt_getlasterror_str() << std::endl;
			srt_close(this->srtSocket);
			return;
		}

		std::cout << "Start SRT Relay Server Port: " << this->bindPort << std::endl;
		this->acceptThread = std::make_shared<std::jthread>([=](std::stop_token stopToken) {
			this->acceptLoop(stopToken);
		});
	}

	void FFSrtAcceptorBase::stop() {
		if (this->acceptThread != nullptr) {
			this->acceptThread->request_stop();
		}

		srt_close(this->srtSocket);
		this->srtSocket = SRT_INVALID_SOCK;
		
		if (this->acceptThread != nullptr) {
			this->acceptThread->join();
			this->acceptThread = nullptr;
		}
	}

	void FFSrtAcceptorBase::acceptLoop(std::stop_token stopToken) {
		while (stopToken.stop_requested() == false) {
			SRTSOCKET clientSrtSocket;
			struct sockaddr_in clientAddr;
			int addrLen = sizeof(clientAddr);

			clientSrtSocket = srt_accept(this->srtSocket, (struct sockaddr*)&clientAddr, &addrLen);
			if (clientSrtSocket == SRT_INVALID_SOCK) {
				std::cout << "Client Accept Error: " << srt_getlasterror_str() << std::endl;
				std::cout << "Current Port: " << this->bindPort << std::endl;
				continue;
			}
			std::cout << "Accept Client Current Port: " << this->bindPort << std::endl;

			this->onAccept(clientSrtSocket);
		}
	}
}