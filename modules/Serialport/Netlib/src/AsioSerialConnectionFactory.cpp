#include <Connection/AsioSerialConnectionFactory.hpp>
#include <Connection/Connection.hpp>
#include <boost/asio/serial_port.hpp>

namespace netlib
{
   std::shared_ptr<IConnection> AsioSerialConnectionFactory::create(
       boost::asio::io_context &asioContext, std::string_view portname, ITSQueue<OwnedMessage> &qIn)
   {
      boost::asio::serial_port newPort(asioContext, std::string(portname));
      return std::make_shared<Connection>(asioContext, std::move(newPort), std::string(portname),
                                          qIn);
   }
}    // namespace netlib