#pragma once
#include <Connection/IconnectionFactory.hpp>
#include <ServerBase/AsioEventLoop.hpp>

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