set(PROJECT EventDispatch)

set(SOURCES
        src/EventEmitter.cpp
        src/EventCompletionLatch.cpp
)

add_subdirectory(interfaces IEventDispatch)

add_library(${PROJECT} STATIC ${SOURCES})

target_include_directories(${PROJECT} PRIVATE
        ${CMAKE_SOURCE_DIR}/Config/cfg
        ${CMAKE_SOURCE_DIR}/Config
)

target_include_directories(${PROJECT} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT} PRIVATE
        MicroStd
)

target_link_libraries(${PROJECT} PUBLIC
        IEventDispatch
)

