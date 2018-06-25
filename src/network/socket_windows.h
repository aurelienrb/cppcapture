#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define LogSocketError(...) LogError(__VA_ARGS__, ", err=", WSAGetLastError())

namespace cppcapture {
    void InitWinsock();
}
