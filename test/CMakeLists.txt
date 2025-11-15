set(SOURCES
        main.cpp

        decode.cpp
        transmux.cpp
        streaming.cpp
)

set(UTIL_SOURCE 
        main.cpp

        thread.cpp
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FFMPEGPP_TEST ffmpegpp_test)
set(UTIL_TEST util_test)

add_executable(${FFMPEGPP_TEST} ${SOURCES})
add_executable(${UTIL_TEST} ${UTIL_SOURCE})

enable_testing()
add_test(AllTestsInMain main)

find_package(GTest CONFIG REQUIRED)

# FFMPEGPP test
target_link_libraries(${FFMPEGPP_TEST} PRIVATE  ${PROJECT_NAME}
                                                GTest::gtest
                                                GTest::gtest_main
                                                GTest::gmock
                                                GTest::gmock_main)
target_include_directories(${FFMPEGPP_TEST} PRIVATE ${FFMPEGPP_INCLUDE_DIR})

# UTIL test
target_link_libraries(${UTIL_TEST} PRIVATE  ${PROJECT_NAME}
                                                GTest::gtest
                                                GTest::gtest_main
                                                GTest::gmock
                                                GTest::gmock_main)
target_include_directories(${UTIL_TEST} PRIVATE ${FFMPEGPP_INCLUDE_DIR})


install(TARGETS ${FFMPEGPP_TEST}
        DESTINATION ${FFMPEGPP_INSTALL_DIR}/test)
if (WIN32)
    install(
        DIRECTORY
            $<TARGET_FILE_DIR:${FFMPEGPP_TEST}>/
        DESTINATION
            ${FFMPEGPP_INSTALL_DIR}/test
        CONFIGURATIONS
            Debug Release RelWithDebInfo MinSizeRel
        FILES_MATCHING
            PATTERN "*.dll"
    )
endif()