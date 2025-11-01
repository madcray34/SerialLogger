#pragma once
#include <Connection/IconnectionFactory.hpp>

namespace netlib
{
   class AsioSerialConnectionFactory final : public IConnectionFactory
   {
      public:
      AsioSerialConnectionFactory()           = default;
      ~AsioSerialConnectionFactory() override = default;
      std::shared_ptr<IConnection> create(boost::asio::io_context &asioContext,
                                          std::string_view         portName,
                                          ITSQueue<OwnedMessage>  &qIn) override;
   };
}    // namespace netlib