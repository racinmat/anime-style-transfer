cmake_minimum_required(VERSION 2.8)
project(ibeosdk_unittests)

# parse unittests/common files
#######################################################

file(GLOB COMMON_SOURCES *.cpp)
file(GLOB COMMON_HEADERS *.hpp)

SET (UNITTEST_COMMON_SOURCES ${COMMON_SOURCES} PARENT_SCOPE)
SET (UNITTEST_COMMON_HEADERS ${COMMON_HEADERS} PARENT_SCOPE)

#######################################################
