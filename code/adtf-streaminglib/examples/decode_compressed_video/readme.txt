/**
 *
 * Demo Compressed Video
 *
 * @file
 * Copyright &copy; Audi Electronics Venture GmbH. All rights reserved.
 *
 * @author               $Author: voigtlpi $
 * @date                 $Date: 2009-07-16 15:36:37 +0200 (Do, 16 Jul 2009) $
 * @version              $Revision: 10093 $
 *
 * @remarks
 *
 */
 
/**
 * \page page_decode_compressed_video Decode Compressed Video
 *
 * Implements an example program to decompress of compressed video streams
 * Remember: on windows the streaming lib dll must be in your path, on Linux in your LD_LIBRARAY_PATH
 * 
 * \par Location
 * \code
 *    ./examples/decode_compressed_video/
 * \endcode
 *
 * \par This example shows:
 * \li how to find a compressed video stream
 * \li how to decompress the video data. Available codes are: adtf (rice), vfw and ffmpeg.
 * 
 * \par The Implementation for the Compressed Video
 * \include decode_compressed_video.cpp
 *
 */