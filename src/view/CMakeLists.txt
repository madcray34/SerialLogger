set(SOURCES 
    "Plotter.cpp" 
    "RenderFileExplorer.cpp")

add_library(${LIBRARY_VIEW_NAME} STATIC 
    ${SOURCES})

target_include_directories(${LIBRARY_VIEW_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_VIEW_NAME} PUBLIC 
    ${LIBRARY_MODEL_NAME}
    imgui::imgui
    implot::implot
    fmt::fmt-header-only
    Boost::circular_buffer)

#Use the following when switching to shared library
#target_compile_definitions(${LIBRARY_VIEW_NAME} PRIVATE BUILDING_VIEW_DLL)


if (ENABLE_WARNINGS)
    target_set_warnings(TARGET ${LIBRARY_VIEW_NAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if (ENABLE_LTO)
    target_enable_lto(
        ${LIBRARY_VIEW_NAME}
        ${ENABLE_LTO}
    )
endif()