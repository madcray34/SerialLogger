#include <PortScanner/WindowsCOMPortScanner.hpp>
#include <windows.h>
#include <setupapi.h>
#include <iostream>
#include <windows.h>
#include <cstring>

namespace netlib
{
   const std::vector<std::string>& WindowsCOMPortScanner::getAvailableCOMPorts()
   {
      ports.clear();

      // Get the device information set for COM ports
      HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT, nullptr, nullptr,
                                              DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

      if (hDevInfo == INVALID_HANDLE_VALUE)
      {
         std::cerr << "Error getting device information set." << std::endl;
         return ports;
      }

      SP_DEVINFO_DATA devInfoData;
      devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
      DWORD i            = 0;
      // Buffer to store the port name

      // Iterate through all devices
      while (SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData))
      {
         // Query the device's port name using SPDRP_FRIENDLYNAME
         if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr,
                                               reinterpret_cast<PBYTE>(portName.data()),
                                               sizeof(portName), nullptr))
         {
            // Skip already connected ports by checking the set
            if (m_connectedPorts.find(portName.data()) != m_connectedPorts.end())
            {
               ++i;    // Increment to the next device
               continue;
            }

            m_connectedPorts.insert(portName.data());

            const char* comPos = std::strstr(portName.data(), "COM");
            if (comPos != nullptr)
            {
               const char* closeParenPos = std::strchr(comPos, ')');

               // If a closing parenthesis was found, extract the COM port
               if (closeParenPos != nullptr)
               {
                  ports.emplace_back(comPos, closeParenPos - comPos);
               }
            }
         }
         ++i;    // Increment to the next device
      }

      SetupDiDestroyDeviceInfoList(hDevInfo);
      return ports;
   }
}    // namespace netlib