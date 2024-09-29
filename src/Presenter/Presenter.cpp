#include <Presenter/Presenter.hpp>

void Presenter::start()
{
   // Pass to the model the view callback through a lambda function
   m_model.startReceivingData(
       [this](SensorData data)
       {
          // Forward data to the View
          m_viewCallback(data);
       });
}

void Presenter::stop()
{
   m_model.stopReceivingData();
}