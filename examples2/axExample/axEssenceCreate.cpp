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
			1,  /* FIXME - Can this be other than 1? */
			axPictureDef,
			codec,
			editRate,
			sampleRate,
			kAAFCompressionEnable,
			nullLocator,
			ContainerAAF ) );

	// CreateEssence caused a TimelineMobSlot, an EssenceData and a
	// SourceMob to be created. We want to get an EssenceDescriptor interface
	// so that we cat describe the data.  The EssenceDescriptor is accessed via
	// the SourceMob.  So... let's get the source mob.  It is hanging off
	// mob slot 1.
	AxMobSlotIter axMobIter( masterMob.GetSlots() );
	AxMobSlotIter::Pair nextSlot = axMobIter.NextOne();
	if ( !nextSlot.first ) {
		throw AxEx( L"Where's the beef?" );
	}
	if ( axMobIter.NextOne().first ) {
		throw AxEx( L"More slots than expected." );
	};

	AxMobSlot axMobSlot( nextSlot.second );

	// The AxMobSlot::GetSegment() will return a segment interface.  We know that
	// this is really a source clip, so "cast up" to source clip.
	AxSourceClip axSourceClip( AxQueryInterface<IAAFSegment,IAAFSourceClip>(
		axMobSlot.GetSegment(), IID_IAAFSourceClip ) );

	AxSourceMob sourceMob( AxQueryInterface<IAAFMob,IAAFSourceMob>(
		axContentStorage.LookupMob( axSourceClip.GetSourceID() ), IID_IAAFSourceMob ) );

	// Get the essence descriptor, and "cast up" to CDCIDescriptor.  How do I
	// know it should be a CDCIDescriptor... easy, I look at a dump of the file
	// before writing the next fragment of code, and noticed that the
	// EssenceDescription property of the SourceMob was an object of
	// type CDCIDescriptor.  Of course, we also used the kAAFCodecCDCI codec
	// but then, the JPEG coded uses a CDCI descriptor as well.

	AxCDCIDescriptor cdciDesc(
		AxQueryInterface<IAAFEssenceDescriptor,IAAFCDCIDescriptor>(
			sourceMob.GetEssenceDescriptor(), IID_IAAFCDCIDescriptor ) );

	// loads of parameters to set...
	aafRect_t rect = {0,0,720,576};
	aafVideoLineMap_t startScanLine = {0,1};
	aafFrameLayout_e  layout = kAAFMixedFields;
	aafUInt32 componentWidth = 8;
	aafUInt32 horizontalSubsampling = 2; // 2 means 4:2:2
	aafUInt32 verticalSubsampling = 1;
	aafUInt32 colorRange = 255;

	// DigitalImageDescriptor
	cdciDesc.SetStoredView( rect.ySize, rect.xSize );
	cdciDesc.SetSampledView( rect.ySize, rect.xSize, rect.xOffset, rect.yOffset );
	cdciDesc.SetDisplayView( rect.ySize, rect.xSize, rect.xOffset, rect.yOffset );
	cdciDesc.SetFrameLayout( layout );
	cdciDesc.SetVideoLineMap( 2, startScanLine );

	// CDCIDescriptor
	cdciDesc.SetComponentWidth( componentWidth );
	cdciDesc.SetHorizontalSubsampling( horizontalSubsampling );
	cdciDesc.SetVerticalSubsampling( verticalSubsampling );
	cdciDesc.SetColorRange( colorRange );

	// EssenceDescriptor
	// Nothing to do here.  We could add additional locators if necessary.

	// Now, set the same set of EssenceFormatSpecifiers

 	AxEssenceFormat axEssenceFormat( axEssenceAccess.GetEmptyFileFormat() );

	// Well known format sepecifers - See AAFEssenceFormat.h
	// Note, there are more format specifiers than this.
	
	axEssenceFormat.AddFormatSpecifier( kAAFStoredRect,   rect );
	axEssenceFormat.AddFormatSpecifier( kAAFSampledRect,  rect );
	axEssenceFormat.AddFormatSpecifier( kAAFDisplayRect,  rect );
	axEssenceFormat.AddFormatSpecifier( kAAFFrameLayout,  layout );
	axEssenceFormat.AddFormatSpecifier( kAAFVideoLineMap, startScanLine	);

	// These are the format specifiers used exclusively by the CDCI codec.
	// See AAFEssenceFormat.h.  Note, there is no VerticalSubsampling
	// format specifier.. why not?!?!

	axEssenceFormat.AddFormatSpecifier( kAAFCDCICompWidth, componentWidth );
	axEssenceFormat.AddFormatSpecifier( kAAFCDCIHorizSubsampling, horizontalSubsampling );
	axEssenceFormat.AddFormatSpecifier( kAAFCDCIColorRange, colorRange );

	// hmmm this is not part of the CDIC descriptor... do I really need it?
	aafColorSpace_t colorSpace = kAAFColorSpaceYUV;
	axEssenceFormat.AddFormatSpecifier( kAAFPixelFormat, colorSpace );

	// That's it for the format specifiers...
	axEssenceAccess.PutFileFormat( axEssenceFormat );

	const int numSamples = 1;  // frames
	const int numBytes = numSamples * rect.xSize * rect.ySize * 2;

	auto_ptr<aafUInt8> pixels( new aafUInt8 [numBytes] );

	// Write 2 frames.  Must write one sample at a time.

	AxEssenceAccess::WriteResult written = 
			axEssenceAccess.WriteSamples( numSamples,
						      numBytes,
						      reinterpret_cast<aafDataBuffer_t>(pixels.get()) );
		
    if ( written.samplesWritten != numSamples && written.bytesWritten == numBytes ) {
			throw AxEx( L"Image WriteSamples size mismatch" );
	}

	written = 
		axEssenceAccess.WriteSamples( numSamples,
					      numBytes,
					      reinterpret_cast<aafDataBuffer_t>(pixels.get()) );
	
	if ( written.samplesWritten != numSamples && written.bytesWritten == numBytes ) {
		throw AxEx( L"Image WriteSamples size mismatch" );
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

	// CreateEssence caused a TimelineMobSlot, an EssenceData and a
	// SourceMob to be created
	// Get The WAVDescriptor interface so that the WAV summary info
	// can be set.
		
	AxMobSlotIter axMobIter( masterMob.GetSlots() );
	AxMobSlotIter::Pair nextSlot = axMobIter.NextOne();
	if ( !nextSlot.first ) {
		throw AxEx( L"Where's the tofu?" );
	}
	if ( axMobIter.NextOne().first ) {
		throw AxEx( L"More slots than expected." );
	};

	AxMobSlot axMobSlot( nextSlot.second );

	// The AxMobSlot::GetSegment() will return a segment interface.  We know that
	// this is really a source clip, so "cast up" to source clip.
	AxSourceClip axSourceClip( AxQueryInterface<IAAFSegment,IAAFSourceClip>(
		axMobSlot.GetSegment(), IID_IAAFSourceClip ) );

	AxSourceMob sourceMob( AxQueryInterface<IAAFMob,IAAFSourceMob>(
		axContentStorage.LookupMob( axSourceClip.GetSourceID() ), IID_IAAFSourceMob ) );

	// Get the essence descriptor, and "cast up" to the WAVDescriptor.

	AxWAVEDescriptor wavDesc(
		AxQueryInterface<IAAFEssenceDescriptor,IAAFWAVEDescriptor>(
			sourceMob.GetEssenceDescriptor(), IID_IAAFWAVEDescriptor ) );

	// At this point, one must call wavDesc.SetSummary( size, pBuf ).  Only
	// problem is... the buffer contents are not defined anywhere, you must
	// use the header from a wave file, or build a fake one.  Yuck!
	//
	// So...revert to format specifiers.  The ExportAudioExample has a function
	// called loadWAVEHeader() that will read a wave file header and return
	// the sample rate, number of channels, and the bits per sample.  The
	// the reate is set when you create the essence (above), the bit per sample
	// is set via format specifier, but the number or channels is set no where.
	// So... what is one to do?  For the moment, I will simply set the sample
	// size using a format specifier.

	AxEssenceFormat axEssenceFormat( axEssenceAccess.GetEmptyFileFormat() );

	aafUInt32 sampleSize = 16;
	axEssenceFormat.AddFormatSpecifier( kAAFAudioSampleBits, sizeof( sampleSize ),
					    reinterpret_cast<aafUInt8*>(&sampleSize) );
	
	axEssenceAccess.PutFileFormat( axEssenceFormat );

	// FIXME FIXME - Data generation and write will be redone.
	// Please don't be aghast.

	const int numSamples = 2 * rateHz / 25;  // 2 pal frames in duration.
	aafUInt16 samples[numSamples];
	
	AxEssenceAccess::WriteResult written = 
		axEssenceAccess.WriteSamples( numSamples,
					      sizeof(samples[0])*numSamples,
					      reinterpret_cast<aafDataBuffer_t>(samples) );

	if ( written.samplesWritten != numSamples ) {
		throw AxEx( L"Audio WriteSamples size mismatch" );
	}

	axEssenceAccess.CompleteWrite();
}

} // end of namespace

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
	AxMobIter::Pair next;

	// AxMasterMob has no copy ctor or operator=, hence use a pointer
	typedef map< AxString, auto_ptr<AxMasterMob> > MobMap;
	MobMap mobMap;

	for( next = axMobIter.NextOne();
		 next.first;
		 next = axMobIter.NextOne() ) { 

			 auto_ptr<AxMasterMob> axMasterMob(
				 new AxMasterMob( AxQueryInterface<IAAFMob,IAAFMasterMob>(
					next.second, IID_IAAFMasterMob ) ) );

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
