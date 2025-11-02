#pragma once

namespace netlib::core
{
   class IEventLoop
   {
      public:
      virtual ~IEventLoop() = default;
      virtual void start()  = 0;
      virtual void stop()   = 0;
   };
}    // namespace netlib