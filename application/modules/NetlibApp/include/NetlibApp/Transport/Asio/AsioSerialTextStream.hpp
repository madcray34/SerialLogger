#pragma once
#include <NetlibCore/Transport/ITextStream.hpp>
#include <NetlibApp/Event/Asio/AsioEventLoop.hpp>
#include <functional>

namespace netlib
{
   struct SerialOptions;
}

namespace netlib
{
   class AsioSerialTextStream final : public core::ITextStream
   {
      public:
      AsioSerialTextStream(AsioEventLoop &eventLoop, std::string portname,
                           unsigned int baud = 9600);

      AsioSerialTextStream(AsioEventLoop &eventLoop, std::string portname, SerialOptions &options);

      ~AsioSerialTextStream() override;

      void open(std::function<void(const std::error_code &)> handler) override;

      bool isOpen() const noexcept override;

      void close() noexcept override;

      // Asynchronous read a line (until newline character)
      void asyncReadLine(
          std::function<void(const std::error_code &, std::string)> handler) override;

      // Asynchronous write a line (appends newline character)
      [[maybe_unused]] void asyncWriteLine(
          std::string_view                                          line,
          std::function<void(const std::error_code &, std::size_t)> handler) override;

      private:
      struct Impl;                      // Forward declaration of the implementation struct
      std::unique_ptr<Impl> m_pImpl;    // Pointer to implementation idiom to hide details into the
                                        // cpp file
   };
}    // namespace netlib