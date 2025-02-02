set(SOURCES 
    "Server.cpp")

add_library(${LIBRARY_NETSERVER_NAME} STATIC 
    ${SOURCES})

target_include_directories(${LIBRARY_NETSERVER_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_NETSERVER_NAME} PUBLIC
    ${LIBRARY_NET_NAME}
    ${LIBRARY_MODEL_NAME}
    Boost::system
    Boost::asio)

#Use the following when switching to shared library
#target_compile_definitions(${LIBRARY_VIEW_NAME} PRIVATE BUILDING_VIEW_DLL)


if (ENABLE_WARNINGS)
    target_set_warnings(TARGET ${LIBRARY_NETSERVER_NAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if (ENABLE_LTO)
    target_enable_lto(
        ${LIBRARY_NETSERVER_NAME}
        ${ENABLE_LTO}
    )
endif()