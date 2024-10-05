#pragma once
#include <ServerBase/IServerBase.hpp>
#include <TSQueue/Message.hpp>
#include <TSQueue/ITSQueue.hpp>
#include <PortScanner/ICOMPortScanner.hpp>
#include <Connection/Connection.hpp>

#ifdef _WIN32
   #define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <deque>
#include <chrono>
#include <unordered_set>

namespace netlib
{
   template<typename T>
   class ITSQueue;

   class ServerBase : public IServerBase
   {
      public:
      ServerBase(ITSQueue<OwnedMessage>& msgIn, COMPortScanner& portScanner,
                 std::chrono::seconds periodicity);
      virtual ~ServerBase();

      bool start() override;
      void stop() override;
      void waitForClientConnection() override;
      void update(int32_t nMaxMessages = -1, bool _wait = true) override;

      protected:
      virtual bool onClientConnect(std::shared_ptr<Connection> client) override;
      virtual void onClientDisconnect(std::shared_ptr<Connection> client) override;
      virtual void onMessage(std::shared_ptr<Connection>    _client,
                             [[maybe_unused]] std::string&& _msg) override;

      // Thread Safe Queue for incoming message packets
      ITSQueue<OwnedMessage>& m_qMsgIn;

      // Container of active validated connections
      std::deque<std::shared_ptr<Connection>> m_deqConnections;

      COMPortScanner& m_portS;

      // Asio related containers
      boost::asio::io_context   m_asioContext;
      std::thread               m_threadContext;
      boost::asio::steady_timer m_asyncTimer;

      std::chrono::seconds m_periodicity;
      // Clients will be identified in the "wider system" via an ID
      uint32_t nIDCounter = 21000;

      std::unordered_set<std::string> m_connectedPorts;
   };
}    // namespace netlib