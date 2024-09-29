#include <Server/Server.hpp>
#include <Connection/Connection.hpp>

namespace netlib
{
   bool CustomServer::onClientConnect(std::shared_ptr<Connection> client)
   {
      return true;
   }

   void CustomServer::onClientDisconnect(std::shared_ptr<Connection> client)
   {
      std::cout << "Removing client [" << client->getID() << "]\n";
   }

   void CustomServer::onMessage(std::shared_ptr<Connection> client, [[maybe_unused]] Message & _msg)
   {}
}    // namespace netlib