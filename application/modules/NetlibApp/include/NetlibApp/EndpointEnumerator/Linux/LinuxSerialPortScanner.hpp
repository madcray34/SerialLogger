#pragma once

#include <NetlibCore/EndPointEnumerator/ISerialPortScanner.hpp>

#include <string_view>
#include <vector>

namespace netlib
{
   class LinuxSerialPortScanner final : public core::ISerialPortScanner
   {
      public:
      const std::vector<std::string> &getAvailableSerialPorts() override;

      static bool isSerialDeviceNode(std::string_view deviceNode) noexcept;

      private:
      std::vector<std::string> m_ports;
   };
}    // namespace netlib
