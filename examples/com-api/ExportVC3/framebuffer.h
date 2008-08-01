//=---------------------------------------------------------------------=
//
// $Id$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 2008, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef _FRAMEBUFFER_H_
#define _FRAMEBUFFER_H_

typedef unsigned char byte;

// aligned malloc()
// TODO: move to #include "mxflib/system.h"

#ifdef _WIN32
// Windows 32. _aligned_malloc() and _aligned_free()

// Compiler-specific
#ifdef _MSC_VER
#else // _MSC_VER
#endif // _MSC_VER

#include <malloc.h>
static byte* malign( size_t size, size_t alignment ) { return (byte*)_aligned_malloc( size, alignment ); }
static void malign_free ( void * pmem ) {  _aligned_free( pmem); }

#elif defined(__APPLE__) // _WIN32

// Mac OSX: malloc() always aligned 16. no posix_memalign() or memalign()
#include <stdlib.h>
static byte* malign( size_t size, size_t alignment ) { return (byte*)malloc( size ); }
static void malign_free ( void * pmem ) {  free( pmem); }

#elif defined(__sun) // _WIN32

// Solaris 10 doesn't have posix_memalign(), but does have memalign()
#include <stdlib.h>
static byte* malign( size_t size, size_t alignment ) { return (byte*)memalign( alignment, size ); }
static void malign_free ( void * pmem ) {  free( pmem); }

#else // _WIN32

// linux and other gcc: posix_memalign() and free()
//				also supported by BSD since FreeBSD 7.0
#include <stdlib.h>
static byte* malign( size_t size, size_t alignment ) 
{ 
  void *mem; 
  if( 0==posix_memalign( &mem, alignment, size ))
    return (byte*)mem; 
  else // posix_memalign failed
    return (byte*)malloc( size );
} 

static void malign_free ( void * pmem ) {  free( pmem); }

#endif // _WIN32


// Aligned Buffer
class abuffer
{
protected:
	abuffer() { _mem=0; _size=0; }

public:
	abuffer(size_t size, size_t alignment=16) { _mem = malign( size, alignment ); _size=(_mem)?size:0; }

	~abuffer() { malign_free(_mem); }

	operator byte*() { return _mem; }
	byte& operator[](int i) { return _mem[i]; } 

	size_t size(){ return _size; }

private:
	byte *_mem;
	size_t _size;
};

// Buffer Layouts
enum _BufferLayout{
	Layout_Unknown,
	Layout_Raw,			// compressed, per m_EssenceFormat
	Layout_YUV,			// really 3 bytes per pixel
	Layout_RGB,			// 3 bytes per pixel
	Layout_UYVY,		// 4 bytes per 2 pixels
	Layout_P_UYVY,		// frame of packed 2 lsbs (1 byte per 2 pixels), then 4 bytes per 2 pixels
	Layout_UYVYI,		// 8 bytes per 2 pixels, msb-aligned, little-endian
	Layout_UYVYM,		// 8 bytes per 2 pixels, msb-aligned, big-endian
	Layout_BGRA,		// 4 bytes per pixel
	Layout_BGRAI,		// 8 bytes per pixel, msb-aligned, little-endian
};
typedef enum _BufferLayout BufferLayout_t;


// TODO: merge these into static functions of class framebuffer

//! LoadXYUV() - Local function to load an image file and optionally convert to target buffer layout
int LoadXYUV( BufferLayout_t target, byte *video_buffer, int len, int H, int V, const char *input_filename, 
			  bool isLittleEndian=true, bool isBigEndian=false, bool isP_UYVY=false );

//! LoadXRGB() - Local function to load an image file and optionally convert to target buffer layout
int LoadXRGB( BufferLayout_t target, byte *video_buffer, int len, int H, int V, const char *input_filename, 
			  bool isLittleEndian=true, bool isBigEndian=false, bool hasA=false, bool isRGB=false );

//! LoadRaw() - Local function to load a complete file - presumably containing a raw bitstream
int LoadRaw( BufferLayout_t target, byte *video_buffer, int len, const char *input_filename );

// Generate a video buffer containing uncompressed video representing the familiar colour bars test signal
int LoadXBars( BufferLayout_t target, byte *video_buffer, int len, int H, int V );

#endif // _FRAMEBUFFER_H_
