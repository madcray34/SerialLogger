#pragma once
#include <model/Imodel.hpp>
#include <string_view>
#include <cstdint>
#include <mutex>

namespace netlib
{
   template<typename T>
   class ITSQueue;
};

class Plotter
{
   static constexpr uint16_t c_size{ 100 };

   public:
   Plotter(netlib::core::ITSQueue<std::string> &_q);
   ~Plotter() = default;
   void Draw(std::string_view label);
   void update(std::string &&data);

   private:
   void DrawSelection();
   void DrawPlot();

   private:
   uint16_t                             m_lastInsertedValue = {};
   netlib::core::ITSQueue<std::string> &m_Q;
};

void render(Plotter &window_obj);
