#pragma once
#include <NetlibCore/Queue/ITSQueue.hpp>
#include <NetlibCore/Queue/types/Message.hpp>
#include <NetlibCore/Connection/IConnection.hpp>

#include <memory>
#include <string_view>

namespace netlib::core
{
   class IConnectionFactory
   {
      public:
      virtual ~IConnectionFactory() = default;

      /**
       * @brief Create a new connection object
       *
       * @param asioContext Reference to asio::io_context
       * @param name Port name
       * @param qIn Reference to incoming message queue
       * @return std::shared_ptr<IConnection>
       */
      virtual std::shared_ptr<IConnection> create(std::string_view        name,
                                                  ITSQueue<OwnedMessage> &qIn) = 0;
   };
}    // namespace netlib::core
