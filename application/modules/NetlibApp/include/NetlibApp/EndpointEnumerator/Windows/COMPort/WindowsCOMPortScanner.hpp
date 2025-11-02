#pragma once
#include <NetlibCore/EndPointEnumerator/ISerialPortScanner.hpp>
#include <unordered_set>
#include <array>

namespace netlib
{
   class WindowsCOMPortScanner : public core::ISerialPortScanner
   {
      public:
      WindowsCOMPortScanner()
      {
         ports.reserve(netlib::core::c_maxPorts);
         m_connectedPorts.reserve(netlib::core::c_maxPorts);
      };
      ~WindowsCOMPortScanner() override = default;

      const std::vector<std::string> &getAvailableCOMPorts() override;

      private:
      std::vector<std::string>         ports;
      std::array<char, 256>            portName{};
      std::unordered_set<const char *> m_connectedPorts;
   };
}    // namespace netlib