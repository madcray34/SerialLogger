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
      {
         m_connectedPorts.reserve(netlib::core::c_maxPorts);
      };
      ~WindowsCOMPortScanner() override = default;

      std::vector<std::string> getAvailableSerialPorts() override;

      private:
      std::array<char, 256>           portName{};
      std::unordered_set<std::string> m_connectedPorts;
   };
}    // namespace netlib