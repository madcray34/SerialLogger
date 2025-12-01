#pragma once

#include <EventDispatch/IEventHandler.hpp>
#include <EventDispatch/IEventEmitter.hpp>

#include <functional>
#include <std/mutex>
#include <cassert>
#include <optional>

namespace tanlibs::eventdispatch
{
    /**
     * @class Slot
     * @tparam T The type of the data that can be passed to the Slot.
     * @brief A template interface for a Slot in a signal-Slot mechanism.
     *
     * A Slot is a function or method that can be connected to a signal, and will be called when the signal is emitted.
     */
    template<typename T>
    class EventHandler : public virtual IEventHandler<T>
    {
        public:
        /**
         * @brief Delete copy constructor.
         */
        EventHandler(const EventHandler&) = delete;

        /**
         * @brief Delete move constructor.
         */
        EventHandler(EventHandler&&) = delete;

        /**
         * @brief Delete assignment operator.
         */
        EventHandler& operator=(const EventHandler&) = delete;

        /**
         * @brief Default constructor without parameters.
         */
        EventHandler() : m_signal(std::nullopt)
        {}

        /**
         * @brief Construct a new Slot object and connect it to a signal.
         * @param signal The signal to connect to.
         */
        EventHandler(IEventEmitter<T>& signal) : m_signal(signal)
        {
            signal.connect(*this);
        }

        /**
         * @brief Construct a new Slot object, connect it to a signal and set a callback.
         * @param signal The signal to connect to.
         * @param callback The callback to set.
         */
        EventHandler(IEventEmitter<T>& signal, const std::function<void(T)>& callback)
            : m_signal(signal)
            , m_callback(callback)
        {
            signal.connect(*this);
        }

        /**
         * @brief Construct a new Slot object, connect it to a signal and set a callback.
         * @param signal The signal to connect to.
         * @param callback The callback to set.
         */
        EventHandler(IEventEmitter<T>& signal, std::function<void(T)>&& callback)
            : m_signal(signal)
            , m_callback(std::move(callback))
        {
            signal.connect(*this);
        }

        /**
         * @brief Disconnect from the signal on destruction.
         */
        ~EventHandler() override
        {
            if (m_signal)
                m_signal->get().disconnect(*this);
        }

        /**
         * @brief Set the callback function.
         * @param callback The new callback function.
         */
        void setCallback(const std::function<void(T)>& callback) override;

        /**
         * @brief Set the callback function.
         * @param callback The new callback function.
         */
        void setCallback(std::function<void(T)>&& callback) override;

        /**
         * @brief Connect to a signal after construction.
         * @param signal The signal to connect to.
         */
        void connectToSignal(IEventEmitter<T>& signal)
        {
            m_signal = signal;
            signal.connect(*this);
        }

        protected:
        /**
         * @brief Execute the callback function with parameters.
         * @param parameters The parameters to pass to the callback function.
         */
        void execute(const T& parameters) override;

        protected:
        std::optional<std::reference_wrapper<IEventEmitter<T>>> m_signal;      ///< The signal to connect to.
        std::function<void(T)>                                 m_callback;    ///< The callback function.
        std::mutex                                             m_mutex;       ///< Mutex for thread safety.
    };

    template<typename T>
    void EventHandler<T>::setCallback(const std::function<void(T)>& callback)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        assert(m_callback == nullptr && "Callback already set");
        m_callback = callback;
    }

    template<typename T>
    void EventHandler<T>::setCallback(std::function<void(T)>&& callback)
    {
        std::lock_guard<std::mutex> lock(m_mutex);

        assert(m_callback == nullptr && "Callback already set");
        m_callback = std::move(callback);
    }

    template<typename T>
    void EventHandler<T>::execute(const T& parameters)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_callback)
        {
            m_callback(parameters);
        }
    }

    // Specialization for void payload
    template<>
    class EventHandler<void> : public virtual IEventHandler<void>
    {
        public:
        EventHandler(const EventHandler&)            = delete;
        EventHandler(EventHandler&&)                 = delete;
        EventHandler& operator=(const EventHandler&) = delete;

        EventHandler() : m_signal(std::nullopt)
        {}

        EventHandler(IEventEmitter<void>& signal) : m_signal(signal)
        {
            signal.connect(*this);
        }

        EventHandler(IEventEmitter<void>& signal, const std::function<void()>& callback)
            : m_signal(signal)
            , m_callback(callback)
        {
            signal.connect(*this);
        }

        EventHandler(IEventEmitter<void>& signal, std::function<void()>&& callback)
            : m_signal(signal)
            , m_callback(std::move(callback))
        {
            signal.connect(*this);
        }

        ~EventHandler() override
        {
            if (m_signal)
                m_signal->get().disconnect(*this);
        }

        void setCallback(const std::function<void()>& callback) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            assert(m_callback == nullptr && "Callback already set");
            m_callback = callback;
        }

        void setCallback(std::function<void()>&& callback) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            assert(m_callback == nullptr && "Callback already set");
            m_callback = std::move(callback);
        }

        void connectToSignal(IEventEmitter<void>& signal)
        {
            m_signal = signal;
            signal.connect(*this);
        }

        protected:
        void execute() override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_callback)
            {
                m_callback();
            }
        }

        protected:
        std::optional<std::reference_wrapper<IEventEmitter<void>>> m_signal;
        std::function<void()>                                m_callback;
        std::mutex                                           m_mutex;
    };
    }    // namespace tanlibs::eventdispatch