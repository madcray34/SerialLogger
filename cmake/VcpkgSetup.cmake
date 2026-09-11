if(DEFINED CMAKE_TOOLCHAIN_FILE)
    set(_vcpkg_toolchain "${CMAKE_TOOLCHAIN_FILE}")
elseif(DEFINED VCPKG_ROOT AND NOT VCPKG_ROOT STREQUAL "")
    set(_vcpkg_root "${VCPKG_ROOT}")
elseif(DEFINED ENV{VCPKG_ROOT})
    set(_vcpkg_root "$ENV{VCPKG_ROOT}")
else()
    set(_vcpkg_root "${CMAKE_CURRENT_LIST_DIR}/../external/vcpkg")
    set(_vcpkg_toolchain "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake")
endif()

if(NOT DEFINED _vcpkg_toolchain)
    set(_vcpkg_toolchain "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake")
endif()
if(NOT EXISTS "${_vcpkg_toolchain}")
    if(CMAKE_HOST_WIN32)
        set(_bootstrap_command "bootstrap-vcpkg.bat")
    else()
        set(_bootstrap_command "./bootstrap-vcpkg.sh")
    endif()

    message(FATAL_ERROR
        "vcpkg toolchain not found at '${_vcpkg_toolchain}'. "
        "Install vcpkg, run '${_bootstrap_command}' in its directory, and set VCPKG_ROOT.")
endif()

set(CMAKE_TOOLCHAIN_FILE "${_vcpkg_toolchain}" CACHE FILEPATH "vcpkg toolchain file")
