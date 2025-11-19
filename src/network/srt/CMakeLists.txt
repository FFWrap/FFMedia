set(SOURCES
        FFSrtStream.cpp
        FFSrtSocketBase.cpp
        FFSrtPublisherSession.cpp
        FFSrtSubscriberSession.cpp
        FFSrtStreamChannel.cpp
        FFSrtStreamManager.cpp
        FFSrtAcceptorBase.cpp
        FFSrtPublisherAcceptor.cpp
        FFSrtSubscriberAcceptor.cpp
        FFSrtRelayServer.cpp
)

set(UPDATED_SOURCES "") 
foreach(SOURCE IN LISTS SOURCES)
    list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()

target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})