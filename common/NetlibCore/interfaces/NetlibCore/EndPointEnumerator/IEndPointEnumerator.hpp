#pragma once
#include <string>
#include <vector>

namespace netlib::core
{
   struct CommEndPoint
   {
      std::string portName;    // e.g., "COM3" or "/dev/ttyUSB0" or "tcp://127.0.0.1:5000"
      //   std::string description;
      //   std::string hardwareId;
   };

   class IEndPointEnumerator
   {
      public:
      virtual ~IEndPointEnumerator()                                   = default;
      virtual const std::vector<CommEndPoint> &getAvailableEndPoints() = 0;
   };
}    // namespace netlib