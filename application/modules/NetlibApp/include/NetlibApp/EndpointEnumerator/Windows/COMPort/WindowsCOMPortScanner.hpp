#pragma once
#include <NetlibCore/EndPointEnumerator/ISerialPortScanner.hpp>
#include <unordered_set>
#include <array>
#include <string>

namespace netlib
{
   class WindowsCOMPortScanner : public core::ISerialPortScanner
   {
      public:
      WindowsCOMPortScanner()
         : ports()
         , portName{}
         , m_connectedPorts()
      {
         ports.reserve(netlib::core::c_maxPorts);
         m_connectedPorts.reserve(netlib::core::c_maxPorts);
      };
      ~WindowsCOMPortScanner() override = default;

      const std::vector<std::string> &getAvailableSerialPorts() override;

      private:
      std::vector<std::string>        ports;
      std::array<char, 256>           portName{};
      std::unordered_set<std::string> m_connectedPorts;
   };
}    // namespace netlib