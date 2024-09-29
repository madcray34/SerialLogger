#pragma once
#include <vector>
#include <string>

namespace netlib
{
   class COMPortScanner
   {
      public:
      virtual ~COMPortScanner() = default;

      // The interface method that perform COM port scanning
      virtual std::vector<std::string> getAvailableCOMPorts() = 0;
   };
}    // namespace netlib