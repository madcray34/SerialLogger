#pragma once
#include <vector>
#include <string>

namespace netlib
{
   static constexpr auto c_maxPorts = 16;
   class ISerialPortScanner
   {
      public:
      virtual ~ISerialPortScanner() = default;

      // The interface method that perform COM port scanning
      virtual const std::vector<std::string> &getAvailableCOMPorts() = 0;
   };
}    // namespace netlib