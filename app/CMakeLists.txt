set(EXE_SOURCES
    "main.cpp")

# set(EXE_INCLUDES
#     "./")

add_executable(${EXECUTABLE_NAME} ${EXE_SOURCES})

# target_include_directories(${EXECUTABLE_NAME} PUBLIC
#     ${EXE_INCLUDES})

target_link_libraries(${EXECUTABLE_NAME} PRIVATE 
    ${LIBRARY_MODEL_NAME}
    ${LIBRARY_VIEW_NAME}
    ${LIBRARY_PRESENTER_NAME}
    ${LIBRARY_NET_NAME}
    ${LIBRARY_NETSERVER_NAME}
    glfw
    OpenGL::GL
    setupapi)

# add_dependencies(${EXECUTABLE_NAME} ${LIBRARY_VIEW_NAME})

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

# # Set output directory for the executable
# set_target_properties(${EXECUTABLE_NAME} 
#     PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
#     ${CMAKE_BINARY_DIR}/bin)

# # Copy the DLL to the executable's directory after building
# add_custom_command(TARGET ${EXECUTABLE_NAME}  
#     POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${LIBRARY_VIEW_NAME}> $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>)