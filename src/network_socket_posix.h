#pragma once

#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define LogSocketError(...) LogError(__VA_ARGS__, ", err=", errno)

// Manage Win32 compatibility
typedef int SOCKET;
inline void closesocket(SOCKET sock) {
    close(sock);
}
inline void InitWinsock() {
}
