#include <NetlibCore/EndPointEnumerator/SerialPortScannerAdapter.hpp>
#include <NetlibCore/Queue/TSQueue.hpp>
#include <NetlibCore/Queue/types/Message.hpp>

#include <NetlibApp/Connection/Asio/AsioSerialConnectionFactory.hpp>
#include <NetlibApp/EndpointEnumerator/Linux/COMPort/LinuxCOMPortScanner.hpp>
#include <NetlibApp/Event/Asio/AsioEventLoop.hpp>
#include <NetlibApp/Event/Asio/AsioTimer.hpp>

#include <application/AppConnectionSupervisor.hpp>

#include <atomic>
#include <boost/asio/io_context.hpp>
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <thread>

namespace
{
   std::atomic<bool> g_running{true};

   void signalHandler(int /*signal*/)
   {
      g_running = false;
   }
}    // namespace

int main(int, char **)
{
   std::signal(SIGINT, signalHandler);
   std::signal(SIGTERM, signalHandler);

   boost::asio::io_context                    asioContext;
   netlib::AsioEventLoop                      eventLoop{asioContext};
   netlib::AsioTimerFactory                   timerFactory{eventLoop};
   netlib::AsioSerialConnectionFactory        connFactory{eventLoop};
   netlib::LinuxCOMPortScanner                portScanner;
   netlib::core::SerialPortScannerAdapter     adapter{portScanner};
   netlib::core::TSQueue<netlib::core::OwnedMessage> msgIn;

   netlib::AppConnectionSupervisor connectionSupervisor{
      msgIn, adapter, connFactory, eventLoop, timerFactory, std::chrono::seconds(5)};

   if (!connectionSupervisor.start())
   {
      std::cerr << "Failed to start connection supervisor\n";
      return EXIT_FAILURE;
   }

   connectionSupervisor.startMessagePump();

   std::cout << "SerialLogger started. Press Ctrl-C to stop.\n";

   while (g_running)
   {
      connectionSupervisor.update(10, true);
   }

   connectionSupervisor.stopMessagePump();
   connectionSupervisor.stop();

   std::cout << "SerialLogger stopped.\n";
   return EXIT_SUCCESS;
}
