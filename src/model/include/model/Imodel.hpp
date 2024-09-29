#pragma once
#include <cstdint>
#include <functional>

struct SensorData
{
    int64_t timestamp;
    float displacement;
    float voltage;
    float coil_current;
};

class IModel
{
public:
    virtual void startReceivingData(std::function<void(SensorData)> callback) = 0;
};