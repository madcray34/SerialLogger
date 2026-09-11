#pragma once
#include <model/Imodel.hpp>
#include <string_view>
#include <cstdint>
#include <cstddef>
#include <deque>
#include <mutex>
#include <string>

namespace netlib
{
   template<typename T>
   class ITSQueue;
};

class LogViewer
{
   // Upper bound on how many lines are retained; old lines are dropped once exceeded.
   static constexpr std::size_t c_size{ 1'000'000 };

   public:
   LogViewer(netlib::core::ITSQueue<std::string> &_q);
   ~LogViewer() = default;
   void Draw(std::string_view label);
   void update(std::string &&data);

   private:
   void DrawSelection();
   void DrawPlot();
   void copySelectionToClipboard(const std::deque<std::string> &lines) const;

   private:
   uint16_t                             m_lastInsertedValue = {};
   netlib::core::ITSQueue<std::string> &m_Q;

   bool m_autoScroll     = true;
   bool m_isDragging     = false;
   int  m_selectionAnchor = -1;
   int  m_selectionCursor = -1;
};

void render(LogViewer &window_obj);
