#pragma once
#include <model/Imodel.hpp>
#include <mutex>


class Model : public IModel
{
public:
    Model() {};
    ~Model() = default;

    // Start data reception in a separate thread
    void startReceivingData(std::function<void(SensorData)> callback);

private:
    std::mutex m_mutex;
};