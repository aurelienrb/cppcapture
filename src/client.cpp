#include "debugging.h"
#include "cppcapture/client.h"

namespace cppcapture {

    Client & Client::Instance() {
        static Client s_instance;
        return s_instance;
    }

    Client::Client() {
        CPPCAPTURE_LOGINFO("creating new client");
    }

    Client::~Client() {
        CPPCAPTURE_LOGINFO("destroying client");
        if (m_channel) {
            CPPCAPTURE_LOGDEBUG("flushing channel");
            m_channel->Flush();
        }
    }

    void Client::operator+=(const Event & event) {
        assert(m_channel);
        if (!m_channel) {
            CPPCAPTURE_LOGERROR("can't send the event: channel configuration was not done");
            return;
        }

        // make sure we don't leak an exception
        try {
            CPPCAPTURE_LOGINFO("sending event on channel");
            m_channel->SendEvent(event);
        } catch (const std::exception & e) {
            CPPCAPTURE_LOGERROR("caught an exception while sending event: ", e.what());
        }
    }
} // namespace cppcapture
