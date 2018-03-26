cmake_minimum_required(VERSION 2.8)

#######################################################
OPTION(OPTIMIZE = OFF)

#######################################################
# Version numbering
#######################################################
SET(IBEOSDK_VERSION 5.2.2)

#######################################################
# path definition
#######################################################
SET(IBEOSDK_LIBDIR ${CMAKE_SOURCE_DIR}/libs )
SET(IBEOSDK_BINDIR ${CMAKE_SOURCE_DIR}/bin)

OPTION (WITH_JPEG_SUPPORT "Enable jpeg support. Need libjpeg be installed." OFF)
OPTION (WITH_MONGODB_SUPPORT "Enable support for Mongo DB, needs mongo-client and headers to e installed." OFF)

OPTION (WITH_DEMOS "Enable build of demos" ON)
OPTION (WITH_TOOLS "Enable build of tools" OFF)
OPTION (WITH_UNITTESTS "Build unittests" OFF)

OPTION (LINK_STATIC_BOOST "Enable usage of static boost libs instead of dynamic" OFF)

IF (WIN32)
	SET (USE_THIS_BOOST_VERSION_INSTEAD "" CACHE STRING "If set, cmake will try to find this boost version. E.g. 1.55")
ENDIF(WIN32)


#######################################################
# FLAGS
#######################################################
if(UNIX)
    SET(CMAKE_CXX_FLAGS "-Wall -ansi -Winit-self -Wno-long-long -Wformat -Wenum-compare")
    IF(WITH_MONGODB_SUPPORT)
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-strict-aliasing")
    ELSE()
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic -Wextra -Wshadow -Wconversion -Wsign-conversion -pedantic  -Wsign-conversion -Wshadow -Wconversion")
    ENDIF()
endif()

IF(LINK_STATIC_BOOST)
    set(Boost_USE_STATIC_LIBS ON)
ELSE()
    set(Boost_USE_STATIC_LIBS OFF)
ENDIF()

IF(${OPTIMIZE})
	SET(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -O3 -DNDBEUG")
ELSE()
	SET(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -g")
ENDIF()

#######################################################
# Documentation
#######################################################

find_package(Doxygen)
if(DOXYGEN_FOUND)
	add_custom_target(doc
	${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/src/Doxyfile
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src
	COMMENT "Generating API documentation with Doxygen" VERBATIM
	)
endif(DOXYGEN_FOUND)

#######################################################
# Zip archive
#######################################################

add_custom_target(archive
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	COMMAND cd ${CMAKE_CURRENT_BINARY_DIR} && make doc
	COMMAND cd ${CMAKE_SOURCE_DIR}
	COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/IbeoSDK${IBEOSDK_VERSION}.zip
	COMMAND git archive --format=zip -o IbeoSDK${IBEOSDK_VERSION}.zip HEAD
	COMMAND zip -9 IbeoSDK${IBEOSDK_VERSION}.zip ${CMAKE_SOURCE_DIR}/src/docs/html/*
	COMMAND mv IbeoSDK${IBEOSDK_VERSION}.zip ${CMAKE_CURRENT_BINARY_DIR}
)

#######################################################

add_subdirectory(src)
