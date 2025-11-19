#pragma once

extern "C" {
#include "srt/srt.h"
}

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif


namespace ff {
	static void FFSrtInitialize() {
#ifdef _WIN32
		WSADATA wsa;
		WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
		
		srt_startup();
	}

	static void FFSrtFinalize() {
		srt_cleanup();

#ifdef _WIN32
		WSACleanup();
#endif
	}
}