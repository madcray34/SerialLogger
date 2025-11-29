set(PROJECT PointerSafety)

add_library(${PROJECT} INTERFACE)

target_include_directories(${PROJECT} INTERFACE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)
    
if (ENABLE_LTO)
    target_enable_lto(
        ${LIBRARY_NETCORE_NAME}
        ${ENABLE_LTO}
    )
endif()