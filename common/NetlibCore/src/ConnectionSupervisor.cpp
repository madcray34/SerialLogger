#include <NetlibCore/Connection/ConnectionSupervisor.hpp>

using namespace std::chrono_literals;

namespace netlib::core
{
   ConnectionSupervisor::ConnectionSupervisor(ITSQueue<OwnedMessage> &msgIn,
                                              IEndPointEnumerator    &endpoints,
                                              IConnectionFactory     &connFactory,
                                              IEventLoop &eventLoop, ITimerFactory &timer,
                                              std::chrono::seconds periodicity)
       : m_qMsgIn(msgIn)
       , m_endpoints(endpoints)
       , m_connFactory(connFactory)
       , m_eventLoop(eventLoop)
       , m_asyncTimerFactory(timer)
       , m_asyncTimer(m_asyncTimerFactory.createTimer(periodicity))
       , m_periodicity(periodicity)
       , m_threadContext()
       , m_deqConnections()
   {}

   ConnectionSupervisor::~ConnectionSupervisor()
   {
      stop();
   }

   bool ConnectionSupervisor::start()
   {
      try
      {
         /**
          * @brief Issue a task to the asio context - This is important as it will prime the
          * context with "work", and stop it from exiting immediately. Since this is a server, we
          * want it primed ready to handle clients trying to connect.
          */
         waitForClientConnection();

         // Launch the asio context in its own thread
         m_threadContext = std::thread([this]() { m_eventLoop.start(); });
      }
      catch (std::exception &e)
      {
         // Something prohibited the server from listening
         std::cerr << "[SERVER] Exception: " << e.what() << "\n";
         return false;
      }

      std::cout << "[SERVER] Started!\n";
      return true;
   }

   void ConnectionSupervisor::stop()
   {
      m_eventLoop.stop();

      if (m_threadContext.joinable())
         m_threadContext.join();

      std::cout << "[SERVER] Stopped!\n";
   }

   void ConnectionSupervisor::waitForClientConnection()
   {
      // Set the timer to expire.
      m_asyncTimer->expires_after(m_periodicity);

      m_asyncTimer->async_wait(
          [this](const std::error_code ec)
          {
             if (!ec)
             {
                // Call the function to get available communication points
                const auto &eps = m_endpoints.getAvailableEndPoints();
                if (!eps.empty())
                {
                   for (const auto &ep : eps)
                   {
                      std::cout << " Detected endpoint- " << ep.portName << std::endl;

                      try
                      {
                         auto newconn = m_connFactory.create(ep.portName, m_qMsgIn);

                         // Give the user server a chance to deny connection
                         if (onClientConnect(newconn))
                         {
                            // Connection allowed, so add to container of new connections
                            m_deqConnections.push_back(std::move(newconn));



                            // And very important! Issue a task to the connection's
                            // asio context to sit and wait for bytes to arrive!
                            m_deqConnections.back()->connectToClient(nIDCounter++);

                            std::cout << "[" << m_deqConnections.back()->getID()
                                      << "] Connection Approved\n"
                                      << std::endl;
                         }
                         else
                         {
                            newconn->disconnect();
                            std::cout << "[-----] Connection Denied\n" << std::endl;

                            // Connection will go out of scope with no pending tasks, so will
                            // get destroyed automagically due to the wonder of smart pointers
                         }
                      }
                      catch (std::exception &e)
                      {
                         // If port was not opened a runtime error was launched
                         std::cerr << "Error opening serial port: " << e.what() << std::endl;
                      }
                   }
                }
             }
             else
             {
                // Error has occurred during acceptance
                std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
             }

             // Prime the asio context with more work - again simply wait for
             // another connection...
             waitForClientConnection();
          });

      return;
   }

   void ConnectionSupervisor::update(int32_t nMaxMessages, bool _wait)
   {
      auto msgIn = false;
      if (_wait)
      {
         msgIn = m_qMsgIn.wait_for(500ms);
      }

      if (_wait && !msgIn && m_qMsgIn.empty())
         return;

      // Process as many messages as you can up to the value
      // specified
      int32_t nMessageCount = 0;
      while (nMessageCount < nMaxMessages && !m_qMsgIn.empty())
      {
         // Pass to message handler
         onMessage(std::move(m_qMsgIn.pop_front()));

         nMessageCount++;
      }
   }

   bool ConnectionSupervisor::onClientConnect([[maybe_unused]] std::shared_ptr<IConnection> client)
   {
      return false;
   }

   void ConnectionSupervisor::onClientDisconnect([[maybe_unused]] std::shared_ptr<IConnection> client)
   {}

   void ConnectionSupervisor::onMessage([[maybe_unused]] OwnedMessage &&_msg)
   {}


}    // namespace netlib::core
