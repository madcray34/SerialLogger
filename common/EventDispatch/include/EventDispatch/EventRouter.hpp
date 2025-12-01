#pragma once

#include <EventDispatch/IEventEmitter.hpp>
#include <EventDispatch/IEventHandler.hpp>
#include <EventDispatch/IEventRouter.hpp>
#include <unordered_map>
#include <vector>
#include <functional>
#include <mutex>
#include <algorithm>

namespace tanlibs::eventdispatch
{
    /**
     * @class SignalFilter
     * @tparam FilterKeyType The type used for filtering (e.g., event ID).
     * @tparam PayloadType The type of the data emitted with the signal.
     * @brief A signal class that only executes slots connected to a specific filter key.
     */
    // Generic case for non-void payload
    template<typename FilterKeyType, typename PayloadType>
    class EventRouter : public IEventRouter<FilterKeyType, PayloadType>
    {
        public:
        EventRouter()                               = default;
        ~EventRouter() override                     = default;
        EventRouter(const EventRouter&)             = delete;
        EventRouter& operator=(const EventRouter&)  = delete;

        // IEventRouter interface overrides
        void connect(const FilterKeyType& key, IEventHandler<PayloadType>& slot) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_slots[key].push_back(std::ref(slot));
        }

        void disconnect(const FilterKeyType& key, IEventHandler<PayloadType>& slot) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto&                       vec = m_slots[key];
            vec.erase(std::remove_if(vec.begin(), vec.end(), [&](auto& ref) { return &ref.get() == &slot; }), vec.end());
            if (vec.empty())
                m_slots.erase(key);
        }

        void disconnect(const FilterKeyType& key) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_slots.erase(key);
        }

        void emit(const FilterKeyType& key, const PayloadType& payload) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto                        it = m_slots.find(key);
            if (it != m_slots.end())
            {
                for (auto& slot : it->second)
                {
                    slot.get().execute(payload);
                }
            }
        }

        private:
        std::unordered_map<FilterKeyType, std::vector<std::reference_wrapper<IEventHandler<PayloadType>>>> m_slots;
        std::mutex                                                                                 m_mutex;
    };

    // Specialization for void payload
    template<typename FilterKeyType>
    class EventRouter<FilterKeyType, void> : public IEventRouter<FilterKeyType, void>
    {
        public:
        EventRouter()                               = default;
        ~EventRouter() override                     = default;
        EventRouter(const EventRouter&)             = delete;
        EventRouter& operator=(const EventRouter&)  = delete;

        void connect(const FilterKeyType& key, IEventHandler<void>& slot) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_slots[key].push_back(std::ref(slot));
        }

        void disconnect(const FilterKeyType& key, IEventHandler<void>& slot) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto&                       vec = m_slots[key];
            vec.erase(std::remove_if(vec.begin(), vec.end(), [&](auto& ref) { return &ref.get() == &slot; }), vec.end());
            if (vec.empty())
                m_slots.erase(key);
        }

        void disconnect(const FilterKeyType& key) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_slots.erase(key);
        }

        void emit(const FilterKeyType& key) override
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto                        it = m_slots.find(key);
            if (it != m_slots.end())
            {
                for (auto& slot : it->second)
                {
                    slot.get().execute();
                }
            }
        }

        private:
        std::unordered_map<FilterKeyType, std::vector<std::reference_wrapper<IEventHandler<void>>>> m_slots;
        std::mutex                                                                          m_mutex;
    };
    }    // namespace tanlibs::eventdispatch
