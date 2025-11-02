#pragma once
#include <NetlibCore/Connection/ConnectionSupervisor.hpp>
#include <model/Imodel.hpp>
#include <atomic>

namespace netlib
{
   class CustomServer : public core::ConnectionSupervisor
   {
      public:
      CustomServer(core::ITSQueue<core::OwnedMessage> &msgIn, core::IEndPointEnumerator &endpoints,
                   core::IConnectionFactory &connFactory, core::IEventLoop &eventLoop,
                   core::ITimerFactory &timer, std::chrono::seconds periodicity, IModel &_model)
          : core::ConnectionSupervisor(msgIn, endpoints, connFactory, eventLoop, timer, periodicity)
          , stopMonitoring(false)
          , m_model(_model)
      {}

      ~CustomServer()
      {
         stopMonitoringQueue();
      }

      void startMonitoringQueue();

      void stopMonitoringQueue()
      {
         stopMonitoring = true;    // Signal the monitoring thread to stop
      }

      protected:
      bool onClientConnect(std::shared_ptr<core::IConnection> client) override;

      /**
       * @brief Called when a client appears to have disconnected
       *
       * @param client
       */
      void onClientDisconnect(std::shared_ptr<core::IConnection> client) override;


      /**
       * @brief Called when a message arrives
       *
       * @param client
       * @param msg
       */
      void onMessage([[maybe_unused]] netlib::core::OwnedMessage &&_msg) override;

      private:
      std::atomic<bool> stopMonitoring;
      IModel           &m_model;
   };
}    // namespace netlib