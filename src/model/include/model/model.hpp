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
   Model(netlib::ITSQueue<std::string>& _q);

   ~Model()
   {
      stopReceivingData();
   }

   // Start data reception in a separate thread
   void startReceivingData(std::function<void(std::string)> callback) override;
   void stopReceivingData() override
   {
      stopReceiving = true;
   };

   void pushMessage(std::string& _msg) override;

   private:
   std::mutex                     m_mutex;
   std::atomic<bool>              stopReceiving;
   netlib::ITSQueue<std::string>& m_Q;
};