#pragma once
#include <NetlibCore/Transport/ITextStream.hpp>
#include <NetlibApp/Event/Asio/AsioEventLoop.hpp>
#include <memory>

#include <boost/asio/serial_port.hpp>
#include <boost/asio/streambuf.hpp>

namespace netlib
{
   class AsioSerialTextStream : public ITextStream
   {
      public:
      AsioSerialTextStream(AsioEventLoop                            &eventLoop,
                           std::shared_ptr<boost::asio::serial_port> port);

      virtual ~AsioSerialTextStream();

      void open(std::function<void(const std::error_code &)> handler) override;

      bool isOpen() const noexcept override;

      void close() noexcept override;

      // Asynchronous read a line (until newline character)
      void asyncReadLine(
          std::function<void(const std::error_code &, std::string)> handler) override;

      // Asynchronous write a line (appends newline character)
      void asyncWriteLine(
          const std::string_view                                   &line,
          std::function<void(const std::error_code &, std::size_t)> handler) override;

      private:
      std::shared_ptr<boost::asio::serial_port> m_port;
      boost::asio::streambuf                    m_streamBuffer;
   };
}    // namespace netlib