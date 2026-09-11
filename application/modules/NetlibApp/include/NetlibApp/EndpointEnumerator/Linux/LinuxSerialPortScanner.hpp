#pragma once

#include <NetlibCore/EndPointEnumerator/ISerialPortScanner.hpp>

#include <string>
#include <string_view>
#include <vector>

namespace netlib
{
   class LinuxSerialPortScanner final : public core::ISerialPortScanner
   {
      public:
      std::vector<std::string> getAvailableSerialPorts() override;

      static bool isSerialDeviceNode(std::string_view deviceNode) noexcept;
   };
}    // namespace netlib
