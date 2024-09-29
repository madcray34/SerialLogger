#include <array>
#include <cmath>
#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <iostream>
#include <view/Plotter.hpp>

/**
 * @brief Here we define every aspect of what it should be drawn in the Frame window
 * 
 * @param label 
 */
void Plotter::Draw(std::string_view label)
{

    /**
     * @brief predefined ImGui flags to manage window properties, those will be passed down to the ImGui::Begin function as 3rd argument
     */
    constexpr static auto window_flags = ImGuiWindowFlags_None;

    constexpr static auto windowSize = ImVec2(920.0F, 720.0F);
    constexpr static auto windowPos = ImVec2(0.0F, 0.0F);

    /**
     * @brief Construct a new Im Gui:: Set Next Window Pos and Size objects to properly put into settings the next "imgui sub-window" called with Begin()
     */
    // ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
    ImGui::SetNextWindowSize(windowSize);
    /*
     * @brief Start a new "imgui sub-windows" inside our main frame window (something that is render inside)
     * we must also call then an end window. This Begin, End work like a stack (in general ImGui work like a stack)
     * Whenever we want to create a new window we push data to stack, and at the end the stack pointer must go to where it was before Begin.
     */
    ImGui::Begin(label.data(), nullptr, window_flags);
    /**
     * @brief In between Begin/End we have to create the content of this sub-window      
     */

    DrawSelection();
    DrawPlot();

    ImGui::End();
}


void Plotter::update(SensorData data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    timestamps.push_back(static_cast<float>(data.timestamp));
    displacements.push_back(data.displacement);
    voltages.push_back(data.voltage);
    coil_currents.push_back(data.coil_current);
};

void Plotter::DrawSelection()
{
    for (const auto &[key, value] : c_functionNameMap)
    {
        if (key == Function::TIMESTAMP)
            continue;

        auto selected = m_selectedFunctions.contains(key);

        if (ImGui::Checkbox(value.data(), &selected))
        {
            if (selected)
            {
                m_selectedFunctions.insert(key);
            }
            else
            {
                m_selectedFunctions.erase(key);
            }
        }
    }
}

void Plotter::DrawPlot()
{
    static auto xs = std::array<double, c_size>{};
    static auto ys = std::array<double, c_size>{};

    if (m_selectedFunctions.size() == 0 ||
        m_selectedFunctions.size() == 1 && *m_selectedFunctions.begin() == Function::TIMESTAMP ||
        timestamps.empty())
    {
        ImPlot::BeginPlot("###plot", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoTitle);
        ImPlot::EndPlot();
        return;
    }

    if (ImPlot::BeginPlot("LSM data", ImVec2(-1.0F, -1.0F), ImPlotFlags_NoFrame))
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        ImPlot::SetupAxes("x", "y");
        for (const auto &function : m_selectedFunctions)
        {
            for (int i = 0; i < timestamps.size() - 1; i++)
            {
                xs[i] = i;
                ys[i] = evaluateFunction(function, i);
            }

            const auto plot_label = c_functionNameMap.at(function);
            ImPlot::PlotLine(plot_label.data(),
                             xs.data(),
                             ys.data(),
                             static_cast<int>(timestamps.size()));
        }
        ImPlot::EndPlot();
    }
}

float &Plotter::evaluateFunction(Function fun, int idx)
{
    switch (fun)
    {
    case Function::DISPLACEMENT:
        return displacements[idx];
    case Function::OUTPUTVOLTAGE:
        return voltages[idx];
    case Function::COIL_CURRENT:
        return coil_currents[idx];
    default:
        return zero;
    }
}

void render(Plotter &window_obj)
{
    window_obj.Draw("Plotter");
}
