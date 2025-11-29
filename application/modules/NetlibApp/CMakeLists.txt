set(SOURCES 
    "./src/WindowsCOMPortScanner.cpp"
    "./src/AsioSerialConnectionFactory.cpp"
    "./src/AsioSerialTextStream.cpp"
    )

add_library(${LIBRARY_NETAPP_NAME} STATIC 
    ${SOURCES})  

target_include_directories(${LIBRARY_NETAPP_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_NETAPP_NAME} PUBLIC 
    ${LIBRARY_NETCORE_NAME}
    PointerSafety
    Boost::system
    Boost::asio)

target_link_libraries(${LIBRARY_NETAPP_NAME} PRIVATE 
    ws2_32      # Link to the Winsock library
    )
    
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