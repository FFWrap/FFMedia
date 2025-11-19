#include "network/srt/FFSrtStreamChannel.hpp"


namespace ff {
	FFSrtStreamChannel::FFSrtStreamChannel() {

	}

	FFSrtStreamChannel::~FFSrtStreamChannel() {

	}

	FFSrtStreamChannelPtr FFSrtStreamChannel::create() {
		FFSrtStreamChannelPtr channel = std::make_shared<FFSrtStreamChannel>();

		return channel;
	}

	void FFSrtStreamChannel::pushPacket(FFSrtStreamPtr buffer) {

	}

	void FFSrtStreamChannel::attachPublisher(std::shared_ptr<FFSrtPublisherSession> session) {
		
	}

	void FFSrtStreamChannel::detachPublisher(std::shared_ptr<FFSrtPublisherSession> session) {

	}

	void FFSrtStreamChannel::attachSubscriber(std::shared_ptr<FFSrtSubscriberSession> session) {

	}

	void FFSrtStreamChannel::detachSubscriber(std::shared_ptr<FFSrtSubscriberSession> session) {

	}

	void FFSrtStreamChannel::start() {

	}

	void FFSrtStreamChannel::stop() {

	}

	void FFSrtStreamChannel::startBoradcastLoop() {

	}
};