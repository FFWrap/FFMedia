#include "network/srt/FFSrtSubscriberSession.hpp"
#include "network/srt/FFSrtStreamChannel.hpp"

namespace ff {
	FFSrtSubscriberSession::FFSrtSubscriberSession(SRTSOCKET srtSocket, FFSrtStreamChannelPtr srtStreamChannel)
	: FFSrtSocketBase(srtSocket)
	, srtStreamChannel(srtStreamChannel) 
	{
		this->streamId = srtStreamChannel->getStreamId();
	}

	FFSrtSubscriberSession::~FFSrtSubscriberSession() {

	}

	FFSrtSubscriberSessionPtr FFSrtSubscriberSession::create(SRTSOCKET srtSocket, FFSrtStreamChannelPtr srtStreamChannel) {
		FFSrtSubscriberSessionPtr session = std::make_shared<FFSrtSubscriberSession>(srtSocket, srtStreamChannel);

		return session;
	}

	void FFSrtSubscriberSession::enqueuePacket(FFSrtStreamPtr srtStream) {

	}

	void FFSrtSubscriberSession::onStart() {

	}

	void FFSrtSubscriberSession::onLoop() {

	}

	void FFSrtSubscriberSession::onStop() {

	}
}