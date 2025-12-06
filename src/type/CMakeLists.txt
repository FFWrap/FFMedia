set(SOURCES
        FFAVRational.cpp
        FFAVPacket.cpp
        FFAVInputContext.cpp
        FFAVOutputContext.cpp
        FFAVCodecParameters.cpp
        FFAVFrame.cpp
        FFAVDecodeStream.cpp
        FFAVEncodeStream.cpp
        FFAVChannelLayout.cpp
        FFAVCodecContext.cpp
        FFAVSwsContext.cpp
        FFAVCodecContextHWFormat.cpp
        FFAVCodecContextHWCpu.cpp
        FFAVCodecContextHWCuda.cpp
        FFAVCodecContextFactory.cpp

        impl/FFAVPacketImpl.cpp
        impl/FFAVFormatContextImpl.cpp
        impl/FFAVCodecParametersImpl.cpp
        impl/FFAVFrameImpl.cpp
        impl/FFAVStreamImpl.cpp
        impl/FFAVChannelLayoutImpl.cpp
        impl/FFAVCodecContextImpl.cpp
        impl/FFAVSwsContextImpl.cpp
)

set(UPDATED_SOURCES "")
foreach(SOURCE IN LISTS SOURCES)
        list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()
 
target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})