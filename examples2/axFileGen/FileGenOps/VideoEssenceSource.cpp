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

	return pack32( y, cb, cr );
}

// Convert one line of pixels
void convert_line_rgbx_to_ycrcbx( aafUInt32* linergbx,
                                  aafUInt32* lineycrcbx,
								  int width )
{
	int i;
	for (i = 0; i < width; i++, linergbx++, lineycrcbx) {
		*lineycrcbx = rgb_pixel_to_ycrcb( *linergbx );
	}
}

void convert_line_444yuvx_to_422uyvy( aafUInt32* yuv,
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

void convert_rgbx_to_yuv422( int width, int height, int stride, aafUInt8* rgbxBuf,
						int& convertedBufSize, aafUInt8*& convertedBuf )
{
	int uyvyBufStride = width*4/2;             // 2 pixels fit in 4 in bytes

	// Currently, this routine requires that strides are 32 bit aligned.
	assert( 0 == uyvyBufStride % 4 );
	assert( 0 == stride % 4 );	

	int uyvyBufSize = uyvyBufStride*height;  // in bytes
	aafUInt32* uyvyBuf = new aafUInt32[ uyvyBufSize/4 ];

	// Store one line of 8 bit per component 444yuvx in this buffer.
	aafUInt32* yuvxLineBuf = new aafUInt32[ width ];

	int i;

	for ( i = 0; i < height; i++ ) {

		convert_line_rgbx_to_ycrcbx( reinterpret_cast<aafUInt32*>(rgbxBuf + i*stride),
									 yuvxLineBuf, width );
		
		convert_line_444yuvx_to_422uyvy( yuvxLineBuf, uyvyBuf + i*uyvyBufStride/4, width );					  
	}	

	delete yuvxLineBuf;

	convertedBufSize = uyvyBufSize;
	convertedBuf = reinterpret_cast<aafUInt8*>(uyvyBuf);
}

void convert_rgbx_to_yuvx4444( int width, int height, int stride, aafUInt8* rgbxBuf,
						int& convertedBufSize, aafUInt8*& convertedBuf )
{
	int uyvyBufStride = width*4;             // in bytes

	// Currently, this routine requires that strides are 32 bit aligned.
	assert( 0 == uyvyBufStride % 4 );
	assert( 0 == stride % 4 );	

	int uyvyBufSize = uyvyBufStride*height;  // in bytes
	aafUInt32* uyvyBuf = new aafUInt32[ uyvyBufSize/4 ];

	int i;

	for ( i = 0; i < height; i++ ) {

		convert_line_rgbx_to_ycrcbx( reinterpret_cast<aafUInt32*>(rgbxBuf + i*stride),
									 uyvyBuf + i*uyvyBufStride/4, width );
		
	}	
	
	convertedBufSize = uyvyBufSize;
	convertedBuf = reinterpret_cast<aafUInt8*>(uyvyBuf);
}

// caller takes ownership of allocated pointer
void create_rgbx_bars_image( int width, int height, int& stride, int& bufSize, aafUInt8*& pBuf )
{
	// buf size in 32 bit words.
	int rgbxBufSize = height*width;

	aafUInt32* rgbxbuf = new aafUInt32[ rgbxBufSize ];

	colorbars_test_pattern( rgbxbuf, width, 1, sizeof(aafUInt32)*width, 8*sizeof(aafUInt32) );

	copy_first_line_to_all_lines( rgbxbuf, height, 4*width );

	pBuf = reinterpret_cast<aafUInt8*>(rgbxbuf);
	bufSize = sizeof(aafUInt32) * rgbxBufSize;
	stride = width*4;
}

//=---------------------------------------------------------------------=

class BarsSource : public AxFGOp, public SampleSource { 

public:

  enum Format_e {
   RGBX4444,
   YUVX4444,
   YUV422,
   YUV411,
   YUV420
  };

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
	int _bitsPaddingPerComponent;
	int _bitsPerComponent;
	int _sampVert;
	int _sampHorz;
	Format_e _format;
	aafColorSpace_t _colorSpace;
	AxString _descName;
};


AXFG_OP_FACTORY_DECLARATION(
  BarsSource,           
  L"BarsSource",
  L"Implements the BarsSampleSource protocol",
  L"BarsSampleSourceName  CDCIDescriptorName color_space num_frames",
  L"CDCIDescriptor used to determine size.  color_space is aafColorSpace_t.",
  5,
  5 ) 

BarsSource::~BarsSource()
{}

void BarsSource::Execute( const std::vector<AxString>& argv )
{
	AxString sourceName   = argv[1];
	AxString cdciDescName = argv[2];
	AxString colorSpace   = argv[3];
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

	_colorSpace = ColorSpaceParams::GetInstance().Find( *this, colorSpace );

	if ( _sampHorz == 1 && _sampVert == 1 && _colorSpace == kAAFColorSpaceRGB ) {
		_format = RGBX4444;
	}
	else if ( _sampHorz == 1 && _sampVert == 1 && _colorSpace == kAAFColorSpaceYUV ) {
		_format = YUVX4444;
	}
	else if ( _sampHorz == 2 && _sampVert == 1 && _colorSpace == kAAFColorSpaceYUV ) {
		_format = YUV422;
	}
// Support pending
#if 0
	else if ( _sampHorz == 2 && _sampVert == 2 && _colorSpace == kAAFColorSpaceYUV ) {
		_format = YUV420;
	}
	else if ( _sampHorz == 4 && _sampVert == 1 && _colorSpace == kAAFColorSpaceYUV ) {
		_format = YUV411;
	}
#endif
	else {
		throw AxFGOpUsageEx( *this, L"unsupported format" );
	}

	_bitsPerComponent = axDesc.GetComponentWidth();
	_bitsPaddingPerComponent = axDesc.GetPaddingBits();

	// Currently only support 8 bits per component
	if ( _bitsPerComponent != 8 ) {
		throw AxFGOpUsageEx( *this, L"only 8 bits per component supported" );
	}

	if ( _bitsPaddingPerComponent != 0 ) {
		throw AxFGOpUsageEx( *this, L"non-zero padding not supported" );
	}


	// If mixed, or separate, fields then simply double the height.
	// This would not be correct for a more complex image.  In that case,
	// and if the format was YUV420, the color space conversion and resampling
	// would need to occur before interlacing or concantenating the image.
	if( axDesc.GetFrameLayout() == kAAFSeparateFields ||
		axDesc.GetFrameLayout() == kAAFMixedFields ) {
		_height *= 2;
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

	int rgbBufSize;
	int rgbBufStride;
	aafUInt8 *rgbBuf;

	// This function allocates an 8 bits per component image and
	// fills it with a color bar pattern.
	// The component width is enforced in the Execute method.
	create_rgbx_bars_image( _width, _height, rgbBufStride, rgbBufSize, rgbBuf );
	
	int convertedBufSize;
	aafUInt8* convertedBuf;
		
	// Next, perform color space conversion and resampling.
	switch ( _format ) {

		case RGBX4444:
			// Nothing to convert.
			convertedBufSize = rgbBufSize;
			convertedBuf = rgbBuf;
			break;

		case YUVX4444:
			convert_rgbx_to_yuvx4444( _width, _height, rgbBufStride, rgbBuf,
									  convertedBufSize, convertedBuf );
			break;

		case YUV422:
			convert_rgbx_to_yuv422( _width, _height, rgbBufStride, rgbBuf,
									convertedBufSize, convertedBuf );
			break;
#if 0
// Support pending
		case YUV411:
			convert_rgbx_to_yuv411( _width, _height, rgbBufStride, rgbBuf,
									convertedBufSize, convertedBuf );
			break;

		case YUV420:
			convert_rgbx_to_yuv420( _width, _height, rgbBufStride, rgbBuf, 
									convertedBufSize, convertedBuf );
			break;
#endif

		default:
			assert(0);
	};

	auto_ptr<aafUInt8> bufferToGiveUp( convertedBuf );
	
	auto_ptr<SampleSrcBuffer> srcBuffer(
		new SimpleSampleSrcBuffer( numSamples, convertedBufSize, bufferToGiveUp ) );
	
	_count++;

	return srcBuffer;
}


} // end of namespace

