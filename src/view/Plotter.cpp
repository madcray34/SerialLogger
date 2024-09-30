#include <array>
#include <cmath>
#include <fmt/format.h>
#include <imgui.h>
#include <implot.h>
#include <iostream>
#include <view/Plotter.hpp>
#include <TSQueue/ITSQueue.hpp>


Plotter::Plotter(netlib::ITSQueue<std::string> &_q) : m_Q(_q)
{
   m_Q.resize(static_cast<size_t>(1000));
}

/**
 * @brief Here we define every aspect of what it should be drawn in the Frame window
 *
 * @param label
 */
void Plotter::Draw(std::string_view label)
{
   /**
    * @brief predefined ImGui flags to manage window properties, those will be passed down to the
    * ImGui::Begin function as 3rd argument
    */
   constexpr static auto window_flags = ImGuiWindowFlags_None;

   constexpr static auto windowSize = ImVec2(920.0F, 720.0F);
   constexpr static auto windowPos  = ImVec2(0.0F, 0.0F);

   /**
    * @brief Construct a new Im Gui:: Set Next Window Pos and Size objects to properly put into
    * settings the next "imgui sub-window" called with Begin()
    */
   // ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
   ImGui::SetNextWindowSize(windowSize);
   /*
    * @brief Start a new "imgui sub-windows" inside our main frame window (something that is render
    * inside) we must also call then an end window. This Begin, End work like a stack (in general
    * ImGui work like a stack) Whenever we want to create a new window we push data to stack, and at
    * the end the stack pointer must go to where it was before Begin.
    */
   ImGui::Begin(label.data(), nullptr, window_flags);
   /**
    * @brief In between Begin/End we have to create the content of this sub-window
    */

   DrawSelection();
   ImGui::Separator();
   DrawPlot();

   ImGui::End();
}


void Plotter::update(std::string &data)
{
   m_Q.push_back(data);
};

void Plotter::DrawSelection()
{
   // Empty function for now it will be usefull in future
   ImGui::Text("WILL BE FILLED IN THE FUTURE WITH FILTERING OPTIONS ....");
}

void Plotter::DrawPlot()
{
   // Range-based for loop
   std::string dataString{};
   for (auto &item : m_Q)
   {
      ImGui::Text("%s", item.c_str());
      ImGui::Separator();
   }
}

void render(Plotter &window_obj)
{
   window_obj.Draw("Plotter");
}
