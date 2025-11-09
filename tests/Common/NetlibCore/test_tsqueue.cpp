#include <gtest/gtest.h>
#include <NetlibCore/Queue/TSQueue.hpp>
#include <thread>
#include <chrono>
#include <memory>
#include <vector>

using namespace std::chrono_literals;
using namespace netlib::core;

class TSQueueTest : public ::testing::Test
{
   protected:
   void SetUp() override
   {
      ASSERT_TRUE(queue.empty());
   }

   TSQueue<int> queue;
};

TEST_F(TSQueueTest, StartsEmpty)
{
   EXPECT_TRUE(queue.empty());
   EXPECT_EQ(queue.count(), static_cast<size_t>(0));
}

TEST_F(TSQueueTest, PushBackAddsElementToBack)
{
   queue.push_back(42);

   EXPECT_FALSE(queue.empty());
   EXPECT_EQ(queue.count(), static_cast<size_t>(1));
   EXPECT_EQ(queue.front(), 42);
   EXPECT_EQ(queue.back(), 42);
}

TEST_F(TSQueueTest, PushFrontAddsElementToFront)
{
   queue.push_back(2);
   queue.push_front(1);

   EXPECT_EQ(queue.count(), static_cast<size_t>(2));
   EXPECT_EQ(queue.front(), 1);
   EXPECT_EQ(queue.back(), 2);
}

TEST_F(TSQueueTest, PopFrontReturnsAndRemovesFrontElement)
{
   queue.push_back(1);
   queue.push_back(2);

   auto value = queue.pop_front();

   EXPECT_EQ(value, 1);
   EXPECT_EQ(queue.count(), static_cast<size_t>(1));
   EXPECT_EQ(queue.front(), 2);
}

TEST_F(TSQueueTest, PopBackReturnsAndRemovesBackElement)
{
   queue.push_back(1);
   queue.push_back(2);

   auto value = queue.pop_back();

   EXPECT_EQ(value, 2);
   EXPECT_EQ(queue.count(), static_cast<size_t>(1));
   EXPECT_EQ(queue.back(), 1);
}

TEST_F(TSQueueTest, IteratorsProvideSnapshot)
{
   queue.push_back(1);
   queue.push_back(2);
   queue.push_back(3);

   // take a thread-safe snapshot and iterate the copy
   const auto             dq = queue.to_deque();
   const std::vector<int> values(dq.begin(), dq.end());
   EXPECT_EQ(values, (std::vector<int>{ 1, 2, 3 }));

   const TSQueue<int>    &constQueue = queue;
   const auto             constDq    = constQueue.to_deque();
   const std::vector<int> constValues(constDq.begin(), constDq.end());
   EXPECT_EQ(constValues, values);
}

TEST_F(TSQueueTest, WaitBlocksUntilItemIsAvailable)
{
   std::thread producer(
       [this]()
       {
          std::this_thread::sleep_for(5ms);
          queue.push_back(99);
       });

   queue.wait();
   producer.join();

   EXPECT_FALSE(queue.empty());
   EXPECT_EQ(queue.front(), 99);
}

TEST_F(TSQueueTest, WaitWithExitFlagReturnsWhenItemArrives)
{
   bool        exitFlag = false;
   std::thread producer(
       [this]()
       {
          std::this_thread::sleep_for(5ms);
          queue.push_back(17);
       });

   queue.wait(exitFlag);
   producer.join();

   EXPECT_EQ(queue.count(), static_cast<size_t>(1));
   EXPECT_EQ(queue.front(), 17);
}

TEST_F(TSQueueTest, WaitWithExitFlagHonorsExit)
{
   bool exitFlag = true;

   auto start = std::chrono::steady_clock::now();
   queue.wait(exitFlag);
   auto duration = std::chrono::steady_clock::now() - start;

   EXPECT_LT(duration, 50ms);
   EXPECT_TRUE(queue.empty());
}

TEST_F(TSQueueTest, WaitForTimesOutWhenQueueIsEmpty)
{
   const bool signaled = queue.wait_for(5ms);

   EXPECT_FALSE(signaled);
   EXPECT_TRUE(queue.empty());
}

TEST_F(TSQueueTest, WaitForReturnsTrueWhenItemArrives)
{
   std::thread producer(
       [this]()
       {
          std::this_thread::sleep_for(5ms);
          queue.push_back(21);
       });

   const bool signaled = queue.wait_for(50ms);
   producer.join();

   EXPECT_TRUE(signaled);
   EXPECT_EQ(queue.pop_front(), 21);
}
