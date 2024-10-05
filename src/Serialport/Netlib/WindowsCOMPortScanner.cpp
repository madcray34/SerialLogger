#include <PortScanner/WindowsCOMPortScanner.hpp>
#include <windows.h>
#include <setupapi.h>
#include <iostream>

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
      static char portName[256];

      // Iterate through all devices
      while (SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData))
      {
         // Query the device's port name using SPDRP_FRIENDLYNAME
         if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr,
                                               reinterpret_cast<PBYTE>(portName), sizeof(portName),
                                               nullptr))
         {
            const char* comPos = std::strstr(portName, "COM");
            if (comPos != nullptr)
            {
               const char* closeParenPos = std::strchr(comPos, ')');

               // If a closing parenthesis was found, extract the COM port
               if (closeParenPos != nullptr)
               {
                  // Extract COM port name between "COM" and ")"
                  std::string comPort(comPos, closeParenPos - comPos);
                  ports.emplace_back(std::move(comPort));
               }
            }
         }
         ++i;    // Increment to the next device
      }

      SetupDiDestroyDeviceInfoList(hDevInfo);
      return ports;
   }
}    // namespace netlib