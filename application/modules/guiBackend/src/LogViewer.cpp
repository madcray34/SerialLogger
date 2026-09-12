#include <NetlibCore/Queue/ITSQueue.hpp>
#include <view/LogViewer.hpp>
#include <fmt/format.h>
#include <iostream>
#include <imgui.h>
#include <array>
#include <algorithm>
#include <cmath>


LogViewer::LogViewer(netlib::core::ITSQueue<std::string> &_q) : m_Q(_q)
{}

/**
 * @brief Here we define every aspect of what it should be drawn in the Frame window
 *
 * @param label
 */
void LogViewer::Draw(std::string_view label)
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


void LogViewer::update(std::string &&data)
{
   m_Q.push_back(std::move(data));
   while (m_Q.count() > c_size)
   {
      m_Q.pop_front();
   }
};

void LogViewer::DrawSelection()
{
   // Toggle button pinning the view to the newest message as it arrives. Disable it to freely
   // scroll/select older lines; re-enabling immediately jumps back to the bottom.
   const bool wasAutoScroll = m_autoScroll;
   if (wasAutoScroll)
   {
      ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
   }
   if (ImGui::Button(m_autoScroll ? "Auto-scroll: On" : "Auto-scroll: Off"))
   {
      m_autoScroll = !m_autoScroll;
   }
   if (wasAutoScroll)
   {
      ImGui::PopStyleColor();
   }
}

void LogViewer::copySelectionToClipboard(const std::deque<std::string> &lines) const
{
   if (m_selectionAnchor < 0 || m_selectionCursor < 0)
   {
      return;
   }

   const auto low  = static_cast<std::size_t>(std::min(m_selectionAnchor, m_selectionCursor));
   const auto high = static_cast<std::size_t>(std::max(m_selectionAnchor, m_selectionCursor));

   std::string clipboard;
   for (auto index = low; index <= high && index < lines.size(); ++index)
   {
      clipboard += lines[index];
      clipboard += '\n';
   }
   ImGui::SetClipboardText(clipboard.c_str());
}

void LogViewer::DrawPlot()
{
   ImGui::BeginChild("LogScrollRegion", ImVec2(0.0F, 0.0F), false,
                     ImGuiWindowFlags_HorizontalScrollbar);

   // Render under a read (shared) lock instead of copying the whole queue every frame: with
   // ImGuiListClipper only the visible rows are ever touched, so this scales to huge logs.
   m_Q.withLock(
       [this](const std::deque<std::string> &lines)
       {
          const int lineCount = static_cast<int>(lines.size());

          // Drop a selection that no longer fits (e.g. after the log was cleared).
          if (m_selectionAnchor >= lineCount || m_selectionCursor >= lineCount)
          {
             m_selectionAnchor = -1;
             m_selectionCursor = -1;
          }

          ImGuiListClipper clipper;
          clipper.Begin(lineCount);
          while (clipper.Step())
          {
             for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; ++i)
             {
                const auto &line = lines[static_cast<std::size_t>(i)];
                const bool  selected =
                    m_selectionAnchor >= 0 &&
                    i >= std::min(m_selectionAnchor, m_selectionCursor) &&
                    i <= std::max(m_selectionAnchor, m_selectionCursor);

                ImGui::PushID(i);
                // Selectable's label doubles as its ImGui ID; an empty label avoids any
                // "##"/"###" sequences in arbitrary log text being misinterpreted as ID markup.
                ImGui::Selectable("", selected);
                const bool rowClicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);
                const bool rowHovered = ImGui::IsItemHovered();
                ImGui::SameLine(0.0F, 0.0F);
                ImGui::TextUnformatted(line.c_str());
                if (rowClicked)
                {
                   // A plain click selects just this line; holding Shift extends the range
                   // from the existing anchor (same behaviour a double-click on a single line
                   // produces, since it resolves to the same line being (re)selected).
                   if (!ImGui::GetIO().KeyShift)
                   {
                      m_selectionAnchor = i;
                   }
                   m_selectionCursor = i;
                   m_isDragging      = true;
                }
                else if (m_isDragging && rowHovered &&
                         ImGui::IsMouseDown(ImGuiMouseButton_Left))
                {
                   m_selectionCursor = i;
                }
                ImGui::PopID();
                ImGui::Separator();
             }
          }
          clipper.End();

          if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
          {
             m_isDragging = false;
          }

          const bool wantsCopy = ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C);
          if (wantsCopy && ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows))
          {
             copySelectionToClipboard(lines);
          }
       });

   if (m_autoScroll)
   {
      ImGui::SetScrollHereY(1.0F);
   }

   ImGui::EndChild();
}

void render(LogViewer &window_obj)
{
   static constexpr auto viewName1 = "LogViewer";
   window_obj.Draw(viewName1);
}
