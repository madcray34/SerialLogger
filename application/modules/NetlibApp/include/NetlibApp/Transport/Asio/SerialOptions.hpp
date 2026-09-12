#pragma once

// TODO: This could be moved to a more general path including application configs and options

namespace netlib
{
   struct SerialOptions
   {
      unsigned int baudRate      = 115200;
      unsigned int characterSize = 8;

      enum class Parity
      {
         None = 0,
         Odd,
         Even
      } parity = Parity::None;

      enum class StopBits
      {
         One = 0,
         OnePointFive,
         Two
      } stopBits = StopBits::One;

      enum class FlowControl
      {
         None = 0,
         Software,
         Hardware
      } flowControl = FlowControl::None;

      friend constexpr bool operator==(const SerialOptions &, const SerialOptions &) = default;
   };
}    // namespace netlib