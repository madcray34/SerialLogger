set(SOURCES
    "./src/AsioSerialConnectionFactory.cpp"
    "./src/AsioSerialTextStream.cpp"
    )

if(WIN32)
    list(APPEND SOURCES "./src/WindowsCOMPortScanner.cpp")
elseif(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBUDEV REQUIRED IMPORTED_TARGET libudev)
    list(APPEND SOURCES "./src/LinuxSerialPortScanner.cpp")
else()
    message(FATAL_ERROR "Serial-port discovery is supported only on Windows and Linux.")
endif()

add_library(${LIBRARY_NETAPP_NAME} STATIC 
    ${SOURCES})  

target_include_directories(${LIBRARY_NETAPP_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_NETAPP_NAME} PUBLIC 
    ${LIBRARY_NETCORE_NAME}
    Boost::system
    Boost::asio)

if(WIN32)
    target_link_libraries(${LIBRARY_NETAPP_NAME} PRIVATE setupapi ws2_32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${LIBRARY_NETAPP_NAME} PRIVATE PkgConfig::LIBUDEV)
endif()
    
if (ENABLE_WARNINGS)
    target_set_warnings(TARGET ${LIBRARY_NETAPP_NAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if (ENABLE_LTO)
    target_enable_lto(
        ${LIBRARY_NETAPP_NAME}
        ${ENABLE_LTO}
    )
endif()