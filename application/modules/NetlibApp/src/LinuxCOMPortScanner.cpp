#include <NetlibApp/EndpointEnumerator/Linux/COMPort/LinuxCOMPortScanner.hpp>

#include <filesystem>
#include <regex>

namespace netlib
{
   const std::vector<std::string> &LinuxCOMPortScanner::getAvailableSerialPorts()
   {
      m_ports.clear();

      static const std::regex portRegex{R"(tty(USB\d+|ACM\d+))"};

      for (const auto &entry : std::filesystem::directory_iterator("/dev"))
      {
         const auto name = entry.path().filename().string();
         if (std::regex_match(name, portRegex))
         {
            m_ports.emplace_back(entry.path().string());
         }
      }

      return m_ports;
   }
}    // namespace netlib
