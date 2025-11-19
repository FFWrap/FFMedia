#include "network/srt/FFSrtPublisherSession.hpp"
#include "network/srt/FFSrtStreamChannel.hpp"

namespace ff {
	FFSrtPublisherSession::FFSrtPublisherSession(SRTSOCKET srtSocket, FFSrtStreamChannelPtr srtStreamChannel)
	: FFSrtSocketBase(srtSocket)
	, srtStreamChannel(srtStreamChannel)
	{
		this->streamId = srtStreamChannel->getStreamId();
	}

	FFSrtPublisherSession::~FFSrtPublisherSession() {

	}

	FFSrtPublisherSessionPtr FFSrtPublisherSession::create(SRTSOCKET srtSocket, FFSrtStreamChannelPtr srtStreamChannel) {
		FFSrtPublisherSessionPtr session = std::make_shared<FFSrtPublisherSession>(srtSocket, srtStreamChannel);
		
		return session;
	}

	void FFSrtPublisherSession::onStart() {

	}
	
	void FFSrtPublisherSession::onLoop() {

	}

	void FFSrtPublisherSession::onStop() {

	}
}