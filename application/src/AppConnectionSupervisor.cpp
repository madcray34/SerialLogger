#include <application/AppConnectionSupervisor.hpp>
#include <thread>

namespace netlib
{
   AppConnectionSupervisor::AppConnectionSupervisor(
       core::ITSQueue<core::OwnedMessage> &msgIn, core::IEndPointEnumerator &endpoints,
       core::IConnectionFactory &connFactory, core::IEventLoop &eventLoop,
       core::ITimerFactory &timer, std::chrono::seconds periodicity, IModel &_model)
       : core::ConnectionSupervisor(msgIn, endpoints, connFactory, eventLoop, timer, periodicity)
       , m_model(_model)
   {}

   AppConnectionSupervisor::~AppConnectionSupervisor()
   {
      stopMessagePump();
   }

   void AppConnectionSupervisor::startMessagePump()
   {
      m_monitor = std::jthread(
          [this](std::stop_token st)
          {
             while (!st.stop_requested())
             {
                core::ConnectionSupervisor::update(10, true);
             }
          });    // Run in the background
   }


   void AppConnectionSupervisor::stopMessagePump()
   {
      if (m_monitor.joinable())
      {
         m_monitor.request_stop();
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