#pragma once
#include <chrono>
#include <functional>
#include <system_error>

namespace netlib::core
{
   class ITimer
   {
      public:
      virtual ~ITimer() = default;

      virtual void expires_after(const std::chrono::seconds &duration) = 0;

      virtual void async_wait(std::function<void(const std::error_code &)> handler) = 0;
   };

   class ITimerFactory
   {
      public:
      virtual ~ITimerFactory() = default;

      virtual std::shared_ptr<ITimer> createTimer(std::chrono::seconds initial) = 0;
   };
}    // namespace netlib