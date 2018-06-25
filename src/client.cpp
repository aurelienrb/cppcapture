#include "debugging.h"
#include "cppcapture/client.h"

namespace cppcapture {
    Client::Client() {
        LogInfo("creating new client");
    }
    Client::~Client() {
        LogInfo("destroying client");
        if (m_channel) {
            LogInfo("flushing channel attached to Client");
            m_channel->Flush();
        }
    }

    void Client::Send(const Event & event) {
        assert(m_channel);
        LogInfo("sending new event from Client");
        if (!m_channel) {
            LogError("can't send the event: channel configuration was not done");
            return;
        }

        // make sure we don't leak an exception
        try {
            m_channel->SendEvent(event);
        } catch (const std::exception & e) {
            LogError("caught an exception while sending event: ", e.what());
        }
    }
} // namespace cppcapture
