add_executable(
    netcore_tests
    Common/NetlibCore/test_tsqueue.cpp
)

target_link_libraries(netcore_tests PRIVATE
    ${LIBRARY_NETCORE_NAME}
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(netcore_tests 
    DISCOVERY_MODE PRE_TEST
    WORKING_DIRECTORY $<TARGET_FILE_DIR:netcore_tests>
)

add_executable(
    netlibapp_serial_tests
    Common/NetlibApp/test_serial_options.cpp
)

target_include_directories(netlibapp_serial_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/application/modules/NetlibApp/include
)

target_link_libraries(netlibapp_serial_tests PRIVATE
    GTest::gtest_main
)

gtest_discover_tests(netlibapp_serial_tests
    DISCOVERY_MODE PRE_TEST
    WORKING_DIRECTORY $<TARGET_FILE_DIR:netlibapp_serial_tests>
)

if(UNIX AND NOT APPLE)
    add_executable(
        netlibapp_linux_tests
        Common/NetlibApp/test_linux_serial_port_scanner.cpp
    )

    target_link_libraries(netlibapp_linux_tests PRIVATE
        ${LIBRARY_NETAPP_NAME}
        GTest::gtest_main
    )

    gtest_discover_tests(netlibapp_linux_tests
        DISCOVERY_MODE PRE_TEST
        WORKING_DIRECTORY $<TARGET_FILE_DIR:netlibapp_linux_tests>
    )
endif()
