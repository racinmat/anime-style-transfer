cmake_minimum_required(VERSION 2.8)
project(ibeosdk_unittests)

MESSAGE(STATUS "#########################   UNITTESTS   #############################")

add_subdirectory(common)

#######################################################
# parse unittests files
#######################################################

file(GLOB UNITTEST_SOURCES *.cpp)

include_directories(${CMAKE_SOURCE_DIR}/src)

#######################################################
# boost
#######################################################
IF (WIN32)
	IF (USE_THIS_BOOST_VERSION_INSTEAD)
		PrepareForBoost(${USE_THIS_BOOST_VERSION_INSTEAD})
		FIND_PACKAGE(Boost ${USE_THIS_BOOST_VERSION_INSTEAD} EXACT REQUIRED COMPONENTS ${NeededBoostComponents} unit_test_framework)
	ELSE()
		find_package(Boost REQUIRED COMPONENTS ${NeededBoostComponents} unit_test_framework)
	ENDIF()
ELSE()
	find_package(Boost REQUIRED COMPONENTS ${NeededBoostComponents} unit_test_framework)
ENDIF()
include_directories(${Boost_INCLUDE_DIRS})

#######################################################
# create unittests
#######################################################
foreach(unittest ${UNITTEST_SOURCES})
	if(${MSVC})
		source_group("sources" FILES ${unittest})
		
		foreach(f ${UNITTEST_COMMON_HEADERS})
			file(RELATIVE_PATH rp ${CMAKE_CURRENT_SOURCE_DIR} ${f})
			get_filename_component(folder ${rp} DIRECTORY)

			set(p "headers/${folder}")
			string(REPLACE / \\ p ${p})

			source_group(${p} FILES ${f})
		endforeach()
		
		foreach(f ${UNITTEST_COMMON_SOURCES})
			file(RELATIVE_PATH rp ${CMAKE_CURRENT_SOURCE_DIR} ${f})
			get_filename_component(folder ${rp} DIRECTORY)

			set(p "sources/${folder}")
			string(REPLACE / \\ p ${p})

			source_group(${p} FILES ${f})
		endforeach()
		
	endif()

	get_filename_component(unittestName "${unittest}" NAME_WE)
	add_executable(${unittestName} ${unittest} ${UNITTEST_COMMON_SOURCES} ${UNITTEST_COMMON_HEADERS})
	target_link_libraries(${unittestName} ibeosdk ${Boost_LIBRARIES})

	if(${MSVC})
		target_compile_options(${unittestName} PUBLIC /MP)
		target_compile_definitions(${unittestName} PUBLIC _WIN32 _DEBUG _WINDOWS NTDDI_VERSION=0x06010000 _WIN32_WINNT=0x0601 WINVER=0x0601 BOOST_ALL_NO_LIB _CRT_SECURE_NO_WARNINGS _CONSOLE)
		IF (NOT WITH_MONGODB_SUPPORT)
			target_compile_definitions(${unittestName} PUBLIC WIN32_LEAN_AND_MEAN)
		ENDIF(NOT WITH_MONGODB_SUPPORT)
	endif()


	# copy to bin folder
	add_custom_command(TARGET ${unittestName} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${unittestName}> ${IBEOSDK_BINDIR})
endforeach()

#######################################################
