#pragma once

#include "network/srt/FFSrtStream.hpp"

#include <memory>
#include <vector>
#include <set>
#include <string>

namespace ff {
	class FFSrtStreamChannel;
	using FFSrtStreamChannelPtr = std::shared_ptr<FFSrtStreamChannel>;

	class FFSrtSubscriberSession;
	class FFSrtPublisherSession;

	class FFSrtStreamChannel : public std::enable_shared_from_this<FFSrtStreamChannel> {
	public:
		explicit FFSrtStreamChannel();
		virtual ~FFSrtStreamChannel();

	public:
		static FFSrtStreamChannelPtr create();

	public:
		void pushPacket(FFSrtStreamPtr buffer);
		
		void attachPublisher(std::shared_ptr<FFSrtPublisherSession> session);
		void detachPublisher(std::shared_ptr<FFSrtPublisherSession> session);

		void attachSubscriber(std::shared_ptr<FFSrtSubscriberSession> session);
		void detachSubscriber(std::shared_ptr<FFSrtSubscriberSession> session);

		void start();
		void stop();

		std::string getStreamId() { return this->streamId; }

	private:
		void startBoradcastLoop();

	private:
		std::string streamId;
		std::weak_ptr<FFSrtPublisherSession> publisherSession;
		std::set<std::weak_ptr<FFSrtSubscriberSession>> subscriberSessions;
	};
};