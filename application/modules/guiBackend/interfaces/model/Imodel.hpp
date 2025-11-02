#pragma once
#include <NetlibCore/Queue/types/Message.hpp>
#include <functional>
#include <cstdint>
#include <string>

class IModel
{
   public:
   virtual ~IModel()                                                             = default;
   virtual void startReceivingData(std::function<void(std::string &&)> callback) = 0;
   virtual void stopReceivingData()                                              = 0;
   virtual void pushMessage(netlib::OwnedMessage &&_msg)                         = 0;
   virtual void startSavingToFile()                                              = 0;
   virtual void stopSavingToFile()                                               = 0;
   virtual bool isSavingToFile() const                                           = 0;
};