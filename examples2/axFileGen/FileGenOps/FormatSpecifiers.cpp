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

#include "ParamMaps.h"
#include "WaveHeader.h"

#include <axFileGen.h>

#include <AxEssence.h>

#include <AAFEssenceFormats.h>

namespace {

AXFG_OP(
  CDCIFormatSpecifiersSet,          
  L"CDCIFormatSpecifiersSet",
  L"Set CDCI codec format specifiers.",
  L"CDCIDescriptorNAme EssenceAccessName colorspace",
  L"",
  4,
  4 ) 

CDCIFormatSpecifiersSet::~CDCIFormatSpecifiersSet()
{}

void CDCIFormatSpecifiersSet::Execute( const std::vector<AxString>& argv )
{
	AxString descName    = argv[1];
	AxString accessName  = argv[2];
	AxString colorSpace = argv[3];

	IAAFCDCIDescriptorSP spDesc;
	GetInstance( descName ).GetCOM( spDesc );
	AxCDCIDescriptor axDesc( spDesc );

	IAAFEssenceAccessSP spAccess;
	GetInstance( accessName ).GetCOM( spAccess );
	AxEssenceAccess axAccess( spAccess );

	AxEssenceFormat axEssenceFormat( axAccess.GetEmptyFileFormat() );
		
	aafUInt32 tmpWidth;
	aafUInt32 tmpHeight;

	aafRect_t storedRect = { 0,0,0,0 };
	axDesc.GetStoredView( tmpHeight, tmpWidth );
	storedRect.ySize = tmpHeight;
	storedRect.xSize = tmpWidth;
	axEssenceFormat.AddFormatSpecifier( kAAFStoredRect, storedRect );

	aafRect_t sampledRect = { 0,0,0,0 };
	axDesc.GetSampledView( tmpHeight, tmpWidth, sampledRect.xOffset, sampledRect.yOffset );
	sampledRect.ySize = tmpHeight;
	sampledRect.xSize = tmpWidth;
	axEssenceFormat.AddFormatSpecifier( kAAFSampledRect, sampledRect );

	aafRect_t displayRect = { 0,0,0,0 };
	axDesc.GetSampledView( tmpHeight, tmpWidth, displayRect.xOffset, displayRect.yOffset );
	displayRect.ySize = tmpHeight;
	displayRect.xSize = tmpWidth;
	axEssenceFormat.AddFormatSpecifier( kAAFDisplayRect, displayRect );


	
	axEssenceFormat.AddFormatSpecifier(
		kAAFCDCIHorizSubsampling,
		axDesc.GetHorizontalSubsampling() );

	// Note, a vertical subsampling property also exists in the CDCIDescriptor,
	// howevever, there is not an equivalent format specifier.


	axEssenceFormat.AddFormatSpecifier( kAAFFrameLayout,
										axDesc.GetFrameLayout() );

	axEssenceFormat.AddFormatSpecifier( kAAFCompression,
										axDesc.GetCompression() );

	// Why is there no pixel format parameter on the descriptor?
	// FIXME - The SDK calls this specifier "PixelFormat", but the CDCI
	// codec checks to see if equals kAAFColorSpaceRGB or kAAFColorSpaceYUV.
	axEssenceFormat.AddFormatSpecifier( kAAFPixelFormat,
		ColorSpaceParams::GetInstance().Find( *this, colorSpace ) );

	aafInt32 lineMap[2];
	axDesc.GetVideoLineMap( 2, lineMap );
	axEssenceFormat.AddFormatSpecifier( kAAFVideoLineMap, lineMap );

	axEssenceFormat.AddFormatSpecifier( kAAFCDCIBlackLevel,
										axDesc.GetBlackReferenceLevel() );
	axEssenceFormat.AddFormatSpecifier( kAAFCDCIWhiteLevel,
										axDesc.GetWhiteReferenceLevel() );
	axEssenceFormat.AddFormatSpecifier( kAAFCDCIColorRange,
										axDesc.GetColorRange() );

	axAccess.PutFileFormat( axEssenceFormat );
}

//=---------------------------------------------------------------------=


AXFG_OP(
  JPEGFormatSpecifiersSet,          
  L"JPEGFormatSpecifiersSet",
  L"Set JPEG codec format specifiers.",
  L"CDCIEssenceDescriptorName  EssenceAccessName pixel_format",
  L"Values set: StoredRect, SampledRect, DisplayRect, CDCIHorizSubsampling, FrameLayout, VideoLineMap, Compression, PixelFormat",
  4,
  4 ) 

JPEGFormatSpecifiersSet::~JPEGFormatSpecifiersSet()
{}

void JPEGFormatSpecifiersSet::Execute( const std::vector<AxString>& argv )
{
	AxString descName     = argv[1];
	AxString accessName   = argv[2];
	AxString pixelFormat  = argv[3];

	IAAFCDCIDescriptorSP spDesc;
	GetInstance( descName ).GetCOM( spDesc );
	AxCDCIDescriptor axDesc( spDesc );

	IAAFEssenceAccessSP spAccess;
	GetInstance( accessName ).GetCOM( spAccess );
	AxEssenceAccess axAccess( spAccess );

	AxEssenceFormat axEssenceFormat( axAccess.GetEmptyFileFormat() );
	
	aafRect_t rect = { 0,0,0,0 };
	aafUInt32 w;
	aafUInt32 h;
	aafInt32 xoff;
	aafInt32 yoff;

	axDesc.GetStoredView( w, h );
	rect.xSize = w;
	rect.ySize = h;
	axEssenceFormat.AddFormatSpecifier( kAAFStoredRect, rect );
		
	axDesc.GetSampledView( w, h, xoff, yoff );
	rect.xSize = w;
	rect.ySize = h;
	rect.xOffset = xoff;
	rect.yOffset = yoff;
	axEssenceFormat.AddFormatSpecifier( kAAFSampledRect, rect );
	
	axDesc.GetDisplayView( w, h, xoff, yoff );
	rect.xSize = w;
	rect.ySize = h;
	rect.xOffset = xoff;
	rect.yOffset = yoff;
	axEssenceFormat.AddFormatSpecifier( kAAFDisplayRect, rect );

	axEssenceFormat.AddFormatSpecifier( kAAFCDCIHorizSubsampling,
										axDesc.GetHorizontalSubsampling() );


	axEssenceFormat.AddFormatSpecifier( kAAFFrameLayout,
										axDesc.GetFrameLayout() );

	axEssenceFormat.AddFormatSpecifier( kAAFCompression,
										axDesc.GetCompression() );

	
	// FIXME - This is not expressed in the format descriptor - why?
	axEssenceFormat.AddFormatSpecifier( kAAFPixelFormat,
		PixelFormatParams::GetInstance().Find( *this, pixelFormat ) );

	// FIXME - If the SetVideoLineMap call only set one value,
	// how do we know that... and does it matter?  Must we infer the
	// line count from the frame layout?
	aafInt32 lineMap[2];
	axDesc.GetVideoLineMap( 2, lineMap );
	axEssenceFormat.AddFormatSpecifier( kAAFVideoLineMap, lineMap );


	axAccess.PutFileFormat( axEssenceFormat );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  WAVEFormatSpecifiers,          
  L"WAVEFormatSpecifiers",
  L"Set WAVE codec format specifiers.",
  L"WaveEssenceDescriptorName  EssenceAccessName",
  L"",
  3,
  3 ) 

WAVEFormatSpecifiers::~WAVEFormatSpecifiers()
{}

void WAVEFormatSpecifiers::Execute( const std::vector<AxString>& argv )
{
	AxString descName   = argv[1];
	AxString accessName = argv[2];

	IAAFWAVEDescriptorSP spDesc;
	GetInstance( descName ).GetCOM( spDesc );
	AxWAVEDescriptor axDesc( spDesc );

	IAAFEssenceAccessSP spAccess;
	GetInstance( accessName ).GetCOM( spAccess );
	AxEssenceAccess axAccess( spAccess );

	AxBuffer<aafUInt8> waveHeaderBuffer = axDesc.GetSummary();

	WaveHeader waveHeader( waveHeaderBuffer );

	AxEssenceFormat axEssenceFormat( axAccess.GetEmptyFileFormat() );

	aafUInt32 sampleSize = waveHeader.GetBitsPerSample();
	axEssenceFormat.AddFormatSpecifier( kAAFAudioSampleBits, sampleSize );
}

} // end of namespace


