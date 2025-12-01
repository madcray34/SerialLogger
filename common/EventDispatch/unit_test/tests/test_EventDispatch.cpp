#include <EventDispatch/EventDispatchMock.hpp>
#include <EventDispatch/EventEmitter.hpp>
#include <EventDispatch/AwaitableEventEmitter.hpp>
#include <EventDispatch/EventHandler.hpp>
#include <EventDispatch/AwaitableEventHandler.hpp>
#include <std/thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace tanlibs;
using namespace testing;

using namespace std::chrono_literals;

TEST(EventDispatch, EventEmitterTestConnect)
{
    eventdispatch::EventEmitterMock<int> EventEmitter;

    EXPECT_CALL(EventEmitter, connect(_)).Times(1);
    eventdispatch::EventHandler<int> EventHandler(EventEmitter);

    EXPECT_CALL(EventEmitter, disconnect(_)).Times(1);
}

TEST(EventDispatch, createInstance)
{
    eventdispatch::EventEmitter<int>   EventEmitter;
    eventdispatch::EventHandlerMock<int> EventHandler;
    EventEmitter.connect(EventHandler);
    EXPECT_CALL(EventHandler, execute(10)).Times(1);
    EventEmitter.emit(10);
}

TEST(EventDispatch, multipleEventHandlers)
{
    eventdispatch::EventEmitter<int>   EventEmitter;
    eventdispatch::EventHandlerMock<int> EventHandler1;
    eventdispatch::EventHandlerMock<int> EventHandler2;
    eventdispatch::EventHandlerMock<int> EventHandler3;

    EventEmitter.connect(EventHandler1);
    EventEmitter.connect(EventHandler2);
    EventEmitter.connect(EventHandler3);

    EXPECT_CALL(EventHandler1, execute(10)).Times(1);
    EXPECT_CALL(EventHandler2, execute(10)).Times(1);
    EXPECT_CALL(EventHandler3, execute(10)).Times(1);
    EventEmitter.emit(10);
}

TEST(EventDispatch, multipleEventHandlersDisconnectOne)
{
    eventdispatch::EventEmitter<int>   EventEmitter;
    eventdispatch::EventHandlerMock<int> EventHandler1;
    eventdispatch::EventHandlerMock<int> EventHandler2;
    eventdispatch::EventHandlerMock<int> EventHandler3;

    EventEmitter.connect(EventHandler1);
    EventEmitter.connect(EventHandler2);
    EventEmitter.connect(EventHandler3);

    EXPECT_CALL(EventHandler1, execute(10)).Times(1);
    EXPECT_CALL(EventHandler2, execute(10)).Times(1);
    EXPECT_CALL(EventHandler3, execute(10)).Times(1);
    EventEmitter.emit(10);

    EventEmitter.disconnect(EventHandler2);

    EXPECT_CALL(EventHandler1, execute(10)).Times(1);
    EXPECT_CALL(EventHandler2, execute(10)).Times(0);
    EXPECT_CALL(EventHandler3, execute(10)).Times(1);
    EventEmitter.emit(10);
}

TEST(EventDispatch, multipleEventHandlersDisconnectAll)
{
    eventdispatch::EventEmitter<int>   EventEmitter;
    eventdispatch::EventHandlerMock<int> EventHandler1;
    eventdispatch::EventHandlerMock<int> EventHandler2;
    eventdispatch::EventHandlerMock<int> EventHandler3;

    EventEmitter.connect(EventHandler1);
    EventEmitter.connect(EventHandler2);
    EventEmitter.connect(EventHandler3);

    EXPECT_CALL(EventHandler1, execute(10)).Times(1);
    EXPECT_CALL(EventHandler2, execute(10)).Times(1);
    EXPECT_CALL(EventHandler3, execute(10)).Times(1);
    EventEmitter.emit(10);

    EventEmitter.disconnect();

    EXPECT_CALL(EventHandler1, execute(10)).Times(0);
    EXPECT_CALL(EventHandler2, execute(10)).Times(0);
    EXPECT_CALL(EventHandler3, execute(10)).Times(0);
    EventEmitter.emit(10);
}

TEST(EventDispatch, multipleEventHandlersConnectTwice)
{
    eventdispatch::EventEmitter<int>   EventEmitter;
    eventdispatch::EventHandlerMock<int> EventHandler1;

    EventEmitter.connect(EventHandler1);
    EventEmitter.connect(EventHandler1);

    EXPECT_CALL(EventHandler1, execute(10)).Times(2);
    EventEmitter.emit(10);
}

TEST(EventDispatch, call_callback)
{
    eventdispatch::CallableMock<int> callable;

    eventdispatch::EventEmitter<int> EventEmitter;
    eventdispatch::EventHandler<int>   EventHandler{ EventEmitter, [&callable](const int& par) { callable(par); } };

    EXPECT_CALL(callable, call(10)).Times(1);
    EventEmitter.emit(10);

    EventEmitter.disconnect();
    EXPECT_CALL(callable, call(10)).Times(0);
    EventEmitter.emit(10);

    EventEmitter.connect(EventHandler);
    EXPECT_CALL(callable, call(10)).Times(1);
    EventEmitter.emit(10);
}

// add a test for the callback function and mutliple EventHandlers
TEST(EventDispatch, callbackMultipleEventHandlers)
{
    eventdispatch::CallableMock<int> callable;

    eventdispatch::EventEmitter<int> EventEmitter;
    eventdispatch::EventHandler<int>   EventHandler1{ EventEmitter, [&callable](const int& par) { callable(par); } };
    eventdispatch::EventHandler<int>   EventHandler2{ EventEmitter, [&callable](const int& par) { callable(par); } };
    eventdispatch::EventHandler<int>   EventHandler3{ EventEmitter, [&callable](const int& par) { callable(par); } };

    EXPECT_CALL(callable, call(10)).Times(3);
    EventEmitter.emit(10);

    EventEmitter.disconnect();
    EXPECT_CALL(callable, call(10)).Times(0);
    EventEmitter.emit(10);

    EventEmitter.connect(EventHandler1);
    EventEmitter.connect(EventHandler2);
    EventEmitter.connect(EventHandler3);
    EXPECT_CALL(callable, call(10)).Times(3);
    EventEmitter.emit(10);
}

// add similar test only use setCallback
TEST(EventDispatch, setCallbackMultipleEventHandlers)
{
    eventdispatch::CallableMock<int> callable;

    eventdispatch::EventEmitter<int> EventEmitter;
    eventdispatch::EventHandler<int>   EventHandler1{ EventEmitter };
    eventdispatch::EventHandler<int>   EventHandler2{ EventEmitter };
    eventdispatch::EventHandler<int>   EventHandler3{ EventEmitter };

    EventHandler1.setCallback([&callable](const int& par) { callable(par); });
    EventHandler2.setCallback([&callable](const int& par) { callable(par); });
    EventHandler3.setCallback([&callable](const int& par) { callable(par); });

    EXPECT_CALL(callable, call(10)).Times(3);
    EventEmitter.emit(10);

    EventEmitter.disconnect();
    EXPECT_CALL(callable, call(10)).Times(0);
    EventEmitter.emit(10);

    EventEmitter.connect(EventHandler1);
    EventEmitter.connect(EventHandler2);
    EventEmitter.connect(EventHandler3);
    EXPECT_CALL(callable, call(10)).Times(3);
    EventEmitter.emit(10);
}

TEST(EventDispatch, dontSetCallbackMultipleEventHandlers)
{
    eventdispatch::CallableMock<int> callable;

    eventdispatch::EventEmitter<int> EventEmitter;
    eventdispatch::EventHandler<int>   EventHandler1{ EventEmitter };
    eventdispatch::EventHandler<int>   EventHandler2{ EventEmitter };
    eventdispatch::EventHandler<int>   EventHandler3{ EventEmitter };

    EXPECT_CALL(callable, call(10)).Times(0);
    EventEmitter.emit(10);

    EventEmitter.disconnect();
    EXPECT_CALL(callable, call(10)).Times(0);
    EventEmitter.emit(10);

    EventEmitter.connect(EventHandler1);
    EventEmitter.connect(EventHandler2);
    EventEmitter.connect(EventHandler3);
    EXPECT_CALL(callable, call(10)).Times(0);
    EventEmitter.emit(10);
}


TEST(EventDispatch, AwaitableEventHandler)
{
    std::condition_variable       waitForThread;
    std::mutex                    mutex;
    eventdispatch::CallableMock<int> callable;
    bool                          threadSync = false;

    eventdispatch::EventEmitter<int>     EventEmitter;
    eventdispatch::AwaitableEventHandler<int> EventHandler1{ EventEmitter, [&callable](const int& par) { callable(par); } };

    std::atomic_bool is_blocking = false;

    auto&& lambdaWaitForThread = [&]()
    {
        std::unique_lock<std::mutex> lock(mutex);
        waitForThread.wait(lock, [&] { return threadSync; });
        threadSync = false;
        std::cout << "    -Thread wait finished" << std::endl;
    };

    auto&& runThread = [&]()
    {
        is_blocking = true;
        {
            std::unique_lock<std::mutex> lock(mutex);
            threadSync = true;
        }
        // notify thread started
        std::cout << "    -Notify thread started" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        waitForThread.notify_one();

        std::cout << "    -Thread waiting for EventHandler" << std::endl;
        EventHandler1.wait();
        std::cout << "    -EventHandler executed" << std::endl;
        is_blocking = false;

        {
            std::unique_lock<std::mutex> lock(mutex);
            threadSync = true;
        }
        std::cout << "    -Notify EventHandler executed" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        waitForThread.notify_one();
    };
    // start the receiver thread
    static std::aligned_storage_t<3000> stack;
    std::threadex::attributes           atributes;
    atributes.set_name("thread");
    atributes.set_priority(std::threadex::priority::above_normal);
    atributes.set_stack(stack);
    auto&& thread = std::threadex(atributes, runThread);

    // wait for the thread to start
    lambdaWaitForThread();
    EXPECT_EQ(is_blocking.load(), true);

    // wait for the EventHandler.wait()
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_CALL(callable, call(10)).Times(1);
    EventEmitter.emit(10);
    std::cout << "    -emitted EventEmitter" << std::endl;

    // wait for the EventHandler to finish
    lambdaWaitForThread();
    EXPECT_EQ(is_blocking.load(), false);

    EventEmitter.disconnect();
    EXPECT_CALL(callable, call(10)).Times(0);
    EventEmitter.emit(10);

    thread.join();
}

TEST(EventDispatch, AwaitableEventEmitter)
{
    eventdispatch::AwaitableEventEmitter<int> EventEmitter;

    std::condition_variable       waitForThread;
    std::mutex                    mutex;
    eventdispatch::CallableMock<int> callable;
    bool                          threadSync = false;


    std::atomic_bool is_blocking = false;

    auto&& lambdaWaitForThread = [&]()
    {
        std::unique_lock<std::mutex> lock(mutex);
        waitForThread.wait(lock, [&] { return threadSync; });
        threadSync = false;
        std::cout << "    [main] wait for thread finished" << std::endl;
    };

    auto&& runThread = [&]()
    {
        is_blocking = true;
        {
            std::unique_lock<std::mutex> lock(mutex);
            threadSync = true;
        }
        // notify thread started
        std::cout << "    [thread] started" << std::endl;
        waitForThread.notify_one();

        std::cout << "    [thread] waiting in EventEmitter.wait()" << std::endl;
        EventEmitter.wait();
        std::cout << "    [thread] EventEmitter.wait() returned" << std::endl;
        is_blocking = false;
    };


    // thread is not running
    EXPECT_EQ(is_blocking.load(), false);

    // start the receiver thread
    static std::aligned_storage_t<3000> stack{};
    std::threadex::attributes           atributes;
    atributes.set_name("thread");
    atributes.set_priority(std::threadex::priority::above_normal);
    atributes.set_stack(stack);
    auto&& thread = std::threadex(atributes, runThread);

    // wait for the thread to start
    lambdaWaitForThread();
    EXPECT_EQ(is_blocking.load(), true);
    // start t

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EventEmitter.emit(10);
    thread.join();

    EXPECT_EQ(is_blocking.load(), false);
}

TEST(EventDispatch, createEventHandlerFromAwaitableEventEmitter)
{
    eventdispatch::AwaitableEventEmitter<int> AwaitableEventEmitter;
    eventdispatch::EventHandler<int>         EventHandler(AwaitableEventEmitter);

    eventdispatch::CallableMock<int> callable;
    EventHandler.setCallback([&callable](const int& par) { callable(par); });

    EXPECT_CALL(callable, call(10)).Times(1);
    AwaitableEventEmitter.emit(10);
}


