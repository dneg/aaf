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

	std::pair<int, std::auto_ptr<aafUInt8> > waveHeaderBuffer = axDesc.GetSummary();

	WaveHeader waveHeader( waveHeaderBuffer );

	AxEssenceFormat axEssenceFormat( axAccess.GetEmptyFileFormat() );

	aafUInt32 sampleSize = waveHeader.GetBitsPerSample();
	axEssenceFormat.AddFormatSpecifier( kAAFAudioSampleBits, sampleSize );
}

} // end of namespace


