#include <application/AppConnectionSupervisor.hpp>
#include <thread>

namespace netlib
{
   void AppConnectionSupervisor::startMessagePump()
   {
      std::thread(
          [this]()
          {
             while (!stopMonitoring)
             {
                core::ConnectionSupervisor::update(10, true);
             }
          })
          .detach();    // Run in the background
   }

   bool AppConnectionSupervisor::onClientConnect(std::shared_ptr<core::IConnection> client)
   {
      std::cout << "Connected client [" << client->getPortName() << "]" << std::endl;
      return true;
   }

   void AppConnectionSupervisor::onClientDisconnect(std::shared_ptr<core::IConnection> client)
   {
      std::cout << "Removing client [" << client->getPortName() << "]" << std::endl;
   }

   void AppConnectionSupervisor::onMessage(netlib::core::OwnedMessage &&_msg)
   {
      std::cout << _msg.m_msg;
      if (!_msg.m_msg.empty() && _msg.m_msg.back() != '\n')
      {
         std::cout << '\n';
      }
   }
}    // namespace netlib