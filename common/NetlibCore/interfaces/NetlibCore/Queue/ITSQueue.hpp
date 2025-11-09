#pragma once
#include <deque>
#include <chrono>

/**
 * @brief Interface for Thread Safe double ended, waiting queue. Practical for mockups.
 *
 */
namespace netlib::core
{
   template<typename T>
   class ITSQueue
   {
      public:
      using iterator                                  = typename std::deque<T>::iterator;
      using const_iterator                            = typename std::deque<T>::const_iterator;
      virtual ~ITSQueue()                             = default;
      virtual std::deque<T> to_deque() const          = 0;
      virtual const T      &front()                   = 0;
      virtual const T      &back()                    = 0;
      virtual T             pop_front()               = 0;
      virtual T             pop_back()                = 0;
      virtual void          push_back(const T &item)  = 0;
      virtual void          push_back(T &&item)       = 0;
      virtual void          push_front(const T &item) = 0;
      virtual void          push_front(T &&item)      = 0;
      virtual bool          empty()                   = 0;
      virtual size_t        count()                   = 0;
      virtual void          clear()                   = 0;
      virtual void          wait()                    = 0;
      virtual void          wait(const bool &exit)    = 0;
      virtual bool          wait_for(std::chrono::milliseconds timeout) = 0;
   };
}    // namespace netlib::core