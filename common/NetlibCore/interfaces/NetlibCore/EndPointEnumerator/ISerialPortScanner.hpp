#pragma once
#include <vector>
#include <string>

namespace netlib::core
{
   static constexpr auto c_maxPorts = 16;
   class ISerialPortScanner
   {
      public:
      virtual ~ISerialPortScanner() = default;

      // The interface method that perform serial-port scanning
      virtual const std::vector<std::string> &getAvailableSerialPorts() = 0;
   };
}    // namespace netlib