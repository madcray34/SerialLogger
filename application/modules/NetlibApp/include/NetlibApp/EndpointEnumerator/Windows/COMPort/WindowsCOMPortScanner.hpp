#pragma once
#include <NetlibCore/EndPointEnumerator/ISerialPortScanner.hpp>
#include <array>
#include <string>

namespace netlib
{
   class WindowsCOMPortScanner : public core::ISerialPortScanner
   {
      public:
      WindowsCOMPortScanner()           = default;
      ~WindowsCOMPortScanner() override = default;

      std::vector<std::string> getAvailableSerialPorts() override;

      private:
      std::array<char, 256> portName{};
   };
}    // namespace netlib