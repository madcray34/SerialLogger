#pragma once
#include <NetlibCore/Event/ITimer.hpp>
#include <NetlibApp/Event/Asio/AsioEventLoop.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>

namespace netlib
{
   class AsioTimer final : public core::ITimer
   {
      public:
      AsioTimer(boost::asio::io_context &context, std::chrono::seconds initial)
          : m_timer(context, initial)
      {}

      void expires_after(const std::chrono::seconds &duration) override
      {
         m_timer.expires_after(duration);
      }

      void async_wait(std::function<void(const std::error_code &)> handler) override
      {
         m_timer.async_wait(std::move(handler));
      }

      private:
      boost::asio::steady_timer m_timer;
   };

   class AsioTimerFactory final : public core::ITimerFactory
   {
      public:
      AsioTimerFactory(AsioEventLoop &eventLoop) : m_eventLoop(eventLoop)
      {}

      std::shared_ptr<core::ITimer> createTimer(std::chrono::seconds initial) override
      {
         return std::make_shared<AsioTimer>(m_eventLoop.getContext(), initial);
      }

      private:
      AsioEventLoop &m_eventLoop;
   };
}    // namespace netlib
