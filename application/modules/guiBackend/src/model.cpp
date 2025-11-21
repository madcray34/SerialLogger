#include <NetlibCore/Queue/ITSQueue.hpp>
#include <model/Model.hpp>
#include <fstream>
#include <chrono>
#include <thread>

Model::Model(netlib::core::ITSQueue<std::string> &_qf, netlib::core::ITSQueue<std::string> &_qs,
             const std::string saveFileName)
    : m_stopReceiving(false)
    , m_stopSaving(true)
    , m_savingThreadRunning(false)
    , m_muxCB()
    , m_Qf(_qf)
    , m_Qs(_qs)
    , m_savingThread()
    , m_savingPath(saveFileName)
{}

void Model::startReceivingData(std::function<void(std::string &&)> callback)
{
   // Create a new thread that is updating data in the background
   std::thread(
       [this, callback]()
       {
          while (!m_stopReceiving)
          {
             // Wait for a message to arrive in the queue
             m_Qf.wait(m_stopReceiving);

             if (m_stopReceiving)
                break;    // Check for stop signal after waiting

             // Process all messages in the queue
             while (!m_Qf.empty())
             {
                std::string message = m_Qf.pop_front();

                // Lock for data update and call the presenter callback
                std::lock_guard<std::mutex> lock(m_muxCB);
                callback(std::move(message));
             }
          }
       })
       .detach();    // Run in the background
}


void Model::pushMessage(netlib::core::OwnedMessage &&_msg)
{
   if (!m_stopSaving)
   {
      std::string copy = _msg.m_msg;
      m_Qs.push_back(std::move(copy));
   }
   m_Qf.push_back(std::move(_msg.m_msg));
}

// Function to start the saving thread
void Model::startSavingToFile()
{
   // Check if the thread is already running
   if (m_savingThreadRunning)
   {
      return;
   }

   m_stopSaving          = false;    // Ensure the stop flag is reset
   m_savingThreadRunning = true;

   // Launch a new thread for saving
   m_savingThread = std::thread(
       [this]()
       {
          std::ofstream outFile(m_savingPath, std::ios::app);
          if (!outFile.is_open())
          {
             // std::cerr << "Failed to open the file!" << std::endl;
             return;
          }

          while (!m_stopSaving)
          {
             // Wait for a message to arrive in the queue
             m_Qs.wait(m_stopSaving);

             if (m_stopSaving)
                break;    // Check for stop signal after waiting

             std::string message = m_Qs.pop_front();    // Get the message from the queue
             outFile << message;                        // Write message to file
             outFile.flush();                           // Ensure data is written to disk
          }

          outFile.close();    // Close file when done
          m_savingThreadRunning = false;
       });
}

void Model::stopSavingToFile()
{
   if (!m_savingThreadRunning)
   {
      // std::cout << "File saving is not running." << std::endl;
      return;
   }

   m_stopSaving = true;    // Signal to stop the thread
   m_Qs.clear();

   if (m_savingThread.joinable())
   {
      m_savingThread.join();    // Wait for the thread to finish
   }

   // std::cout << "File saving stopped." << std::endl;
}