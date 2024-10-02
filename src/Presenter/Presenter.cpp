#include <Presenter/Presenter.hpp>

void Presenter::start()
{
   // Pass to the model the view callback through a lambda function
   m_model.startReceivingData(
       [this](std::string data)
       {
          // Forward data to the View
          m_viewCallback(data);
       });

   m_model.startSavingToFile();
}

void Presenter::stop()
{
   m_model.stopReceivingData();
   m_model.stopSavingToFile();
}