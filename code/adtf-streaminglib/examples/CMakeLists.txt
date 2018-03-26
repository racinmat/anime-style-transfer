if(NOT WIN32)
    find_path(ADTF_FFMPEG_DIR include/libavformat/avformat.h)

    if(NOT ADTF_FFMPEG_DIR)
        message(WARNING "Unable to find FFmpeg. Please specify the path to the FFmpeg SDK. FFmpeg support has been disabled")
    else(NOT ADTF_FFMPEG_DIR)
        message(STATUS "FFmpeg found")
        include_directories(${ADTF_FFMPEG_DIR}/include)
        link_directories(${ADTF_FFMPEG_DIR}/lib)
        add_definitions(-DFFMPEG_AVAILABLE)
    endif(NOT ADTF_FFMPEG_DIR)
endif(NOT WIN32)

add_subdirectory(additionaldata)
add_subdirectory(candump)
add_subdirectory(canwriter)
#add_subdirectory(decode_compressed_video)
#add_subdirectory(encode_compressed_video)
add_subdirectory(copychunks)
add_subdirectory(dummywriter)
add_subdirectory(eds)
add_subdirectory(fileaccess)
add_subdirectory(mediadescription)
add_subdirectory(referencedfiles)

if (WIN32)
    install(FILES
        ${ADTF_STREAMING_DIR}/lib/${ADTF_STREAMING_LIB}D_${ADTF_STREAMING_LIB_VERSION}.dll
        DESTINATION bin/debug
        CONFIGURATIONS Debug
    )
    install(FILES
        ${ADTF_STREAMING_DIR}/lib/${ADTF_STREAMING_LIB}_${ADTF_STREAMING_LIB_VERSION}.dll
        DESTINATION bin
        CONFIGURATIONS RelWithDebInfo
    )
    install(FILES
        ${ADTF_STREAMING_DIR}/lib/${ADTF_STREAMING_LIB}_${ADTF_STREAMING_LIB_VERSION}.dll
        DESTINATION bin
        CONFIGURATIONS Release
    )
else (WIN32)
    install(FILES
        ${ADTF_STREAMING_DIR}/lib/lib${ADTF_STREAMING_LIB}D_${ADTF_STREAMING_LIB_VERSION}.so
        DESTINATION bin/debug
        CONFIGURATIONS Debug
    )
    install(FILES
        ${ADTF_STREAMING_DIR}/lib/lib${ADTF_STREAMING_LIB}_${ADTF_STREAMING_LIB_VERSION}.so
        DESTINATION bin
        CONFIGURATIONS RelWithDebInfo
    )
    install(FILES
        ${ADTF_STREAMING_DIR}/lib/lib${ADTF_STREAMING_LIB}_${ADTF_STREAMING_LIB_VERSION}.so
        DESTINATION bin
        CONFIGURATIONS Release
    )
endif (WIN32)
