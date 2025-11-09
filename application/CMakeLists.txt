add_subdirectory(modules ./modules)

set(EXE_SOURCES
    "./src/application.cpp"
    "./src/AppConnectionSupervisor.cpp")

add_executable(${EXECUTABLE_NAME} ${EXE_SOURCES})

target_include_directories(${LIBRARY_NETAPP_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
    ${LIBRARY_NETCORE_NAME}
    ${LIBRARY_NETAPP_NAME}
    ${LIBRARY_GUI_BACKEND_NAME}
    glfw
    OpenGL::GL
    setupapi
    ws2_32
    imgui::imgui
    implot::implot
)

target_compile_definitions(${EXECUTABLE_NAME} PRIVATE 
    ENABLE_ALLOC_TRACING
)


if(ENABLE_CLANG_FORMAT)
    add_dependencies(${EXECUTABLE_NAME} run_clang_format)
endif()

if (ENABLE_WARNINGS)
    target_set_warnings(TARGET ${EXECUTABLE_NAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if (ENABLE_LTO)
    target_enable_lto(
        ${EXECUTABLE_NAME}
        ${ENABLE_LTO}
    )
endif()

# Set the source root directory
set(SOURCE_ROOT "${CMAKE_SOURCE_DIR}")

# Append folder and filename to create paths
set(SOURCE_LOGS_DIR "${SOURCE_ROOT}/logs")
set(OUTPUT_FILE_PATH "${SOURCE_LOGS_DIR}/output.txt")

# Pass the paths as preprocessor macros
target_compile_definitions(${EXECUTABLE_NAME} PRIVATE 
    SOURCE_ROOT_DIR="${SOURCE_ROOT}"
    SOURCE_LOGS_DIR="${SOURCE_LOGS_DIR}"
    OUTPUT_FILE_PATH="${OUTPUT_FILE_PATH}"
)

# # Set output directory for the executable
# set_target_properties(${EXECUTABLE_NAME} 
#     PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
#     ${CMAKE_BINARY_DIR}/bin)

# # Copy the DLL to the executable's directory after building
# add_custom_command(TARGET ${EXECUTABLE_NAME}  
#     POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIBRARY_VIEW_NAME}> $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)