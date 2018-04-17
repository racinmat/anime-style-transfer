cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(dat-unpacker)
enable_testing()

if( NOT CMAKE_BUILD_TYPE )
  set( CMAKE_BUILD_TYPE Release CACHE STRING
       "Choose the type of build, options are: None Debug Release RelWithDebInfo
MinSizeRel."
       FORCE )
endif()

set(ADTF_STREAMING_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../external/adtf-streaminglib)
set(IBEO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../external/ibeo)
set(IBEO_INCLUDE ${IBEO_DIR}/src)
set(IBEO_LIB_DIR ${IBEO_DIR}/libs)

find_package(Boost COMPONENTS program_options system thread date_time filesystem REQUIRED)
find_package(ADTF_STREAMING REQUIRED)
find_package(Qt5Core)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${ADTF_STREAMING_LIBRARY_INCLUDE_DIRS}
    ${IBEO_INCLUDE}
    include
    )

link_directories(
    ${Boost_LIBRARY_DIRS}
    ${ADTF_STREAMING_LIBRARY_DIRS}
    ${IBEO_LIB_DIR}
    )

file(GLOB dat-unpacker_SRC
    src/*.cpp
	include/*.h
)

add_executable(dat-unpacker
    ${dat-unpacker_SRC}
)

target_link_libraries(dat-unpacker ${ADTF_STREAMING_LIB} ${Boost_FILESYSTEM_LIBRARY} ${Boost_PROGRAM_OPTIONS_LIBRARY} Qt5::Core ibeosdk_d ${Boost_LIBRARIES})
