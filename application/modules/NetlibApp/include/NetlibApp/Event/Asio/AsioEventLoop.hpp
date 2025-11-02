#pragma once
#include <NetlibCore/Event/IEventLoop.hpp>
#include <boost/asio/io_context.hpp>

namespace netlib
{
   class AsioEventLoop final : public core::IEventLoop
   {
      public:
      AsioEventLoop(boost::asio::io_context &asioContext) : m_asioContext(asioContext)
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
      boost::asio::io_context &m_asioContext;
   };
}    // namespace netlib