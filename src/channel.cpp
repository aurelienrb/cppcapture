#include "raven/channel.h"

#include <algorithm>
#include <mutex>
#include <vector>

static std::mutex s_allChannelsMutex;
static std::vector<raven::Channel *> s_allChannels;

namespace raven{
    void FlushAllChannels() {
        std::lock_guard<std::mutex> lock{ s_allChannelsMutex };
        for (const auto & c : s_allChannels) {
            c->Flush();
        }
    }

    Channel::Channel() {
        // track the existence of this instance
        std::lock_guard<std::mutex> lock{ s_allChannelsMutex };
        s_allChannels.push_back(this);
    }

    Channel::~Channel() {
        // forget this instance
        std::lock_guard<std::mutex> lock{ s_allChannelsMutex };
        s_allChannels.erase(std::remove(s_allChannels.begin(), s_allChannels.end(), this), s_allChannels.end());
    }
}
