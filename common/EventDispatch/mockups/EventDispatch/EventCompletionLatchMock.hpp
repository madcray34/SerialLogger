#pragma once

#include <EventDispatch/ICompletionNotifier.hpp>
#include <EventDispatch/ICompletionWaiter.hpp>
#include <gmock/gmock.h>

namespace tanlibs::eventdispatch::sync
{
    class CompletionNotifierMock : public ICompletionNotifier
    {
        public:
        MOCK_METHOD(void, notify, (), (override));
    };

    class CompletionWaiterMock : public ICompletionWaiter
    {
        public:
        MOCK_METHOD(void, wait, (), (override));
        MOCK_METHOD(bool, wait, (const std::chrono::milliseconds& timeout), (override));
        MOCK_METHOD(bool, waitUntil, (const time_point& timePoint), (override));
    };

}    // namespace tanlibs::eventdispatch::sync
