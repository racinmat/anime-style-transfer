add_executable(eds
    eds.cpp
)

target_link_libraries(eds ${ADTF_STREAMING_LIB})

if(NOT WIN32)
    #this way the streaming lib will be loaded from the same directory that the binary resides in
    set_target_properties(eds PROPERTIES INSTALL_RPATH "$ORIGIN/")
endif(NOT WIN32)

install(TARGETS eds DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS eds DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS eds DESTINATION bin CONFIGURATIONS Release)
