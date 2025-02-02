#pragma once
#include <memory>
#include <connection/Iconnection.hpp>
#include <tsQueue/types/message.hpp>

#ifdef _WIN32
   #define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio.hpp>

namespace netlib
{
   template<typename T>
   class ITSQueue;

   class Connection : public std::enable_shared_from_this<Connection>, public IConnection
   {
      public:
      /**
       * @brief Construct a new Connection objectConstructor: Connect to asio::io_context, transfer
       * the serial_port and Provide reference to incoming message queue
       * @param asioContext
       * @param port
       * @param qIn
       */
      Connection(boost::asio::io_context& asioContext, boost::asio::serial_port port,
                 std::string name, ITSQueue<OwnedMessage>& qIn);

      virtual ~Connection()
      {}

      uint32_t getID() const override;

      void connectToClient(uint32_t uid = 0) override;

      void disconnect() override;

      bool isConnected() const override;

      std::string_view getPortName() const override;

      private:
      /**
       * @brief ASYNC - Prime context ready to read a message body. If this function is called, a
       * header has already been read, and that header request we read a body, The space for that
       * body has already been allocated in the temporary message object, so just wait for the bytes
       * to arrive...
       */
      void ReadBody();

      /**
       * @brief Once a full message is received, add it to the incoming queue, converting it to an
       * "owned message", by initialising with a shared pointer from this connection object.
       */
      void AddToIncomingMessageQueue(const std::size_t& len);

      void configureSerialPort(boost::asio::serial_port& port);

      protected:
      // This context is shared with the whole asio instance
      boost::asio::io_context& m_asioContext;

      // Each connection has a unique serial port
      boost::asio::serial_port m_port;

      // Reference to the port name
      std::string m_portName;

      // This references the incoming queue of the parent object
      ITSQueue<OwnedMessage>& m_qMessagesIn;

      // Incoming messages are constructed asynchronously, so we will
      // store the part assembled message here, until it is ready
      // std::string m_msgTemporaryIn{};
      boost::asio::streambuf m_streamBuffer;

      bool m_bConnectionEstablished = false;

      uint32_t id = 0;
   };
}    // namespace netlib