#pragma once
#include <model/Imodel.hpp>
#include <mutex>


class Model : public IModel
{
   public:
   Model() : stopReceiving(false) {};

   ~Model()
   {
      stopReceivingData();
   }

   // Start data reception in a separate thread
   void startReceivingData(std::function<void(SensorData)> callback) override;
   void stopReceivingData() override
   {
      stopReceiving = true;
   };

   private:
   std::mutex        m_mutex;
   std::atomic<bool> stopReceiving;
};