#pragma once

#include <NetlibCore/EndPointEnumerator/ISerialPortScanner.hpp>
#include <string>
#include <vector>

namespace netlib
{
   class LinuxCOMPortScanner : public core::ISerialPortScanner
   {
      public:
      LinuxCOMPortScanner() = default;
      ~LinuxCOMPortScanner() override = default;

      const std::vector<std::string> &getAvailableSerialPorts() override;

      private:
      std::vector<std::string> m_ports;
   };
}    // namespace netlib
