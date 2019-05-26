#include "socket.h"
#include "debugging.h"

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>

#define CPPCAPTURE_LOG_SOCKET_ERROR(...) CPPCAPTURE_LOGINFO(__VA_ARGS__, " err=", WSAGetLastError())

static void InitWinsock() {
    // don't use a static initialization object that calls WSAStartup() in its constructor because
    // it can make the code hang on VC++ if this function is itself called from another static object constructor
    // it seems there's a deadlock in atexit, not really sure why
    // Note: we don't cleanup Winsock on purpose since it's not really useful
    static bool s_initDone = false;
    if (!s_initDone) {
        s_initDone = true;
        CPPCAPTURE_LOGDEBUG("initializing winsock");
        WSADATA WSAData;
        if (int err = WSAStartup(MAKEWORD(2, 0), &WSAData)) {
            CPPCAPTURE_LOGERROR("failed to init Winsock, err=", err);
        }
    }
}

#else
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define CPPCAPTURE_LOG_SOCKET_ERROR(...) CPPCAPTURE_LOGINFO(__VA_ARGS__, " errno=", errno)

// Win32 compatibility
typedef int SOCKET;
static void closesocket(SOCKET sock) {
    close(sock);
}
static void InitWinsock() {
}
static constexpr int INVALID_SOCKET = -1;
static constexpr int SOCKET_ERROR   = -1;
#endif

#define LOG_SOCKET_SEND(data) // CPPCAPTURE_LOGDEBUG("sending data on socket:\n", data);

#include <memory>

/*
static bool HasDataToRead(SOCKET sock) {
    u_long pendingSize;
    if (ioctlsocket(sock, FIONREAD, &pendingSize) != NO_ERROR) {
        CPPCAPTURE_LOG_SOCKET_ERROR("failed to peek socket data size");
        return false;
    }
    return (pendingSize > 0);
}
*/
static std::string BlockingRead(SOCKET sock) {
    std::string response;
    for (;;) {
        char recvbuf[1024]; // typical response size is ~600 bytes
        const int recvSize = recv(sock, recvbuf, sizeof(recvbuf), MSG_PEEK);
        if (recvSize > 0) {
            // we received data
            response.append(recvbuf, static_cast<size_t>(recvSize));
            if (static_cast<size_t>(recvSize) < sizeof(recvbuf)) {
                break; // done
            }
        } else if (recvSize == 0) {
            CPPCAPTURE_LOGINFO("socket connection was gracefully closed by remote peer");
            break;
        } else {
            CPPCAPTURE_LOG_SOCKET_ERROR("failed to receive socket data");
            break;
        }
    }
    return response;
}

static std::string resolvedIP(struct addrinfo & info) {
    if (info.ai_family == AF_INET) {
        // IPv4
        if (const char * result = inet_ntoa(reinterpret_cast<struct sockaddr_in *>(info.ai_addr)->sin_addr)) {
            return result;
        }
    } else if (info.ai_family == AF_INET6) {
        // IPv6
        const auto addr6 = reinterpret_cast<struct sockaddr_in6 *>(info.ai_addr);
        char buffer[INET6_ADDRSTRLEN];
#ifdef _WIN32
        DWORD count = sizeof(buffer);
        if (WSAAddressToStringA(reinterpret_cast<LPSOCKADDR>(addr6), sizeof(*addr6), nullptr, buffer, &count) == 0) {
            return buffer;
        }
#else
        if (const char * result = inet_ntop(info.ai_family, &addr6->sin6_addr, buffer, sizeof(buffer))) {
            return result;
        }
#endif
    }
    return "";
}

// returns empty string on failure
static SOCKET connectToHost(const std::string & hostName, int portNum) {
    InitWinsock();

    const std::string portStr = std::to_string(portNum);
    CPPCAPTURE_LOGDEBUG("connecting to ", hostName, " on port ", portStr);

    struct addrinfo hints {};
    hints.ai_family   = AF_UNSPEC;   // IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP only
    struct addrinfo * servinfo;
    if (int status = getaddrinfo(hostName.c_str(), portStr.c_str(), &hints, &servinfo)) {
        CPPCAPTURE_LOGERROR("failed to resolve host ", hostName, ": ", gai_strerror(status));
        return INVALID_SOCKET;
    }

    // make sure to free the list
    std::unique_ptr<struct addrinfo, std::function<void(struct addrinfo *)>> deleter{ servinfo, &freeaddrinfo };

    // iterate the list of addresses
    for (struct addrinfo * p = servinfo; p != nullptr; p = p->ai_next) {
        assert(p->ai_socktype == SOCK_STREAM);

        // try to connect
        CPPCAPTURE_LOGINFO("connecting to IP ", resolvedIP(*p));
        SOCKET result = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (result == INVALID_SOCKET) {
            CPPCAPTURE_LOG_SOCKET_ERROR("failed to create socket");
        } else {
            if (connect(result, p->ai_addr, static_cast<int>(p->ai_addrlen)) == SOCKET_ERROR) {
                CPPCAPTURE_LOG_SOCKET_ERROR("failed to connect to ", hostName);
                // close this socket to try again if possible
                closesocket(result);
            } else {
                CPPCAPTURE_LOGDEBUG("connected to host");
                return result;
            }
        }
    }

    return INVALID_SOCKET;
}

namespace cppcapture {
    Socket::Socket(const std::string & hostName, int portNum) {
        CPPCAPTURE_LOGINFO("opening socket to ", hostName);
        m_socket = connectToHost(hostName, portNum);
        if (m_socket == INVALID_SOCKET) {
            CPPCAPTURE_LOGERROR("failed to connect to ", hostName);
        }
    }

    Socket::~Socket() {
        if (m_socket != INVALID_SOCKET) {
            CPPCAPTURE_LOGDEBUG("closing socket");
            closesocket(m_socket);
        }
    }

    bool Socket::send(const std::string & data) {
        std::string response;
        return send(data, response);
    }

    bool Socket::send(const std::string & data, std::string & response) {
        // ensure propre Windows socket init before use
        InitWinsock();

        // send the request
        LOG_SOCKET_SEND(data);
        if (::send(m_socket, data.c_str(), static_cast<int>(data.length()), 0) < 0) {
            CPPCAPTURE_LOG_SOCKET_ERROR("failed to send data on socket");
            return false;
        }

        // wait for the response of the server before closing the socket (or our request will be lost)
        response = BlockingRead(m_socket);
        CPPCAPTURE_LOGDEBUG("response from remote server:\n", response);
        return true;
    }
} // namespace cppcapture
