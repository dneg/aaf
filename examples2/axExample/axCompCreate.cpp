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
#include <AxComponent.h>
#include <AxMob.h>
#include <AxMobSlot.h>
#include <AxEssence.h>
#include <AxEx.h>
#include <AxUtil.h>

#include <AAFStoredObjectIDs.h>
#include <AAFDataDefs.h>
#include <AAFOperationDefs.h>

#include <map>

#include <iostream>

namespace {

IAAFSourceClipSP
CreateSourceClipToAppendToSequence( AxDictionary& axDictionary,
				    AxMasterMob& sourceMob,
				    AxSequence& sequenceToAppendTo )
{
	// Search for the first slot in the source mob that matches the
	// data def of the sequenceToAppendTo.

	AxDataDef sequenceDataDef( sequenceToAppendTo.GetDataDef() );
	aafUID_t sequenceDataDefAUID = sequenceDataDef.GetAUID();

	AxMobSlotIter axSourceMobIter( sourceMob.GetSlots() );

	IAAFMobSlotSP nextSlot;
	bool notAtEnd;

	// If AxTimelineMobSlot had a default ctor and assigment operator
	// we could use an AxTimelineMobSlot here, instead of an IAAF...
	IAAFTimelineMobSlotSP spSourceSlot;

	for( notAtEnd = axSourceMobIter.NextOne( nextSlot );
	     notAtEnd;
	     notAtEnd = axSourceMobIter.NextOne( nextSlot ) ) {
		
		// First of all, the slot should be a timeline slot.
		// This will throw an exception due to a bad interface
		// query if it is not.

		AxTimelineMobSlot axTimelineMobSlot(
			AxQueryInterface<IAAFMobSlot,IAAFTimelineMobSlot>(
				nextSlot, IID_IAAFTimelineMobSlot ) );
		
		// Does it match the sequence's data def?

		AxDataDef axDataDef( axTimelineMobSlot.GetDataDef() );
		if ( sequenceDataDefAUID == axDataDef.GetAUID() ) {
			spSourceSlot = static_cast<IAAFTimelineMobSlotSP>( axTimelineMobSlot );
			break;
		}
	}

	if ( !notAtEnd ) {
		throw AxEx( L"Failed to find matching data definition." );
	}

	AxTimelineMobSlot sourceSlot( spSourceSlot );

	//
	// Create a source clip refering to the slot that we just
	// found, and append it to the sequence.
	//

	// FIXME - Is the sourceRef origin *always* just the slot's origin?

	aafSourceRef_t sourceRef;
	sourceRef.sourceID = sourceMob.GetMobID();
	sourceRef.sourceSlotID =  sourceSlot.GetSlotID();
	sourceRef.startTime = sourceSlot.GetOrigin();

	AxSegment axSegment( sourceSlot.GetSegment() );

	AxSourceClip axSourceClip(
		AxCreateInstance<IAAFSourceClip>(
			axDictionary, AUID_AAFSourceClip, IID_IAAFSourceClip ) );

	axSourceClip.Initialize( sequenceDataDef, axSegment.GetLength(), sourceRef );
	
	return axSourceClip;
}

} // end of namespace

void AxCreateCompositionExample( AxFile& axFile,
							     AxCmdLineArgs& args )
{
	// FIXME - This function is too long.
	
	
	// As usual, the header and dictionary are required.
	AxHeader axHeader( axFile.getHeader() );
	AxDictionary axDictionary( axHeader.GetDictionary() );
		
	AxContentStorage axContentStorage( axHeader.GetContentStorage() );
	aafSearchCrit_t	criteria;
	criteria.searchTag = kAAFByMobKind;
	criteria.tags.mobKind = kAAFMasterMob;
	AxMobIter axMobIter( axContentStorage.GetMobs( &criteria ) );
	IAAFMobSP nextMob;
	bool notAtEnd;
	typedef map< AxString, AxAutoPtr<AxMasterMob> > MobMap;
	MobMap mobMap;

	for( notAtEnd = axMobIter.NextOne( nextMob );
	     notAtEnd;
	     notAtEnd = axMobIter.NextOne( nextMob ) ) { 
 	         AxAutoPtr<AxMasterMob> axMasterMob(
			    new AxMasterMob( AxQueryInterface<IAAFMob,IAAFMasterMob>(
				nextMob,
				IID_IAAFMasterMob ) ) );

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
	
	// Create the composition mob off which we with hang our
	// audio, video source clips and transitions.
	
	AxCompositionMob axCompMob(
		AxCreateInstance<IAAFCompositionMob>( axDictionary,
						      AUID_AAFCompositionMob,
						      IID_IAAFCompositionMob ) );
	axHeader.AddMob( axCompMob );

	aafRational_t editRate = {25, 1};
	
	int videoSlotID = 1;
	AxString videoSlotName( L"Video Timeline" );

	int audioSlotID = 2;
	AxString audioSlotName( L"Audio Timeline" );
		
	//
	// Create one sequence for each time line (i.e. track).
	//

	AxDataDef axAudioDataDef( axDictionary.LookupDataDef( DDEF_Sound ) );
	AxSequence axAudioSequence(	AxCreateInstance<IAAFSequence>( axDictionary,
									AUID_AAFSequence,
									IID_IAAFSequence ) );
	axAudioSequence.Initialize( axAudioDataDef );

	AxDataDef axVideoDataDef( axDictionary.LookupDataDef( DDEF_Picture ) );
	AxSequence axVideoSequence(	AxCreateInstance<IAAFSequence>( axDictionary,
									AUID_AAFSequence,
									IID_IAAFSequence ) );
	axVideoSequence.Initialize( axVideoDataDef );

	
	//
	// Append one timeline slot per sequence to the composition mob.
	//

	axCompMob.AppendNewTimelineSlot( editRate,
					 axVideoSequence,
					 videoSlotID,
					 videoSlotName,
					 0		// origin (aafPosition_t)
					 );
	

	axCompMob.AppendNewTimelineSlot( editRate,
					 axAudioSequence,
					 audioSlotID,
					 audioSlotName,
					 0		// origin (aafPosition_t)
					 );
	
	//
	// Create source clips and transitions.
	//

	// First audio....

	AxSourceClip axAudioClipA ( 
		CreateSourceClipToAppendToSequence( axDictionary, *mobMap[ audioA ], axAudioSequence ) );
	
	AxSourceClip axAudioClipB ( 
		CreateSourceClipToAppendToSequence( axDictionary, *mobMap[ audioB ], axAudioSequence ) );
	
	if ( !axDictionary.isKnownTypeDef( kAAFEffectMonoAudioDissolve ) )
	{
		// FIXME - Once again, there is little documentation concerning
		// this.  I'm not certain this is correct.  For the moment,
		// I simply attempt to make it correct enough for the type
		// lookup to succeed and the file get generated.

		AxOperationDef axOpDef( AxCreateInstance<IAAFOperationDef> ( axDictionary,
									     AUID_AAFOperationDef,
									     IID_IAAFOperationDef ) );

		axOpDef.Initialize( kAAFEffectMonoAudioDissolve,
				    L"Example Mono Dissolve",
				    L"No timewarp, bypass track 0, 2 mono audio inputs" );

	    axOpDef.SetIsTimeWarp( false );
		axOpDef.SetCategory( kAAFEffectMonoAudioDissolve );
	    axOpDef.SetBypass( 0 );
	    axOpDef.SetNumberInputs( 2 );
	    axOpDef.SetDataDef( axAudioDataDef );

	    axDictionary.RegisterOperationDef( axOpDef );
	}


	AxOperationDef axMonoAudioDslvOpDef( axDictionary.LookupOperationDef( kAAFEffectMonoAudioDissolve ) );


	AxOperationGroup axMonoAudioDslvOpGroup( AxCreateInstance<IAAFOperationGroup>( axDictionary,
										       AUID_AAFOperationGroup,
										       IID_IAAFOperationGroup ) );

	// FIXME - Duration... that's in samples - right?
	// We will overlap the segment by 1 pal frame (at 44100 samples/sec).
	// FIXME - Assumed rate and duration of source material.
	axMonoAudioDslvOpGroup.Initialize( axAudioDataDef, 44100/25, axMonoAudioDslvOpDef );

	// FIXME Documentation and examples are sketchy here - I'm
	// not certain why this isn't necessary.
	//axMonoAudioDslvOpGroup.AppendInputSegment( axAudioClipA );
	//axMonoAudioDslvOpGroup.AppendInputSegment( axAudioClipB );

	AxTransition axMonoAudioDslvTransition( AxCreateInstance<IAAFTransition>( axDictionary,
										  AUID_AAFTransition,
										  IID_IAAFTransition ) );

	axMonoAudioDslvTransition.Initialize( axAudioDataDef, 44100/25, 0,
					      axMonoAudioDslvOpGroup );

	// Next video...

	AxSourceClip axVideoClipA (
		CreateSourceClipToAppendToSequence( axDictionary, *mobMap[ videoA ], axVideoSequence ) );
		
	AxSourceClip axVideoClipB (
		CreateSourceClipToAppendToSequence( axDictionary, *mobMap[ videoB ], axVideoSequence ) );

	//
	// Setup a video dissolve
	//

	if ( !axDictionary.isKnownTypeDef( kAAFEffectVideoDissolve ) )
	{
		// FIXME - Once again, there is little documentation concerning
		// this.  I don't know if this is correct.  For the moment,
		// I simply attempt to make it correct enought for the type
		// lookup to succed and the file get generated.

		AxOperationDef axOpDef( AxCreateInstance<IAAFOperationDef> ( axDictionary,
									     AUID_AAFOperationDef,
									     IID_IAAFOperationDef ) );


		axOpDef.Initialize( kAAFEffectVideoDissolve,
				    L"Example Mono Dissolve",
				    L"No timewarp, bypass track 0, 2 video inputs" );


		axOpDef.SetIsTimeWarp( false );
		axOpDef.SetCategory( kAAFEffectVideoDissolve );
		axOpDef.SetBypass( 0 );
		axOpDef.SetNumberInputs( 2 );
		axOpDef.SetDataDef( axAudioDataDef );

		axDictionary.RegisterOperationDef( axOpDef );
	}

	AxOperationDef axVideoDslvOpDef( axDictionary.LookupOperationDef( kAAFEffectVideoDissolve ) );
	
	AxOperationGroup axVideoDslvOpGroup( AxCreateInstance<IAAFOperationGroup>( axDictionary,
										   AUID_AAFOperationGroup,
										   IID_IAAFOperationGroup ) );
	
	// Overlap by one frame.
	// FIXME - Hardcoded.
	axVideoDslvOpGroup.Initialize( axVideoDataDef, 1, axVideoDslvOpDef );
	
	// FIXME - Based on the example in the Dev. Guide, I would have expected
	// this to be necessary.  Nope.  If I do this I get: "Object not attached"
	// violated in routine "OMStorable::~OMStorable" 
	// axVideoDslvOpGroup.AppendInputSegment( axVideoClipA );
	// axVideoDslvOpGroup.AppendInputSegment( axVideoClipB );


	AxTransition axVideoDslvTransition( AxCreateInstance<IAAFTransition>( axDictionary,
									      AUID_AAFTransition,
									      IID_IAAFTransition ) );

	axVideoDslvTransition.Initialize( axVideoDataDef, 1, 0,
					  axVideoDslvOpGroup );

	//
	// Append the series of components to the sequences to form each
	// track.  The components order is:
	// [first segment of essence] [transition] [second segment of essence]
	//

	axAudioSequence.AppendComponent( axAudioClipA );
	axAudioSequence.AppendComponent( axMonoAudioDslvTransition );
	axAudioSequence.AppendComponent( axAudioClipB );

	axVideoSequence.AppendComponent( axVideoClipA );
	axVideoSequence.AppendComponent( axVideoDslvTransition );
	axVideoSequence.AppendComponent( axVideoClipB );
}
