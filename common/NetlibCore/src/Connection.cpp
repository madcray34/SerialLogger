#include <NetlibCore/Connection/Connection.hpp>

namespace netlib
{
   Connection::Connection(std::shared_ptr<netlib::ITextStream> textStream, std::string name,
                          ITSQueue<OwnedMessage> &qIn)
       : m_textStream(textStream), m_portName(name), m_qMessagesIn(qIn)
   {}

   uint32_t Connection::getID() const noexcept
   {
      return id;
   }


   void Connection::connectToClient(uint32_t uid)
   {
      if (isConnected())
      {
         id = uid;

         // A client has attempted to connect and we want to read the content.
         _readBody();
      }
   }

   void Connection::disconnect()
   {
      if (isConnected())
      {
         m_textStream->close();
      }
   }

   bool Connection::isConnected() const
   {
      return m_textStream->isOpen();
   }


   std::string_view Connection::getPortName() const noexcept
   {
      return m_portName;
   }

   void Connection::_readBody()
   {
      auto _ptr = shared_from_this();
      m_textStream->asyncReadLine(
          [_ptr](const std::error_code &ec, std::string line)
          {
             // This lambda will be called when the line has been read from whatever application
             // transport object is being used.
             if (!ec)    // not in error and avoid dangling pointers
             {
                if (_ptr)
                {
                   OwnedMessage message{ _ptr, std::move(line) };
                   if (_ptr)    // To make sure pointer is still valid
                   {
                      _ptr->m_qMessagesIn.push_back(std::move(message));
                      _ptr->_readBody();
                   }
                   else
                   {
                      std::cout << " Dagling pointer for connection.\n";
                   }
                }
                else
                {
                   std::cout << " Dagling pointer for connection.\n";
                }
             }
             else if (ec)
             {
                // TODO for the future improve error handling
                std::cout << "[" << _ptr->getID() << "] Read Body Fail: " << ec.message() << "\n";
                _ptr->m_textStream->close();
             }
          });
   }
}    // namespace netlib
