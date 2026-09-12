#include <application/AppConnectionSupervisor.hpp>

namespace netlib
{
   void AppConnectionSupervisor::startMessagePump()
   {
      if (m_monitor.joinable())
      {
         return;
      }

      m_monitor = std::jthread(
          [this](std::stop_token stopToken)
          {
             while (!stopToken.stop_requested())
             {
                core::ConnectionSupervisor::update(10, true);
             }
          });
   }

   void AppConnectionSupervisor::stopMessagePump()
   {
      if (m_monitor.joinable())
      {
         m_monitor.request_stop();
         m_monitor.join();
      }
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

   void AppConnectionSupervisor::onMessage([[maybe_unused]] netlib::core::OwnedMessage &&_msg)
   {
      m_model.pushMessage(std::move(_msg));
   }
}    // namespace netlib