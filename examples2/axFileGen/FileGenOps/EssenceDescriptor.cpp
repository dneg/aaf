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
#include "Rate.h"
#include "WaveHeader.h"

#include <axFileGen.h>

#include <AxMob.h>
#include <AxEssence.h>
#include <AxDictionary.h>

#include <assert.h>

namespace {

//=---------------------------------------------------------------------=

AXFG_OP(
  GetEssenceDescriptor,           
  L"GetEssenceDescriptor",
  L"Get and essence descriptor (any type) from a source mob.",
  L"DescriptorName SourceMobName",
  L"",
  3,
  3 ) 

GetEssenceDescriptor::~GetEssenceDescriptor()
{}

void GetEssenceDescriptor::Execute( const std::vector<AxString>& argv )
{
	AxString descName = argv[1];
	AxString mobName  = argv[2];

	IAAFSourceMobSP spMob;
	GetInstance( mobName ).GetCOM( spMob );
	AxSourceMob axMob( spMob );

	SetCOM( axMob.GetEssenceDescriptor() );
	RegisterInstance( descName );
}

//=---------------------------------------------------------------------=

// FIXME - 13 arguments are difficult to manage.

AXFG_OP(
  CDCIDescSet,           
  L"CDCIDescSet",
  L"Set minimal CDCI essence descriptor parameters.",
  L"DescriptorName image_width image_height horz_subsampl vert_subsamp codec frame_layout "
    L"black_ref white_ref color_range [line_map_one] [line_map_two]",
  L"",
  11,
  13 ) 

CDCIDescSet::~CDCIDescSet()
{}

void CDCIDescSet::Execute( const std::vector<AxString>& argv )
{
	AxString  descName       = argv[1];
	AxString  width          = argv[2];
	AxString  height         = argv[3];
	AxString  sampHorz       = argv[4];
	AxString  sampVert       = argv[5];
	AxString codecName       = argv[6];
	AxString frameLayout     = argv[7];
 	AxString black_ref_level = argv[8];
  	AxString white_ref_level = argv[9];
	AxString color_range     = argv[10];
	
	std::pair<bool,AxString> line_map_one( false, L"" );
	if ( argv.size() > 11 ) {
		line_map_one.first = true;
		line_map_one.second = argv[11];
	}

	std::pair<bool,AxString> line_map_two( false, L"" );
	if ( argv.size() > 12 ) {
		line_map_one.first = true;
		line_map_one.second = argv[11];
	}
	
	IAAFCDCIDescriptorSP spCDCIDesc;
	GetInstance( descName ).GetCOM( spCDCIDesc );
	AxCDCIDescriptor axCDCIDesc( spCDCIDesc );

	aafRect_t rect;
	rect.xOffset = 0;
	rect.yOffset = 0;
	rect.xSize   = AxStringUtil::strtol( width );
	rect.ySize   = AxStringUtil::strtol( height );
	
	axCDCIDesc.SetStoredView ( rect.ySize, rect.xSize );
	axCDCIDesc.SetSampledView( rect.ySize, rect.xSize, rect.xOffset, rect.yOffset );
	axCDCIDesc.SetDisplayView( rect.ySize, rect.xSize, rect.xOffset, rect.yOffset );

	axCDCIDesc.SetHorizontalSubsampling( AxStringUtil::strtol( sampHorz ) );
	axCDCIDesc.SetVerticalSubsampling( AxStringUtil::strtol( sampVert ) );

	axCDCIDesc.SetCompression(
		CodecIDParams::GetInstance().Find( *this, codecName ) );

	axCDCIDesc.SetFrameLayout(
		FrameLayoutParams::GetInstance().Find( *this, frameLayout ) );

	axCDCIDesc.SetBlackReferenceLevel( AxStringUtil::strtol( black_ref_level ) );
	axCDCIDesc.SetWhiteReferenceLevel( AxStringUtil::strtol( white_ref_level ) );
	axCDCIDesc.SetColorRange( AxStringUtil::strtol( color_range ) );

	aafInt32 lineMap[2];
	int linemapSize = 0;

	if ( line_map_one.first ) {
		lineMap[0] = AxStringUtil::strtol( line_map_one.second );
		linemapSize++;
	}

	if ( line_map_two.first ) {
		assert( line_map_one.first );
		lineMap[1] = AxStringUtil::strtol( line_map_two.second );
		linemapSize++;
	}

	axCDCIDesc.SetVideoLineMap( 2, lineMap );
}

//=---------------------------------------------------------------------=


AXFG_OP(
  WAVEDescSet,           
  L"WAVEDescSet",
  L"Set WAVE parameters.",
  L"DescriptorName SampleRateName num_samples bytes_per_sample bits_per_sample num_channels",
  L"SampleRateName is a reference to a Rate object.  The denominator must be one.",
  7,
  7 ) 

WAVEDescSet::~WAVEDescSet()
{
}

void WAVEDescSet::Execute( const std::vector<AxString>& argv )
{
	AxString descName        = argv[1];
	AxString rateName        = argv[2];
	AxString numSamples      = argv[3];
	AxString bytesPerSample  = argv[4];
	AxString bitsPerSample   = argv[5];
	AxString numChannels     = argv[6];


	IAAFWAVEDescriptorSP spWAVEDesc;
	GetInstance( descName ).GetCOM( spWAVEDesc );
	AxWAVEDescriptor axWaveDesc( spWAVEDesc );

	aafRational_t rate = RateOpToRational( GetInstance(rateName) );

	if ( 1 != rate.denominator ) {
		throw AxFGOpUsageEx( *this, L"Rate denominator must be one." );
	}

	WaveHeader waveHeader( rate.numerator,
						   AxStringUtil::strtol( numSamples ),
						   AxStringUtil::strtol( bytesPerSample ),
						   AxStringUtil::strtol( bitsPerSample ),
						   AxStringUtil::strtol( numChannels ) );

	// Size and buffer pointer passed via an AxBuffer.
	AxBuffer<aafUInt8> header = waveHeader.GetHeader();

	axWaveDesc.SetSummary( header.GetSize(), header.GetPtr().get() );

}


//=---------------------------------------------------------------------=

AXFG_OP(
  WAVEDesc,
  L"WAVEDesc",
  L"Create buggy WAVE descriptor.",
  L"FileName DescriptorName",
  L"This is intended to test a bug.",
  3,
  3 ) 

WAVEDesc::~WAVEDesc()
{
}

void WAVEDesc::Execute( const std::vector<AxString>& argv )
{
	AxString fileName = argv[1];
	AxString descName = argv[2];

	IAAFWAVEDescriptorSP spDesc;
	AxCreateInstance( DictionaryFromFileOp( fileName ), spDesc );

	SetCOM( spDesc );
	RegisterInstance( descName );
}

//=---------------------------------------------------------------------=

AXFG_OP(
  AIFCDesc,           
  L"AIFCDesc",
  L"Create buggy AIFC descriptor.",
  L"FileName DescriptorName",
  L"This is intended to test a bug.",
  3,
  3 ) 

AIFCDesc::~AIFCDesc()
{
}

void AIFCDesc::Execute( const std::vector<AxString>& argv )
{
	AxString fileName = argv[1];
	AxString descName = argv[2];

	IAAFAIFCDescriptorSP spDesc;
	AxCreateInstance( DictionaryFromFileOp( fileName ), spDesc );

	SetCOM( spDesc );
	RegisterInstance( descName );
}

} // end of namespace


