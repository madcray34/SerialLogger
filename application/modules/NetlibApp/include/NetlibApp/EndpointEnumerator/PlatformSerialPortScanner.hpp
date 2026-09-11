#pragma once

#if defined(_WIN32)
   #include <NetlibApp/EndpointEnumerator/Windows/COMPort/WindowsCOMPortScanner.hpp>
namespace netlib
{
   using PlatformSerialPortScanner = WindowsCOMPortScanner;
}
#elif defined(__linux__)
   #include <NetlibApp/EndpointEnumerator/Linux/LinuxSerialPortScanner.hpp>
namespace netlib
{
   using PlatformSerialPortScanner = LinuxSerialPortScanner;
}
#else
   #error "Serial-port discovery is supported only on Windows and Linux."
#endif
