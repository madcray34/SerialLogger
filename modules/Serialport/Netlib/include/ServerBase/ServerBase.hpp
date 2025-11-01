#pragma once
#include <serverBase/IserverBase.hpp>
#include <tsQueue/types/message.hpp>
#include <tsQueue/ItsQueue.hpp>
#include <connection/Iconnection.hpp>
#include <connection/IconnectionFactory.hpp>
#include <portScanner/IendPointEnumerator.hpp>

#define ASIO_STANDALONE
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <deque>
#include <chrono>


namespace netlib
{
   class ServerBase : public IServerBase
   {
      public:
      ServerBase(ITSQueue<OwnedMessage> &msgIn, IEndPointEnumerator &endpoints,
                 IConnectionFactory &connFactory, std::chrono::seconds periodicity);
      ~ServerBase() override;

      bool start() override;
      void stop() override;
      void waitForClientConnection() override;
      void update(int32_t nMaxMessages = -1, bool _wait = true) override;

      protected:
      bool onClientConnect(std::shared_ptr<IConnection> client) override;
      void onClientDisconnect(std::shared_ptr<IConnection> client) override;
      void onMessage([[maybe_unused]] netlib::OwnedMessage &&_msg) override;

      private:
      // Thread Safe Queue for incoming message packets
      ITSQueue<OwnedMessage> &m_qMsgIn;
      IEndPointEnumerator    &m_endpoints;
      IConnectionFactory     &m_connFactory;

      // Container of active validated connections
      std::deque<std::shared_ptr<IConnection>> m_deqConnections;

      // Asio related containers
      boost::asio::io_context   m_asioContext;
      std::thread               m_threadContext;
      boost::asio::steady_timer m_asyncTimer;

      std::chrono::seconds m_periodicity;

      // Clients will be identified in the "wider system" via an ID
      uint32_t nIDCounter = 21000;
   };
}    // namespace netlib