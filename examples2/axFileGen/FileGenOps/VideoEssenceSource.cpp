//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
//=---------------------------------------------------------------------=

#include "SampleSource.h"
#include "ParamMaps.h"

#include <axFileGen.h>

#include <AxEssence.h>

#include <assert.h>
#include <string.h>

namespace {


// old C code... cut/paste/modify... in need of cleaning

inline aafUInt32 pack32( unsigned char a, unsigned char  b, unsigned char c, unsigned char d = 0 )
{
	return ( (a << 24) | (b << 16)  | (c << 8) | d );
}

inline void unpack32( unsigned char& x, unsigned char& y, unsigned char& z, aafUInt32 val32)
{
	x = val32>>24;
	y = val32>>16;
	z = val32;
}

void  colorbars_test_pattern( void* fb_addr,
						      int width, int height,
							  int bytes_per_line,
							  int bpp )
{
  int r, g, b;
  int i;
  unsigned char* fb = (unsigned char*)fb_addr;

  struct { int r; int g; int b; } bars[9];

  memset( (void*)fb, 0, height*bytes_per_line );

  i = 1;
  for( g = 0xff; g >= 0; g -= 0xff ) {
	  for( r = 0xff; r >= 0; r -= 0xff ) { 
	      for( b = 0xff; b >= 0; b -= 0xff ) {
			   bars[i].r = 3*r/4;
			   bars[i].g = 3*g/4;
			   bars[i].b = 3*b/4;
			   i++;
		  }
	  }
  }
    
  for(i = 0; i < width; i++) {
    int color_index = 1 + (i / (width/7));
      assert( 32 == bpp );
      *(unsigned char*)(fb + 4*i) = pack32( bars[color_index].r,
								            bars[color_index].g,
								            bars[color_index].b );
  }

  for(i = 1; i < height; i++) {
    memcpy( (void*)(fb + i*bytes_per_line),
	    (const void*)(fb + (i-1)*bytes_per_line),
	    bytes_per_line);
  };
}

void copy_first_line_to_all_lines( void* fb_addr,
								   int height,
								   int bytes_per_line )
{
  int i;
  unsigned char* fb = (unsigned char*)fb_addr;

  for(i = 1; i < height; i++) {
    memcpy( (void*)(fb + i*bytes_per_line),
	    (const void*)(fb + (i-1)*bytes_per_line),
	    bytes_per_line);
  };
}

// Reference: Technical Intoduction To Digital Video,
// Charles Poyton, Eq 9.7

// Y'601     |  16 |            |   76.544  150.727   2.184 |
// Cb     =  | 128 |  + (1/256) |  -44.182  -86.740 130.922 |
// Cr        | 128 |            |  130.922 -109.631 -21.291 |

inline aafUInt32 rgb_pixel_to_ycrcb( aafUInt32 val )
{
	unsigned char r;
	unsigned char g;
	unsigned char b;

	unpack32( r, g, b, val );

	unsigned char y;
	unsigned char cb;
	unsigned char cr;

	y  = (unsigned char)( 16.0  + (  76.544*r + 150.727*g +   2.184*b )/256.0 );
	cb = (unsigned char)( 128.0 + ( -44.182*r -  86.740*g + 130.922*b )/256.0 );
	cr = (unsigned char)( 128.0 + ( 130.922*r - 109.631*g -  21.291*b )/256.0 );

	return pack32( r, g, b );
}

// Convert one line of pixels
void convert_line_rgb_to_ycrcb( aafUInt32* line,
 									   int width )    // number of 32 bit rgbx pixels
{
	int i;
	for (i = 0; i < width; i++, line++) {
		*line = rgb_pixel_to_ycrcb( *line );
	}
}

void convert_444yuv_to_422uyvy( aafUInt32* yuv,
								aafUInt32* uyvy,
								int width )
{
	int i;

	assert( width % 2 == 0 );

	for( i = 0; i < width; i += 2 ) {

		unsigned char y1;
		unsigned char u1;
		unsigned char v1;
	
		unsigned char y2;
		unsigned char u2;
		unsigned char v2;
	
		unpack32( y1, u1, v1, yuv[i] );
		unpack32( y2, u2, v2, yuv[i+1] );

		pack32( (u1+u2)/2, y1, (v1+v2)/2, y2 );
	}

}

void convert_444yuv_to_411uyvy( aafUInt32* yuv,
								aafUInt32* uyvy,
								int width )
{
	int i;

	assert( width % 4 == 0 );

	for( i = 0; i < width; i += 4 ) {

		unsigned char y1;
		unsigned char u1;
		unsigned char v1;
	
		unsigned char y2;
		unsigned char u2;
		unsigned char v2;
	
		unsigned char y3;
		unsigned char u3;
		unsigned char v3;

		unsigned char y4;
		unsigned char u4;
		unsigned char v4;

		unpack32( y1, u1, v1, yuv[i] );
		unpack32( y2, u2, v2, yuv[i+1] );
		unpack32( y2, u2, v2, yuv[i+2] );
		unpack32( y3, u3, v3, yuv[i+4] );

		pack32( (u1+u2+u3+u4)/4, y1, (v1+v2+v3+v4)/4, y2 );
	}

}

// caller takes ownership of allocated pointer
void create_422uyvy_bars_image( int width, int height, int* pBufSize, aafUInt8** ppBuf )
{
	// This is just one line.  rgbx  = one pixel per 32 bpp
	aafUInt32* rgbxlinebuf  = new aafUInt32[ width ];
	
	// buf size in 32 bit words
	int uyvyBufSize = height*width/2;

	aafUInt32* uyvybuf = new aafUInt32[ uyvyBufSize ];  

	colorbars_test_pattern( rgbxlinebuf, width, 1, sizeof(aafUInt32)*width, 8*sizeof(aafUInt32) );

	// This is a 444rgb to 444yuv conversion.
	convert_line_rgb_to_ycrcb( rgbxlinebuf, width );

	// Now convert to 422.
	// Call it yuv now... same as crcb for these purposes.
	convert_444yuv_to_422uyvy( rgbxlinebuf, uyvybuf, width );  

	delete rgbxlinebuf;

	copy_first_line_to_all_lines( uyvybuf, height, 2*width );

	*ppBuf = reinterpret_cast<aafUInt8*>(uyvybuf);
	*pBufSize = sizeof(aafUInt32) * uyvyBufSize;
}

// caller takes ownership of allocated pointer
void create_444rgbx_bars_image( int width, int height, int* pBufSize, aafUInt8** ppBuf )
{
	// buf size in 32 bit words.
	int rgbxBufSize = height*width; 

	aafUInt32* rgbxbuf = new aafUInt32[ rgbxBufSize ];

	colorbars_test_pattern( rgbxbuf, width, 1, sizeof(aafUInt32)*width, 8*sizeof(aafUInt32) );

	copy_first_line_to_all_lines( rgbxbuf, height, 4*width );

	*ppBuf = reinterpret_cast<aafUInt8*>(rgbxbuf);
	*pBufSize = sizeof(aafUInt32) * rgbxBufSize;
}


//=---------------------------------------------------------------------=

class BarsSource : public AxFGOp, public SampleSource { 

public:

  BarsSource ( const AxString& opName )
    : AxFGOp( opName ),
	  _count(0),
	  _numFrames(0),
	  _width(0),
	  _height(0)
	{}

  virtual ~BarsSource();

  virtual void Execute( const std::vector<AxString>& args );

  // SampleSource methods
  virtual void Reset();
  virtual std::auto_ptr< SampleSrcBuffer > GetNext();

private:
	int _count;
	int _numFrames;
	int _width;
	int _height;
	int _sampVert;
	int _sampHorz;
	aafPixelFormat_t _pixelFormat;
	AxString _descName;
};


AXFG_OP_FACTORY_DECLARATION(
  BarsSource,           
  L"BarsSource",
  L"Implements the BarsSampleSource protocol",
  L"BarsSampleSourceName  CDCIDescriptorName pixel_format num_frames",
  L"CDCIDescriptor used to determine size.  pixel_form is aafPixelFormat_t.",
  5,
  5 ) 

BarsSource::~BarsSource()
{}

void BarsSource::Execute( const std::vector<AxString>& argv )
{
	AxString sourceName   = argv[1];
	AxString cdciDescName = argv[2];
	AxString pixelFormat  = argv[3];
	AxString numFrames    = argv[4];

	IAAFCDCIDescriptorSP spDesc;
	GetInstance( cdciDescName ).GetCOM( spDesc );
	AxCDCIDescriptor axDesc( spDesc );

	// Hang on to this - it is useful in error messages.
	_descName = cdciDescName;

	_numFrames = AxStringUtil::strtol( numFrames );

	aafUInt32 w;
	aafUInt32 h;
	axDesc.GetStoredView( w, h );
	_width  = w;
	_height = h;

	_sampHorz = axDesc.GetHorizontalSubsampling();
	_sampVert = axDesc.GetVerticalSubsampling();

	// For the moment, require 1x1 sampling.
	// The DVExpress dump shows 1x1 sampling, but format is supposedly,
	// yuv so it doesn't mean chroma... assumed to be something used
	// by the codec only.
	if ( ! ( _sampHorz == 1 && _sampVert == 1 ) ) {
		throw AxFGOpUsageEx( *this, L"unsupported sampling" );
	}

	_pixelFormat = PixelFormatParams::GetInstance().Find( *this, pixelFormat );

	if ( _pixelFormat == kAAFPixNone ) {
		throw AxFGOpUsageEx( *this, L"\"" + pixelFormat + L"\" unsupported" );
	}

	RegisterInstance( sourceName );
}

void BarsSource::Reset()
{
	_count = 0;
}

std::auto_ptr< SampleSrcBuffer > BarsSource::GetNext()
{
	using namespace std;

	if ( _count == _numFrames ) {
		return auto_ptr<SampleSrcBuffer>( new SimpleSampleSrcBuffer() );
	}

	int numSamples = 1;
	int numBytes; 
	aafUInt8* buf;

	if ( _pixelFormat == kAAFPixRGBA ) {
		
		create_444rgbx_bars_image( _width, _height, &numBytes, &buf );
	
	}
	else if ( _pixelFormat == kAAFPixYUV ) {

		// Assume tthat PixYUV means 422
		create_422uyvy_bars_image( _width, _height, &numBytes, &buf );
	
	}
	else {
		throw AxFGEx( L"bad implementation" );
	}

	auto_ptr<aafUInt8> bufferToGiveUp( buf );
	
	auto_ptr<SampleSrcBuffer> srcBuffer(
		new SimpleSampleSrcBuffer( numSamples, numBytes, bufferToGiveUp ) );
	
	_count++;

	return srcBuffer;
}


} // end of namespace

