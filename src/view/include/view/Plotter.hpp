#pragma once
#include <cstdint>
#include <model/Imodel.hpp>
#include <mutex>
#include <string_view>
#include <TSQueue/Message.hpp>

namespace netlib
{
   template<typename T>
   class ITSQueue;
};

class Plotter
{
   static constexpr uint16_t c_size{ 100 };

   public:
   Plotter(netlib::ITSQueue<netlib::Message>& _q);
   ~Plotter() = default;
   void Draw(std::string_view label);
   void update(netlib::Message data);

   private:
   void DrawSelection();
   void DrawPlot();

   private:
   uint16_t                           m_lastInsertedValue = {};
   netlib::ITSQueue<netlib::Message>& m_Q;
};

void render(Plotter& window_obj);
