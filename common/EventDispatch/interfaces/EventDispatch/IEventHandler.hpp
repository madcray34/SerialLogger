#pragma once
#include <functional>

namespace tanlibs::eventdispatch
{
    /**
     * @class ISlot
     * @tparam T The type of the data that can be passed to the slot.
     * @brief A template interface for a slot in a signal-slot mechanism.
     *
     * A slot is a function or method that can be connected to a signal, and will be called when the signal is emitted.
     */
    template<typename T>
    class IEventHandler
    {
        public:
        virtual ~IEventHandler() = default;

        /**
         * @brief Set the function to be called when the slot is triggered.
         * @param callback The function to set as the slot function.
         */
        virtual void setCallback(const std::function<void(T)>& callback) = 0;

        /**
         * @brief Set the function to be called when the slot is triggered.
         * @param callback The function to set as the slot function.
         */
        virtual void setCallback(std::function<void(T)>&& callback) = 0;

        protected:
        /**
         * @brief Call operator for the slot.
         * @param parameters The data to pass to the slot.
         */
        virtual void execute(const T& parameters) = 0;

        template<typename U>
        friend class EventEmitter;
        template<typename U>
        friend class AwaitableEventEmitter;
        template<typename U, typename V>
        friend class EventRouter;
    };

    /**
     * @class ISlot<void>
     * @brief Specialization of ISlot for void payloads.
     *
     * This specialization is used when the slot does not require any payload data.
     */
    template<>
    class IEventHandler<void>
    {
        public:
        /**
         * @brief Default constructor.
         */
        IEventHandler() = default;
        /**
         * @brief Virtual destructor.
         */
        virtual ~IEventHandler() = default;

        /**
         * @brief Set the function to be called when the slot is triggered.
         * @param callback The function to set as the slot function.
         */
        virtual void setCallback(const std::function<void()>& callback) = 0;

        /**
         * @brief Set the function to be called when the slot is triggered.
         * @param callback The function to set as the slot function.
         */
        virtual void setCallback(std::function<void()>&& callback) = 0;

        protected:
        /**
         * @brief Call operator for the slot.
         */
        virtual void execute() = 0;

        template<typename U>
        friend class EventEmitter;
        template<typename U>
        friend class AwaitableEventEmitter;
        template<typename U, typename V>
        friend class EventRouter;
    };
    }    // namespace tanlibs::eventdispatch