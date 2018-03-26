if(WIN32)
    set(CODEC_VFW_HDRS codec_vfw/codec_vfw.h)
    set(CODEC_VFW_SRCS codec_vfw/codec_vfw.cpp)
else(WIN32)
    if(ADTF_FFMPEG_DIR)
        set(CODEC_FFMPEG_HDRS codec_ffmpeg/codec_ffmpeg.h)
        set(CODEC_FFMPEG_SRCS codec_ffmpeg/codec_ffmpeg.cpp)
    endif(ADTF_FFMPEG_DIR)
endif(WIN32)

set(CODEC_ADTF_HDRS codec_adtf/codec_adtf.h codec_adtf/rice.h codec_adtf/codec_adtf.h)
set(CODEC_ADTF_SRCS codec_adtf/codec_adtf.cpp codec_adtf/rice.cpp codec_adtf/codec_adtf.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(encode_compressed_video
    ${CODEC_ADTF_HDRS}
    ${CODEC_ADTF_SRCS}
    ${CODEC_FFMPEG_HDRS}
    ${CODEC_FFMPEG_SRCS}
    ${CODEC_VFW_HDRS}
    ${CODEC_VFW_SRCS}

    encode_compressed_video.cpp
    codec_intf.h
    stdafx.h
    stdafx.cpp
)

target_link_libraries(encode_compressed_video adtfstreaming)

if (WIN32)
    target_link_libraries(encode_compressed_video Vfw32)
else (WIN32)
    if(ADTF_FFMPEG_DIR)
        target_link_libraries(encode_compressed_video avutil avformat avcodec)
    endif(ADTF_FFMPEG_DIR)
    set_target_properties(encode_compressed_video PROPERTIES INSTALL_RPATH "$ORIGIN/;${ADTF_FFMPEG_DIR}/lib")
endif (WIN32)

install(TARGETS encode_compressed_video DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS encode_compressed_video DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS encode_compressed_video DESTINATION bin CONFIGURATIONS Release)