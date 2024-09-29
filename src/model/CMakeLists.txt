set(MODEL_SOURCES 
    "model.cpp")

add_library(${LIBRARY_MODEL_NAME} STATIC 
    ${MODEL_SOURCES})

target_include_directories(${LIBRARY_MODEL_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

#target_link_libraries(${LIBRARY_MODEL_NAME} PUBLIC )

if (ENABLE_WARNINGS)
    target_set_warnings(TARGET ${LIBRARY_MODEL_NAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if (ENABLE_LTO)
    target_enable_lto(
        ${LIBRARY_MODEL_NAME}
        ${ENABLE_LTO}
    )
endif()