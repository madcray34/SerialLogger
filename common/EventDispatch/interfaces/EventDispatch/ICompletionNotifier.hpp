#pragma once

namespace tanlibs::eventdispatch::sync
{
    /**
     * @brief Interface for notifying waiters about event completion.
     */
    class ICompletionNotifier
    {
        public:
        virtual ~ICompletionNotifier() = default;
        virtual void notify() = 0;
    };

}    // namespace tanlibs::eventdispatch::sync
