#include <NetlibApp/Transport/Asio/SerialOptions.hpp>

#include <gtest/gtest.h>

TEST(SerialOptionsTest, DefaultsToTypicalDebugUartConfiguration)
{
   const netlib::SerialOptions options;

   EXPECT_EQ(options.baudRate, 115200U);
   EXPECT_EQ(options.characterSize, 8U);
   EXPECT_EQ(options.parity, netlib::SerialOptions::Parity::None);
   EXPECT_EQ(options.stopBits, netlib::SerialOptions::StopBits::One);
   EXPECT_EQ(options.flowControl, netlib::SerialOptions::FlowControl::None);
}

TEST(SerialOptionsTest, ComparesEverySetting)
{
   const netlib::SerialOptions defaults;
   auto                        changed = defaults;

   changed.baudRate = 9600;

   EXPECT_NE(changed, defaults);
}
