#include <PortScanner/WindowsCOMPortScanner.hpp>
#include <windows.h>
#include <setupapi.h>
#include <iostream>

namespace netlib
{
   std::vector<std::string> WindowsCOMPortScanner::getAvailableCOMPorts()
   {
      // Reserve space for expected COM ports
      std::vector<std::string> ports;
      ports.reserve(16);

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

      // Iterate through all devices
      while (SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData))
      {
         // Buffer to store the port name
         char portName[256];

         // Query the device's port name using SPDRP_FRIENDLYNAME
         if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &devInfoData, SPDRP_FRIENDLYNAME, nullptr,
                                               reinterpret_cast<PBYTE>(portName), sizeof(portName),
                                               nullptr))
         {
            std::string friendlyStr(portName);

            // Find "COM" in the friendly name and extract the COM port
            std::size_t comPos = friendlyStr.find("COM");
            if (comPos != std::string::npos)
            {
               std::size_t closeParenPos = friendlyStr.find(")", comPos);

               // If a closing parenthesis was found, extract the COM port
               if (closeParenPos != std::string::npos)
               {
                  // Extract the substring starting from "COM"
                  std::string comPort = friendlyStr.substr(comPos, closeParenPos - comPos);
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