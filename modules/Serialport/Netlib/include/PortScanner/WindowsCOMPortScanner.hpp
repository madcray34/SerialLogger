#pragma once
#include <portScanner/IcomPortScanner.hpp>
#include <unordered_set>
#include <array>

namespace netlib
{
   class WindowsCOMPortScanner : public ICOMPortScanner
   {
      public:
      WindowsCOMPortScanner()
      {
         ports.reserve(c_maxCOMports);
         m_connectedPorts.reserve(netlib::c_maxCOMports);
      };
      ~WindowsCOMPortScanner() override = default;

      const std::vector<std::string> &getAvailableCOMPorts() override;

      private:
      std::vector<std::string>         ports;
      std::array<char, 256>            portName{};
      std::unordered_set<const char *> m_connectedPorts;
   };
}    // namespace netlib