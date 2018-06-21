#include "raven/client.h"
#include "utils.h"

#include <cassert>

namespace raven {
    void Client::Send(const Event & event) {
        assert(m_channel);
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
} // namespace raven
