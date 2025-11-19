#include "network/srt/FFSrtRelayServer.hpp"

namespace ff {
	FFSrtRelayServer::FFSrtRelayServer() {
		this->publisherAcceptor = std::make_shared<FFSrtPublisherAcceptor>(*this, "0.0.0.0", 10000);
		this->subscriberAcceptor = std::make_shared<FFSrtSubscriberAcceptor>(*this, "0.0.0.0", 20000);

	}

	FFSrtRelayServer::~FFSrtRelayServer() {

	}

	void FFSrtRelayServer::start() {
		this->publisherAcceptor->start();
		this->subscriberAcceptor->start();
	}

	void FFSrtRelayServer::stop() {
		this->publisherAcceptor->stop();
		this->subscriberAcceptor->stop();

		for (auto& iter : this->publisherSessions) {
			iter->stop();
		}

		for (auto& iter : this->subscriberSessions) {
			iter->stop();
		}
	}

	void FFSrtRelayServer::onPublisherAccepted(SRTSOCKET srtSocket) {
		std::cout << "onPublisherAccepted" << std::endl;

		char streamId[513] = { 0, };
		int streamIdLength = 0;

		int result = srt_getsockflag(srtSocket, SRTO_STREAMID, streamId, &streamIdLength);
		if (result != 0) {
			srt_close(srtSocket);
			return;
		}

		FFSrtStreamChannelPtr streamChannel = this->streamManager.getOrCreate(streamId);
		FFSrtPublisherSessionPtr publisherSession = FFSrtPublisherSession::create(srtSocket, streamChannel);
		
		{
			std::lock_guard lockguard(this->publisherSessionMutex);
			this->publisherSessions.insert(publisherSession);
		}

		publisherSession->start();
	}

	void FFSrtRelayServer::onSubscriberAccepted(SRTSOCKET srtSocket) {
		std::cout << "onSubscriberAccepted" << std::endl;

		char streamId[513] = { 0, };
		int streamIdLength = 0;

		int result = srt_getsockflag(srtSocket, SRTO_STREAMID, streamId, &streamIdLength);
		if (result != 0) {
			srt_close(srtSocket);
		}

		FFSrtStreamChannelPtr streamChannel = this->streamManager.getOrCreate(streamId);
		FFSrtSubscriberSessionPtr subscriberSession = FFSrtSubscriberSession::create(srtSocket, streamChannel);

		{
			std::lock_guard lockguard(this->publisherSessionMutex);
			this->subscriberSessions.insert(subscriberSession);
		}

		subscriberSession->start();
	}
}