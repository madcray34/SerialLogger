#include <Connection/Connection.hpp>
#include <TSQueue/ITSQueue.hpp>

namespace netlib
{
   Connection::Connection(boost::asio::io_context& asioContext, boost::asio::serial_port port,
                          std::string name, ITSQueue<owned_message>& qIn)
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


   void Connection::exampleMethod()
   {
      // Check if shared_from_this is valid
      if (auto self = shared_from_this())
      {
         std::cout << "Shared pointer to Connection is valid." << std::endl;
         std::cout << "Use count: " << self.use_count() << std::endl;    // Should be > 1 if
                                                                         // valid
      }
      else
      {
         std::cout << "Failed to get shared pointer." << std::endl;
      }
   }

   void Connection::ReadBody()
   {
      // TODO Trick for the future to not have to wait forever if data is not coming close the port
      //  m_timer.expires_after(std::chrono::minutes(10));    // Set a timeout of 10 seconds
      //  m_timer.async_wait(
      //      [this](const std::error_code& ec)
      //      {
      //         if (!ec)
      //         {
      //            std::cout << "[" << id << "] Read Timeout: No data received within 10
      //            seconds.\n"; m_port.close();    // Close the connection if no data is received
      //            within the timeout
      //         }
      //      });
      boost::asio::async_read_until(
          m_port, boost::asio::dynamic_buffer(m_msgTemporaryIn.body), '\n',
          [this](std::error_code ec, [[maybe_unused]] std::size_t length)
          {
             if (!ec)
             {
                // std::cout << "[" << id << "] Read " << length << " bytes successfully.\n";
                //  Add to incoming message queue
                AddToIncomingMessageQueue();
             }
             else
             {
                std::cout << "[" << id << "] Read Body Fail: " << ec.message() << "\n";
                m_port.close();
             }
          });
   }

   void Connection::AddToIncomingMessageQueue()
   {
      std::shared_ptr<Connection> _ptr = this->shared_from_this();
      if (_ptr)    // avoid dangling pointers
         m_qMessagesIn.push_back({ _ptr, m_msgTemporaryIn });

      m_msgTemporaryIn.body.clear();    // or reset if needed
      // We must now prime the asio context to receive the next message. It
      // wil just sit and wait for bytes to arrive, and the message construction
      // process repeats itself.
      ReadBody();
   }

   void Connection::configureSerialPort(boost::asio::serial_port& port)
   {
      // Set baud rate
      port.set_option(boost::asio::serial_port_base::baud_rate(9600));

      // Set character size (default is usually 8 bits)
      port.set_option(boost::asio::serial_port_base::character_size(8));

      // Set flow control (none by default)
      port.set_option(boost::asio::serial_port_base::flow_control(
          boost::asio::serial_port_base::flow_control::none));

      // Set parity (none by default)
      port.set_option(
          boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));

      // Set stop bits (1 stop bit by default)
      port.set_option(
          boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
   }
}    // namespace netlib
