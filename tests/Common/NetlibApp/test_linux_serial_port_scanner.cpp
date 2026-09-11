#include <NetlibApp/EndpointEnumerator/Linux/LinuxSerialPortScanner.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <vector>

TEST(LinuxSerialPortScannerTest, AcceptsUsbSerialDeviceNodes)
{
   EXPECT_TRUE(netlib::LinuxSerialPortScanner::isSerialDeviceNode("/dev/ttyUSB0"));
   EXPECT_TRUE(netlib::LinuxSerialPortScanner::isSerialDeviceNode("/dev/ttyACM0"));
}

TEST(LinuxSerialPortScannerTest, RejectsConsoleAndNonTtyDeviceNodes)
{
   EXPECT_FALSE(netlib::LinuxSerialPortScanner::isSerialDeviceNode("/dev/tty"));
   EXPECT_FALSE(netlib::LinuxSerialPortScanner::isSerialDeviceNode("/dev/tty0"));
   EXPECT_FALSE(netlib::LinuxSerialPortScanner::isSerialDeviceNode("/dev/ttyprintk"));
   EXPECT_FALSE(netlib::LinuxSerialPortScanner::isSerialDeviceNode("/dev/pts/0"));
}

TEST(LinuxSerialPortScannerTest, ReturnsSortedUniqueDeviceNodes)
{
   netlib::LinuxSerialPortScanner scanner;
   const auto                    &ports = scanner.getAvailableSerialPorts();

   EXPECT_TRUE(std::is_sorted(ports.begin(), ports.end()));
   EXPECT_EQ(std::adjacent_find(ports.begin(), ports.end()), ports.end());
}
