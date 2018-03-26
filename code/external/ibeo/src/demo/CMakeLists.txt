cmake_minimum_required(VERSION 2.8)
project(ibeosdk_demos)

MESSAGE(STATUS "#########################   DEMO   #############################")

# parse demo files
#######################################################
# Collect all demo files
#######################################################
file(GLOB DEMO_SOURCES *.cpp)


#######################################################
# Remove database demos if no Mongo DB support has been
# activated
#######################################################
IF (NOT WITH_MONGODB_SUPPORT)
	SET(DBREGEXP "([Dd]atabase|Db)")

	SET (RES)
	FOREACH (v IN LISTS DEMO_SOURCES)
		IF (${v} MATCHES ${DBREGEXP})
		ELSE()
			LIST(APPEND RES ${v})
		ENDIF()
	ENDFOREACH(v)
	SET(DEMO_SOURCES ${RES})
ENDIF()

#######################################################


include_directories(${CMAKE_SOURCE_DIR}/src)


#######################################################
# boost
#######################################################

IF (WIN32)
	IF (USE_THIS_BOOST_VERSION_INSTEAD)
		PrepareForBoost(${USE_THIS_BOOST_VERSION_INSTEAD})
		FIND_PACKAGE(Boost ${USE_THIS_BOOST_VERSION_INSTEAD} EXACT REQUIRED COMPONENTS ${NeededBoostComponents})
	ELSE()
		find_package(Boost REQUIRED COMPONENTS ${NeededBoostComponents})
	ENDIF()
ELSE()
	find_package(Boost REQUIRED COMPONENTS ${NeededBoostComponents})
ENDIF()
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

if (WITH_MONGODB_SUPPORT)
	get_filename_component(MONGOLIBPATH ${MONGO_CLIENT_DLL} DIRECTORY)
	link_directories(${MONGOLIBPATH})
endif (WITH_MONGODB_SUPPORT)

#######################################################
# create demos
#######################################################
foreach(demo ${DEMO_SOURCES})
	if(${MSVC})
		source_group("sources" FILES ${demo})
	endif()

	get_filename_component(demoName "${demo}" NAME_WE)
	if ( (${MSVC} STREQUAL "") OR (NOT ${demoName} STREQUAL "IbeoSdkDataLoggerDemo") )
		add_executable(${demoName} ${demo})

		IF(WITH_MONGODB_SUPPORT)
			TARGET_LINK_LIBRARIES (${demoName} ${MONGO_CLIENT_DLL} DbgHelp.lib)
		ENDIF(WITH_MONGODB_SUPPORT)

		#target_link_libraries(${demoName} -Wl,--whole-archive ibeosdk -Wl,--no-whole-archive)
		if (${MSVC})
		    set(SDKLINKLIB "ibeosdk")
		else()
		    set(SDKLINKLIB -Wl,--whole-archive ibeosdk -Wl,--no-whole-archive)
		endif()
		target_link_libraries(${demoName} ${SDKLINKLIB})

		if(${MSVC})
			target_compile_options(${demoName} PUBLIC /MP)
			SetVSCompileDefintions()
			target_compile_definitions(${demoName} PUBLIC ${VSCompileDefinitions})
			target_compile_definitions(${demoName} PUBLIC WIN32_LEAN_AND_MEAN)
		endif()

		# copy to bin folder
		add_custom_command(TARGET ${demoName} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${demoName}> ${IBEOSDK_BINDIR})
	endif()
endforeach()

#######################################################
