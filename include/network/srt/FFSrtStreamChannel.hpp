#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>

namespace ff {
	class FFSrtSubscriberSession;
	class FFSrtPublisherSession;

	class FFSrtStreamChannel : public std::enable_shared_from_this<FFSrtStreamChannel> {
	public:
		explicit FFSrtStreamChannel();
		virtual ~FFSrtStreamChannel();

	public:
		void pushPacket(std::vector<char> buffer);
		
		void attachPublisher(std::shared_ptr<FFSrtPublisherSession> session);
		void detachPublisher(std::shared_ptr<FFSrtPublisherSession> session);

		void attachSubscriber(std::shared_ptr<FFSrtSubscriberSession> session);
		void detachSubscriber(std::shared_ptr<FFSrtSubscriberSession> session);

		void start();
		void stop();

	private:
		void startBoradcastLoop();

	private:
		std::string streamId;
		std::weak_ptr<FFSrtPublisherSession> publisherSession;
		std::set<std::weak_ptr<FFSrtSubscriberSession>> subscriberSessions;
	};
};