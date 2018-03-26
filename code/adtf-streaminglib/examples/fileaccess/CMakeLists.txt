add_executable(fileaccess
    fileaccess.cpp
)

target_link_libraries(fileaccess ${ADTF_STREAMING_LIB})

if(NOT WIN32)
    #this way the streaming lib will be loaded from the same directory that the binary resides in
    set_target_properties(fileaccess PROPERTIES INSTALL_RPATH "$ORIGIN/")
endif(NOT WIN32)

install(TARGETS fileaccess DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS fileaccess DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS fileaccess DESTINATION bin CONFIGURATIONS Release)
