cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

cmake_policy(SET CMP0002 NEW)
cmake_policy(SET CMP0003 NEW)
cmake_policy(SET CMP0011 NEW)

project(adtf_streaminglib_examples)

find_package (ADTF_STREAMING 2.9.0 PATHS ${CMAKE_CURRENT_SOURCE_DIR} REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS true)

set(ADTF_BUILD_EXAMPLES true)
add_subdirectory(examples)
