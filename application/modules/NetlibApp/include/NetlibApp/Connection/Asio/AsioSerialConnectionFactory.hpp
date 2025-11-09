#pragma once
#include <NetlibCore/Connection/IConnectionFactory.hpp>
#include <NetlibApp/Event/Asio/AsioEventLoop.hpp>
#include <NetlibApp/Transport/Asio/SerialOptions.hpp>

namespace netlib
{
   class AsioSerialConnectionFactory final : public core::IConnectionFactory
   {
      public:
      explicit AsioSerialConnectionFactory(AsioEventLoop &ev) : m_eventLoop(ev) {};
      ~AsioSerialConnectionFactory() override = default;
      std::shared_ptr<core::IConnection> create(std::string_view                    portName,
                                                core::ITSQueue<core::OwnedMessage> &qIn) override;

      private:
      AsioEventLoop &m_eventLoop;
   };
}    // namespace netlib