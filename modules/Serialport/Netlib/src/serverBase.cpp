#include <serverBase/serverBase.hpp>
#include <tsQueue/ItsQueue.hpp>
#include <chrono>
#include <portScanner/IcomPortScanner.hpp>
#include <connection/connection.hpp>

namespace netlib
{
   ServerBase::ServerBase(ITSQueue<OwnedMessage>& msgIn, COMPortScanner& portScanner,
                          std::chrono::seconds periodicity)
       : m_qMsgIn(msgIn)
       , m_portS(portScanner)
       , m_asioContext()
       , m_asyncTimer(m_asioContext, periodicity)
   {
      m_periodicity = periodicity;
   }

   ServerBase::~ServerBase()
   {
      stop();
   }

   bool ServerBase::start()
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
         m_threadContext = std::thread([this]() { m_asioContext.run(); });
      }
      catch (std::exception& e)
      {
         // Something prohibited the server from listening
         std::cerr << "[SERVER] Exception: " << e.what() << "\n";
         return false;
      }

      std::cout << "[SERVER] Started!\n";
      return true;
   }

   void ServerBase::stop()
   {
      m_asioContext.stop();

      if (m_threadContext.joinable())
         m_threadContext.join();

      std::cout << "[SERVER] Stopped!\n";
   }

   void ServerBase::waitForClientConnection()
   {
      // Set the timer to expire.
      m_asyncTimer.expires_after(m_periodicity);

      m_asyncTimer.async_wait(
          [this](const std::error_code ec)
          {
             if (!ec)
             {
                // Call the function to get available COM ports
                const std::vector<std::string>& comPorts = m_portS.getAvailableCOMPorts();
                if (!comPorts.empty())
                {
                   for (const auto& port : comPorts)
                   {
                      std::cout << " Detected COM PORT- " << port << std::endl;

                      try
                      {
                         boost::asio::serial_port newPort{ m_asioContext, port.data() };
                         // Create a new connection to handle this port
                         auto newconn = std::make_shared<Connection>(
                             m_asioContext, std::move(newPort), port, m_qMsgIn);

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
                      catch (std::exception& e)
                      {
                         // If port was not opened a runtime error was launched
                         std::cerr << "Error opening serial port: " << e.what() << std::endl;
                         waitForClientConnection();
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

   void ServerBase::update(int32_t nMaxMessages, bool _wait)
   {
      if (_wait)
      {
         m_qMsgIn.wait();
      }

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

   bool ServerBase::onClientConnect(std::shared_ptr<Connection> client)
   {
      return false;
   }

   void ServerBase::onClientDisconnect(std::shared_ptr<Connection> client)
   {}

   void ServerBase::onMessage([[maybe_unused]] netlib::OwnedMessage&& _msg)
   {}


}    // namespace netlib
