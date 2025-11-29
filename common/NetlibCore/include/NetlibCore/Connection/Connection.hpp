#pragma once
#include <NetlibCore/Connection/IConnection.hpp>
#include <NetlibCore/Queue/ITSQueue.hpp>
#include <NetlibCore/Queue/types/Message.hpp>
#include <NetlibCore/Transport/ITextStream.hpp>

#include <memory>

namespace netlib::core
{
   class Connection final : public IConnection, public std::enable_shared_from_this<Connection>
   {
      public:
      /**
       * @brief Construct a new Connection object
       */
      Connection(std::shared_ptr<ITextStream> textStream, std::string name,
                 ITSQueue<OwnedMessage> &qIn);

      ~Connection() override = default;


      uint32_t getID() const noexcept override;

      void connectToClient(uint32_t uid = 0) override;

      void disconnect() override;

      bool isConnected() const override;

      std::string_view getPortName() const noexcept override;

      private:
      /**
       * @brief ASYNC - Prime context ready to read a message body. If this function is called, a
       * header has already been read, and that header request we read a body, The space for that
       * body has already been allocated in the temporary message object, so just wait for the bytes
       * to arrive...
       */
      void _readBody();

      /**
       * @brief Once a full message is received, add it to the incoming queue, converting it to an
       * "owned message", by initialising with a shared pointer from this connection object.
       */
      void AddToIncomingMessageQueue(const std::size_t &len);

      protected:
      const std::shared_ptr<ITextStream> m_textStream;

      // Reference to the port name
      std::string m_portName;

      // This references the incoming queue of the parent object
      ITSQueue<OwnedMessage> &m_qMessagesIn;

      bool m_bConnectionEstablished = false;

      uint32_t id = 0;
   };
}    // namespace netlib