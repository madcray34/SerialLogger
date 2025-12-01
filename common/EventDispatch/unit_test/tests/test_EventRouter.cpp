#include <EventDispatch/EventDispatchMock.hpp>
#include <EventDispatch/EventRouter.hpp>
#include <EventDispatch/EventHandler.hpp>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace tanlibs;
using namespace testing;

using KeyType     = int;
using PayloadType = int;

class EventRouterTest : public ::testing::Test
{
    protected:
    eventdispatch::EventRouter<KeyType, PayloadType> filter;
    eventdispatch::EventHandlerMock<PayloadType>              EventHandler;
    eventdispatch::EventHandlerMock<PayloadType>              EventHandler1;
    eventdispatch::EventHandlerMock<PayloadType>              EventHandler2;
};

TEST_F(EventRouterTest, ConnectAndEmitSingleEventHandler)
{
    filter.connect(1, EventHandler);
    EXPECT_CALL(EventHandler, execute(42)).Times(1);
    filter.emit(1, 42);
}

TEST_F(EventRouterTest, ConnectMultipleEventHandlersSameKey)
{
    filter.connect(2, EventHandler1);
    filter.connect(2, EventHandler2);
    EXPECT_CALL(EventHandler1, execute(99)).Times(1);
    EXPECT_CALL(EventHandler2, execute(99)).Times(1);
    filter.emit(2, 99);
}

TEST_F(EventRouterTest, ConnectEventHandlersDifferentKeys)
{
    filter.connect(3, EventHandler1);
    filter.connect(4, EventHandler2);
    EXPECT_CALL(EventHandler1, execute(11)).Times(1);
    EXPECT_CALL(EventHandler2, execute(22)).Times(1);
    filter.emit(3, 11);
    filter.emit(4, 22);
}

TEST_F(EventRouterTest, DisconnectEventHandlerFromKey)
{
    filter.connect(5, EventHandler1);
    filter.connect(5, EventHandler2);
    filter.disconnect(5, EventHandler1);
    EXPECT_CALL(EventHandler1, execute(testing::_)).Times(0);
    EXPECT_CALL(EventHandler2, execute(77)).Times(1);
    filter.emit(5, 77);
}

TEST_F(EventRouterTest, DisconnectAllEventHandlersFromKey)
{
    filter.connect(6, EventHandler1);
    filter.connect(6, EventHandler2);
    filter.disconnect(6);
    EXPECT_CALL(EventHandler1, execute(testing::_)).Times(0);
    EXPECT_CALL(EventHandler2, execute(testing::_)).Times(0);
    filter.emit(6, 88);
}

TEST_F(EventRouterTest, EmitWithNoConnectedEventHandlers)
{
    EXPECT_CALL(EventHandler, execute(testing::_)).Times(0);
    filter.emit(7, 123);
}

class EventRouterVoidTest : public ::testing::Test
{
    protected:
    eventdispatch::EventRouter<KeyType, void> filter;
    VoidEventHandlerMock                            EventHandler;
    VoidEventHandlerMock                            EventHandler1;
    VoidEventHandlerMock                            EventHandler2;
};

TEST_F(EventRouterVoidTest, VoidPayloadConnectAndEmit)
{
    filter.connect(8, EventHandler);
    EXPECT_CALL(EventHandler, execute()).Times(1);
    filter.emit(8);
}

TEST_F(EventRouterVoidTest, VoidPayloadDisconnectEventHandler)
{
    filter.connect(9, EventHandler1);
    filter.connect(9, EventHandler2);
    filter.disconnect(9, EventHandler1);
    EXPECT_CALL(EventHandler1, execute()).Times(0);
    EXPECT_CALL(EventHandler2, execute()).Times(1);
    filter.emit(9);
}

TEST_F(EventRouterVoidTest, VoidPayloadDisconnectAllEventHandlers)
{
    filter.connect(10, EventHandler1);
    filter.connect(10, EventHandler2);
    filter.disconnect(10);
    EXPECT_CALL(EventHandler1, execute()).Times(0);
    EXPECT_CALL(EventHandler2, execute()).Times(0);
    filter.emit(10);
}

// Test callback integration with EventHandler
class EventRouterCallbackTest : public ::testing::Test
{
    protected:
    eventdispatch::EventRouter<KeyType, PayloadType> filter;
};

TEST_F(EventRouterCallbackTest, EventHandlerCallbackIntegration)
{
    int                           called = 0;
    eventdispatch::EventHandler<PayloadType> EventHandler;
    EventHandler.setCallback([&called](PayloadType v) { called = v; });
    filter.connect(11, EventHandler);
    filter.emit(11, 555);
    EXPECT_EQ(called, 555);
}

TEST_F(EventRouterCallbackTest, EventHandlerCallbackMultipleKeys)
{
    int                           calledA = 0, calledB = 0;
    eventdispatch::EventHandler<PayloadType> EventHandlerA, EventHandlerB;
    EventHandlerA.setCallback([&calledA](PayloadType v) { calledA = v; });
    EventHandlerB.setCallback([&calledB](PayloadType v) { calledB = v; });
    filter.connect(12, EventHandlerA);
    filter.connect(13, EventHandlerB);
    filter.emit(12, 100);
    filter.emit(13, 200);
    EXPECT_EQ(calledA, 100);
    EXPECT_EQ(calledB, 200);
}

class EventRouterVoidCallbackTest : public ::testing::Test
{
    protected:
    eventdispatch::EventRouter<KeyType, void> filter;
};

TEST_F(EventRouterVoidCallbackTest, VoidEventHandlerCallbackIntegration)
{
    int                    called = 0;
    eventdispatch::EventHandler<void> EventHandler;
    EventHandler.setCallback([&called]() { called = 1; });
    filter.connect(14, EventHandler);
    filter.emit(14);
    EXPECT_EQ(called, 1);
}


