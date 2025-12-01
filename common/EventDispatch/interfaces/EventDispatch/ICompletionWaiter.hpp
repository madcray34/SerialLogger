#pragma once

#include <std/chrono>

namespace tanlibs::eventdispatch::sync
{
    /**
     * @brief Interface representing a waitable completion primitive for event dispatching.
     */
    class ICompletionWaiter
    {
        public:
        using time_point   = std::chrono::time_point<std::chrono::steady_clock>;
        virtual ~ICompletionWaiter() = default;

        virtual void wait() = 0;
        virtual bool wait(const std::chrono::milliseconds& timeout) = 0;
        virtual bool waitUntil(const time_point& timePoint) = 0;
    };

}    // namespace tanlibs::eventdispatch::sync
