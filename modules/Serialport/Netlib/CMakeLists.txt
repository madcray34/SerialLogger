set(SOURCES 
    "./src/connection.cpp"
    "./src/serverBase.cpp"
    "./src/windowsCOMPortScanner.cpp")

add_library(${LIBRARY_NET_NAME} STATIC 
    ${SOURCES})

add_subdirectory(interfaces ./interfaces)    

target_include_directories(${LIBRARY_NET_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_NET_NAME} PUBLIC 
    ${LIBRARY_INET_NAME}
    Boost::system
    Boost::asio)

target_link_libraries(${LIBRARY_NET_NAME} PRIVATE 
    ws2_32      # Link to the Winsock library
    )
    
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