#pragma once
#include <vector>
#include <string>

namespace netlib
{
   static constexpr auto c_maxCOMports = 16;
   class ICOMPortScanner
   {
      public:
      virtual ~ICOMPortScanner() = default;

      // The interface method that perform COM port scanning
      virtual const std::vector<std::string> &getAvailableCOMPorts() = 0;
   };
}    // namespace netlib