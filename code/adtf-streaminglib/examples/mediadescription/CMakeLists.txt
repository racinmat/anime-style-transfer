add_executable(mediadescription
    mediadescription.h
    mediadescription.cpp
)

target_link_libraries(mediadescription ${ADTF_STREAMING_LIB})

if(NOT WIN32)
    #this way the streaming lib will be loaded from the same directory that the binary resides in
    set_target_properties(mediadescription PROPERTIES INSTALL_RPATH "$ORIGIN/")
endif(NOT WIN32)

install(TARGETS mediadescription DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS mediadescription DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS mediadescription DESTINATION bin CONFIGURATIONS Release)
install(
    FILES
    stream1_media_desc.xml
    stream2_media_desc.xml
    DESTINATION bin/debug CONFIGURATIONS Debug
)
install(
    FILES
    stream1_media_desc.xml
    stream2_media_desc.xml
    DESTINATION bin CONFIGURATIONS RelWithDebInfo
)
