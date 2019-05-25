#include "cppcapture/channel.h"
#include "debugging.h"

#include <algorithm>
#include <mutex>
#include <vector>

static std::mutex s_allChannelsMutex;
static std::vector<cppcapture::Channel *> s_allChannels;

namespace cppcapture {
    void FlushAllChannels() {
        std::lock_guard<std::mutex> lock{ s_allChannelsMutex };
        for (const auto & c : s_allChannels) {
            c->Flush();
        }
    }

    Channel::Channel(std::string name) : m_name(std::move(name)) {
        CPPCAPTURE_LOGDEBUG("creating channel ", m_name);

        // track the existence of this instance
        std::lock_guard<std::mutex> lock{ s_allChannelsMutex };
        s_allChannels.push_back(this);
    }

    Channel::~Channel() {
        CPPCAPTURE_LOGDEBUG("destroying channel ", m_name);

        // forget this instance
        std::lock_guard<std::mutex> lock{ s_allChannelsMutex };
        s_allChannels.erase(std::remove(s_allChannels.begin(), s_allChannels.end(), this), s_allChannels.end());
    }
} // namespace cppcapture
