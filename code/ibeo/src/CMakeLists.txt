cmake_minimum_required(VERSION 2.8)

# ######################################################

FUNCTION (PrepareForBoost version)
#	SET (Boost_DEBUG ON)
	if(${MSVC})
		if (LINK_STATIC_BOOST)
			set(Boost_USE_STATIC_LIBS ON PARENT_SCOPE)
		else (LINK_STATIC_BOOST)
			set(Boost_USE_STATIC_LIBS OFF PARENT_SCOPE)
		endif (LINK_STATIC_BOOST)
		set(Boost_USE_MULTITHREADED ON PARENT_SCOPE)
		set(Boost_USE_STATIC_RUNTIME OFF PARENT_SCOPE)
	endif()
	SET(BOOST_NO_SYSTEM_PATHS ON PARENT_SCOPE)
	SET(BOOST_ROOT "C:\\" PARENT_SCOPE)

	FIND_PACKAGE(Boost ${version} EXACT)
		IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
		SET (PLATFORM lib64-msvc)
	ELSE()
		SET (PLATFORM lib32-msvc)
	ENDIF()

	IF (MSVC14)
		SET (LIBDIR ${PLATFORM}-14.0)
	ELSEIF (MSVC12)
		SET (LIBDIR ${PLATFORM}-12.0)
	ELSEIF (MSVC11)
		SET (LIBDIR ${PLATFORM}-11.0)
	ELSEIF (MSVC10)
		SET (LIBDIR ${PLATFORM}-10.0)
	ELSEIF (MSVC90)
		SET (LIBDIR ${PLATFORM}-9.0)
	ENDIF()

	SET (BOOST_LIBRARYDIR ${Boost_INCLUDE_DIRS}/${LIBDIR} PARENT_SCOPE)
ENDFUNCTION()

FUNCTION (SetVSCompileDefintions)
	SET(CompDefs "_WIN32;_WINDOWS;_CRT_SECURE_NO_WARNINGS;_CONSOLE")
	SET(CompDefs ${CompDefs} "NTDDI_VERSION=0x06010000;_WIN32_WINNT=0x0601;WINVER=0x0601")
	if (NOT LINK_STATIC_BOOST)
		SET(CompDefs ${CompDefs} "BOOST_ALL_DYN_LINK") #BOOST_ALL_NO_LIB
	endif (NOT LINK_STATIC_BOOST)
	SET (VSCompileDefinitions ${CompDefs} PARENT_SCOPE)
ENDFUNCTION()

#######################################################
# Boost Components
#######################################################

IF (WITH_MONGODB_SUPPORT)
    SET(NeededBoostComponents system thread program_options date_time filesystem)
ELSE()
    SET(NeededBoostComponents system thread program_options date_time)
# ibeosdk: add chrono regex
# tests: add unit_test_framework
ENDIF(WITH_MONGODB_SUPPORT)

# ######################################################

IF (WITH_MONGODB_SUPPORT)
    IF (UNIX)
        SET (MONGOCLIENT_INCLUDE_SEARCH_PATHS
             /usr/include
             /usr/local/include
             /opt/mongo-client/2.6.9/include)
        SET (MONGOCLIENT_DLL_SEARCH_PATHS
             /usr/lib
             /usr/local/lib
             /opt/mongo-client/2.6.9/lib)
    ELSE()
        # On Windows, use our local copy only and no extra ones
        SET (MONGOCLIENT_INCLUDE_SEARCH_PATHS
             ${CMAKE_SOURCE_DIR}/mongo-client/include
             NO_DEFAULT_PATH)
        SET (MONGOCLIENT_DLL_SEARCH_PATHS
             ${MONGOCLIENT_INCLUDE_DIR}/../lib
             NO_DEFAULT_PATH)
    ENDIF()

#    ##################################################
    FIND_PATH(MONGOCLIENT_INCLUDE_DIR
              NAMES mongo/db/json.h
              PATHS ${MONGOCLIENT_INCLUDE_SEARCH_PATHS})
    IF (NOT MONGOCLIENT_INCLUDE_DIR)
        MESSAGE (SEND_ERROR "*** The header file <mongo/db/json.h> was not found, but is required. ***")
    ENDIF (NOT MONGOCLIENT_INCLUDE_DIR)
    INCLUDE_DIRECTORIES (${MONGOCLIENT_INCLUDE_DIR})

#    ##################################################
    FIND_LIBRARY (MONGO_CLIENT_DLL
                  NAMES libmongoclient.a
                  PATHS ${MONGOCLIENT_DLL_SEARCH_PATHS}
                  NO_CMAKE_FIND_ROOT_PATH)
    IF (NOT MONGO_CLIENT_DLL)
        MESSAGE (SEND_ERROR "*** The libmongoclient.a library was not found, but is required. Please install mongodb-dev. ***")
    ENDIF (NOT MONGO_CLIENT_DLL)
#    TARGET_LINK_LIBRARIES (${PROJECT_NAME} ${MONGO_CLIENT_DLL})

#	INCLUDEPATH += -I/opt/mongo-client/2.6.9/include/
#	CXXFLAGS += -Wno-ignored-qualifiers -Wno-unused-parameter -Wno-strict-aliasing
ENDIF(WITH_MONGODB_SUPPORT)

# ######################################################

add_subdirectory(ibeosdk)

# ######################################################

IF (WITH_DEMOS)
	add_subdirectory(demo)
ENDIF()

IF (WITH_TOOLS)
	add_subdirectory(tools)
ENDIF()

IF (WITH_UNITTESTS)
	add_subdirectory(unittests)
ENDIF()
