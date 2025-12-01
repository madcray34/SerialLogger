#pragma once

#include <EventDispatch/IEventEmitter.hpp>
#include <EventDispatch/ICompletionWaiter.hpp>
#include <EventDispatch/ICompletionNotifier.hpp>

namespace tanlibs::eventdispatch
{
    /**
     * @class ISignalWaiter
     * @tparam T The type of the data that can be emitted with the signal.
     * @brief A template interface for a signal in a signal-slot mechanism that can be waited on.
     *
     * A signal represents a state change or an event. It can be connected to slots,
     * which are functions or methods that will be called when the signal is emitted.
     */
    template<typename T>
    class IAwaitableEventEmitter : public virtual IEventEmitter<T>
                                 , public sync::ICompletionWaiter
                                 , protected sync::ICompletionNotifier
    {
        public:
        /**
         * @brief Virtual destructor.
         */
        virtual ~IAwaitableEventEmitter() = default;
    };
    }    // namespace tanlibs::eventdispatch