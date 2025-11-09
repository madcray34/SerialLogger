cmake_minimum_required(VERSION 3.30)

project(
    "SerialLogger"
    VERSION 1.0.0
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project/Library Names
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
set(EXECUTABLE_NAME SerialLogger)
set(LIBRARY_GUI_BACKEND_NAME guiBackend)
set(LIBRARY_IGUI_BACKEND_NAME I${LIBRARY_GUI_BACKEND_NAME})
set(LIBRARY_NETCORE_NAME NetlibCore)
set(LIBRARY_INETCORE_NAME I${LIBRARY_NETCORE_NAME})
set(LIBRARY_NETAPP_NAME NetlibApp)

# Options
option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable to treat warnings as errors." OFF)
option(ENABLE_CLANG_FORMAT "Enable to add clang-format." OFF)
option(ENABLE_CLANG_TIDY "Enable to add clang tidy." OFF)
option(ENABLE_LTO "Enable Link Time Optimization." OFF) 
option(ENABLE_TESTS "Enable unit tests." OFF)

if(ENABLE_WARNINGS)
    include(Warnings)
endif()

include(Tools) # needs clang tidy and clang format
include(ConfigSafeGuards)

if(ENABLE_LTO)
    include(LTO)
endif()

if(ENABLE_CLANG_FORMAT)
    add_clang_format_target()
endif()

if(ENABLE_CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")  # Enable all checks
endif()

include(VcpkgSetup)  # Now we load the vcpkg logic from the separate file
include(${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

find_package(fmt CONFIG REQUIRED QUIET)
find_package(glfw3 CONFIG REQUIRED QUIET)
find_package(imgui CONFIG REQUIRED QUIET)
find_package(OpenGL REQUIRED QUIET)
find_package(Boost REQUIRED COMPONENTS asio system QUIET)
if (Boost_FOUND)
    message(STATUS "Boost found: ${Boost_VERSION}")
    message(STATUS "Include dir: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Libraries: ${Boost_LIBRARIES}")
endif()

if(ENABLE_TESTS)
    find_package(GTest CONFIG REQUIRED QUIET)
    enable_testing()
    add_subdirectory(tests ./tests)
    message(STATUS "Unit tests enabled.")
endif()

add_compile_definitions(NOMINMAX)

if(WIN32)
    # target Windows 10
    add_compile_definitions(_WIN32_WINNT=0x0A00)
endif()

add_subdirectory(common ./common)
add_subdirectory(application ./application)

# add_clang_tidy_msvc_to_target(${PROJECT_NAME})