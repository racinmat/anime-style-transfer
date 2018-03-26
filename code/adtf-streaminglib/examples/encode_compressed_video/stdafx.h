/**
 *
 * ADTF File Access example
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved
 *
 * $Author: VG8D3AW $
 * $Date: 2010-04-16 10:06:05 +0200 (Fr, 16 Apr 2010) $
 * $Revision: 3303 $
 *
 * @remarks
 *
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <memory>
using namespace std;

#include <adtf_streaming.h>
using namespace adtfstreaming;

#include "codec_intf.h"

#include "codec_adtf/codec_adtf.h"

// system includes
#ifdef WIN32
    #include <windows.h>
    #include <Vfw.h>
    #include "codec_vfw/codec_vfw.h"
#else
    #ifdef FFMPEG_AVAILABLE
        #include "codec_ffmpeg/codec_ffmpeg.h"
    #endif
#endif

