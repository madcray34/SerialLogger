#pragma once
#include <EventDispatch/IEventHandler.hpp>
#include <vector>

namespace tanlibs::eventdispatch
{
    /**
     * @class ISignalFilter
     * @tparam FilterKeyType The type used for filtering (e.g., event ID).
     * @tparam PayloadType The type of the data emitted with the signal.
     * @brief Interface for a signal that supports connecting/disconnecting slots by filter key.
     */
    // Generic case for non-void payload
    template<typename FilterKeyType, typename PayloadType>
    class IEventRouter
    {
        public:
        virtual ~IEventRouter() = default;

        /**
         * @brief Connect a slot to a specific filter key.
         * @param key The filter key to connect to.
         * @param slot The slot to connect.
         */
        virtual void connect(const FilterKeyType& key, IEventHandler<PayloadType>& slot) = 0;

        /**
         * @brief Disconnect a slot from a specific filter key.
         * @param key The filter key.
         * @param slot The slot to disconnect.
         */
        virtual void disconnect(const FilterKeyType& key, IEventHandler<PayloadType>& slot) = 0;

        /**
         * @brief Disconnect all slots from a specific filter key.
         * @param key The filter key.
         */
        virtual void disconnect(const FilterKeyType& key) = 0;

        /**
         * @brief Emit the signal for a specific filter key and payload.
         * @param key The filter key to emit for.
         * @param payload The data to emit with the signal.
         */
        virtual void emit(const FilterKeyType& key, const PayloadType& payload) = 0;
    };

    // Specialization for void payload
    template<typename FilterKeyType>
    class IEventRouter<FilterKeyType, void>
    {
        public:
        virtual ~IEventRouter() = default;

        /**
         * @brief Connect a slot to a specific filter key.
         * @param key The filter key to connect to.
         * @param slot The slot to connect.
         */
        virtual void connect(const FilterKeyType& key, IEventHandler<void>& slot) = 0;

        /**
         * @brief Disconnect a slot from a specific filter key.
         * @param key The filter key.
         * @param slot The slot to disconnect.
         */
        virtual void disconnect(const FilterKeyType& key, IEventHandler<void>& slot) = 0;

        /**
         * @brief Disconnect all slots from a specific filter key.
         * @param key The filter key.
         */
        virtual void disconnect(const FilterKeyType& key) = 0;

        /**
         * @brief Emit the signal for a specific filter key.
         * @param key The filter key to emit for.
         */
        virtual void emit(const FilterKeyType& key) = 0;
    };
}    // namespace tanlibs::eventdispatch
