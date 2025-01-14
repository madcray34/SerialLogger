#pragma once
#include <cstdint>
#include <string_view>

namespace netlib
{
   static constexpr uint16_t c_bufferCapacity = 1 << 14;
   /**
    * @brief Interface class for the connection object.
    *
    */
   class IConnection
   {
      /**
       * @brief This ID is used system wide - its how clients will understand other clients exist
       across the whole system.
       * @return uint32_t
       */
      virtual uint32_t getID() const = 0;

      /**
       * @brief Connect to the client, i.e. the serial port provided.
       * @param uid
       */
      virtual void connectToClient(uint32_t uid = 0) = 0;

      /**
       * @brief disconnect.
       */
      virtual void disconnect() = 0;

      /**
       * @brief Wheter there is a connection active or not
       * @return boolean.
       */
      virtual bool isConnected() const = 0;

      /**
       * @brief Get the Port Name object
       *
       * @return std::string
       */
      virtual std::string_view getPortName() const = 0;
   };
}    // namespace netlib