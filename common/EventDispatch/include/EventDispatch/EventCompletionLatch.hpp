#pragma once

#include <EventDispatch/ICompletionWaiter.hpp>
#include <EventDispatch/ICompletionNotifier.hpp>

#include <std/mutex>
#include <std/condition_variable>

namespace tanlibs::eventdispatch::sync
{
    /**
     * @class EventCompletionLatch
     * @brief Lightweight completion primitive used by awaitable emitters/handlers to block until an event finishes.
     */
    class EventCompletionLatch : public ICompletionWaiter, public ICompletionNotifier
    {
        public:
        EventCompletionLatch();
        ~EventCompletionLatch() override = default;

        void wait() override;
        bool wait(const std::chrono::milliseconds& timeout) override;
        bool waitUntil(const ICompletionWaiter::time_point& timePoint) override;
        void notify() override;

        private:
        std::mutex              m_mutex;    ///< protects condition variable while waiting
        std::condition_variable m_cv;       ///< guarded condition variable used for waiting
    };

}    // namespace tanlibs::eventdispatch::sync
