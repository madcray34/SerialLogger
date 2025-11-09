#include <cstdint>
#include <filesystem>

// ImGui libraries
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_opengl3_loader.h"
#include <imgui.h>
#include <implot.h>

// GL libraries
#if defined(IMGUI_IMPL_OPENGL_ES2)
   #include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

// Common network core libraries
#include <NetlibCore/Queue/types/Message.hpp>
#include <NetlibCore/Queue/TSQueue.hpp>
#include <NetlibCore/EndPointEnumerator/SerialPortScannerAdapter.hpp>

// Application defined libraries
// Network application libraries
#include <NetlibApp/EndpointEnumerator/Windows/COMPort/WindowsCOMPortScanner.hpp>
#include <NetlibApp/Connection/Asio/AsioSerialConnectionFactory.hpp>
#include <NetlibApp/Event/Asio/AsioEventLoop.hpp>
#include <NetlibApp/Event/Asio/AsioTimer.hpp>

// GUI application backend libraries
#include <presenter/presenter.hpp>
#include <model/model.hpp>
#include <view/plotter.hpp>
#include <view/renderFileExplorer.hpp>

// Aplication libraries
#include <application/AppConnectionSupervisor.hpp>

#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
   #pragma comment(lib, "legacy_stdio_definitions")
#endif

#ifndef SOURCE_ROOT_DIR
   #define SOURCE_ROOT_DIR "Undefined"
#endif

#ifndef SOURCE_LOGS_DIR
   #define SOURCE_LOGS_DIR "Undefined"
#endif

#ifndef OUTPUT_FILE_PATH
   #define OUTPUT_FILE_PATH "Undefined"
#endif

// TEMPORARY JUST FOR CHECKING EFFICIENCY
#include <iostream>
#include <atomic>

#ifdef ENABLE_ALLOC_TRACING
std::atomic<int> allocation_count = 0;

void *operator new(std::size_t size)
{
   allocation_count++;
   std::cout << "Allocated " << size << " bytes, total allocations: " << allocation_count.load()
             << std::endl;
   return std::malloc(size);
}

void operator delete(void *ptr) noexcept
{
   std::free(ptr);
}
#endif


constexpr auto WINDOW_WIDTH  = std::uint32_t{ 1280 };
constexpr auto WINDOW_HEIGHT = std::uint32_t{ 720 };

namespace fs = std::filesystem;

static void glfw_error_callback(int error, const char *description);

void start_cycle()
{
   glfwPollEvents();
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
}

void end_cycle(GLFWwindow *const window, const ImVec4 &clear_color, ImGuiConfigFlags &configFlags)
{
   int display_w = 0;
   int display_h = 0;
   glfwGetFramebufferSize(window, &display_w, &display_h);
   glViewport(0, 0, display_w, display_h);
   glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                clear_color.z * clear_color.w, clear_color.w);
   glClear(GL_COLOR_BUFFER_BIT);
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
   // Update and Render additional Platform Windows
   // (Platform functions may change the current OpenGL context, so we save/restore it to make it
   // easier to paste this code elsewhere.
   //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
   if (configFlags & ImGuiConfigFlags_ViewportsEnable)
   {
      GLFWwindow *backup_current_context = glfwGetCurrentContext();
      ImGui::UpdatePlatformWindows();
      ImGui::RenderPlatformWindowsDefault();
      glfwMakeContextCurrent(backup_current_context);
   }
   glfwSwapBuffers(window);
}


void drawmenu(GLFWwindow *const window, bool &show_file_explorer, Model &model)
{
   // TODO create a class to properly handle the menu bar
   /**
    * @brief Those string are made static to not construct them every frame
    */
   static constexpr auto c_menu_file              = "File";
   static constexpr auto c_menu_exit              = "Exit";
   static constexpr auto c_menu_exit_shortcut     = "Alt+F4";
   static constexpr auto c_menu_fileExp           = "OpenFileExplorer";
   static constexpr auto c_menu_fileExp_shortcut  = "Alt+f";
   static constexpr auto c_menu_fileSave          = "Start saving";
   static constexpr auto c_menu_fileSave_shortcut = "Ctrl+s";
   static constexpr auto c_menu_fileStop          = "Stop saving";
   static constexpr auto c_menu_fileStop_shortcut = "Shift+Ctrl+s";

   // Begin Menu Bar
   if (ImGui::BeginMenuBar())
   {
      if (ImGui::BeginMenu(c_menu_file))
      {
         if (ImGui::MenuItem(c_menu_fileExp, c_menu_fileExp_shortcut))
         {
            show_file_explorer = !show_file_explorer;
         }

         const auto &saving = model.isSavingToFile();
         if (ImGui::MenuItem(saving ? c_menu_fileStop : c_menu_fileSave,
                             saving ? c_menu_fileStop_shortcut : c_menu_fileSave_shortcut))
         {
            saving ? model.stopSavingToFile() : model.startSavingToFile();
         }

         if (ImGui::MenuItem(c_menu_exit, c_menu_exit_shortcut))
         {
            glfwSetWindowShouldClose(window, true);
         }
         ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
   }
}


int main(int, char **)
{
   // Setup window
   glfwSetErrorCallback(glfw_error_callback);
   if (!glfwInit())
   {
      return 1;
   }

   // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
   // GL ES 2.0 + GLSL 100
   const char *glsl_version = "#version 100";
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
   glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
   // GL 3.2 + GLSL 150
   const char *glsl_version = "#version 150";
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);    // 3.2+ only
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);              // Required on Mac
#else
   // GL 3.0 + GLSL 130
   const char *glsl_version = "#version 130";
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif


   // Create window with graphics context
   auto *window =
       glfwCreateWindow(static_cast<std::int32_t>(WINDOW_WIDTH),
                        static_cast<std::int32_t>(WINDOW_HEIGHT), "Gui", nullptr, nullptr);
   if (window == nullptr)
   {
      return 1;
   }
   glfwMakeContextCurrent(window);
   glfwSwapInterval(1);    // Enable vsync

   // Setup Dear ImGui context
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO &io = ImGui::GetIO();
   io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable Docking
   io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;    // Enable Multi-Viewport / Platform
                                                          // Windows
   ImGui::StyleColorsDark();


   auto &style = ImGui::GetStyle();
   // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look
   // identical to regular ones.
   if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
   {
      style.WindowRounding              = 0.0f;
      style.Colors[ImGuiCol_WindowBg].w = 1.0f;
   }
   style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0, 1.0, 1.0, 1.0);
   style.Colors[ImGuiCol_TableBorderLight]  = ImVec4(1.0, 1.0, 1.0, 1.0);

   // Setup Platform/Renderer backends
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init(glsl_version);

   FileExplorer                       fileExplorer{ SOURCE_LOGS_DIR };
   netlib::core::TSQueue<std::string> plotterQueue;
   Plotter                            plotter{ plotterQueue };

   netlib::core::TSQueue<std::string> modelFq;
   netlib::core::TSQueue<std::string> modelSq;
   Model                              model{ modelFq, modelSq, OUTPUT_FILE_PATH };

   // Instantiate the Presenter, passing the model and plotter's update method as the callback
   Presenter presenter(model, [&](std::string &&data) { plotter.update(std::move(data)); });

   // Start data reception in a separate thread
   presenter.start();

   // ITSQueue<OwnedMessage> &msgIn, COMPortScanner &portScanner,
   //    std::chrono::seconds periodicity
   netlib::core::TSQueue<netlib::core::OwnedMessage> myQueue;
   static netlib::WindowsCOMPortScanner              portScanner;
   static netlib::core::SerialPortScannerAdapter     adapter{ portScanner };
   static boost::asio::io_context                    asioContext;
   static netlib::AsioEventLoop                      eventLoop{ asioContext };
   static netlib::AsioTimerFactory                   timerFactory{ eventLoop };
   static netlib::AsioSerialConnectionFactory        connFactory{ eventLoop };

   netlib::AppConnectionSupervisor connectionSupervisor{
      myQueue, adapter, connFactory, eventLoop, timerFactory, std::chrono::seconds(5), model
   };
   connectionSupervisor.start();
   connectionSupervisor.startMessagePump();
   const auto clear_color = ImVec4(30.0F / 255.0F, 30.0F / 255.0F, 30.0F / 255.0F, 1.00f);
   ImPlot::CreateContext();

   bool show_file_explorer{ false };

   // This is the main windows frame of the application, it runs untill the suer clicks exit
   while (!glfwWindowShouldClose(window))
   {
      start_cycle();
      ImGui::NewFrame();

      // Main DockSpace
      ImGuiViewport *viewport = ImGui::GetMainViewport();
      ImGui::SetNextWindowPos(viewport->Pos);
      ImGui::SetNextWindowSize(viewport->Size);
      ImGui::SetNextWindowViewport(viewport->ID);

      // Define window flags to make the DockSpace full screen
      ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
      ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
      window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
      window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

      ImGui::Begin("DockSpace", nullptr, window_flags);
      ImGui::PopStyleVar(2);

      // DockSpace
      ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
      ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

      drawmenu(window, show_file_explorer, model);

      if (show_file_explorer)
      {
         // render(fileExplorer);
      }

      render(plotter);
      ImGui::End();    // End the main DockSpace window
      ImGui::Render();

      end_cycle(window, clear_color, io.ConfigFlags);
   }
   ImPlot::DestroyContext();

   // Cleanup
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();

   glfwDestroyWindow(window);
   glfwTerminate();

   connectionSupervisor.stopMessagePump();
   presenter.stop();
   return 0;
}

void glfw_error_callback(int error, const char *description)
{
   fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
