#pragma once
#include <presenter/Ipresenter.hpp>
#include <model/Imodel.hpp>
#include <functional>
#include <mutex>

class Presenter : public IPresenter
{
   public:
   Presenter(IModel &model, std::function<void(std::string &&)> callback)
       : m_model{ model }, m_viewCallback{ callback } {};
   ~Presenter() = default;

   // Start data reception in a separate thread
   void start();
   void stop();

   private:
   IModel                          &m_model;
   std::function<void(std::string)> m_viewCallback;
};