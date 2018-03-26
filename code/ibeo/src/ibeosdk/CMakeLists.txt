cmake_minimum_required(VERSION 2.8)

# SDK project itself (without demos)
project(ibeosdk)

MESSAGE(STATUS "#########################   IBEOSKD   #############################")


# run prebuild script for windows
#######################################################
if(WIN32)
	execute_process(COMMAND generateDataBlockList.bat
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()


#take all source and header files
#######################################################
file(GLOB_RECURSE SDK_SRCS *.cpp)
file(GLOB_RECURSE SDK_HDRS *.hpp *.h)

#######################################################
# Remove jpeg support files if no jpeg support has been
# activated.
#######################################################
IF (NOT WITH_JPEG_SUPPORT)
	file(GLOB JPEGSUPP_SRCS jpegsupport/*)
	LIST(REMOVE_ITEM SDK_SRCS ${JPEGSUPP_SRCS})

	file(GLOB_RECURSE JPEGSUPP_HDRS jpegsupport/*.hpp jpegsupport/*.h)
	LIST(REMOVE_ITEM SDK_HDRS ${JPEGSUPP_HDRS})
ENDIF()

#######################################################
# Remove database sources and headers if no Mongo DB
# support has been activated.
#######################################################
IF (NOT WITH_MONGODB_SUPPORT)
	SET(DBREGEXP "[Dd]atabase")

	SET (RES)
	FOREACH (v IN LISTS SDK_SRCS)
		IF (${v} MATCHES ${DBREGEXP})
		ELSE()
			LIST(APPEND RES ${v})
		ENDIF()
	ENDFOREACH(v)
	SET(SDK_SRCS ${RES})

	SET (RES)
	FOREACH (v IN LISTS SDK_HDRS)
		IF (${v} MATCHES ${DBREGEXP})
		ELSE()
			LIST(APPEND RES ${v})
		ENDIF()
	ENDFOREACH(v)
	SET(SDK_HDRS ${RES})
ENDIF()


#######################################################
# include directories
#######################################################
include_directories(${CMAKE_SOURCE_DIR}/src)

# find boost and include stuff
#######################################################

IF (WIN32)
	IF (USE_THIS_BOOST_VERSION_INSTEAD)
		PrepareForBoost(${USE_THIS_BOOST_VERSION_INSTEAD})
		FIND_PACKAGE(Boost ${USE_THIS_BOOST_VERSION_INSTEAD} EXACT REQUIRED COMPONENTS ${NeededBoostComponents} chrono regex)
	ELSE()
		find_package(Boost REQUIRED COMPONENTS ${NeededBoostComponents} chrono regex)
	ENDIF()
ELSE()
	find_package(Boost REQUIRED COMPONENTS ${NeededBoostComponents} chrono regex)
ENDIF()
include_directories(${Boost_INCLUDE_DIRS})

# targets
#######################################################

if(UNIX)
	add_library(${PROJECT_NAME} STATIC ${SDK_SRCS} ${SDK_HDRS})
	set_target_properties(${PROJECT_NAME} PROPERTIES
		VERSION ${IBEOSDK_VERSION}
		LIBRARY_OUTPUT_DIRECTORY ${IBEOSDK_LIBDIR}
		ARCHIVE_OUTPUT_DIRECTORY ${IBEOSDK_LIBDIR})
	target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} -lrt)
endif()


# ######################################################################


if(${MSVC})
	# create soruce filters for vs project
	foreach(f ${SDK_SRCS})
		# path relative to ibeosdk folder
		file(RELATIVE_PATH rp ${CMAKE_CURRENT_SOURCE_DIR} ${f})
		get_filename_component(folder ${rp} DIRECTORY)

		set(p "Source Files/${folder}")
		string(REPLACE / \\ p ${p})

		source_group(${p} FILES ${f})
	endforeach()

	# create header filters for vs project
	foreach(f ${SDK_HDRS})
		# path relative to ibeosdk folder
		file(RELATIVE_PATH rp ${CMAKE_CURRENT_SOURCE_DIR} ${f})
		get_filename_component(folder ${rp} DIRECTORY)

		set(p "Header Files/${folder}")
		string(REPLACE / \\ p ${p})

		source_group(${p} FILES ${f})
	endforeach()
	
	# static on windows
	add_library(${PROJECT_NAME} STATIC ${SDK_SRCS} ${SDK_HDRS})
	set_target_properties(${PROJECT_NAME} PROPERTIES
		VERSION "${IBEOSDK_MAJOR_VERSION}.${IBEOSDK_MINOR_VERSION}.${IBEOSDK_PATCH_VERSION}"
		LIBRARY_OUTPUT_DIRECTORY ${IBEOSDK_LIBDIR})
	target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${IBEOSDK_LIBDIR})
			
	target_compile_options(${PROJECT_NAME} PUBLIC /MP)# -D_WIN32 -D_DEBUG -D_WINDOWS -D_WIN32_WINNT=0x0601 -DBOOST_ALL_NO_LIB -DWIN32_LEAN_AND_MEAN -D_CRT_SECURE_NOWARNINGS)
	target_compile_definitions(${PROJECT_NAME} PUBLIC _WIN32 _WINDOWS _WIN32_WINNT=0x0601 BOOST_ALL_NO_LIB WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS _CONSOLE)

	SetVSCompileDefintions()
	target_compile_definitions(${PROJECT_NAME} PUBLIC ${VSCompileDefinitions})

#	target_compile_definitions(${PROJECT_NAME} PUBLIC _WIN32 _WINDOWS NTDDI_VERSION=0x06010000 _WIN32_WINNT=0x0601 WINVER=0x0601 BOOST_ALL_NO_LIB _CRT_SECURE_NO_WARNINGS _CONSOLE)
	IF (NOT WITH_MONGODB_SUPPORT)
		target_compile_definitions(${PROJECT_NAME} PUBLIC WIN32_LEAN_AND_MEAN)
	ENDIF(NOT WITH_MONGODB_SUPPORT)
endif()

#######################################################

# Check for libjpeg header and use our local copy if no other was found.
IF (WITH_JPEG_SUPPORT)
    IF (UNIX)
        SET (JPEG_INCLUDE_SEARCH_PATHS
             /usr/include
             /usr/local/include
             ${CMAKE_SOURCE_DIR}/libjpeg/include
             )
        SET (JPEG_DLL_SEARCH_PATHS
             /usr/lib /usr/local/lib
            )
    ELSE (UNIX)
        # On Windows, use our local copy only and no extra ones
        SET (JPEG_INCLUDE_SEARCH_PATHS
             ${CMAKE_SOURCE_DIR}/libjpeg/include
             NO_DEFAULT_PATH
             )
        SET (JPEG_DLL_SEARCH_PATHS
             ${JPEG_INCLUDE_DIR}/../lib
             NO_DEFAULT_PATH
            )
    ENDIF (UNIX)

    FIND_PATH (JPEG_INCLUDE_DIR
               NAMES jpeglib.h
               PATHS ${JPEG_INCLUDE_SEARCH_PATHS}
               )
    IF (NOT JPEG_INCLUDE_DIR)
        MESSAGE (SEND_ERROR "*** The header file <jpeglib.h> was not found, but is required. ***")
    ENDIF (NOT JPEG_INCLUDE_DIR)
    INCLUDE_DIRECTORIES (${JPEG_INCLUDE_DIR})

#    ##################################################
#    Check for jpeg dll (i.e. the shared library file)

    FIND_LIBRARY (JPEG_DLL
                  NAMES jpeg
                  PATHS ${JPEG_DLL_SEARCH_PATHS}
                  NO_CMAKE_FIND_ROOT_PATH
                 )
    IF (NOT JPEG_DLL)
        MESSAGE (SEND_ERROR "*** The jpeg library was not found, but is required. Please install libjpeg8-dev (for Ubuntu >=12.04) or libjepg62-dev (for Ubuntu <12.04). ***")
    ENDIF (NOT JPEG_DLL)
    TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${JPEG_DLL})
ENDIF(WITH_JPEG_SUPPORT)

#######################################################

#######################################################
