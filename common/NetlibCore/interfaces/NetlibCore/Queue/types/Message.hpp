#pragma once
#include <cstdint>
#include <iostream>
#include <bit>
#include <memory>
#include <NetlibCore/Connection/IConnection.hpp>

namespace netlib::core
{
   /**
    * @brief Forward declare the connection
    */

   /**
    * @brief An "owned" message is identical to a regular message, but it is associated with
   a connection. On a server, the owner would be the client that sent the message,
   on a client the owner would be the server.
    */

   struct OwnedMessage
   {
      std::shared_ptr<IConnection> m_remote = nullptr;
      std::string                  m_msg;

      OwnedMessage() = default;

      OwnedMessage(const std::shared_ptr<IConnection> &otherRemote, std::string &&otherMsg) noexcept
          : m_remote(otherRemote), m_msg(std::move(otherMsg))
      {}

      // Move constructor
      OwnedMessage(std::shared_ptr<IConnection> &&otherRemote, std::string &&otherMsg) noexcept
          : m_remote(std::move(otherRemote)), m_msg(std::move(otherMsg))
      {}

      // Move constructor
      OwnedMessage(std::shared_ptr<IConnection> &&otherRemote) noexcept
          : m_remote(std::move(otherRemote))
      {}
   };
}    // namespace netlib::core
