#include <PortScanner/WindowsCOMPortScanner.hpp>
#include <windows.h>
#include <setupapi.h>
#include <iostream>

namespace netlib
{
   std::vector<std::string> WindowsCOMPortScanner::getAvailableCOMPorts()
   {
      std::vector<std::string> ports;
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

      while (SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData))
      {
         char buffer[256];
         if (SetupDiGetDeviceInstanceIdA(hDevInfo, &devInfoData, buffer, sizeof(buffer), nullptr))
         {
            std::string deviceInstanceId = buffer;
            if (deviceInstanceId.find("COM") != std::string::npos)
            {
               std::string comPort = deviceInstanceId.substr(deviceInstanceId.find("COM"));
               ports.push_back(comPort);
            }
         }
         i++;
      }

      SetupDiDestroyDeviceInfoList(hDevInfo);
      return ports;
   }
}    // namespace netlib