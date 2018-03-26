set(TARGET_NAME referencedfiles)

add_executable(${TARGET_NAME}
    referencedfiles.cpp
)

target_link_libraries(${TARGET_NAME} ${ADTF_STREAMING_LIB})

if(NOT WIN32)
    #this way the streaming lib will be loaded from the same directory that the binary resides in
    set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_RPATH "$ORIGIN/")
endif(NOT WIN32)

install(TARGETS ${TARGET_NAME} DESTINATION bin/debug CONFIGURATIONS Debug)
install(TARGETS ${TARGET_NAME} DESTINATION bin CONFIGURATIONS RelWithDebInfo)
install(TARGETS ${TARGET_NAME} DESTINATION bin CONFIGURATIONS Release)