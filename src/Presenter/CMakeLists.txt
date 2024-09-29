set(SOURCES 
    "Presenter.cpp")

add_library(${LIBRARY_PRESENTER_NAME} STATIC 
    ${SOURCES})

target_include_directories(${LIBRARY_PRESENTER_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${LIBRARY_PRESENTER_NAME} PUBLIC ${LIBRARY_MODEL_NAME})

if (ENABLE_WARNINGS)
    target_set_warnings(TARGET ${LIBRARY_PRESENTER_NAME}
        ENABLE ${ENABLE_WARNINGS}
        AS_ERRORS ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if (ENABLE_LTO)
    target_enable_lto(
        ${LIBRARY_PRESENTER_NAME}
        ${ENABLE_LTO}
    )
endif()