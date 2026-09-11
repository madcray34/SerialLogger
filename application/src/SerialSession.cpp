#include <application/SerialSession.hpp>

#include <boost/asio/error.hpp>
#include <boost/asio/post.hpp>

#include <chrono>
#include <future>
#include <iostream>
#include <utility>

namespace netlib
{
   namespace
   {
      // Poll for newly attached/removed serial ports so the UI stays up to date without
      // requiring the user to press "Refresh" manually.
      constexpr auto c_portRefreshInterval = std::chrono::seconds(2);
   }    // namespace

   SerialSession::SerialSession(PlatformSerialPortScanner          &scanner,
                                AsioSerialConnectionFactory        &factory,
                                boost::asio::io_context            &context,
                                core::ITSQueue<core::OwnedMessage> &messages, IModel &model)
       : m_scanner(scanner)
       , m_factory(factory)
       , m_context(context)
       , m_messages(messages)
       , m_model(model)
   {}

   SerialSession::~SerialSession()
   {
      stop();
   }

   void SerialSession::start()
   {
      if (m_running)
      {
         return;
      }

      m_context.restart();
      m_workGuard = std::make_unique<
          boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>(
          boost::asio::make_work_guard(m_context));
      m_running     = true;
      m_ioThread    = std::jthread([this]() { m_context.run(); });
      m_messagePump = std::jthread(
          [this](std::stop_token stopToken)
          {
             while (!stopToken.stop_requested())
             {
                if (!m_messages.wait_for(std::chrono::milliseconds(100)))
                {
                   continue;
                }

                while (!m_messages.empty())
                {
                   m_model.pushMessage(std::move(m_messages.pop_front()));
                }
             }
          });
      // Dedicated thread for periodic port scanning, kept independent from the message pump so
      // that a slow OS device enumeration call can never delay draining incoming UART data.
      m_portWatcher = std::jthread(
          [this](std::stop_token stopToken)
          {
             constexpr auto pollStep = std::chrono::milliseconds(100);
             while (!stopToken.stop_requested())
             {
                refresh();
                auto waited = std::chrono::milliseconds(0);
                while (waited < c_portRefreshInterval && !stopToken.stop_requested())
                {
                   std::this_thread::sleep_for(pollStep);
                   waited += pollStep;
                }
             }
          });
   }

   void SerialSession::stop()
   {
      if (!m_running)
      {
         return;
      }

      auto closed       = std::make_shared<std::promise<void>>();
      auto closedFuture = closed->get_future();
      boost::asio::post(m_context,
                        [this, closed]()
                        {
                           closeConnection();
                           {
                              std::lock_guard lock(m_snapshotMutex);
                              m_snapshot.state = SerialSessionState::Disconnected;
                              m_snapshot.connectedPort.clear();
                              m_snapshot.error.clear();
                           }
                           closed->set_value();
                        });
      closedFuture.wait();

      if (m_messagePump.joinable())
      {
         m_messagePump.request_stop();
         m_messagePump.join();
      }

      if (m_portWatcher.joinable())
      {
         m_portWatcher.request_stop();
         m_portWatcher.join();
      }

      m_workGuard->reset();
      m_context.stop();
      if (m_ioThread.joinable())
      {
         m_ioThread.join();
      }

      m_workGuard.reset();
      m_running = false;
   }

   void SerialSession::refresh()
   {
      std::vector<std::string> ports;
      {
         // Serializes access to the scanner, whose Windows implementation reuses an internal
         // buffer that is not safe to call concurrently from the polling and GUI threads.
         std::lock_guard scannerLock(m_scannerMutex);
         ports = m_scanner.getAvailableSerialPorts();
      }
      std::lock_guard lock(m_snapshotMutex);
      m_snapshot.ports = std::move(ports);
   }

   void SerialSession::connect(std::string port, SerialOptions options)
   {
      if (!m_running || port.empty())
      {
         return;
      }

      boost::asio::post(m_context,
                        [this, port = std::move(port), options]()
                        {
                           closeConnection();

                           {
                              std::lock_guard lock(m_snapshotMutex);
                              m_snapshot.options       = options;
                              m_snapshot.connectedPort = port;
                              m_snapshot.error.clear();
                              m_snapshot.state = SerialSessionState::Connecting;
                           }

                           try
                           {
                              m_connection = m_factory.create(port, m_messages, options);
                              m_connection->setErrorHandler([this](const std::error_code &error)
                                                            { setError(error); });
                              m_connection->connectToClient(1);

                              std::lock_guard lock(m_snapshotMutex);
                              m_snapshot.state = SerialSessionState::Connected;
                              std::cout << "Connected client [" << port << "]" << std::endl;
                           }
                           catch (const std::exception &error)
                           {
                              setError(std::make_error_code(std::errc::io_error));
                              std::lock_guard lock(m_snapshotMutex);
                              m_snapshot.error = error.what();
                              std::cerr << "Failed to connect client [" << port
                                       << "]: " << error.what() << std::endl;
                           }
                        });
   }

   void SerialSession::disconnect()
   {
      if (!m_running)
      {
         return;
      }

      boost::asio::post(m_context,
                        [this]()
                        {
                           const auto port = snapshot().connectedPort;
                           closeConnection();
                           std::lock_guard lock(m_snapshotMutex);
                           m_snapshot.state = SerialSessionState::Disconnected;
                           m_snapshot.connectedPort.clear();
                           m_snapshot.error.clear();
                           if (!port.empty())
                           {
                              std::cout << "Removing client [" << port << "]" << std::endl;
                           }
                        });
   }

   SerialSessionSnapshot SerialSession::snapshot() const
   {
      std::lock_guard lock(m_snapshotMutex);
      return m_snapshot;
   }

   void SerialSession::closeConnection()
   {
      if (m_connection)
      {
         m_connection->disconnect();
         m_connection.reset();
      }
   }

   void SerialSession::setError(const std::error_code &error)
   {
      if (error == std::errc::operation_canceled)
      {
         return;
      }

      std::lock_guard lock(m_snapshotMutex);
      m_snapshot.state = SerialSessionState::Error;
      m_snapshot.error = error.message();
   }
}    // namespace netlib
