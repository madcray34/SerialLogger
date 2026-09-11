#pragma once
#include <NetlibCore/Queue/ITSQueue.hpp>

#include <condition_variable>
#include <mutex>

namespace netlib::core
{
   /**
    * @brief Thread safe, double ended, waiting queue.
    * @tparam T - Data type
    */
   template<typename T>
   class TSQueue : public ITSQueue<T>
   {
      public:
      TSQueue()                   = default;
      TSQueue(const TSQueue<T> &) = delete;
      virtual ~TSQueue() override
      {
         clear();
      }

      /**
       * @brief Return a copy of the queue contents for safe iteration.
       */
      std::deque<T> to_deque() const override
      {
         std::unique_lock lock(muxQueue);
         return deqQueue;
      }

      /**
       * @brief Runs @p fn with read access to the underlying deque while holding the lock.
       * Avoids copying the whole container, unlike to_deque().
       */
      void withLock(const std::function<void(const std::deque<T> &)> &fn) const override
      {
         std::unique_lock lock(muxQueue);
         fn(deqQueue);
      }

      /**
       * @brief Return and maintains item at front of Queue.
       * @return const T&
       */
      const T &front() override
      {
         std::unique_lock lock(muxQueue);
         return deqQueue.front();
      }

      /**
       * @brief Returns and maintains item at back of Queue.
       * @return const T&
       */
      const T &back() override
      {
         std::unique_lock lock(muxQueue);
         return deqQueue.back();
      }

      /**
       * @brief Removes and returns item from front of Queue.
       * @return T
       */
      T pop_front() override
      {
         std::unique_lock lock(muxQueue);
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
         std::unique_lock lock(muxQueue);
         T                result = std::move(deqQueue.back());
         deqQueue.pop_back();
         return result;
      }

      /**
       * @brief Adds an item to back of Queue.
       * @param item
       */
      void push_back(const T &item) override
      {
         std::unique_lock lock(muxQueue);
         deqQueue.emplace_back(item);
         lock.unlock();
         cvBlocking.notify_one();
      }

      /**
       * @brief Adds an item to back of Queue.
       * @param item
       */
      void push_back(T &&item) override
      {
         std::unique_lock lock(muxQueue);
         deqQueue.emplace_back(std::move(item));
         lock.unlock();
         cvBlocking.notify_one();
      }

      /**
       * @brief Adds an item to front of Queue.
       * @param item
       */
      void push_front(const T &item) override
      {
         std::unique_lock lock(muxQueue);
         deqQueue.emplace_front(item);
         lock.unlock();
         cvBlocking.notify_one();
      }

      /**
       * @brief Adds an item to front of Queue.
       * @param item
       */
      void push_front(T &&item) override
      {
         std::unique_lock lock(muxQueue);
         deqQueue.emplace_front(std::move(item));
         lock.unlock();
         cvBlocking.notify_one();
      }

      /**
       * @brief Returns true if Queue has no items.
       * @return true
       * @return false
       */
      bool empty() override
      {
         std::unique_lock lock(muxQueue);
         return deqQueue.empty();
      }

      /**
       * @brief Returns number of items in Queue.
       * @return size_t
       */
      size_t count() override
      {
         std::unique_lock lock(muxQueue);
         return deqQueue.size();
      }

      /**
       * @brief Clears Queue.
       */
      void clear() override
      {
         std::unique_lock lock(muxQueue);
         deqQueue.clear();
         lock.unlock();
         cvBlocking.notify_one();
      }

      /**
       * @brief wait.
       */
      void wait() override
      {
         std::unique_lock lock(muxQueue);
         cvBlocking.wait(lock, [this]() { return !deqQueue.empty(); });
      }

      void wait(const bool &exit) override
      {
         std::unique_lock lock(muxQueue);

         // Wait until either exit is true, or the queue is no longer empty
         cvBlocking.wait(lock, [&]() { return exit || !deqQueue.empty(); });
      }

      bool wait_for(std::chrono::milliseconds timeout) override
      {
         std::unique_lock lock(muxQueue);
         return cvBlocking.wait_for(lock, timeout, [this]() { return !deqQueue.empty(); });
      }

      protected:
      // A single mutex guards both the deque and the condition variable: using separate
      // locks for data access and for wait/notify previously caused a lock-order inversion
      // (push_back took muxQueue then muxBlocking, while wait_for took muxBlocking then
      // muxQueue via empty()), which could deadlock the queue under sustained concurrent
      // producer/consumer traffic.
      mutable std::mutex     muxQueue;
      std::deque<T>          deqQueue;
      std::condition_variable cvBlocking;

      private:
      /**
       * NOTE:
       * These begin()/end() helpers acquire the lock only while the iterator is retrieved.
       * The lock is released when the function returns, so the returned iterator refers to the
       * internal deque and is NOT safe against concurrent mutations (push/pop/clear) by other
       * threads. For thread-safe iteration, use to_deque() to get a snapshot copy.
       */
      typename std::deque<T>::iterator begin()
      {
         std::unique_lock lock(muxQueue);
         return deqQueue.begin();
      }

      typename std::deque<T>::iterator end()
      {
         std::unique_lock lock(muxQueue);
         return deqQueue.end();
      }

      typename std::deque<T>::const_iterator begin() const
      {
         std::unique_lock lock(muxQueue);
         return deqQueue.begin();
      }

      typename std::deque<T>::const_iterator end() const
      {
         std::unique_lock lock(muxQueue);
         return deqQueue.end();
      }
   };
}    // namespace netlib::core