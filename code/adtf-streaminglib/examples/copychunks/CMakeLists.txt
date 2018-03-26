add_executable(copychunks
    copychunks.cpp
)

target_link_libraries(copychunks ${ADTF_STREAMING_LIB})

if(NOT WIN32)
    #this way the streaming lib will be loaded from the same directory that the binary resides in
    set_target_properties(copychunks PROPERTIES INSTALL_RPATH "$ORIGIN/")
endif(NOT WIN32)

install(TARGETS copychunks DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS copychunks DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS copychunks DESTINATION bin CONFIGURATIONS Release)
