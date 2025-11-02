#include <NetlibApp/Connection/Asio/AsioSerialConnectionFactory.hpp>
#include <NetlibApp/Transport/Asio/AsioSerialTextStream.hpp>
#include <NetlibCore/Connection/Connection.hpp>


namespace netlib
{
   std::shared_ptr<core::IConnection> AsioSerialConnectionFactory::create(
       std::string_view portname, core::ITSQueue<core::OwnedMessage> &qIn)
   {
      auto textStream = std::make_shared<AsioSerialTextStream>(m_eventLoop, std::string(portname));
      return std::make_shared<core::Connection>(std::move(textStream), std::string(portname), qIn);
   }
}    // namespace netlib