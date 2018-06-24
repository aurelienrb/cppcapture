#pragma once

#include "raven/event.h"

#include <memory>
#include <string>

namespace raven {
    // FlushAllChannels calls Flush() on all the existing channel instances.
    // The typical use case is on program termination to make sure all channels complete their tasks before exiting.
    void FlushAllChannels();

    using ChannelPtr = std::shared_ptr<class Channel>;

    // Channel is the interface used by the Client to send encoded events to a specific target.
    // Typically the Channel will send over the network, but it could act as a proxy to
    // route the events to other locations such as a third party logger or the console output.
    class Channel {
    public:
        virtual ~Channel();

        // disable copy
        Channel(const Channel &) = delete;
        void operator=(const Channel &) = delete;

        // Called by the Client to send a new event. If the implementation does not complete the operation immediately,
        // Flush() must also be overriden to allow the caller to request the completion of all the pending tasks.
        // Note that it must be thread safe!
        virtual void SendEvent(const Event & e) = 0;

        // Called to ask the channel to ensure that all the events it received so far have been properly sent.
        // Idealy, the function should not take too long to complete and return after a few seconds at most.
        // More specifically, ig the connection is down, it should not hang waiting for the connection to be up again.
        // It must also be possible to call it from any thread.
        // Special consideration: should Flush() continue to accept new events via SendEvent() is up to the
        // implementation
        virtual void Flush() {
        }

    protected:
        // Channel constructor keeps tracks of of the created instances
        Channel();
    };
} // namespace raven
