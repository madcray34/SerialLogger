#pragma once
#include <ServerBase/IServerBase.hpp>
#include <deque>

#ifdef _WIN32
   #define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>

namespace netlib
{
   template<typename T>
   class ITSQueue;

   class COMPortScanner;

   class Connection;

   class ServerBase : public IServerBase
   {
      public:
      ServerBase(ITSQueue<owned_message>& msgIn, COMPortScanner& portScanner,
                 std::chrono::seconds periodicity);
      virtual ~ServerBase();

      bool start() override;
      void stop() override;
      void waitForClientConnection() override;
      void update(int32_t nMaxMessages = -1, bool _wait = true) override;

      protected:
      virtual bool onClientConnect(std::shared_ptr<Connection> client) override;
      virtual void onClientDisconnect(std::shared_ptr<Connection> client) override;
      virtual void onMessage(std::shared_ptr<Connection> _client,
                             [[maybe_unused]] Message&   _msg) override;

      // Thread Safe Queue for incoming message packets
      ITSQueue<owned_message>& m_qMsgIn;

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
   };
}    // namespace netlib