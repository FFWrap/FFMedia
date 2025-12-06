add_executable(decode decode.cpp)
target_link_libraries(decode PRIVATE  ${PROJECT_NAME})
target_include_directories(decode PRIVATE ${FFMEDIA_INCLUDE_DIR})

add_executable(transcode transcode.cpp)
target_link_libraries(transcode PRIVATE  ${PROJECT_NAME})
target_include_directories(transcode PRIVATE ${FFMEDIA_INCLUDE_DIR})

add_executable(transmux transmux.cpp)
target_link_libraries(transmux PRIVATE  ${PROJECT_NAME})
target_include_directories(transmux PRIVATE ${FFMEDIA_INCLUDE_DIR})

add_executable(hw_transcode hw_transcode.cpp)
target_link_libraries(hw_transcode PRIVATE  ${PROJECT_NAME})
target_include_directories(hw_transcode PRIVATE ${FFMEDIA_INCLUDE_DIR})

add_executable(hw_decode hw_decode.cpp)
target_link_libraries(hw_decode PRIVATE  ${PROJECT_NAME})
target_include_directories(hw_decode PRIVATE ${FFMEDIA_INCLUDE_DIR})