#include <chrono>
#include <model/model.hpp>
#include <thread>
#include <TSQueue/ITSQueue.hpp>
#include <fstream>

Model::Model(netlib::ITSQueue<std::string>& _qf, netlib::ITSQueue<std::string>& _qs,
             const std::string saveFileName)
    : m_stopReceiving(false), m_stopSaving(false), m_Qf(_qf), m_Qs(_qs), m_savingPath(saveFileName)
{}

void Model::startReceivingData(std::function<void(std::string&&)> callback)
{
   // Create a new thread that is updating data in the background
   std::thread(
       [this, callback]()
       {
          while (!m_stopReceiving)
          {
             // Wait for a message to arrive in the queue
             m_Qf.wait();

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

// Separate thread to handle saving strings to a file periodically
void Model::startSavingToFile()
{
   std::thread(
       [this]()
       {
          std::ofstream outFile(m_savingPath, std::ios::app);
          if (!outFile.is_open())
          {
             return;
          }

          while (!m_stopSaving)
          {
             // Wait for a message to arrive in the queue
             m_Qs.wait();
             std::string message = m_Qs.pop_front();    // Now you move the string for file
                                                        // saving
             outFile << message << std::endl;           // Save message to file
             outFile.flush();                           // Ensure data is written to disk
          }

          outFile.close();    // Close file when done
       })
       .detach();    // Run in the background
}

void Model::pushMessage(std::string&& _msg)
{
   std::string copy = _msg;
   m_Qf.push_back(std::move(_msg));
   m_Qs.push_back(std::move(copy));
}