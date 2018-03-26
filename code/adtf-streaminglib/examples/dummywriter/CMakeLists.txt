add_executable(dummywriter
    canstruct.h
    dummywriter.cpp
)

target_link_libraries(dummywriter ${ADTF_STREAMING_LIB})

if(NOT WIN32)
    #this way the streaming lib will be loaded from the same directory that the binary resides in
    set_target_properties(dummywriter PROPERTIES INSTALL_RPATH "$ORIGIN/")
endif(NOT WIN32)

install(TARGETS dummywriter DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS dummywriter DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS dummywriter DESTINATION bin CONFIGURATIONS Release)
