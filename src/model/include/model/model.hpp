#pragma once
#include <model/Imodel.hpp>
#include <mutex>
#include <condition_variable>

namespace netlib
{
   template<typename T>
   class ITSQueue;
};

class Model : public IModel
{
   public:
   /**
    * @brief Construct a new Model object
    *
    * @param _qf filtering/viewing queue
    * @param _qs saving to file queue
    */
   Model(netlib::ITSQueue<std::string>& _qf, netlib::ITSQueue<std::string>& _qs,
         const std::string saveFileName);

   ~Model()
   {
      stopReceivingData();
   }

   // Start data reception in a separate thread
   void startReceivingData(std::function<void(std::string&&)> callback) override;
   void stopReceivingData() override
   {
      m_stopReceiving = true;
   };

   void pushMessage(std::string&& _msg) override;

   // Start save in a separate thread
   void startSavingToFile() override;
   void stopSavingToFile() override
   {
      m_stopSaving = true;
   };

   private:
   std::mutex                     m_muxCB;
   std::atomic<bool>              m_stopReceiving;
   std::atomic<bool>              m_stopSaving;
   netlib::ITSQueue<std::string>& m_Qf;
   netlib::ITSQueue<std::string>& m_Qs;
   std::string                    m_savingPath;
};