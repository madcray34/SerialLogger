set(VCPKG_DIR ${CMAKE_SOURCE_DIR}/external/vcpkg)

if(NOT EXISTS "${VCPKG_DIR}")
	message(STATUS "vcpkg not found, downloading...")
	execute_process(
		COMMAND git clone https://github.com/microsoft/vcpkg.git ${VCPKG_DIR}
		COMMAND_ERROR_IS_FATAL ANY
		)
    
	execute_process(
        COMMAND bootstrap-vcpkg.bat
        WORKING_DIRECTORY ${VCPKG_DIR}
        COMMAND_ERROR_IS_FATAL ANY
        )
else()
    message(STATUS "Vcpkg found: ${VCPKG_DIR}")
endif()
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_DIR}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
