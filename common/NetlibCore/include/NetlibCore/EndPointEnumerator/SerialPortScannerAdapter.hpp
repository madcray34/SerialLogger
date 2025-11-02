#pragma once
#include <NetlibCore/EndPointEnumerator/IEndPointEnumerator.hpp>
#include <NetlibCore/EndPointEnumerator/ISerialPortScanner.hpp>

namespace netlib
{
   class SerialPortScannerAdapter : public IEndPointEnumerator
   {
      public:
      explicit SerialPortScannerAdapter(ISerialPortScanner &scanner) : m_scanner(scanner)
      {}

      const std::vector<CommEndPoint> &getAvailableEndPoints() override
      {
         const auto &ports = m_scanner.getAvailableCOMPorts();

         _cache.clear();
         _cache.reserve(ports.size());
         for (const auto &portName : ports)
            _cache.emplace_back(portName);

         return _cache;
      }

      private:
      ISerialPortScanner       &m_scanner;
      std::vector<CommEndPoint> _cache;
   };
}    // namespace netlib
