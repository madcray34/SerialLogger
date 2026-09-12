#pragma once

#include <NetlibApp/Connection/Asio/AsioSerialConnectionFactory.hpp>
#include <NetlibApp/EndpointEnumerator/PlatformSerialPortScanner.hpp>
#include <NetlibApp/Transport/Asio/SerialOptions.hpp>
#include <NetlibCore/Queue/ITSQueue.hpp>
#include <NetlibCore/Queue/types/Message.hpp>
#include <model/Imodel.hpp>

#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>

#include <memory>
#include <mutex>
#include <system_error>
#include <string>
#include <thread>
#include <vector>

namespace netlib
{
   enum class SerialSessionState
   {
      Disconnected,
      Connecting,
      Connected,
      Error
   };

   struct SerialSessionSnapshot
   {
      std::vector<std::string> ports;
      SerialOptions            options;
      std::string              connectedPort;
      std::string              error;
      SerialSessionState       state = SerialSessionState::Disconnected;
   };

   class SerialSession
   {
      public:
      SerialSession(PlatformSerialPortScanner &scanner, AsioSerialConnectionFactory &factory,
                    boost::asio::io_context &context, core::ITSQueue<core::OwnedMessage> &messages,
                    IModel &model);
      ~SerialSession();

      SerialSession(const SerialSession &)            = delete;
      SerialSession &operator=(const SerialSession &) = delete;

      void                                start();
      void                                stop();
      void                                refresh();
      void                                connect(std::string port, SerialOptions options);
      void                                disconnect();
      [[nodiscard]] SerialSessionSnapshot snapshot() const;

      private:
      void closeConnection();
      void setError(const std::error_code &error);

      PlatformSerialPortScanner          &m_scanner;
      AsioSerialConnectionFactory        &m_factory;
      boost::asio::io_context            &m_context;
      core::ITSQueue<core::OwnedMessage> &m_messages;
      IModel                             &m_model;
      std::shared_ptr<core::IConnection>  m_connection;
      std::unique_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>
                            m_workGuard;
      std::jthread          m_ioThread;
      std::jthread          m_messagePump;
      std::jthread          m_portWatcher;
      std::mutex            m_scannerMutex;
      mutable std::mutex    m_snapshotMutex;
      SerialSessionSnapshot m_snapshot;
      bool                  m_running = false;
   };
}    // namespace netlib
