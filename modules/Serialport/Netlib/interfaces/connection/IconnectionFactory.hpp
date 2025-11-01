#pragma once
#include <memory>
#include <string_view>
#include <connection/Iconnection.hpp>
#include <tsQueue/ItsQueue.hpp>
#include <tsQueue/types/message.hpp>

#include <boost/asio/io_context.hpp>

namespace netlib
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
      virtual std::shared_ptr<IConnection> create(boost::asio::io_context &asioContext,
                                                  std::string_view         name,
                                                  ITSQueue<OwnedMessage>  &qIn) = 0;
   };
}    // namespace netlib
