#pragma once

#include <EventDispatch/IEventEmitter.hpp>
#include <EventDispatch/IEventHandler.hpp>

#include <vector>
#include <functional>
#include <std/mutex>
#include <atomic>

namespace tanlibs::eventdispatch
{
    /**
     * @class EventEmitter
     * @tparam T The type of the data that can be emitted with the signal.
     * @brief Thread-safe publisher in the event-dispatch mechanism.
     *
     * A signal represents a state change or an event. It can be connected to slots,
     * which are functions or methods that will be called when the signal is emitted.
     */
    template<typename T>
    class EventEmitter : public virtual IEventEmitter<T>
    {
        public:
        EventEmitter(const EventEmitter&)            = delete;
        EventEmitter(EventEmitter&&)                 = delete;
        EventEmitter& operator=(const EventEmitter&) = delete;

        EventEmitter()           = default;
        ~EventEmitter() override = default;

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

        private:
        std::vector<std::reference_wrapper<IEventHandler<T>>> m_slots;
        std::mutex                                    m_mutex;
    };

    template<typename T>
    void EventEmitter<T>::connect(IEventHandler<T>& slot)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_slots.push_back(slot);
    }

    template<typename T>
    void EventEmitter<T>::disconnect(IEventHandler<T>& slot)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto it = m_slots.begin(); it != m_slots.end();)
        {
            if (&(*it).get() == &slot)
            {
                it = m_slots.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    template<typename T>
    void EventEmitter<T>::disconnect()
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_slots.clear();
    }

    template<typename T>
    void EventEmitter<T>::emit(const T& parameters)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        for (auto& slot : m_slots)
        {
            slot.get().execute(parameters);
        }
    }
}    // namespace tanlibs::eventdispatch