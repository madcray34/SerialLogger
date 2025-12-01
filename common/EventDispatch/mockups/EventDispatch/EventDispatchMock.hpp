#pragma once

#include <EventDispatch/IEventEmitter.hpp>
#include <EventDispatch/IAwaitableEventEmitter.hpp>
#include <EventDispatch/IEventHandler.hpp>
#include <EventDispatch/IAwaitableEventHandler.hpp>
#include "EventCompletionLatchMock.hpp"
#include <gmock/gmock.h>


namespace tanlibs::eventdispatch
{
    template<typename T>
    class EventEmitterMock : public IEventEmitter<T>
    {
        public:
        MOCK_METHOD(void, connect, (IEventHandler<T> & slot), (override));
        MOCK_METHOD(void, disconnect, (IEventHandler<T> & slot), (override));
        MOCK_METHOD(void, disconnect, (), (override));
        MOCK_METHOD(void, emit, (const T &value), (override));
    };

    template<typename T>
    class AwaitableEventEmitterMock : public IAwaitableEventEmitter<T>
    {
        public:
        MOCK_METHOD(void, connect, (IEventHandler<T> & slot), (override));
        MOCK_METHOD(void, disconnect, (IEventHandler<T> & slot), (override));
        MOCK_METHOD(void, disconnect, (), (override));
        MOCK_METHOD(void, emit, (const T &value), (override));
        MOCK_METHOD(void, wait, (), (override));
        MOCK_METHOD(bool, wait, (const std::chrono::milliseconds &timeout), (override));
        MOCK_METHOD(bool, waitUntil, (const sync::ICompletionWaiter::time_point &timePoint), (override));
        MOCK_METHOD(void, notify, (), (override));
    };

    template<typename T>
    class EventHandlerMock : public IEventHandler<T>
    {
        public:
        MOCK_METHOD(void, setCallback, (const std::function<void(T)> &callback), (override));
        MOCK_METHOD(void, setCallback, (std::function<void(T)> && callback), (override));
        MOCK_METHOD(void, execute, (const T &parameter), (override));
    };

    template<typename T>
    class ICallable
    {
        public:
        virtual ~ICallable()                  = default;
        virtual void call(const T &parameter) = 0;
    };

    template<typename T>
    class CallableMock : public ICallable<T>
    {
        public:
        MOCK_METHOD(void, call, (const T &parameter), (override));

        void operator()(const T &parameters)
        {
            call(parameters);
        }
    };

    template<typename T>
    class AwaitableEventHandlerMock : public IAwaitableEventHandler<T>
    {
        public:
        MOCK_METHOD(void, execute, (const T &parameter), (override));
        MOCK_METHOD(void, setCallback, (const std::function<void(T)> &callback), (override));
        MOCK_METHOD(void, setCallback, (std::function<void(T)> && callback), (override));
        MOCK_METHOD(void, wait, (), (override));
        MOCK_METHOD(bool, wait, (const std::chrono::milliseconds &timeout), (override));
        MOCK_METHOD(bool, waitUntil, (const sync::ICompletionWaiter::time_point &timePoint), (override));
        MOCK_METHOD(void, notify, (), (override));
    };

    template<>
    class EventHandlerMock<void> : public IEventHandler<void>
    {
        public:
        MOCK_METHOD(void, setCallback, (const std::function<void()> &), (override));
        MOCK_METHOD(void, setCallback, (std::function<void()> &&), (override));
        MOCK_METHOD(void, execute, (), (override));
    };
}    // namespace tanlibs::eventdispatch

// For void payload tests, define a simple mock for IEventHandler<void>
class VoidEventHandlerMock : public tanlibs::eventdispatch::IEventHandler<void>
{
    public:
    MOCK_METHOD(void, setCallback, (const std::function<void()> &), (override));
    MOCK_METHOD(void, setCallback, (std::function<void()> &&), (override));
    MOCK_METHOD(void, execute, (), (override));
};
