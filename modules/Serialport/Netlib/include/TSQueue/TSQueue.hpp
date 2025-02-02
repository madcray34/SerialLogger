#pragma once
#include <shared_mutex>
#include <condition_variable>
#include <tsQueue/ItsQueue.hpp>

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

      typename std::deque<T>::iterator begin() override
      {
         std::shared_lock lock(muxQueue);
         return deqQueue.begin();
      }

      typename std::deque<T>::iterator end() override
      {
         std::shared_lock lock(muxQueue);
         return deqQueue.end();
      }

      typename std::deque<T>::const_iterator begin() const override
      {
         std::shared_lock lock(muxQueue);
         return deqQueue.begin();
      }

      typename std::deque<T>::const_iterator end() const override
      {
         std::shared_lock lock(muxQueue);
         return deqQueue.end();
      }

      public:
      /**
       * @brief Return and maintains item at front of Queue.
       * @return const T&
       */
      const T& front() override
      {
         std::shared_lock lock(muxQueue);
         return deqQueue.front();
      }

      /**
       * @brief Returns and maintains item at back of Queue.
       * @return const T&
       */
      const T& back() override
      {
         std::shared_lock lock(muxQueue);
         return deqQueue.back();
      }

      /**
       * @brief Removes and returns item from front of Queue.
       * @return T
       */
      T pop_front() override
      {
         std::scoped_lock lock(muxQueue);
         T                result = std::move(deqQueue.front());
         deqQueue.pop_front();
         return result;
      }

      /**
       * @brief Removes and returns item from back of Queue.
       * @return T
       */
      T pop_back() override
      {
         std::scoped_lock lock(muxQueue);
         T                result = std::move(deqQueue.back());
         deqQueue.pop_back();
         return result;
      }

      /**
       * @brief Adds an item to back of Queue.
       * @param item
       */
      void push_back(const T& item) override
      {
         std::scoped_lock lock(muxQueue);
         bool             wasEmpty = deqQueue.empty();
         deqQueue.emplace_back(item);
         if (wasEmpty)
         {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
         }
      }

      /**
       * @brief Adds an item to back of Queue.
       * @param item
       */
      void push_back(T&& item) override
      {
         std::scoped_lock lock(muxQueue);
         bool             wasEmpty = deqQueue.empty();
         deqQueue.emplace_back(std::move(item));
         if (wasEmpty)
         {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
         }
      }

      /**
       * @brief Adds an item to front of Queue.
       * @param item
       */
      void push_front(const T& item) override
      {
         std::scoped_lock lock(muxQueue);
         bool             wasEmpty = deqQueue.empty();
         deqQueue.emplace_front(item);
         if (wasEmpty)
         {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
         }
      }

      /**
       * @brief Adds an item to front of Queue.
       * @param item
       */
      void push_front(T&& item) override
      {
         std::scoped_lock lock(muxQueue);
         bool             wasEmpty = deqQueue.empty();
         deqQueue.emplace_front(std::move(item));

         if (wasEmpty)
         {
            std::unique_lock<std::mutex> ul(muxBlocking);
            cvBlocking.notify_one();
         }
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
         std::unique_lock<std::mutex> ul(muxBlocking);
         cvBlocking.notify_one();
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

      void wait(const bool& exit) override
      {
         std::unique_lock<std::mutex> ul(muxBlocking);

         // Wait until either exit is true, or the queue is no longer empty
         cvBlocking.wait(ul, [&]() { return exit || !empty(); });
      }

      template<typename Rep, typename Period>
      bool wait_for(const std::chrono::duration<Rep, Period>& timeout)
      {
         std::unique_lock<std::mutex> ul(muxBlocking);
         return cvBlocking.wait_for(ul, timeout, [this]() { return !this->empty(); });
      }

      protected:
      mutable std::shared_mutex muxQueue;
      std::deque<T>             deqQueue;
      std::condition_variable   cvBlocking;
      std::mutex                muxBlocking;
   };
}    // namespace netlib