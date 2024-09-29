#pragma once
#include <cstdint>
#include <vector>
#include <iostream>

namespace netlib
{
   /**
    * @brief Message class contains a body as std::vector of raw bytes of infomation.
    */
   struct Message
   {
      Message() = default;
      std::vector<uint8_t> body;

      /**
       * @brief size of the message body
       * @return returns size_t of entire message packet in bytes
       */
      constexpr size_t size() const
      {
         return body.size();
      }

      void resize(size_t size)
      {
         body.resize(size);
      }

      /**
       * @brief clear the message body.
       */
      constexpr void clear()
      {
         body.clear();
      }

      /**
       * @brief Get a pointer to the message body data.
       * @return A pointer to the underlying data in the body vector.
       */
      uint8_t *data()
      {
         return body.data();    // Return a pointer to the internal data
      }

      /**
       * @brief Pulls any POD-like data form the message buffer. Convenience Operator overloads.
       * These allow us to add and remove stuff from the body vector as if it were a stack, so First
       * in, Last Out. These are a template in itself, because we dont know what data type the user
       * is pushing or popping, so lets allow them all.
       * @NOTE: It assumes the data type is fundamentally Plain Old Data (POD).
       * @TLDR: Serialise & Deserialise into/from a vector.
       * @tparam DataType
       * @param _msg
       * @param data
       * @return message<T>&
       */
      template<typename DataType>
      friend Message &operator>>(Message &_msg, DataType &data)
      {
         // Ensure the data type is trivially copyable
         static_assert(std::is_trivially_copyable<DataType>::value,
                       "Data type must be trivially copyable for deserialization.");

         // Ensure there is enough data to read
         if (_msg.body.size() < sizeof(DataType))
         {
            throw std::runtime_error("Not enough data in message to deserialize.");
         }

         // Cache the location towards the end of the vector where the pulled data starts
         size_t i = _msg.body.size() - sizeof(DataType);

         // Copy the data from the vector into the user variable
         std::memcpy(&data, _msg.body.data() + i, sizeof(DataType));

         // Shrink the vector to remove the read bytes
         _msg.body.resize(i);

         // Return the message object for chaining
         return _msg;
      }

      /** FUTURE IDEAS AFTER TESTING ON HW
       * @brief Extract a string from the message body. This will consume the entire body as a
       * string.
       * @param data Reference to a std::string where the extracted data will be stored.
       * @return True if the extraction was successful, false otherwise.
       */
      //   bool extractString(std::string &data)
      //   {
      //      if (body.empty())
      //      {
      //         return false;    // No data to extract
      //      }

      //      // Copy the body into the string (can also use std::move to avoid copying if you want)
      //      data = std::move(body);

      //      // Clear the message body after extracting the data
      //      body.clear();

      //      return true;
      //   }
      //    std::string body;  // Store the raw message as a string (since we're working with string
      //    data)
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
      Message                     m_msg;
   };
}    // namespace netlib
