#pragma once

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LogSocketError(...) LogError(__VA_ARGS__, ", err=", errno)

// Manage Win32 compatibility
typedef int SOCKET;
inline void closesocket(SOCKET sock) {
    close(sock);
}
inline void InitWinsock() {
}
