#pragma once
#include <PortScanner/ICOMPortScanner.hpp>

namespace netlib
{
   class WindowsCOMPortScanner : public COMPortScanner
   {
      public:
      WindowsCOMPortScanner()
      {
         ports.reserve(c_maxCOMports);
      };
      ~WindowsCOMPortScanner() override = default;

      const std::vector<std::string>& getAvailableCOMPorts() override;

      private:
      std::vector<std::string> ports;
   };
}    // namespace netlib