#pragma once
#include <cstdint>
#include <functional>
#include <string>
class IModel
{
   public:
   virtual ~IModel()                                                          = default;
   virtual void startReceivingData(std::function<void(std::string)> callback) = 0;
   virtual void stopReceivingData()                                           = 0;
   virtual void pushMessage(std::string& _msg)                                = 0;
};