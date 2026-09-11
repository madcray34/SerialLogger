#pragma once
#include <NetlibCore/EndPointEnumerator/IEndPointEnumerator.hpp>
#include <NetlibCore/EndPointEnumerator/ISerialPortScanner.hpp>

namespace netlib::core
{
   class SerialPortScannerAdapter : public IEndPointEnumerator
   {
      public:
      explicit SerialPortScannerAdapter(ISerialPortScanner &scanner) : m_scanner(scanner)
      {}

      std::vector<CommEndPoint> getAvailableEndPoints() override
      {
         const auto ports = m_scanner.getAvailableSerialPorts();

         std::vector<CommEndPoint> endpoints;
         endpoints.reserve(ports.size());
         for (const auto &portName : ports)
            endpoints.emplace_back(CommEndPoint{ portName });

         return endpoints;
      }

      private:
      ISerialPortScanner &m_scanner;
   };
}    // namespace netlib::core
