#pragma once
#include <boost/circular_buffer.hpp>
#include <cstdint>
#include <model/Imodel.hpp>
#include <mutex>
#include <set>
#include <string_view>
#include <unordered_map>

class Plotter
{
public:
    enum class Function : uint8_t
    {
        TIMESTAMP,
        DISPLACEMENT,
        OUTPUTVOLTAGE,
        COIL_CURRENT
    };

    std::unordered_map<Function, std::string_view> c_functionNameMap = {
        {Function::DISPLACEMENT, "displacement"},
        {Function::OUTPUTVOLTAGE, "dac output voltage"},
        {Function::COIL_CURRENT, "coil current"}};

    static constexpr uint16_t c_size{100};

public:
    Plotter() : m_selectedFunctions{} {};
    ~Plotter() = default;
    void Draw(std::string_view label);
    void update(SensorData data);

private:
    void DrawSelection();
    void DrawPlot();
    float &evaluateFunction(Function fun, int idx);

public:
    std::set<Function> m_selectedFunctions;

private:
    boost::circular_buffer<float> timestamps{c_size};
    boost::circular_buffer<float> displacements{c_size};
    boost::circular_buffer<float> voltages{c_size};
    boost::circular_buffer<float> coil_currents{c_size};
    float zero = {0.0f};
    uint16_t m_lastInsertedValue = {};
    std::mutex m_mutex;
};

void render(Plotter &window_obj);
