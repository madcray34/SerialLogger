#pragma once
#include <NetlibCore/Connection/IConnectionSupervisor.hpp>
#include <NetlibCore/Queue/ITSQueue.hpp>
#include <NetlibCore/Queue/types/Message.hpp>
#include <NetlibCore/Connection/IConnection.hpp>
#include <NetlibCore/Connection/IConnectionFactory.hpp>
#include <NetlibCore/EndPointEnumerator/IEndPointEnumerator.hpp>
#include <NetlibCore/Event/IEventLoop.hpp>
#include <NetlibCore/Event/ITimer.hpp>

#include <thread>
#include <deque>
#include <chrono>


namespace netlib::core
{
   class ConnectionSupervisor : public IConnectionSupervisor
   {
      public:
      ConnectionSupervisor(ITSQueue<OwnedMessage> &msgIn, IEndPointEnumerator &endpoints,
                           IConnectionFactory &connFactory, IEventLoop &eventLoop,
                           ITimerFactory &timer, std::chrono::seconds periodicity);
      ~ConnectionSupervisor() override;

      bool start() override;
      void stop() override;
      void waitForClientConnection() override;
      void update(int32_t nMaxMessages = -1, bool _wait = true) override;

      protected:
      bool onClientConnect(std::shared_ptr<IConnection> client) override;
      void onClientDisconnect(std::shared_ptr<IConnection> client) override;
      void onMessage([[maybe_unused]] OwnedMessage &&_msg) override;

      private:
      // Thread Safe Queue for incoming message packets
      ITSQueue<OwnedMessage>       &m_qMsgIn;
      IEndPointEnumerator          &m_endpoints;
      IConnectionFactory           &m_connFactory;
      IEventLoop                   &m_eventLoop;
      ITimerFactory                &m_asyncTimerFactory;
      const std::shared_ptr<ITimer> m_asyncTimer;
      std::chrono::seconds          m_periodicity;
      std::thread                   m_threadContext;

      // Container of active validated connections
      std::deque<std::shared_ptr<IConnection>> m_deqConnections;

      // Clients will be identified in the "wider system" via an ID
      uint32_t nIDCounter = 21000;    // Start at an arbitrary value
   };
}    // namespace netlib::core