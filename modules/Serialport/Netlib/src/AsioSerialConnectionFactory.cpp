#include <Connection/AsioSerialConnectionFactory.hpp>
#include <Connection/Connection.hpp>
#include <boost/asio/serial_port.hpp>

namespace netlib
{
   std::shared_ptr<IConnection> AsioSerialConnectionFactory::create(std::string_view portname,
                                                                    ITSQueue<OwnedMessage> &qIn)
   {
      boost::asio::serial_port newPort(m_eventLoop.getContext(), std::string(portname));
      return std::make_shared<Connection>(m_eventLoop.getContext(), std::move(newPort),
                                          std::string(portname), qIn);
   }
}    // namespace netlib