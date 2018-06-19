#pragma once

#include "utils.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#define LogSocketError(...) LogError(__VA_ARGS__, ", err=", WSAGetLastError())

// Utility class to init Winsocks
class WinsockInit {
public:
    WinsockInit() {
        WSADATA WSAData;
        if (int err = WSAStartup(MAKEWORD(2, 0), &WSAData)) {
            LogError("failed to init Winsock, err=", err);
        }
    }
    ~WinsockInit() { WSACleanup(); }
};

WinsockInit & InitWinsock() {
    static WinsockInit s_winsockInit;
    return s_winsockInit;
}
