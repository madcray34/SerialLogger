cmake_minimum_required(VERSION 3.30)

project(
    "DataPlotter"
    VERSION 1.0.0
    LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Project/Library Names
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
set(EXECUTABLE_NAME Executable)
set(LIBRARY_MODEL_NAME model)
set(LIBRARY_PRESENTER_NAME Presenter)
set(LIBRARY_VIEW_NAME view)
set(LIBRARY_NET_NAME Netlib)
set(LIBRARY_NETSERVER_NAME Server)

# Options
option(ENABLE_WARNINGS "Enable to add warnings to a target." ON)
option(ENABLE_WARNINGS_AS_ERRORS "Enable to treat warnings as errors." OFF)
option(ENABLE_CLANG_TIDY "Enable to add clang tidy." ON)
option(ENABLE_CLANG_FORMAT "Enable to add clang-format." ON)
option(ENABLE_LTO "Enable Link Time Optimization." OFF) 

if(ENABLE_WARNINGS)
    include(Warnings)
endif()

#include(Tools) # needs clang tidy and clang format
include(ConfigSafeGuards)

if(ENABLE_LTO)
    include(LTO)
endif()

# add_clang_format_target() # needs python 

# set (VCPKG_DIR ${CMAKE_SOURCE_DIR}/external/vcpkg)
# you can set any other vcpkg installation on your system
set(VCPKG_DIR $ENV{VCPKG_PATH})
message(STATUS "Vcpkg path: ${VCPKG_DIR}")
include(${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake)

find_package(fmt CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(implot CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED COMPONENTS circular_buffer asio system)
if (Boost_FOUND)
    message(STATUS "Boost found: ${Boost_VERSION}")
    message(STATUS "Include dir: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Libraries: ${Boost_LIBRARIES}")
endif()

add_compile_definitions(NOMINMAX)

add_subdirectory(src)
add_subdirectory(app)

# add_clang_tidy_msvc_to_target(${PROJECT_NAME})