#include <NetlibApp/Transport/Asio/AsioSerialTextStream.hpp>
#include <NetlibApp/Transport/Asio/SerialOptions.hpp>

#include <boost/asio/io_context.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/read_until.hpp>

#include <algorithm>
#include <deque>

namespace netlib
{
   static std::error_code to_error_code(const boost::system::error_code &ec)
   {
      return std::error_code(ec.value(), std::system_category());
   }

   struct AsioSerialTextStream::Impl
   {
      AsioEventLoop           &m_eventLoop;
      boost::asio::io_context &m_ioContext;
      boost::asio::serial_port m_serialPort;
      boost::asio::streambuf   m_streamBuffer;
      const char               m_lineDelimiter = '\n';
      std::deque<std::string>  m_writeQueue;
      bool                     m_isWriting = false;

      Impl(AsioEventLoop &eventLoop, const std::string &portname, unsigned int baud)
          : m_eventLoop(eventLoop)
          , m_ioContext(eventLoop.getContext())
          , m_serialPort(m_ioContext, portname)    // This constructor creates and opens a serial
                                                   // port for the specified device name.
      {
         m_serialPort.set_option(boost::asio::serial_port_base::baud_rate(baud));
         m_serialPort.set_option(boost::asio::serial_port_base::character_size(8));
         m_serialPort.set_option(boost::asio::serial_port_base::flow_control(
             boost::asio::serial_port_base::flow_control::none));
         m_serialPort.set_option(
             boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
         m_serialPort.set_option(boost::asio::serial_port_base::stop_bits(
             boost::asio::serial_port_base::stop_bits::one));
      };


      Impl(AsioEventLoop &eventLoop, const std::string &portname, SerialOptions &options)
          : m_eventLoop(eventLoop)
          , m_ioContext(eventLoop.getContext())
          , m_serialPort(m_ioContext, portname)    // This constructor creates and opens a serial
                                                   // port for the specified device name.
      {
         // For the next options we need to map from our SerialOptions to boost options
         // The easiest way is to use the same type values (in SerialOptions) as boost defines
         m_serialPort.set_option(boost::asio::serial_port_base::baud_rate(options.baudRate));

         m_serialPort.set_option(
             boost::asio::serial_port_base::character_size(options.characterSize));

         m_serialPort.set_option(boost::asio::serial_port_base::flow_control(
             static_cast<boost::asio::serial_port_base::flow_control::type>(options.flowControl)));

         m_serialPort.set_option(boost::asio::serial_port_base::parity(
             static_cast<boost::asio::serial_port_base::parity::type>(options.parity)));

         m_serialPort.set_option(boost::asio::serial_port_base::stop_bits(
             static_cast<boost::asio::serial_port_base::stop_bits::type>(options.stopBits)));
      };
   };

   AsioSerialTextStream::AsioSerialTextStream(AsioEventLoop &eventLoop, std::string portname,
                                              unsigned int baud /*= 9600*/)
       : m_pImpl(RuntimeCheckedPtr<Impl>(new Impl(eventLoop, portname, baud)))
   {}

   AsioSerialTextStream::AsioSerialTextStream(AsioEventLoop &eventLoop, std::string portname,
                                              SerialOptions &options)
       : m_pImpl(RuntimeCheckedPtr<Impl>(new Impl(eventLoop, portname, options)))
   {}
 
   AsioSerialTextStream::~AsioSerialTextStream() = default;

   void AsioSerialTextStream::open(std::function<void(const std::error_code &)> handler)
   {
      boost::asio::post(m_pImpl->m_ioContext, [handler]() { handler(std::error_code()); });
   }
   
   bool AsioSerialTextStream::isOpen() const noexcept
   {     
      return m_pImpl->m_serialPort.is_open();
   }

   void AsioSerialTextStream::close() noexcept 
   {
      boost::asio::post(m_pImpl->m_ioContext, [this]() { m_pImpl->m_serialPort.close(); });
   }
 
   void AsioSerialTextStream::asyncReadLine(
       std::function<void(const std::error_code &, std::string)> handler)
   {
      boost::asio::async_read_until(
          m_pImpl->m_serialPort, m_pImpl->m_streamBuffer, m_pImpl->m_lineDelimiter,
          [this, handler](const boost::system::error_code &ec, [[maybe_unused]] std::size_t length)
          {
             if (!ec && length > 0)
             {
                std::string line;
                line.reserve(length);

                auto data  = m_pImpl->m_streamBuffer.data();
                auto first = boost::asio::buffers_begin(data);
                auto last  = first + static_cast<std::ptrdiff_t>(length);
                auto nl    = std::find(first, last, m_pImpl->m_lineDelimiter);
                line.assign(first, nl);

                if (!line.empty() && line.back() == '\r')
                {
                   line.pop_back();    // Remove carriage return if present
                }

                m_pImpl->m_streamBuffer.consume(length);
                handler(std::error_code(), line);
             }
             else if (ec)
             {
                handler(to_error_code(ec), std::string());
             }
          });
   }

   void AsioSerialTextStream::asyncWriteLine(
       [[maybe_unused]] std::string_view                         line,
       [[maybe_unused]] std::function<void(const std::error_code &, std::size_t)> handler) {
      // Empty implementation for the moment
      // TODO function body
   };
}    // namespace netlib