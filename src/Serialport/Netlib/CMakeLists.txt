set(SOURCES 
    "Connection.cpp"
    "ServerBase.cpp"
    "WindowsCOMPortScanner.cpp")

add_library(${LIBRARY_NET_NAME} STATIC 
    ${SOURCES})

target_include_directories(${LIBRARY_NET_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_NET_NAME} PUBLIC 
    Boost::system
    Boost::asio)

#Use the following when switching to shared library
#target_compile_definitions(${LIBRARY_VIEW_NAME} PRIVATE BUILDING_VIEW_DLL)


if (ENABLE_WARNINGS)
    target_set_warnings(TARGET ${LIBRARY_NET_NAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if (ENABLE_LTO)
    target_enable_lto(
        ${LIBRARY_NET_NAME}
        ${ENABLE_LTO}
    )
endif()