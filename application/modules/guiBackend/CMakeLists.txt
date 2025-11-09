set(MODEL_SOURCES 
    "./src/model.cpp"
    "./src/plotter.cpp" 
    "./src/presenter.cpp"
    "./src/renderFileExplorer.cpp"
    )

add_library(${LIBRARY_GUI_BACKEND_NAME} STATIC 
    ${MODEL_SOURCES}
    )

add_subdirectory(interfaces ./interfaces)

target_include_directories(${LIBRARY_GUI_BACKEND_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

target_link_libraries(${LIBRARY_GUI_BACKEND_NAME} PUBLIC 
    ${LIBRARY_IGUI_BACKEND_NAME} 
    )

target_link_libraries(${LIBRARY_GUI_BACKEND_NAME} PRIVATE
    imgui::imgui
    fmt::fmt-header-only
    )

if (ENABLE_WARNINGS)
    target_set_warnings(TARGET ${LIBRARY_GUI_BACKEND_NAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS}
        )
endif()

if (ENABLE_LTO)
    target_enable_lto(
        ${LIBRARY_GUI_BACKEND_NAME}
        ${ENABLE_LTO}
    )
endif()