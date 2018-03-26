add_executable(additionaldata
    additionaldata.cpp
)

target_link_libraries(additionaldata ${ADTF_STREAMING_LIB})

if(NOT WIN32)
    #this way the streaming lib will be loaded from the same directory that the binary resides in
    set_target_properties(additionaldata PROPERTIES INSTALL_RPATH "$ORIGIN/")
endif(NOT WIN32)

install(TARGETS additionaldata DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS additionaldata DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS additionaldata DESTINATION bin CONFIGURATIONS Release)
