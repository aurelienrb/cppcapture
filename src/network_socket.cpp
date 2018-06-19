#include "network_socket.h"
#include "utils.h"

#include <cstring>

#ifdef _WIN32
#include "network_socket_windows.h"
#else
#include "network_socket_posix.h"
#endif

// utility RAII object to close the given socket
static raven::Guard SocketCloser(SOCKET s) {
    return raven::Guard{ [s] {
        LogDebug("closing socket");
        closesocket(s);
    } };
}

static std::string Receive(SOCKET sock) {
    std::string response;
    for (;;) {
        char recvbuf[1024]; // typical response size is ~600 bytes
        int recvSize = recv(sock, recvbuf, sizeof(recvbuf), 0);
        if (recvSize > 0) {
            // we received data
            response.append(recvbuf, recvSize);
            if (static_cast<size_t>(recvSize) < sizeof(recvbuf)) {
                break; // done
            }
        } else if (recvSize == 0) {
            LogInfo("socket connection was gracefully closed by remote peer");
            break;
        } else {
            LogSocketError("failed to receive socket data");
            break;
        }
    }
    return response;
}

static bool TryConnect(SOCKET sock, const std::string & hostName, int portNumber) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr)); // use memset to remain compatible with g++ 4.8

    addr.sin_family = AF_INET;
    addr.sin_port   = htons(static_cast<u_short>(portNumber));

    if (inet_addr(hostName.c_str()) == INADDR_NONE) {
        // resolve ip address
        struct hostent * he = gethostbyname(hostName.c_str());
        if (he == nullptr) {
            LogError("failed to resolve IP for ", hostName);
            return false;
        }

        // cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        auto addr_list = reinterpret_cast<struct in_addr **>(he->h_addr_list);
        for (int i = 0; addr_list[i] != nullptr; i++) {
            addr.sin_addr = *addr_list[i];
            LogDebug(hostName, " resolved to ", inet_ntoa(*addr_list[i]));
            // try to connect
            if (connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
                LogDebug("failed to connect, may try again with another IP");
            } else {
                // success!
                LogInfo("connected to ", hostName);
                return true;
            }
        }
        return false;
    } else {
        // plain ip address
        addr.sin_addr.s_addr = inet_addr(hostName.c_str());

        // connect to remote server
        if (connect(sock, reinterpret_cast<sockaddr *>(&addr), sizeof(addr)) < 0) {
            LogSocketError("failed to connect to ", hostName);
            return false;
        }
        return true;
    }
}

namespace raven {
    void SendRawRequest(std::string hostName, int portNumber, const std::string & data) {
        // Windows sockets must be initialized at the process level before being used
        InitWinsock();

        // create the socket
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            LogError("failed to create new socket");
            return; // TODO: reeaddrinfo(addrResult);
        }
        auto closer = SocketCloser(sock);

        // connect to the given host
        LogDebug("connecting to host");
        if (!TryConnect(sock, hostName, portNumber)) {
            LogError("failed to connect to ", hostName, " on port ", portNumber);
            return;
        }

        // send the request
        LogDebug("sending data on socket:\n", data);
        if (send(sock, data.c_str(), static_cast<int>(data.length()), 0) < 0) {
            LogSocketError("failed to send data on socket");
        }

        // only in debug: wait for the response
        LogDebug("response from remote server:\n", Receive(sock));
        (void)&Receive; // mute GCC error "Receive() defined but not used" (in release)
    }
} // namespace raven
