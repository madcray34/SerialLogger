#pragma once
#include <cstdint>
#include <functional>
#include <TSQueue/Message.hpp>

class IModel
{
   public:
   virtual ~IModel()                                                              = default;
   virtual void startReceivingData(std::function<void(netlib::Message)> callback) = 0;
   virtual void stopReceivingData()                                               = 0;
   virtual void pushMessage(netlib::Message& _msg)                                = 0;
};