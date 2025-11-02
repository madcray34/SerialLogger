#pragma once

// TODO: This could be moved to a more general path including application configs and options

namespace netlib
{
   struct SerialOptions
   {
      /* data */
      unsigned int baudRate      = 9600;
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
   };
}    // namespace netlib