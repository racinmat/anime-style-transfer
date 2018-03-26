add_executable(candump
    canstruct.h
    candump.cpp
)

target_link_libraries(candump ${ADTF_STREAMING_LIB})

if(NOT WIN32)
    #this way the streaming lib will be loaded from the same directory that the binary resides in
    set_target_properties(candump PROPERTIES INSTALL_RPATH "$ORIGIN/")
endif(NOT WIN32)

install(TARGETS candump DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS candump DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS candump DESTINATION bin CONFIGURATIONS Release)
