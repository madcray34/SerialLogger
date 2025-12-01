#pragma once

#include <EventDispatch/IEventEmitter.hpp>
#include <EventDispatch/EventHandler.hpp>
#include <EventDispatch/IAwaitableEventHandler.hpp>
#include <EventDispatch/EventCompletionLatch.hpp>

#include <functional>
#include <std/mutex>

namespace tanlibs::eventdispatch
{
    /**
     * @brief A class that waits for a slot to be executed.
     *
     * @tparam T The type of the parameter that the slot accepts.
     */
    template<typename T>
    class AwaitableEventHandler : public IAwaitableEventHandler<T>
                                , protected EventHandler<T>
                                , protected sync::EventCompletionLatch
    {
        public:
        /**
         * @brief Deleted copy constructor.
         */
        AwaitableEventHandler(const AwaitableEventHandler&) = delete;

        /**
         * @brief Deleted move constructor.
         */
        AwaitableEventHandler(AwaitableEventHandler&&) = delete;

        /**
         * @brief Deleted copy assignment operator.
         */
        AwaitableEventHandler& operator=(const AwaitableEventHandler&) = delete;

        /**
         * @brief Constructs a new SlotWaiter.
         *
         * @param signal The signal that this slot is connected to.
         */
        AwaitableEventHandler(IEventEmitter<T>& signal);

        /**
         * @brief Constructs a new SlotWaiter with a callback.
         *
         * @param signal The signal that this slot is connected to.
         * @param callback The function to call when the signal is emitted.
         */
        AwaitableEventHandler(IEventEmitter<T>& signal, const std::function<void(T)>& callback);

        /**
         * @brief Constructs a new SlotWaiter with a callback.
         *
         * @param signal The signal that this slot is connected to.
         * @param callback The function to call when the signal is emitted.
         */
        AwaitableEventHandler(IEventEmitter<T>& signal, std::function<void(T)>&& callback);

        /**
         * @brief Destroys the SlotWaiter.
         */
        ~AwaitableEventHandler() override;

        /**
         * @brief Sets the callback function.
         *
         * @param callback The new callback function.
         */
        void setCallback(const std::function<void(T)>& callback) override;

        /**
         * @brief Sets the callback function.
         *
         * @param callback The new callback function.
         */
        void setCallback(std::function<void(T)>&& callback) override;

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

        /**
         * @brief Executes the slot with the given parameters.
         *
         * @param parameters The parameters to pass to the slot.
         */
        void execute(const T& parameters) override;
    };

    template<typename T>
    AwaitableEventHandler<T>::AwaitableEventHandler(IEventEmitter<T>& signal)
        : EventHandler<T>(signal)
        , sync::EventCompletionLatch()
    {}

    template<typename T>
    AwaitableEventHandler<T>::AwaitableEventHandler(IEventEmitter<T>& signal,
                                                    const std::function<void(T)>& callback)
        : EventHandler<T>(signal, callback)
    {}

    template<typename T>
    AwaitableEventHandler<T>::AwaitableEventHandler(IEventEmitter<T>& signal,
                                                    std::function<void(T)>&& callback)
        : EventHandler<T>(signal, std::move(callback))
    {}

    template<typename T>
    AwaitableEventHandler<T>::~AwaitableEventHandler()
    {}

    template<typename T>
    void AwaitableEventHandler<T>::setCallback(const std::function<void(T)>& callback)
    {
        EventHandler<T>::setCallback(callback);
    }

    template<typename T>
    void AwaitableEventHandler<T>::setCallback(std::function<void(T)>&& callback)
    {
        EventHandler<T>::setCallback(std::move(callback));
    }

    template<typename T>
    void AwaitableEventHandler<T>::execute(const T& parameters)
    {
        EventHandler<T>::execute(parameters);
        notify();
    }

    template<typename T>
    void AwaitableEventHandler<T>::wait()
    {
        sync::EventCompletionLatch::wait();
    }

    template<typename T>
    bool AwaitableEventHandler<T>::wait(const std::chrono::milliseconds& timeout)
    {
        return sync::EventCompletionLatch::wait(timeout);
    }

    template<typename T>
    bool AwaitableEventHandler<T>::waitUntil(const sync::ICompletionWaiter::time_point& timePoint)
    {
        return sync::EventCompletionLatch::waitUntil(timePoint);
    }

    template<typename T>
    void AwaitableEventHandler<T>::notify()
    {
        sync::EventCompletionLatch::notify();
    }

}    // namespace tanlibs::eventdispatch