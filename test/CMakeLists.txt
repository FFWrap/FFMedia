set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()
add_test(AllTestsInMain main)
find_package(GTest CONFIG REQUIRED)


#################################################################### FFMedia
set(FFMEDIA_SOURCE
        main.cpp
        decode.cpp
        transmux.cpp
        streaming.cpp
)
set(FFMPEGPP_TEST ffmedia_test)
add_executable(${FFMPEGPP_TEST} ${FFMEDIA_SOURCE})
target_link_libraries(${FFMPEGPP_TEST} PRIVATE  ${PROJECT_NAME}
                                                GTest::gtest
                                                GTest::gtest_main
                                                GTest::gmock
                                                GTest::gmock_main)
target_include_directories(${FFMPEGPP_TEST} PRIVATE ${FFMEDIA_INCLUDE_DIR})
#################################################################### FFMedia



#################################################################### UTIL
set(UTIL_SOURCE
        main.cpp
        thread.cpp
)
set(UTIL_TEST util_test)
add_executable(${UTIL_TEST} ${UTIL_SOURCE})
target_link_libraries(${UTIL_TEST} PRIVATE  ${PROJECT_NAME}
                                                GTest::gtest
                                                GTest::gtest_main
                                                GTest::gmock
                                                GTest::gmock_main)
target_include_directories(${UTIL_TEST} PRIVATE ${FFMEDIA_INCLUDE_DIR})
#################################################################### UTIL



#################################################################### SRT
set(SRT_RELAY_SERVER_SOURCE
        main.cpp
        #srt_relay.cpp
)
set(SRT_RELAY_TEST srt_relay_test)
add_executable(${SRT_RELAY_TEST} ${SRT_RELAY_SERVER_SOURCE})
target_link_libraries(${SRT_RELAY_TEST} PRIVATE  ${PROJECT_NAME}
                                                GTest::gtest
                                                GTest::gtest_main
                                                GTest::gmock
                                                GTest::gmock_main)
target_include_directories(${SRT_RELAY_TEST} PRIVATE ${FFMEDIA_INCLUDE_DIR})
#################################################################### SRT



install(TARGETS ${FFMPEGPP_TEST}
        DESTINATION ${FFMEDIA_INSTALL_DIR}/test)
if (WIN32)
    install(
        DIRECTORY
            $<TARGET_FILE_DIR:${FFMPEGPP_TEST}>/
        DESTINATION
            ${FFMEDIA_INSTALL_DIR}/test
        CONFIGURATIONS
            Debug Release RelWithDebInfo MinSizeRel
        FILES_MATCHING
            PATTERN "*.dll"
    )
endif()