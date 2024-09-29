#pragma once
#include <model/Imodel.hpp>
#include <mutex>


namespace netlib
{
   template<typename T>
   class ITSQueue;
};

class Model : public IModel
{
   public:
   Model(netlib::ITSQueue<netlib::Message>& _q);

   ~Model()
   {
      stopReceivingData();
   }

   // Start data reception in a separate thread
   void startReceivingData(std::function<void(netlib::Message)> callback) override;
   void stopReceivingData() override
   {
      stopReceiving = true;
   };

   void pushMessage(netlib::Message& _msg) override;

   private:
   std::mutex                         m_mutex;
   std::atomic<bool>                  stopReceiving;
   netlib::ITSQueue<netlib::Message>& m_Q;
};