set(SOURCES 
    "./src/server.cpp")

add_library(${LIBRARY_NETSERVER_NAME} STATIC 
    ${SOURCES})

target_include_directories(${LIBRARY_NETSERVER_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_NETSERVER_NAME} PUBLIC
    ${LIBRARY_IGUI_BACKEND_NAME}
    ${LIBRARY_INET_NAME}
    )

target_link_libraries(${LIBRARY_NETSERVER_NAME} PRIVATE
    ${LIBRARY_NET_NAME}
    )
    

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