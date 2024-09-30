#pragma once
#include <cstdint>
#include <vector>
#include <iostream>

namespace netlib
{
   /**
    * @brief Message class contains a body as std::vector of raw bytes of infomation.
    */
   struct ShittyMessage
   {
      ShittyMessage() = default;

      /**
       * @brief size of the message body
       * @return returns size_t of entire message packet in bytes
       */
      constexpr size_t size() const
      {
         return sbody.size();
      }

      void resize(size_t size)
      {
         sbody.resize(size);
      }

      void clear()
      {
         sbody.clear();
      }

      std::string &getBody()
      {
         return sbody;
      }

      /**
       * @brief Extract a string from the message body. This will consume the entire body as a
       * string.
       * @param data Reference to a std::string where the extracted data will be stored.
       * @return True if the extraction was successful, false otherwise.
       */
      bool extractString(std::string &data)
      {
         if (sbody.empty())
         {
            return false;    // No data to extract
         }

         // use std::move to avoid copying if you want
         data = std::move(sbody);

         // Clear the message body after extracting the data
         sbody.clear();

         return true;
      }

      private:
      std::string sbody;
   };

   /**
    * @brief Forward declare the connection
    */
   class Connection;

   /**
    * @brief An "owned" message is identical to a regular message, but it is associated with
   a connection. On a server, the owner would be the client that sent the message,
   on a client the owner would be the server.
    */
   struct owned_message
   {
      std::shared_ptr<Connection> m_remote = nullptr;
      std::string                 m_msg;
   };
}    // namespace netlib
