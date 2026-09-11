#include <NetlibApp/EndpointEnumerator/Windows/COMPort/WindowsCOMPortScanner.hpp>
#include <windows.h>
#include <setupapi.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstring>

namespace netlib
{
   std::vector<std::string> WindowsCOMPortScanner::getAvailableSerialPorts()
   {
      std::vector<std::string> ports;
      ports.reserve(netlib::core::c_maxPorts);

      // Get the device information set for COM ports
      HDEVINFO hDevInfo = SetupDiGetClassDevs(&GUID_DEVINTERFACE_COMPORT, nullptr, nullptr,
                                              DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

      if (hDevInfo == INVALID_HANDLE_VALUE)
      {
         std::cerr << "Error getting device information set." << std::endl;
         return ports;
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
                  // Extract the COM identifier, e.g. "COM3".
                  std::string comStr(comPos, closeParenPos - comPos);
                  ports.emplace_back(std::move(comStr));
               }
            }
         }
         ++i;    // Increment to the next device
      }

      SetupDiDestroyDeviceInfoList(hDevInfo);
      return ports;
   }
}    // namespace netlib