#include <Connection/Connection.hpp>
#include <TSQueue/ITSQueue.hpp>

namespace netlib
{
   Connection::Connection(boost::asio::io_context& asioContext, boost::asio::serial_port port,
                          std::string name, ITSQueue<OwnedMessage>& qIn)
       : m_asioContext(asioContext), m_port(std::move(port)), m_portName(name), m_qMessagesIn(qIn)
   {
      configureSerialPort(m_port);
   }

   uint32_t Connection::getID() const
   {
      return id;
   }


   void Connection::connectToClient(uint32_t uid)
   {
      if (isConnected())
      {
         id = uid;

         // A client has attempted to connect to the server, we want to read the content.
         ReadBody();
      }
   }

   void Connection::disconnect()
   {
      if (isConnected())
         boost::asio::post(m_asioContext, [this]() { m_port.close(); });
   }

   bool Connection::isConnected() const
   {
      return m_port.is_open();
   }


   std::string_view Connection::getPortName() const
   {
      return m_portName;
   }

   void Connection::ReadBody()
   {
      boost::asio::async_read_until(m_port, m_streamBuffer, '\n',
                                    [this](std::error_code ec, [[maybe_unused]] std::size_t length)
                                    {
                                       if (!ec && length > 0)
                                       {
                                          AddToIncomingMessageQueue(length);
                                       }
                                       else if (ec)
                                       {
                                          // TODO for the future improve error handling
                                          std::cout << "[" << id
                                                    << "] Read Body Fail: " << ec.message() << "\n";
                                          m_port.close();
                                       }
                                    });
   }

   void Connection::AddToIncomingMessageQueue([[maybe_unused]] const std::size_t& len)
   {
      auto _ptr = shared_from_this();
      if (_ptr)    // avoid dangling pointers
      {
         OwnedMessage message{ std::move(_ptr) };
         std::istream is(&m_streamBuffer);
         std::getline(is, message.m_msg);
         m_streamBuffer.consume(len);
         m_qMessagesIn.push_back(std::move(message));
      }
      ReadBody();
   }

   void Connection::configureSerialPort(boost::asio::serial_port& port)
   {
      // Apply serial port configuration options
      port.set_option(boost::asio::serial_port_base::baud_rate(9600));
      port.set_option(boost::asio::serial_port_base::character_size(8));
      port.set_option(boost::asio::serial_port_base::flow_control(
          boost::asio::serial_port_base::flow_control::none));
      port.set_option(
          boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
      port.set_option(
          boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
   }
}    // namespace netlib
