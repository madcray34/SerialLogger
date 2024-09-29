#include <chrono>
#include <model/model.hpp>
#include <thread>
#include <TSQueue/ITSQueue.hpp>

Model::Model(netlib::ITSQueue<netlib::Message>& _q) : stopReceiving(false), m_Q(_q)
{}

void Model::startReceivingData(std::function<void(netlib::Message)> callback)
{
   // Create a new thread that is updating data in the background
   std::thread(
       [this, callback]()
       {
          while (!stopReceiving)
          {
             // Wait for a message to arrive in the queue
             m_Q.wait();

             // Process all messages in the queue
             while (!m_Q.empty())
             {
                netlib::Message message = m_Q.pop_front();

                // Lock for data update and call the presenter callback
                std::lock_guard<std::mutex> lock(m_mutex);
                callback(message);
             }
          }
       })
       .detach();    // Run in the background
}

void Model::pushMessage(netlib::Message& _msg)
{
   m_Q.push_back(std::move(_msg));
}