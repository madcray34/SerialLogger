#pragma once
#include <NetlibCore/Connection/IConnection.hpp>
#include <NetlibCore/Queue/ITSQueue.hpp>
#include <NetlibCore/Queue/types/Message.hpp>
#include <NetlibCore/Transport/ITextStream.hpp>

#include <memory>

namespace netlib::core
{
   class Connection final : public IConnection, private std::enable_shared_from_this<Connection>
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

      std::string_view getConnectionName() const noexcept override;

      private:
      /**
       * @brief Starts the asynchronous receive loop, forwarding each received message to the
       * inbound queue.
       */
      void _readBody();

      protected:
      const std::shared_ptr<ITextStream> m_textStream;

      // Reference to the port name
      std::string m_portName;

      // This references the incoming queue of the parent object
      ITSQueue<OwnedMessage> &m_qMessagesIn;

      bool m_bConnectionEstablished = false;

      uint32_t id = 0;
   };
}    // namespace netlib::core