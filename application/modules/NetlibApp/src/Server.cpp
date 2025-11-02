#include <NetlibApp/Server/Server.hpp>
#include <thread>

namespace netlib
{
   void CustomServer::startMonitoringQueue()
   {
      std::thread(
          [this]()
          {
             while (!stopMonitoring)
             {
                ServerBase::update(10, true);
             }
          })
          .detach();    // Run in the background
   }

   bool CustomServer::onClientConnect(std::shared_ptr<IConnection> client)
   {
      std::cout << "Connected client [" << client->getPortName() << "]" << std::endl;
      return true;
   }

   void CustomServer::onClientDisconnect(std::shared_ptr<IConnection> client)
   {
      std::cout << "Removing client [" << client->getPortName() << "]" << std::endl;
   }

   void CustomServer::onMessage([[maybe_unused]] netlib::OwnedMessage &&_msg)
   {
      m_model.pushMessage(std::move(_msg));
   }
}    // namespace netlib