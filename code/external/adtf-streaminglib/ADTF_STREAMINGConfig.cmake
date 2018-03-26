#######################################################################
#
# ADTF StreamingLib cmake config file
#
# Copyright; Audi Electronics Venture GmbH. All rights reserved.
#
# $Author: belkera $
# $Date: 2009-11-25 11:01:54 +0100 (Wed, 25 Nov 2009) $
# $Revision: 11826 $
#
#######################################################################

## \page page_cmake_overview ADTF CMake Build System
# \li For an introduction into the ADTF CMake build system please have a look at
# \subpage page_cmake_basics.
# \li For instructions on how to build the ADTF StreamingLib examples please
# have a look at \subpage page_cmake_examples.
#
# \page page_cmake_examples Using CMake to build the ADTF StreamingLib Examples
# ADTF StreamingLib ships CMake build files and configurations that require at
# least <b>CMake Version 2.8.4</b>.
# 
# Only the <b>Debug</b> and the <b>RelWithDebInfo</b> configurations are
# supported by ADTF StreamingLib.
# 
# \section Sec1 Building the examples
# 
# Note that if you paste some paths from the clipboard into cmake gui, make sure that you
# replace all backslashes with <b>forward slashes</b> (paste it, then press "..." and ok)!
# 
# -# Launch cmake-gui
# -# Set the source directory to the installation directory of ADTF StreamingLib
#   (e.g C:/Tools/adtf-streaminglib).
# -# Set the build/binary directory to a NEW and different directory (e.g.
#    C:/Work/streaminglib-build)
# -# Press configure.
# -# Click 'yes' when cmake asks to create the build directory.
# -# Select your compiler in the dialog that appears now and press 'Finish'.
# -# You will now get red values in the list, red does not mean wrong but new!
# -# Press Configure again.
# -# Set CMAKE_INSTALL_PREFIX to the installation directory of ADTF StreamingLib
#    (e.g C:/Tools/adtf-streaminglib).
#    You can choose any directory you like.
# -# Press Configure again.
# -# You should now have everything ready and no red values.
# -# On Linux make sure that you set CMAKE_BUILD_TYPE to either "Debug" or
#   "RelWithDebInfo" and if you get an empty ADTF_FONTCONFIG_LIBRARY please
#   install the fontconfig development package of your linux distribution.
# -# Press Generate.
# -# Now go to the build directory:
#    - On Windows open the adtf_streaminglib_examples solution with
#      Visual Studio and build it. Don't forget to build the INSTALL project
#      after everything has been built.
#    - On Linux type 'make install' in the build directory.
# -# done
#
# \page page_cmake_basics ADTF CMake Introduction
# \section sec1 What is CMake?
# CMake is a meta build system that lets you generate native build environments
# for multiple platforms and compilers with a single configuration. For example,
# it enables you generate Visual Studio solutions on Windows and Unix Makefile
# projects on Linux using gcc. This page is intended for introductionary use
# only. For a complete CMake documentation please visit http://www.cmake.org.
#
# \section sec2 Getting CMake
# ADTF StreamingLib requires at least <b>CMake Version 2.8.4</b>.
# \li <b>Windows:</b> Head over to
# http://www.cmake.org/cmake/resources/software.html and download the current
# version there.
# \li <b>Linux:</b> If your distribution provides packages for cmake >= 2.8.4
# just install these packages and your good to go.
# Otherwise head over to http://www.cmake.org/cmake/resources/software.html and
# download the binaries for Linux. Note that the i386 binaries work just fine on
# a 64Bit Linux system. After you have extracted the binaries make sure to put
# the contained 'bin' directory in your PATH variable.
#
# \section sec3 Basic Concepts
# Each CMake project has a CMakeLists.txt file at the root of its directory
# hierarchy. This file defines the project and contains the necessary
# instructions to build it. For example CMakeLists.txt files see section
# \ref secCMakeExamples. CMake supports both so called out-of-source builds and
# in-source builds. We discourage the use of the latter since it will clutter
# your source directory with multiple files. So building a CMake project
# typically involves three directories:
# -# The source directory: this is the directory where the CMakeLists.txt and
#    all other source files (i.e. .cpp and .h files) reside.
# -# The build directory: this is where the native build system and intermediate
#    build artifacts will be put.
# -# The installation directory: this is the directory where everything that
#    makes up the final product will be placed.
#
# Building a CMake project requires the following steps to be carried out:
# -# Creating a native build system using cmake (most likely with cmake-gui)
# -# Building with the native build system, i.e Visual Studio or Make.
#
# \section sec4 Creating a Native Build System
#
# To create a native build system launch cmake-gui and follow these steps:
# -# Enter the source directory on top of the gui.
# -# Enter the build directory, choose a new and empty directory.
# -# Press Configure.
# -# CMake will ask what kind of a project you want to generate, choose the one
#    to your liking.
# -# CMake will now try to find all required external dependencies and will
#    present a list of variables.
# -# Please keep in mind that variables marked red are not necessarily errors
#    but just new.
# -# Now fill those variables and press configure and repeat these steps until
#    the Generate button gets active.
# -# While doing so keep an eye on the log window in the lower part of the
#    cmake-gui that will provide hints on how to resolve problems that might
#    occur.
# -# On Linux make sure that you set CMAKE_BUILD_TYPE to either "Debug" or
#    "RelWithDebInfo".
# -# Hovering over a variable will bring up a descriptive tooltip.
# -# Press Generate.
#
# \section Building
# - Visual Studio: Just open the generated solution file (in the build
#   directory) and build it.
#   \note Build the INSTALL target to copy all files to their correct locations
#          in the installation directory.
# - Linux/Make: Enter the build directory and run 'make install'.
#
# \section secCMakeExamples CMakeLists.txt Examples
#
# \subsection sub4 ADTF Executable
# \code
# # This is required by CMake
# cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)
# # Say that we want to use ADTF Streaming Library
# find_package(ADTF_STREAMING 2.9.0 REQUIRED)
# # Give our project a name
# project(adtf_executable_example)
# # Specify the executable target
# add_executable(demo_exe
#     stdafx.h
#     stdafx.cpp
#     demo_exe.h
#     demo_exe.cpp
# )
# # Link the executable against ADTF Streaming Library
# target_link_libraries(demo_exe
#      ${ADTF_STREAMING_LIB}
# )
# # Specify where it should be installed to
# install(TARGETS demo_exe
#     DESTINATION bin
# )
# \endcode
#
#######################################################################

# execute only once
if (ADTF_STREAMING_FOUND)
    return()
endif(ADTF_STREAMING_FOUND)

cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)

cmake_policy(SET CMP0011 NEW)

include_directories(${ADTF_STREAMING_DIR}/include)
link_directories(${ADTF_STREAMING_DIR}/lib)

set (ADTF_STREAMING_LIB_VERSION ${ADTF_STREAMING_VERSION_MAJOR}${ADTF_STREAMING_VERSION_MINOR}${ADTF_STREAMING_VERSION_PATCH})

if (NOT ADTF_STREAMING_LIB)
    set (ADTF_STREAMING_LIB adtfstreaming)
endif (NOT ADTF_STREAMING_LIB)

if (NOT ADTF_BUILD_STREAMING)
    if (WIN32)
        add_library(${ADTF_STREAMING_LIB} STATIC IMPORTED)
        set_target_properties(${ADTF_STREAMING_LIB} PROPERTIES
            IMPORTED_LOCATION_DEBUG ${ADTF_STREAMING_DIR}/lib/${ADTF_STREAMING_LIB}D_${ADTF_STREAMING_LIB_VERSION}.lib
            IMPORTED_LOCATION_RELWITHDEBINFO ${ADTF_STREAMING_DIR}/lib/${ADTF_STREAMING_LIB}_${ADTF_STREAMING_LIB_VERSION}.lib
            IMPORTED_LOCATION_RELEASE ${ADTF_STREAMING_DIR}/lib/${ADTF_STREAMING_LIB}_${ADTF_STREAMING_LIB_VERSION}.lib
        )
    else (WIN32)
        add_library(${ADTF_STREAMING_LIB} SHARED IMPORTED)
        set_target_properties(${ADTF_STREAMING_LIB} PROPERTIES
            IMPORTED_LOCATION_DEBUG ${ADTF_STREAMING_DIR}/lib/lib${ADTF_STREAMING_LIB}D_${ADTF_STREAMING_LIB_VERSION}.so
            IMPORTED_LOCATION_RELWITHDEBINFO ${ADTF_STREAMING_DIR}/lib/lib${ADTF_STREAMING_LIB}_${ADTF_STREAMING_LIB_VERSION}.so
            IMPORTED_LOCATION_RELEASE ${ADTF_STREAMING_DIR}/lib/lib${ADTF_STREAMING_LIB}_${ADTF_STREAMING_LIB_VERSION}.so
        )
    endif(WIN32)
endif (NOT ADTF_BUILD_STREAMING)
