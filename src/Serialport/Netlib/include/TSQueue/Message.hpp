#pragma once
#include <cstdint>
#include <iostream>
#include <bit>

namespace netlib
{
   /**
    * @brief Forward declare the connection
    */
   class Connection;

   /**
    * @brief An "owned" message is identical to a regular message, but it is associated with
   a connection. On a server, the owner would be the client that sent the message,
   on a client the owner would be the server.
    */

   struct OwnedMessage
   {
      std::shared_ptr<Connection> m_remote = nullptr;
      std::string                 m_msg;

      OwnedMessage() = default;

      // Move constructor
      OwnedMessage(std::shared_ptr<Connection>&& otherRemote, std::string&& otherMsg) noexcept
          : m_remote(std::move(otherRemote)), m_msg(std::move(otherMsg))
      {}

      // Move constructor
      OwnedMessage(std::shared_ptr<Connection>&& otherRemote) noexcept
          : m_remote(std::move(otherRemote))
      {}
   };
}    // namespace netlib
