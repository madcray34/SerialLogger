#pragma once
#include <PortScanner/ICOMPortScanner.hpp>

namespace netlib
{
   class WindowsCOMPortScanner : public COMPortScanner
   {
      public:
      WindowsCOMPortScanner()           = default;
      ~WindowsCOMPortScanner() override = default;

      std::vector<std::string> getAvailableCOMPorts() override;
   };
}    // namespace netlib