#pragma once
#include <NetlibCore/Connection/IconnectionFactory.hpp>
#include <NetlibApp/Event/Asio/AsioEventLoop.hpp>

namespace netlib
{
   class AsioSerialConnectionFactory final : public IConnectionFactory
   {
      public:
      explicit AsioSerialConnectionFactory(AsioEventLoop &ev) : m_eventLoop(ev) {};
      ~AsioSerialConnectionFactory() override = default;
      std::shared_ptr<IConnection> create(std::string_view        portName,
                                          ITSQueue<OwnedMessage> &qIn) override;

      private:
      AsioEventLoop &m_eventLoop;
   };
}    // namespace netlib