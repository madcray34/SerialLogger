#pragma once
#include <deque>
#include <mutex>
#include <condition_variable>
#include <TSQueue/ITSQueue.hpp>

namespace netlib
{
   /**
    * @brief Thread safe, double ended, waiting queue.
    * @tparam T - Data type
    */
   template<typename T>
   class TSQueue : public ITSQueue<T>
   {
      public:
      TSQueue()                  = default;
      TSQueue(const TSQueue<T>&) = delete;
      virtual ~TSQueue() override
      {
         clear();
      }

      public:
      /**
       * @brief Return and maintains item at front of Queue.
       * @return const T&
       */
      const T& front() override
      {
         std::scoped_lock lock(muxQueue);
         return deqQueue.front();
      }

      /**
       * @brief Returns and maintains item at back of Queue.
       * @return const T&
       */
      const T& back() override
      {
         std::scoped_lock lock(muxQueue);
         return deqQueue.back();
      }

      /**
       * @brief Removes and returns item from front of Queue.
       * @return T
       */
      T pop_front() override
      {
         std::scoped_lock lock(muxQueue);
         auto             t = std::move(deqQueue.front());
         deqQueue.pop_front();
         return t;
      }

      /**
       * @brief Removes and returns item from back of Queue.
       * @return T
       */
      T pop_back() override
      {
         std::scoped_lock lock(muxQueue);
         auto             t = std::move(deqQueue.back());
         deqQueue.pop_back();
         return t;
      }

      /**
       * @brief Adds an item to back of Queue.
       * @param item
       */
      void push_back(const T& item) override
      {
         std::scoped_lock lock(muxQueue);
         deqQueue.emplace_back(std::move(item));

         std::unique_lock<std::mutex> ul(muxBlocking);
         cvBlocking.notify_one();
      }

      /**
       * @brief Adds an item to front of Queue.
       * @param item
       */
      void push_front(const T& item) override
      {
         std::scoped_lock lock(muxQueue);
         deqQueue.emplace_front(std::move(item));

         std::unique_lock<std::mutex> ul(muxBlocking);
         cvBlocking.notify_one();
      }

      /**
       * @brief Returns true if Queue has no items.
       * @return true
       * @return false
       */
      bool empty() override
      {
         std::scoped_lock lock(muxQueue);
         return deqQueue.empty();
      }

      /**
       * @brief Returns number of items in Queue.
       * @return size_t
       */
      size_t count() override
      {
         std::scoped_lock lock(muxQueue);
         return deqQueue.size();
      }

      /**
       * @brief Clears Queue.
       */
      void clear() override
      {
         std::scoped_lock lock(muxQueue);
         deqQueue.clear();
      }

      /**
       * @brief wait.
       */
      void wait() override
      {
         while (empty())
         {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.wait(ul);
         }
      }

      protected:
      std::mutex              muxQueue;
      std::deque<T>           deqQueue;
      std::condition_variable cvBlocking;
      std::mutex              muxBlocking;
   };
}    // namespace netlib