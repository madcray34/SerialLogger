#include <NetlibApp/EndpointEnumerator/Windows/COMPort/WindowsCOMPortScanner.hpp>
#include <windows.h>
#include <setupapi.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

namespace netlib
{
   const std::vector<std::string> &WindowsCOMPortScanner::getAvailableSerialPorts()
   {
      m_ports.clear();

      // Get the device information set for COM ports
      HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT, nullptr, nullptr,
                                              DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

      if (hDevInfo == INVALID_HANDLE_VALUE)
      {
         std::cerr << "Error getting device information set." << std::endl;
         return m_ports;
      }

      SP_DEVINFO_DATA devInfoData{};
      devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
      DWORD i            = 0;

      // Buffer to store the port name

      // Iterate through all devices
      while (SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData))
      {
         // Query the device's port name using SPDRP_FRIENDLYNAME
         if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr,
                                               reinterpret_cast<PBYTE>(portName.data()),
                                               static_cast<DWORD>(portName.size()), nullptr))
         {
            const char *comPos = std::strstr(portName.data(), "COM");
            if (comPos != nullptr)
            {
               const char *closeParenPos = std::strchr(comPos, ')');
               if (closeParenPos != nullptr)
               {
                  // estrai la stringa del COM, es. "COM3"
                  std::string comStr(comPos, closeParenPos - comPos);

                  // controlla usando il contenuto (non il puntatore)
                  if (m_connectedPorts.find(comStr) == m_connectedPorts.end())
                  {
                     m_connectedPorts.insert(comStr);
                     m_ports.emplace_back(std::move(comStr));
                  }
               }
            }
         }
         ++i;    // Increment to the next device
      }

      SetupDiDestroyDeviceInfoList(hDevInfo);
      return m_ports;
   }
}    // namespace netlib