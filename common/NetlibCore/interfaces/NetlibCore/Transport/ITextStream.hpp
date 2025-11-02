#pragma once
#include <functional>
#include <string>
#include <string_view>
#include <system_error>

namespace netlib
{
   class ITextStream
   {
      public:
      virtual ~ITextStream() = default;

      virtual void open(std::function<void(const std::error_code &)> handler) = 0;
      virtual bool isOpen() const noexcept                                    = 0;
      virtual void close() noexcept                                           = 0;

      // Asynchronous read a line (until newline character)
      virtual void asyncReadLine(
          std::function<void(const std::error_code &, std::string)> handler) = 0;

      // Asynchronous write a line (appends newline character)
      virtual void asyncWriteLine(
          std::string_view                                          line,
          std::function<void(const std::error_code &, std::size_t)> handler) = 0;
   };
}    // namespace netlib