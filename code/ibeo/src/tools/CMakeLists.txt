cmake_minimum_required(VERSION 2.8)
project(ibeosdk_tools)
MESSAGE(STATUS "#########################   TOOLS   #############################")
#######################################################
# parse tools files
#######################################################

file(GLOB TOOL_SOURCES *.cpp)

include_directories(${CMAKE_SOURCE_DIR}/src)

#######################################################
# boost
#######################################################

IF (WIN32)
	IF (USE_THIS_BOOST_VERSION_INSTEAD)
		PrepareForBoost(${USE_THIS_BOOST_VERSION_INSTEAD})
		FIND_PACKAGE(Boost ${USE_THIS_BOOST_VERSION_INSTEAD} EXACT REQUIRED COMPONENTS ${NeededBoostComponents})
	ELSE()
		find_package(Boost REQUIRED COMPONENTS system ${NeededBoostComponents})
	ENDIF()
ELSE()
	find_package(Boost REQUIRED COMPONENTS ${NeededBoostComponents})
ENDIF()

include_directories(${Boost_INCLUDE_DIRS})

#######################################################
# create demos
#######################################################
foreach(tool ${TOOL_SOURCES})

	if(${MSVC})
		source_group("sources" FILES ${tool})
	endif()

	get_filename_component(toolName "${tool}" NAME_WE)

	add_executable(${toolName} ${tool})
	target_link_libraries(${toolName} ibeosdk)

	if(${MSVC})
		target_compile_options(${toolName} PUBLIC /MP)
		target_compile_definitions(${toolName} PUBLIC _WIN32 _DEBUG _WINDOWS NTDDI_VERSION=0x06010000 _WIN32_WINNT=0x0601 WINVER=0x0601 BOOST_ALL_NO_LIB _CRT_SECURE_NO_WARNINGS _CONSOLE)
		IF (NOT WITH_MONGODB_SUPPORT)
			target_compile_definitions(${toolName} PUBLIC WIN32_LEAN_AND_MEAN)
		ENDIF(NOT WITH_MONGODB_SUPPORT)
	endif()

	# copy to bin folder
	add_custom_command(TARGET ${toolName} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${toolName}> ${IBEOSDK_BINDIR})
endforeach()

#######################################################
