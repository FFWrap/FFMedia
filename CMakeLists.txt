cmake_minimum_required(VERSION 3.6)

project(ffmedia)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (WIN32 AND MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
elseif (LINUX)
    set(CMAKE_CXX_FLAGS -Wall)
	set(CMAKE_CXX_FLAGS_RELEASE "-O3")
	set(CMAKE_CXX_FLAGS_DEBUG "-g")
endif ()

set(FFMEDIA_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build/ffmpegpp)
set(FFMEDIA_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_library(${PROJECT_NAME})

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(SRT REQUIRED srt)
#target_include_directories(${PROJECT_NAME} PRIVATE ${SRT_INCLUDE_DIRS})
#target_link_directories(${PROJECT_NAME} PRIVATE ${SRT_LIBRARY_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${SRT_LIBRARIES})
#target_compile_options(${PROJECT_NAME} PRIVATE ${SRT_CFLAGS_OTHER})
#
#find_package(Drogon CONFIG REQUIRED)
#target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)

find_package(FFMPEG REQUIRED)
target_include_directories(${PROJECT_NAME} PUBLIC ${FFMEDIA_INCLUDE_DIR} ${FFMPEG_INCLUDE_DIRS})
target_link_directories(${PROJECT_NAME} PUBLIC ${FFMPEG_LIBRARY_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${FFMPEG_LIBRARIES})

add_subdirectory (src) 
add_subdirectory (test)

install(TARGETS ${PROJECT_NAME}
        DESTINATION ${FFMEDIA_INSTALL_DIR}/lib)
install(DIRECTORY ${FFMEDIA_INCLUDE_DIR}
        DESTINATION ${FFMEDIA_INSTALL_DIR}
        FILES_MATCHING PATTERN "*.hpp")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/sample.mp4
        DESTINATION ${FFMEDIA_INSTALL_DIR}/test)

