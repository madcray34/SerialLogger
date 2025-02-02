#include <tsQueue/ItsQueue.hpp>
#include <view/plotter.hpp>
#include <fmt/format.h>
#include <iostream>
#include <implot.h>
#include <imgui.h>
#include <array>
#include <cmath>


Plotter::Plotter(netlib::ITSQueue<std::string> &_q) : m_Q(_q)
{}

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


void Plotter::update(std::string &&data)
{
   m_Q.push_back(std::move(data));
   if (m_Q.count() > c_size)
   {
      m_Q.pop_front();
   }
};

void Plotter::DrawSelection()
{
   // Empty function for now it will be usefull in future
   static constexpr auto temporary1 = "WILL BE FILLED IN THE FUTURE WITH FILTERING OPTIONS ....";
   ImGui::Text(temporary1);
}

void Plotter::DrawPlot()
{
   // Range-based for loop
   for (const auto &it : m_Q)
   {
      ImGui::Text(it.c_str());
      ImGui::Separator();
   }
}

void render(Plotter &window_obj)
{
   static constexpr auto viewName1 = "Plotter";
   window_obj.Draw(viewName1);
}
