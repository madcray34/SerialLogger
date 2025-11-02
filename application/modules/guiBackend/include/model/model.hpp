#pragma once
#include <condition_variable>
#include <model/Imodel.hpp>
#include <thread>
#include <atomic>
#include <mutex>

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
   Model(netlib::core::ITSQueue<std::string> &_qf, netlib::core::ITSQueue<std::string> &_qs,
         const std::string saveFileName);

   ~Model()
   {
      stopReceivingData();
      stopSavingToFile();
   }

   // Start data reception in a separate thread
   void startReceivingData(std::function<void(std::string &&)> callback) override;
   void stopReceivingData() override
   {
      m_stopReceiving = true;
   };

   void pushMessage(netlib::core::OwnedMessage &&_msg) override;

   // Start save in a separate thread
   void startSavingToFile() override;
   void stopSavingToFile() override;
   bool isSavingToFile() const
   {
      return !m_stopSaving;
   }

   private:
   std::mutex m_muxCB;

   /**
    * @brief Handling callback to UI
    */
   std::atomic<bool> m_stopReceiving;

   /**
    * @brief Handling saving Thread
    */
   std::atomic<bool> m_stopSaving;
   std::atomic<bool> m_savingThreadRunning;    // Track if the thread is running

   /**
    * @brief Queues for async operations:
    * m_Qf: Thread Safe Double ended queue for handling callback to UI
    * m_Qs: Thread Safe Double ended queue for handling saving to file
    */
   netlib::core::ITSQueue<std::string> &m_Qf;
   netlib::core::ITSQueue<std::string> &m_Qs;

   /**
    * @brief Thread for saving to the file
    */
   std::thread m_savingThread;
   std::string m_savingPath;
};