#pragma once

class IPresenter
{
   public:
   virtual ~IPresenter() = default;
   virtual void start()  = 0;
};