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
//
//=---------------------------------------------------------------------

#include <AxFile.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxContentStorage.h>
#include <AxMetaDef.h>
#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxComponent.h>
#include <AxEssence.h>
#include <AAFEssenceFormats.h>
#include <AxEx.h>
#include <AxUtil.h>

#include <AAFStoredObjectIDs.h>
#include <AAFTypeDefUIDs.h>
#include <AAFDataDefs.h>
#include <AAFCodecDefs.h>
#include <AAFContainerDefs.h>

#include <map>

#include <iostream>

#define USE_MINIMAL_IMAGE_FORMAT_SPECIFIERS 1

namespace {


void AddImageEssence( AxMasterMob& masterMob,
		      AxHeader& axHeader,
		      AxCmdLineArgs& args  )
{
	AxContentStorage axContentStorage( axHeader.GetContentStorage() );
	AxDictionary axDictionary( axHeader.GetDictionary() );

	AxDataDef axPictureDef( axDictionary.LookupDataDef( DDEF_Picture ) );
	
	// FIXME - This should be moved to a centralized location rather than having
	// options strings tested all over the place.
	pair<bool,int> mpegCodecOpt = args.get( "-mpeg2" );
	aafUID_t codec;

#if 0
	// Activate this code if you would like to test the BBC Mpeg codec.
	if ( mpegCodecOpt.first ) {
		codec = kAAFCodecMPEG2;
	}
	else {
#endif
		codec = kAAFCodecJPEG;
#if 0
	}
#endif

	aafRational_t editRate = {25, 1};
	aafRational_t sampleRate = {25, 1};
		
	IAAFEssenceAccessSP spIaafEssenceAccess;

	// We will use ContainerAAF, hence no locator is required.
	IAAFLocatorSP nullLocator; 

	 AxEssenceAccess axEssenceAccess ( 
		masterMob.CreateEssence(
			1,
			axPictureDef,
			codec,
			editRate,
			sampleRate,
			kAAFCompressionEnable,
			nullLocator,
			ContainerAAF ) );

	// CreateEssence caused a TimelineMobSlot, SourceClip, SourceMob, and
	// EssenceData to be created.  This is the scaffolding upon which we add
	// essence data.
	//
	// Before writing essence, we need set the codec's format specifiers.
	// These format specifiers are not persistent, the are made persistent
	// via an EssenceDescriptor object.
	//
	// The EssenceDescriptor is accessed via the SourceMob.  So... let's get
	// the source mob.  The id of the SourceMob is found in the SourceClip that
	// is hanging off of slot 1.

	AxMobSlotIter axMobIter( masterMob.GetSlots() );
	IAAFMobSlotSP nextSlot;
	bool notAtEnd = axMobIter.NextOne( nextSlot );

	if ( !notAtEnd ) {
		throw AxEx( L"Expected at least on MobSlot." );
	}

	IAAFMobSlotSP unused;
	if ( axMobIter.NextOne( unused ) ) {
		throw AxEx( L"More MobSlots are present than expected." );
	};

	AxMobSlot axMobSlot( nextSlot );

	// The AxMobSlot::GetSegment() method will return a segment interface.
	// We know tha this is really a source clip, so cast to source clip.
	AxSourceClip axSourceClip( AxQueryInterface<IAAFSegment,IAAFSourceClip>(
					   			axMobSlot.GetSegment() ) );

	AxSourceMob sourceMob( AxQueryInterface<IAAFMob,IAAFSourceMob>(
		axContentStorage.LookupMob( axSourceClip.GetSourceID() ) ) );

	// Get the essence descriptor, and cast to CDCIDescriptor.  How does one
	// know it should be a CDCIDescriptor?  The EssenceDescriptor type is 
	// determined by the codec type that is passed to IAAFMasterMob::EssenceCreate().
	// You must know, based on experience, what type of EssenceDescriptor(s) to expect.
	// A good way to gain that experience is by studying aaf files using a dump program.

	// In the case of the JPEG, and MPEG2 codec, we expect a CDCI descriptor.

	AxCDCIDescriptor cdciDesc(
		AxQueryInterface<IAAFEssenceDescriptor,IAAFCDCIDescriptor>(
			sourceMob.GetEssenceDescriptor() ) );

	// loads of parameters to set...
	aafRect_t rect = {0,0,720,576};
	aafVideoLineMap_t startScanLine = {0,1};
	aafFrameLayout_e  layout = kAAFMixedFields;
	aafUInt32 componentWidth = 8;
	aafUInt32 horizontalSubsampling = 2; // 2 means 4:2:2
	aafUInt32 verticalSubsampling = 1;
	aafUInt32 colorRange = 255;

	// CDCIDescriptor also implements DigitalImageDescriptor.  Here are some
	// parameters that DigitalImageDescriptor inherits from CDCI descriptor.
	cdciDesc.SetStoredView( rect.ySize, rect.xSize );
	cdciDesc.SetSampledView( rect.ySize, rect.xSize, rect.xOffset, rect.yOffset );
	cdciDesc.SetDisplayView( rect.ySize, rect.xSize, rect.xOffset, rect.yOffset );
	cdciDesc.SetFrameLayout( layout );
	cdciDesc.SetVideoLineMap( 2, startScanLine );

	// CDCIDescriptor parameters.
	cdciDesc.SetComponentWidth( componentWidth );
	cdciDesc.SetHorizontalSubsampling( horizontalSubsampling );
	cdciDesc.SetVerticalSubsampling( verticalSubsampling );
	cdciDesc.SetColorRange( colorRange );

	// EssenceDescriptor parameters.
	// Nothing to do here.  We could add additional locators if necessary.

	// Now, set the same set of essence format specifiers.  We are "talking"
	// to the codec at this point.

 	AxEssenceFormat axEssenceFormat( axEssenceAccess.GetEmptyFileFormat() );

#if USE_MINIMAL_IMAGE_FORMAT_SPECIFIERS
	// Minimal "known good" set of format specifiers.
	axEssenceFormat.AddFormatSpecifier( kAAFStoredRect,   rect );
	axEssenceFormat.AddFormatSpecifier( kAAFCDCIHorizSubsampling, horizontalSubsampling );
	aafColorSpace_t colorSpace = kAAFColorSpaceYUV;
	axEssenceFormat.AddFormatSpecifier( kAAFPixelFormat, colorSpace );
#else
	// Format specifer set that matches the essence descriptor
	// parameters set above.
	// This currently fails on Irix.  IAAFEssenceFormat::AddFormatSpecifer()
	// throws:  HRESULT AxEssence.cpp:51 0x80120069 AAFRESULT_INVALID_PARM_SIZE
	axEssenceFormat.AddFormatSpecifier( kAAFStoredRect,   rect );
	axEssenceFormat.AddFormatSpecifier( kAAFSampledRect,  rect );
	axEssenceFormat.AddFormatSpecifier( kAAFDisplayRect,  rect );
	axEssenceFormat.AddFormatSpecifier( kAAFFrameLayout,  layout );
	axEssenceFormat.AddFormatSpecifier( kAAFVideoLineMap, startScanLine	);

	// These are the format specifiers used by the CDCI, JPEG, and MPEG2 codecs.
	// See AAFEssenceFormat.h.  Note, there is no VerticalSubsampling
	// format specifier.. why not?
	axEssenceFormat.AddFormatSpecifier( kAAFCDCICompWidth, componentWidth );
	axEssenceFormat.AddFormatSpecifier( kAAFCDCIHorizSubsampling, horizontalSubsampling );
	axEssenceFormat.AddFormatSpecifier( kAAFCDCIColorRange, colorRange );

	// This format specifier is not part of the CDIC descriptor, in a real application
	// it would probably have to be deduced from the EssenceDescriptor parameters.
	aafColorSpace_t colorSpace = kAAFColorSpaceYUV;
	axEssenceFormat.AddFormatSpecifier( kAAFPixelFormat, colorSpace );
#endif

	// That's it for the format specifiers...
	axEssenceAccess.PutFileFormat( axEssenceFormat );

	// With the essence descriptor, and format specifier, parameters set, we can
	// now write some data.
	
	const int numSamples = 1;  // frames
	const int numBytes = numSamples * rect.xSize * rect.ySize * 2;

	auto_ptr<aafUInt8> pixels( new aafUInt8 [numBytes] );

	// Load pixels buffer with meaning full data here.

	int samplesStillToWrite = 2;

	while ( samplesStillToWrite > 0 ) {

		AxEssenceAccess::WriteResult written = 
			axEssenceAccess.WriteSamples( numSamples,
									      numBytes,
									      reinterpret_cast<aafDataBuffer_t>(pixels.get()) );

	    if ( written.samplesWritten != numSamples && written.bytesWritten == numBytes ) {
				throw AxEx( L"Image WriteSamples size mismatch" );
		}

		samplesStillToWrite -= written.samplesWritten;
	}

    axEssenceAccess.CompleteWrite();
}


void AddAudioEssence( AxMasterMob& masterMob, AxHeader& axHeader )
{
	AxContentStorage axContentStorage( axHeader.GetContentStorage() );
	AxDictionary axDictionary( axHeader.GetDictionary() );

	AxDataDef axSoundDef( axDictionary.LookupDataDef( DDEF_Sound ) );
	
	const int rateHz = 44100;
	aafRational_t editRate = {rateHz, 1};
	aafRational_t sampleRate = {rateHz, 1};
	
	IAAFEssenceAccessSP spIaafEssenceAccess;
	IAAFLocatorSP null;

	AxEssenceAccess axEssenceAccess ( 
		masterMob.CreateEssence(
			1,  /* FIXME - Can this be other than 1? */
			axSoundDef,
			kAAFCodecWAVE,
			editRate,
			sampleRate,
			kAAFCompressionDisable,
			null,
			ContainerAAF ) );

	// CreateEssence caused a TimelineMobSlot, SourceClip, SourceMob, and
	// EssenceData to be created.  Get The WAVEDescriptor interface so that
	// the WAV summary info can be set.
		
	AxMobSlotIter axMobIter( masterMob.GetSlots() );
	IAAFMobSlotSP nextSlot;
	bool notAtEnd = axMobIter.NextOne( nextSlot );

	if ( !notAtEnd ) {
		throw AxEx( L"Expected at least one MobSlot." );
	}

	IAAFMobSlotSP unused;
	if ( axMobIter.NextOne( unused ) ) {
		throw AxEx( L"More mob slots are present than expected." );
	};

	AxMobSlot axMobSlot( nextSlot );

	// The AxMobSlot::GetSegment() method will return a segment interface.  We know
	// that this is really a source clip, so cast to source clip.
	AxSourceClip axSourceClip( AxQueryInterface<IAAFSegment,IAAFSourceClip>(
		axMobSlot.GetSegment() ) );

	AxSourceMob sourceMob( AxQueryInterface<IAAFMob,IAAFSourceMob>(
		axContentStorage.LookupMob( axSourceClip.GetSourceID() ) ) );

	// Get the essence descriptor, and cast to the WAVEDescriptor.

	AxWAVEDescriptor wavDesc(
		AxQueryInterface<IAAFEssenceDescriptor,IAAFWAVEDescriptor>(
			sourceMob.GetEssenceDescriptor() ) );

	// At this point, one must call wavDesc.SetSummary( size, pBuf ). A file
	// dump indicates this is a UInt8[36].  This is the header information
	// from the wave audio file.  It contains information such as sample
	// rate, sample size, etc.
	// FIXME - Futher documentation on this subject is required.


	// Next, set the format specifiers.  The parameters that must be set
	// are codec dependent.  The values are obtained from the WaveDescriptor
	// summary information.  The following code only sets kAAFAudioSampleBits.
	// Experience has shown that this is the only parameter that *must* be
	// set.

	AxEssenceFormat axEssenceFormat( axEssenceAccess.GetEmptyFileFormat() );

	aafUInt32 sampleSize = 16;
	axEssenceFormat.AddFormatSpecifier( kAAFAudioSampleBits, sizeof( sampleSize ),
					    reinterpret_cast<aafUInt8*>(&sampleSize) );
	
	axEssenceAccess.PutFileFormat( axEssenceFormat );

	// FIXME FIXME - Data generation and write will be redone.
	// Please don't be aghast.

	const int numSamples = 2 * rateHz / 25;  // 2 pal frames in duration.
	aafUInt16 samples[numSamples];

	// At this point one should write meaningful data into the
	// sample buffer.
	
	int samplesStillToWrite = numSamples;

	while ( samplesStillToWrite > 0 ) {
	
		int samplesToWriteThisTime = 100;

		if ( samplesToWriteThisTime > samplesStillToWrite ) {
			samplesToWriteThisTime = samplesStillToWrite;
		}
		
		AxEssenceAccess::WriteResult written = 
			axEssenceAccess.WriteSamples( samplesToWriteThisTime,
						      sizeof(samples[0])*samplesToWriteThisTime,
						      reinterpret_cast<aafDataBuffer_t>(&samples[numSamples-samplesStillToWrite]) );

		if ( written.samplesWritten != samplesToWriteThisTime ) {
			throw AxEx( L"Audio WriteSamples size mismatch" );
		}

		samplesStillToWrite -= written.samplesWritten;
	}

	axEssenceAccess.CompleteWrite();
}

} // end of namespace



// AxCreateEssenceExample - Find the masters mobs we expect.  They are located using
// their Mob names.  Note, there is no requirement that mobs have unique names, hence,
// this approach would be not be adequate in a real application.  A better approach
// would be use to use Mob ID's - which are unique.
// Add essence to each of these mobs.

void AxCreateEssenceExample( AxFile& axFile,
							 AxCmdLineArgs& args )
{
	AxHeader axHeader( axFile.getHeader() );
	AxDictionary axDictionary( axHeader.GetDictionary() );
	AxContentStorage axContentStorage( axHeader.GetContentStorage() );

	aafSearchCrit_t	criteria;
	criteria.searchTag = kAAFByMobKind;
	criteria.tags.mobKind = kAAFMasterMob;
	AxMobIter axMobIter( axContentStorage.GetMobs( &criteria ) );
	IAAFMobSP nextMob;
	bool notAtEnd;

	// Use pointer because AxMasterMob has no copy ctor or assignment
	// operator.
	typedef map< AxString, AxAutoPtr<AxMasterMob> > MobMap;
	MobMap mobMap;

	for( notAtEnd = axMobIter.NextOne( nextMob );
	     notAtEnd;
	     notAtEnd = axMobIter.NextOne( nextMob ) ) { 

			 AxAutoPtr<AxMasterMob> axMasterMob(
					new AxMasterMob( AxQueryInterface<IAAFMob,IAAFMasterMob>( nextMob ) ) );

			 mobMap[ axMasterMob->GetName() ] = axMasterMob;
		 }

	AxString audioA( L"Audio Mob A" );
	AxString audioB( L"Audio Mob B" );
	AxString videoA( L"Video Mob A" );
	AxString videoB( L"Video Mob B" );

	// Verify our mobs exist...
	if ( mobMap.find( audioA ) == mobMap.end() || mobMap.find( audioB ) == mobMap.end() ||
		 mobMap.find( videoA ) == mobMap.end() || mobMap.find( videoB ) == mobMap.end() ) {
		throw AxEx( L"mob not found" );
	}

	// TODO - Create instances of data sources that will generate
	// essence data.

	AddAudioEssence( *mobMap[ audioA ], axHeader );
	mobMap[ audioA ]->AppendComment( L"Essence Comment", L"Audio essence attached." );

	AddAudioEssence( *mobMap[ audioB ], axHeader );
	mobMap[ audioB ]->AppendComment( L"Essence Comment", L"Audio essence attached." );

	AddImageEssence( *mobMap[ videoA ], axHeader, args );
	mobMap[ videoA ]->AppendComment( L"Essence Comment", L"Image essence attached." );

	AddImageEssence( *mobMap[ videoB ], axHeader, args );
	mobMap[ videoB ]->AppendComment( L"Essence Comment", L"Image essence attached." );
}
