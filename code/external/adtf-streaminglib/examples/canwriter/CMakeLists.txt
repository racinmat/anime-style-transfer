add_executable(canwriter
    canstruct.h
    canwriter.cpp
)

target_link_libraries(canwriter ${ADTF_STREAMING_LIB})

if(NOT WIN32)
    #this way the streaming lib will be loaded from the same directory that the binary resides in
    set_target_properties(canwriter PROPERTIES INSTALL_RPATH "$ORIGIN/")
endif(NOT WIN32)

install(TARGETS canwriter DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS canwriter DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS canwriter DESTINATION bin CONFIGURATIONS Release)
