#pragma once
#include <ServerBase/ServerBase.hpp>

namespace netlib
{
   class CustomServer : public ServerBase
   {
      public:
      CustomServer(ITSQueue<owned_message>& msgIn, COMPortScanner& portScanner,
                   std::chrono::seconds periodicity)
          : ServerBase(msgIn, portScanner, periodicity)
      {}

      ~CustomServer() = default;

      protected:
      bool onClientConnect(std::shared_ptr<Connection> client) override;

      /**
       * @brief Called when a client appears to have disconnected
       *
       * @param client
       */
      void onClientDisconnect(std::shared_ptr<Connection> client) override;


      /**
       * @brief Called when a message arrives
       *
       * @param client
       * @param msg
       */
      void onMessage(std::shared_ptr<Connection> client, [[maybe_unused]] Message& _msg) override;
   };
}    // namespace netlib