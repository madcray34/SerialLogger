set(SOURCES 
    "./src/serverBase.cpp"
    "./src/Connection.cpp"
    )

add_library(${LIBRARY_NETCORE_NAME} STATIC 
    ${SOURCES})

add_subdirectory(interfaces ./interfaces)    

target_include_directories(${LIBRARY_NETCORE_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_NETCORE_NAME} PUBLIC 
    ${LIBRARY_INETCORE_NAME})

target_link_libraries(${LIBRARY_NETCORE_NAME} PRIVATE 
    )
    
if (ENABLE_WARNINGS)
    target_set_warnings(TARGET ${LIBRARY_NETCORE_NAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if (ENABLE_LTO)
    target_enable_lto(
        ${LIBRARY_NETCORE_NAME}
        ${ENABLE_LTO}
    )
endif()