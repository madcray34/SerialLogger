#include <EventDispatch/EventCompletionLatch.hpp>

namespace tanlibs::eventdispatch::sync
{
    EventCompletionLatch::EventCompletionLatch() = default;

    void EventCompletionLatch::wait()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock);
    }

    bool EventCompletionLatch::wait(const std::chrono::milliseconds& timeout)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_cv.wait_for(lock, timeout) == std::cv_status::no_timeout;
    }

    bool EventCompletionLatch::waitUntil(const ICompletionWaiter::time_point& timePoint)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_cv.wait_until(lock, timePoint) == std::cv_status::no_timeout;
    }

    void EventCompletionLatch::notify()
    {
        m_cv.notify_all();
    }

}    // namespace tanlibs::eventdispatch::sync
