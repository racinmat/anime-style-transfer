/**
 *
 * Main Library include file.
 * Within windows the linker directive will be added by the pragma state.
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: belkera $
 * $Date: 2011-07-01 08:32:18 +0200 (Fr, 01 Jul 2011) $
 * $Revision: 7973 $
 *
 * @remarks This file is generated. Do not edit!
 *
 */
#ifndef _ADTF_STREAMING_LIBRARY_DLL_ 
#define _ADTF_STREAMING_LIBRARY_DLL_

#ifdef WIN32
    #ifdef STREAMINGLIB_EXPORTS
        #pragma message ("Create ADTF Streaming Library ")
        // export symbols
        #define DOEXPORT __declspec( dllexport )
    #else
        #pragma message ("Use dynamic ADTF Streaming Library ")
        #ifdef _DEBUG
            #pragma comment( lib, "adtfstreamingd_290.lib" )
        #else
            #pragma comment( lib, "adtfstreaming_290.lib" )
        #endif
        #define DOEXPORT __declspec( dllimport )
    #endif
#else
    #ifdef STREAMINGLIB_EXPORTS
        #define DOEXPORT __attribute__ ((visibility("default")))
    #else
        #define DOEXPORT
    #endif
#endif

//standard includes 
#include <stdlib.h>
#include <string.h>

//adtf base types and errors
#include "adtf_base_ref.h"

//streaming lib version
#include "adtf_streaming_version.h"

//adtf streaming lib package headers
#include "adtf_streaming_pkg.h"

#endif //_ADTF_STREAMING_LIBRARY_DLL_
