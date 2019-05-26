#pragma once

#include <string>

namespace cppcapture {
#ifdef _WIN32
    typedef size_t SOCKET;
#else
    typedef int SOCKET;
#endif

    // Socket is a simple implementation based on BSD sockets
    class Socket {
    public:
        Socket(const std::string & hostName, int portNum);
        ~Socket();

        bool send(const std::string & data);
        bool send(const std::string & data, std::string & response);

    private:
        SOCKET m_socket;
    };
} // namespace cppcapture
