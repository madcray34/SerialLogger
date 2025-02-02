#pragma once
#include <model/Imodel.hpp>
#include <serverBase/serverBase.hpp>
#include <atomic>

namespace netlib
{
   class CustomServer : public ServerBase
   {
      public:
      CustomServer(ITSQueue<OwnedMessage>& msgIn, COMPortScanner& portScanner,
                   std::chrono::seconds periodicity, IModel& _model)
          : ServerBase(msgIn, portScanner, periodicity), stopMonitoring(false), m_model(_model)
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
      void onMessage([[maybe_unused]] netlib::OwnedMessage&& _msg) override;

      private:
      std::atomic<bool> stopMonitoring;
      IModel&           m_model;
   };
}    // namespace netlib