#pragma once
#include <NetlibCore/EndPointEnumerator/IEndPointEnumerator.hpp>
#include <NetlibCore/EndPointEnumerator/ISerialPortScanner.hpp>

namespace netlib::core
{
   class SerialPortScannerAdapter : public IEndPointEnumerator
   {
      public:
      explicit SerialPortScannerAdapter(ISerialPortScanner &scanner) : m_scanner(scanner), _cache()
      {}

      const std::vector<CommEndPoint> &getAvailableEndPoints() override
      {
         const auto &ports = m_scanner.getAvailableSerialPorts();

         _cache.clear();
         _cache.reserve(ports.size());
         for (const auto &portName : ports)
            _cache.emplace_back(CommEndPoint{ portName });

         return _cache;
      }

      private:
      ISerialPortScanner       &m_scanner;
      std::vector<CommEndPoint> _cache;
   };
}    // namespace netlib::core
