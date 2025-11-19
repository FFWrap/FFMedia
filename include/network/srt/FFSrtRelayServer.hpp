#pragma once

#include "network/srt/FFSrtStreamManager.hpp"
#include "network/srt/FFSrtPublisherAcceptor.hpp"
#include "network/srt/FFSrtSubscriberAcceptor.hpp"
#include "network/srt/FFSrtPublisherSession.hpp"
#include "network/srt/FFSrtSubscriberSession.hpp"

#include <memory>
#include <mutex>
#include <thread>
#include <unordered_set>

namespace ff {
	class FFSrtRelayServer {
	public:
		explicit FFSrtRelayServer();
		virtual ~FFSrtRelayServer();

	public:
		void start();
		void stop();

		void onPublisherAccepted(SRTSOCKET srtSocket);
		void onSubscriberAccepted(SRTSOCKET srtSocket);


	private:
		FFSrtStreamManager streamManager;

		std::shared_ptr<FFSrtPublisherAcceptor> publisherAcceptor;
		std::shared_ptr<FFSrtSubscriberAcceptor> subscriberAcceptor;

		std::mutex publisherSessionMutex;
		std::mutex subscriberSessionMutex;
		std::unordered_set<std::shared_ptr<FFSrtPublisherSession>> publisherSessions;
		std::unordered_set<std::shared_ptr<FFSrtSubscriberSession>> subscriberSessions;
	};
};