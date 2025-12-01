#pragma once

#include <EventDispatch/IEventHandler.hpp>

namespace tanlibs::eventdispatch
{
    /**
     * @class ISignal
     * @tparam T The type of the data that can be emitted with the signal.
     * @brief A template interface for a signal in a signal-slot mechanism.
     *
     * A signal represents a state change or an event. It can be connected to slots,
     * which are functions or methods that will be called when the signal is emitted.
     */
    template<typename T>
    class IEventEmitter
    {
        public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IEventEmitter() = default;

        /**
         * @brief Connect a slot to this signal.
         * @param slot The slot to connect to the signal.
         * When the signal is emitted, the connected slot will be called.
         */
        virtual void connect(IEventHandler<T>& slot) = 0;

        /**
         * @brief Disconnect a specific slot from this signal.
         * @param slot The slot to disconnect from the signal.
         */
        virtual void disconnect(IEventHandler<T>& slot) = 0;

        /**
         * @brief Disconnect all slot from this signal.
         */
        virtual void disconnect() = 0;

        /**
         * @brief Emit the signal.
         * @param value The data to emit with the signal.
         * All connected slot will be called with this data.
         */
        virtual void emit(const T& value) = 0;
    };

    // Specialization for void payload
    template<>
    class IEventEmitter<void>
    {
        public:
        IEventEmitter() = default;

        virtual ~IEventEmitter() = default;

        /**
         * @brief Connect a slot to this signal.
         * @param slot The slot to connect to the signal.
         * When the signal is emitted, the connected slot will be called.
         */
        virtual void connect(IEventHandler<void>& slot) = 0;

        /**
         * @brief Disconnect a specific slot from this signal.
         * @param slot The slot to disconnect from the signal.
         */
        virtual void disconnect(IEventHandler<void>& slot) = 0;

        /**
         * @brief Disconnect all slot from this signal.
         */
        virtual void disconnect() = 0;

        /**
         * @brief Emit the signal.
         * All connected slot will be called.
         */
        virtual void emit() = 0;
    };
}    // namespace tanlibs::eventdispatch