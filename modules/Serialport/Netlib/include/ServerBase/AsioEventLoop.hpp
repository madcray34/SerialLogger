#pragma once
#include <ServerBase/IEventLoop.hpp>
#include <boost/asio/io_context.hpp>

namespace netlib
{
   class AsioEventLoop final : public IEventLoop
   {
      public:
      AsioEventLoop() : m_asioContext()
      {}

      void start() override
      {
         m_asioContext.run();
      }

      void stop() override
      {
         m_asioContext.stop();
      }

      boost::asio::io_context &getContext()
      {
         return m_asioContext;
      }

      private:
      boost::asio::io_context m_asioContext;
   };
}    // namespace netlib