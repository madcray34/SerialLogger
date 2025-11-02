#pragma once
#include <NetlibCore/Queue/types/Message.hpp>
#include <NetlibCore/Connection/IConnection.hpp>

#include <string>
#include <memory>

namespace netlib
{
   class IServerBase
   {
      public:
      virtual ~IServerBase() = default;

      /**
       * @brief Starts the server.
       * @return true
       * @return false
       */
      virtual bool start() = 0;

      /**
       * @brief Stop the server.
       */
      virtual void stop() = 0;

      /**
       * @brief ASYNC - Instruct asio to wait for connection.
       */
      virtual void waitForClientConnection() = 0;

      /**
       * @brief Force server to process incoming messages.
       *
       * @param nMaxMessages
       * @param wait
       */
      virtual void update(int32_t nMaxMessages = -1, bool wait = true) = 0;

      protected:
      /**
       * @brief Called when a client connects, you can veto the connection by returning false.
       * @param client
       * @return true
       * @return false
       */
      virtual bool onClientConnect(std::shared_ptr<IConnection> client) = 0;

      /**
       * @brief Called when a client appears to have disconnected.
       * @param client
       */
      virtual void onClientDisconnect(std::shared_ptr<IConnection> client) = 0;

      /**
       * @brief Called when a message arrives.
       *
       * @param client
       * @param msg
       */
      virtual void onMessage([[maybe_unused]] netlib::OwnedMessage &&_msg) = 0;
   };
}    // namespace netlib