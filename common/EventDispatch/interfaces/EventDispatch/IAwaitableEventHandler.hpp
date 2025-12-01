#pragma once

#include <EventDispatch/IEventHandler.hpp>
#include <EventDispatch/ICompletionWaiter.hpp>
#include <EventDispatch/ICompletionNotifier.hpp>

namespace tanlibs::eventdispatch
{
    /**
     * @class ISlotWaiter
     * @tparam T The type of the data that can be passed to the slot.
     * @brief A template interface for a slot waiter in a signal-slot mechanism.
     *
     * A slot is a function or method that can be connected to a signal, and will be called when the signal is emitted.
     */
    template<typename T>
    class IAwaitableEventHandler : public virtual IEventHandler<T>
                                 , public sync::ICompletionWaiter
                                 , protected sync::ICompletionNotifier
    {
        public:
        virtual ~IAwaitableEventHandler() = default;
    };
}    // namespace tanlibs::eventdispatch