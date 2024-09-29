#include <Presenter/Presenter.hpp>

void Presenter::start()
{
   // Pass to the model the view callback through a lambda function
   m_model.startReceivingData(
       [this](netlib::Message data)
       {
          // Forward data to the View
          m_viewCallback(data);
       });
}

void Presenter::stop()
{
   m_model.stopReceivingData();
}