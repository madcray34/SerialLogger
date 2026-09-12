#pragma once
#include <NetlibCore/Connection/IConnectionFactory.hpp>
#include <NetlibApp/Event/Asio/AsioEventLoop.hpp>
#include <NetlibApp/Transport/Asio/SerialOptions.hpp>
#include <utility>

namespace netlib
{
   class AsioSerialConnectionFactory final : public core::IConnectionFactory
   {
      public:
      explicit AsioSerialConnectionFactory(AsioEventLoop &ev, SerialOptions options = {})
          : m_eventLoop(ev), m_options(std::move(options))
      {}
      ~AsioSerialConnectionFactory() override = default;
      std::shared_ptr<core::IConnection> create(std::string_view                    portName,
                                                core::ITSQueue<core::OwnedMessage> &qIn) override;
      std::shared_ptr<core::IConnection> create(std::string_view                    portName,
                                                core::ITSQueue<core::OwnedMessage> &qIn,
                                                const SerialOptions                &options);

      private:
      AsioEventLoop &m_eventLoop;
      SerialOptions  m_options;
   };
}    // namespace netlib