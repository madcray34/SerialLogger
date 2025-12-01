#pragma once

#include <EventDispatch/IEventHandler.hpp>
#include <EventDispatch/EventEmitter.hpp>
#include <EventDispatch/IAwaitableEventEmitter.hpp>
#include <EventDispatch/EventCompletionLatch.hpp>

#include <vector>
#include <functional>
#include <std/mutex>
#include <atomic>

namespace tanlibs::eventdispatch
{
    /**
     * @class SignalWaiter
     * @tparam T The type of the data that can be emitted with the signal.
     * @brief A template class for a signal in a signal-slot mechanism that can be waited on.
     *
     * A signal represents a state change or an event. It can be connected to slots,
     * which are functions or methods that will be called when the signal is emitted.
     */
    template<typename T>
    class AwaitableEventEmitter : public IAwaitableEventEmitter<T>
                               , protected EventEmitter<T>
                               , protected sync::EventCompletionLatch
    {
        public:
        AwaitableEventEmitter(const AwaitableEventEmitter&)            = delete;
        AwaitableEventEmitter(AwaitableEventEmitter&&)                 = delete;
        AwaitableEventEmitter& operator=(const AwaitableEventEmitter&) = delete;

        AwaitableEventEmitter()           = default;
        ~AwaitableEventEmitter() override = default;

        /**
         * @brief Connect a slot to this signal.
         * @param slot The slot to connect to the signal.
         * When the signal is emitted, the connected slot will be called.
         */
        void connect(IEventHandler<T>& slot) override;

        /**
         * @brief Disconnect a specific slot from this signal.
         * @param slot The slot to disconnect from the signal.
         */
        void disconnect(IEventHandler<T>& slot) override;

        /**
         * @brief Disconnect all slots from this signal.
         */
        void disconnect() override;

        /**
         * @brief Emit the signal.
         * @param parameters The data to emit with the signal.
         * All connected slots will be called with this data.
         */
        void emit(const T& parameters) override;

        /**
         * @brief Waits for the slot to be executed.
         */
        void wait() override;

        /**
         * @brief Waits for the slot to be executed or for a timeout.
         *
         * @param timeout The maximum time to wait.
         * @return true if the slot was executed, false if the timeout was reached.
         */
        bool wait(const std::chrono::milliseconds& timeout) override;

        /**
         * @brief Waits for the slot to be executed until a certain time point.
         *
         * @param timePoint The time point to wait until.
         * @return true if the slot was executed before the time point, false otherwise.
         */
        bool waitUntil(const sync::ICompletionWaiter::time_point& timePoint) override;

        protected:
        /**
         * @brief Notifies that the slot has been executed.
         */
        void notify() override;

        private:
        std::vector<std::reference_wrapper<IEventHandler<T>>> m_slots;
    };

    template<typename T>
    void AwaitableEventEmitter<T>::connect(IEventHandler<T>& slot)
    {
        EventEmitter<T>::connect(slot);
    }

    template<typename T>
    void AwaitableEventEmitter<T>::disconnect(IEventHandler<T>& slot)
    {
        EventEmitter<T>::disconnect(slot);
    }

    template<typename T>
    void AwaitableEventEmitter<T>::disconnect()
    {
        EventEmitter<T>::disconnect();
    }

    template<typename T>
    void AwaitableEventEmitter<T>::emit(const T& parameters)
    {
        EventEmitter<T>::emit(parameters);
        sync::EventCompletionLatch::notify();
    }

    template<typename T>
    void AwaitableEventEmitter<T>::wait()
    {
        sync::EventCompletionLatch::wait();
    }

    template<typename T>
    bool AwaitableEventEmitter<T>::wait(const std::chrono::milliseconds& timeout)
    {
        return sync::EventCompletionLatch::wait(timeout);
    }

    template<typename T>
    bool AwaitableEventEmitter<T>::waitUntil(const sync::ICompletionWaiter::time_point& timePoint)
    {
        return sync::EventCompletionLatch::waitUntil(timePoint);
    }

    template<typename T>
    void AwaitableEventEmitter<T>::notify()
    {
        sync::EventCompletionLatch::notify();
    }
}    // namespace tanlibs::eventdispatch