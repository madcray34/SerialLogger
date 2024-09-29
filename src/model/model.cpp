#include <chrono>
#include <model/model.hpp>
#include <thread>


void Model::startReceivingData(std::function<void(SensorData)> callback)
{
    //Create a new thread that is updating data in the background
    std::thread([this, callback]() {
        while (true)
        {
            // Simulate receiving data every 10 milliseconds
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            // Generate some dummy data (replace this with actual RPC data reception)
            SensorData data{std::chrono::duration_cast<std::chrono::seconds>(
                                std::chrono::system_clock::now().time_since_epoch())
                                .count(),
                            static_cast<float>(rand() % 100),
                            static_cast<float>((rand() % 500) / 10.0),
                            static_cast<float>((rand() % 200) / 10.0)};

            // Lock for data update
            std::lock_guard<std::mutex> lock(m_mutex);

            // Call the presenter callback with new data
            callback(data);
        }
    }).detach(); // Run in the background
}