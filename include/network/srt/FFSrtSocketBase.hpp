#pragma once

#include <string>
#include <thread>
#include <memory>
#include <stop_token>

extern "C" {
#include "srt/srt.h"
}

namespace ff {
	class FFSrtSocketBase {
	public:
		explicit FFSrtSocketBase(SRTSOCKET srtSocket);
		virtual ~FFSrtSocketBase();

	public:
		void start();
		void stop();

	protected: 
		virtual void onStart();
		virtual void onLoop() = 0;
		virtual void onStop();

	private:
		void run(std::stop_token stop_token);

	private:
		std::shared_ptr<std::jthread> thread;

		SRTSOCKET srtSocket;
	};
};