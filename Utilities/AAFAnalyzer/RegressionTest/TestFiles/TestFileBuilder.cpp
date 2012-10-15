//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

//Regression Test Files files
#include "TestFileBuilder.h"

//Ax files
#include <AxEx.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxDefObject.h>
#include <AxComponent.h>
#include <AxMobSlot.h>
#include <AxEssence.h>
#include <AxUtil.h>
#include <AxMob.h>
#include <AxParameter.h>
#include <AxMetaDef.h>
#include <AxDescriptiveFramework.h>
#include <AxKLVData.h>

//AAF files
#include <AAFResult.h>
#include <AAFExtEnum.h>
#include <AAFDataDefs.h>
#include <AAFCodecDefs.h>
#include <AAFContainerDefs.h>
#include <AAFInterpolatorDefs.h>
#include <AAFOperationDefs.h>
#include <AAFParameterDefs.h>
#include <AAFTypeDefUIDs.h>

//STL files
#include <iostream>
#include <sstream>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer
{

using namespace std;

/*
 *
 * Constructor/Destructor
 *
 */

TestFileBuilder::TestFileBuilder( const char* outFile)
    : _mobCount( 0 ),
      _useLegacyEffectDefinitions( false )
{
    wostringstream ssOutFile;
    ssOutFile << outFile;
    AxString filename( ssOutFile.str().c_str() );


    try {
        if ( AxFile::isAAFFile( filename ) ) {
            throw AxString( L"Error: File already exists" );
        }
    }
    catch( const AxExHResult& ex ) {
        //  AAFRESULT_FILE_NOT_FOUND is okay.  The file does
        // not exist.
        if ( ex.getHResult() != AAFRESULT_FILE_NOT_FOUND ) {
            throw;
        }
    }

    wcout << L"Create new AAF File"<< endl;
    _axFile.OpenNewModify( filename );

    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxDataDef> spAxDataDef( new AxDataDef( AxCreateInstance<IAAFDataDef>( axDictionary ) ) );
    spAxDataDef->Initialize( kAAFDataDef_Unknown, L"Unknown", L"Unknown" );
    axDictionary.RegisterDataDef( *spAxDataDef );
}

TestFileBuilder::~TestFileBuilder()
{
    _axFile.Save();
    _axFile.Close();
}

/*
 *
 * Definitions
 *
 */

void TestFileBuilder::CreateKLVDataDefinition( const AxString& name, const AxString& description )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxKLVDataDef axKLVDef( AxCreateInstance<IAAFKLVDataDefinition> ( axDictionary ) );
    aafUID_t auid = GenerateAUID();

    axDictionary.RegisterKLVDataKey( auid, axDictionary.LookupTypeDef( kAAFTypeID_String ) );

    axKLVDef.Initialize( auid, name, description );
    _namedAUIDs[name] = auid;

    axDictionary.RegisterKLVDataDef( axKLVDef );
}

void TestFileBuilder::CreateTaggedValueDefinition( const AxString& name, const AxString& description )
{	
	
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxTaggedValueDef axTaggedDef( AxCreateInstance<IAAFTaggedValueDefinition> ( axDictionary ) );
    aafUID_t auid = GenerateAUID();

    axTaggedDef.Initialize( auid, name, description );
    _namedAUIDs[name] = auid;

    axDictionary.RegisterTaggedValueDef( axTaggedDef );
}

void TestFileBuilder::CreateOperationDefinition( const AxString& name, const AxString& description )
{	
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxOperationDef axOpDef( AxCreateInstance<IAAFOperationDef> ( axDictionary ) );
    aafUID_t auid = GenerateAUID();

    axOpDef.Initialize( auid, name, description );
    _namedAUIDs[name] = auid;

    aafUID_t dataDefId;
    if ( _useLegacyEffectDefinitions )
    {
      dataDefId = kAAFDataDef_LegacyPicture;
    }
    else
    {
      dataDefId = kAAFDataDef_Picture;
    }
    AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
    axOpDef.SetDataDef( axDataDef );

    axDictionary.RegisterOperationDef( axOpDef );
}

/*
 *
 * Mobs
 *
 */

boost::shared_ptr<AxMob> TestFileBuilder::AddTopLevel( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxCompositionMob> spAxCompMob( new AxCompositionMob( AxCreateInstance<IAAFCompositionMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxCompMob->SetName( name );
    }
    spAxCompMob->SetUsageCode(kAAFUsage_TopLevel);
    spAxCompMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxCompMob );

    return spAxCompMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddLowerLevel( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxCompositionMob> spAxCompMob( new AxCompositionMob( AxCreateInstance<IAAFCompositionMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxCompMob->SetName( name );
    }
    spAxCompMob->SetUsageCode(kAAFUsage_LowerLevel);
    spAxCompMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxCompMob );

    return spAxCompMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddSubClip( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxCompositionMob> spAxCompMob( new AxCompositionMob( AxCreateInstance<IAAFCompositionMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxCompMob->SetName( name );
    }
    spAxCompMob->SetUsageCode(kAAFUsage_SubClip);
    spAxCompMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxCompMob );

    return spAxCompMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddAdjustedClip( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxCompositionMob> spAxCompMob( new AxCompositionMob( AxCreateInstance<IAAFCompositionMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxCompMob->SetName( name );
    }
    spAxCompMob->SetUsageCode(kAAFUsage_AdjustedClip);
    spAxCompMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxCompMob );

    return spAxCompMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddTemplateClip( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxMasterMob> spAxMasterMob( new AxMasterMob( AxCreateInstance<IAAFMasterMob>( axDictionary ) ) );
    spAxMasterMob->Initialize();
    if ( isNamed )
    {
        spAxMasterMob->SetName( name );
    }
    spAxMasterMob->SetUsageCode( kAAFUsage_Template );
    spAxMasterMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxMasterMob );

    return spAxMasterMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddClip( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxMasterMob> spAxMasterMob( new AxMasterMob( AxCreateInstance<IAAFMasterMob>( axDictionary ) ) );
    spAxMasterMob->Initialize();
    if ( isNamed )
    {
        spAxMasterMob->SetName( name );
    }
    spAxMasterMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxMasterMob );

    return spAxMasterMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 intNothing, bool boolNothing )
{
	
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );
	
    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    // A "<file-source>" is used generically in the test (xml) files
    // in cases were the test doesn't care what about the exact type
    // of file source. However, we must add a descriptor of some sort,
    // therefore, we genically add a fake wave header. It would be
    // nice if we didn't need to do this, but as of right now, all
    // other well-known FileDescriptors that are of non-normative
    // essence types are being declared elsewhere and we don't want to
    // confuse between them and a "basic" file source mob.

    AxWAVEDescriptor axFileDes( AxCreateInstance<IAAFWAVEDescriptor>( axDictionary ) );

    aafUInt8* buffer = new aafUInt8[ sizeof(aafUInt8[44]) ];
    AxBuffer<aafUInt8> header = AxBuffer<aafUInt8>( std::auto_ptr<aafUInt8>( buffer ), sizeof( aafUInt8[44] ) );;
    axFileDes.SetSummary( header.GetSize(), header.GetPtr().get() );

    axFileDes.SetSampleRate( sampleRate );
    spAxSrcMob->SetEssenceDescriptor( axFileDes );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );
    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddMonoAudioFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxSoundDescriptor axSoundDes( AxQueryInterface<IAAFPCMDescriptor, IAAFSoundDescriptor>( AxCreateInstance<IAAFPCMDescriptor>( axDictionary ) ) );
    axSoundDes.SetSampleRate( sampleRate );
    axSoundDes.SetChannelCount( 1 );
    spAxSrcMob->SetEssenceDescriptor( axSoundDes );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddMultiChannelAudioFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxSoundDescriptor axSoundDes( AxQueryInterface<IAAFPCMDescriptor, IAAFSoundDescriptor>( AxCreateInstance<IAAFPCMDescriptor>( axDictionary ) ) );
    axSoundDes.SetSampleRate( sampleRate );
    axSoundDes.SetChannelCount( 2 );
    spAxSrcMob->SetEssenceDescriptor( axSoundDes );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddRGBAFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 alphaVal, bool hasAlphaType )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxRGBADescriptor axRGBADes( AxCreateInstance<IAAFRGBADescriptor>( axDictionary ) );
    axRGBADes.SetSampleRate( sampleRate );
    if ( hasAlphaType )
    {
        axRGBADes.SetAlphaTransparency( alphaVal );
    }

    spAxSrcMob->SetEssenceDescriptor( axRGBADes );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddCDCIFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 alphaVal, bool hasAlphaType )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxCDCIDescriptor axCDCIDes( AxCreateInstance<IAAFCDCIDescriptor>( axDictionary ) );
    axCDCIDes.SetSampleRate( sampleRate );
    if ( hasAlphaType )
    {
        axCDCIDes.SetAlphaTransparency( alphaVal );
    }

    spAxSrcMob->SetEssenceDescriptor( axCDCIDes );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddRecordingSource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxRecordingDescriptor axRecording( AxCreateInstance<IAAFRecordingDescriptor>( axDictionary ) );
    axRecording.Initialize();
    spAxSrcMob->SetEssenceDescriptor( axRecording );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddImportSource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxImportDescriptor axImport( AxCreateInstance<IAAFImportDescriptor>( axDictionary ) );
    axImport.Initialize();
    spAxSrcMob->SetEssenceDescriptor( axImport );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddTapeSource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxTapeDescriptor axTape( AxCreateInstance<IAAFTapeDescriptor>( axDictionary ) );
    axTape.Initialize();
    spAxSrcMob->SetEssenceDescriptor( axTape );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddFilmSource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxFilmDescriptor axFilm( AxCreateInstance<IAAFFilmDescriptor>( axDictionary ) );
    spAxSrcMob->SetEssenceDescriptor( axFilm );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

boost::shared_ptr<AxMob> TestFileBuilder::AddAuxiliarySource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    spAxSrcMob->SetName( name );

    AxAuxiliaryDescriptor axAux( AxCreateInstance<IAAFAuxiliaryDescriptor>( axDictionary ) );
    axAux.SetMimeType( L"audio/midi" );
    spAxSrcMob->SetEssenceDescriptor( axAux );
    spAxSrcMob->SetMobID( this->GenerateMobId(name) );

    axHeader.AddMob( *spAxSrcMob );

    return spAxSrcMob;
}

/*
 *
 * Create Components
 *
 */

boost::shared_ptr<AxComponent> TestFileBuilder::CreateTimecode( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxTimecode> axTimecode( new AxTimecode( AxCreateInstance<IAAFTimecode>( axDictionary ) ) );

    aafTimecode_t tc;
    tc.startFrame = 1;
    tc.drop = 1;
    tc.fps = 1;

    axTimecode->Initialize( 0, tc );
    if ( hasLength )
    {
        axTimecode->SetLength( length );
    }
    AddDataDef( *axTimecode, essenceType );

    return axTimecode;

}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateEdgecode( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxEdgecode> axEdgecode( new AxEdgecode( AxCreateInstance<IAAFEdgecode>( axDictionary ) ) );

    aafEdgecode_t ec;
    ec.startFrame = 1;
    ec.filmKind = 1;
    ec.codeFormat = 1;
    ec.header[0] = 1;
    ec.header[1] = 1;
    ec.header[2] = 1;
    ec.header[3] = 1;
    ec.header[4] = 1;
    ec.header[5] = 1;
    ec.header[6] = 1;
    ec.header[7] = 1;

    axEdgecode->Initialize( 0, ec );
    if ( hasLength )
    {
        axEdgecode->SetLength( length );
    }
    AddDataDef( *axEdgecode, essenceType );

    return axEdgecode;

}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateTimecodeStream12M( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxTimecodeStream12M> axTimecodeStream( new AxTimecodeStream12M( AxCreateInstance<IAAFTimecodeStream12M>( axDictionary ) ) );

    if ( hasLength )
    {
        axTimecodeStream->SetLength( length );
    }
    AddDataDef( *axTimecodeStream, essenceType );

    return axTimecodeStream;
}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateEOC( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceClip> axSrcClip( new AxSourceClip( AxCreateInstance<IAAFSourceClip>( axDictionary ) ) );

    aafSourceRef_t srcRef;
    srcRef.sourceID     = AxConstants::NULL_MOBID;
    srcRef.sourceSlotID = 0;
    srcRef.startTime    = 0;

    AddDataDef( *axSrcClip, essenceType );
    if ( hasLength )
    {
        axSrcClip->SetLength( length );
    }
    axSrcClip->SetSourceReference( srcRef );

    return axSrcClip;
}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateOOF( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int startTime, bool hasStartTime )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceClip> axSrcClip( new AxSourceClip( AxCreateInstance<IAAFSourceClip>( axDictionary ) ) );

    aafSourceRef_t srcRef;
    srcRef.sourceID     = this->GenerateMobId(L"OOF");
    srcRef.sourceSlotID = 1;
    if ( hasStartTime )
    {
        srcRef.startTime = startTime;
    }

    AddDataDef( *axSrcClip, essenceType );
    if ( hasLength )
    {
        axSrcClip->SetLength( length );
    }
    axSrcClip->SetSourceReference( srcRef );

    return axSrcClip;
}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateSourceClip( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int startTime, bool hasStartTime)
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSourceClip> axSrcClip( new AxSourceClip( AxCreateInstance<IAAFSourceClip>( axDictionary ) ) );

    if ( hasLength )
    {
        axSrcClip->SetLength( length );
    }
    AddDataDef( *axSrcClip, essenceType );

    aafSourceRef_t srcRef;
    if ( hasStartTime )
    {
        srcRef.startTime = startTime;
    }

    axSrcClip->SetSourceReference( srcRef );

    return axSrcClip;
}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateTransition( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxTransition> axTransition( new AxTransition( AxCreateInstance<IAAFTransition>( axDictionary ) ) );

    AddDataDef( *axTransition, essenceType );
    if ( hasLength )
    {
        axTransition->SetLength( length );
    }
    axTransition->SetCutPoint( 1 );

    return axTransition;
}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateSequence( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxSequence> axSequence( new AxSequence( AxCreateInstance<IAAFSequence>( axDictionary ) ) );
    if ( hasLength )
    {
        axSequence->SetLength( length );
    }
    AddDataDef( *axSequence, essenceType );

    return axSequence;
}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateCommentMarker( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int position, bool hasPosition )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxCommentMarker> axCommentMarker( new AxCommentMarker( AxCreateInstance<IAAFCommentMarker>( axDictionary ) ) );

    if ( hasLength )
    {
        axCommentMarker->SetLength( length );
    }
    if ( hasPosition )
    {
        axCommentMarker->SetPosition( position );
    }
    AddDataDef( *axCommentMarker, essenceType );

    return axCommentMarker;
}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateDescriptiveMarker( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int position, bool hasPosition )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxDescriptiveMarker> axDesMarker( new AxDescriptiveMarker( AxCreateInstance<IAAFDescriptiveMarker>( axDictionary ) ) );

    if ( hasLength )
    {
        axDesMarker->SetLength( length );
    }
    if ( hasPosition )
    {
        axDesMarker->SetPosition( position );
    }
    AddDataDef( *axDesMarker, essenceType );

    return axDesMarker;
}

boost::shared_ptr<AxComponent> TestFileBuilder::CreateOperationGroup( TrackType essenceType, const AxString& opDef, aafLength_t length, bool hasLength, int intNothing, bool boolNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    boost::shared_ptr<AxOperationGroup> axOpGroup( new AxOperationGroup( AxCreateInstance<IAAFOperationGroup>( axDictionary ) ) );

    AddDataDef( *axOpGroup, essenceType );
    if ( hasLength )
    {
        axOpGroup->SetLength( length );
    }
    axOpGroup->SetOperationDef( *(this->GetOperationDef( opDef, axDictionary )) );

    return axOpGroup;
}

/*
 *
 * Fill Components
 *
 */

void TestFileBuilder::InitializeSourceClip( boost::shared_ptr<AxSourceReference> parent, AxMob& child, const AxString& source )
{

    boost::shared_ptr<AxSourceClip> axSrcClip = boost::dynamic_pointer_cast<AxSourceClip>( parent );

    aafSourceRef_t srcRef = axSrcClip->GetSourceReference();
    if (mobIdMap.find(source)!=mobIdMap.end() && source!=L"")
    {
    	srcRef.sourceID = mobIdMap[source];
    	
    }
    else 
    {
    srcRef.sourceID     = child.GetMobID();
    }
    
    srcRef.sourceSlotID = 1;

    axSrcClip->SetSourceReference( srcRef );

}


void TestFileBuilder::AddToTransition( boost::shared_ptr<AxComponent> parent, AxComponent& child )
{
    boost::shared_ptr<AxTransition> axTransition = boost::dynamic_pointer_cast<AxTransition>( parent );

    boost::shared_ptr<AxOperationGroup> axOpGroup( new AxOperationGroup( AxQueryInterface<IAAFComponent, IAAFOperationGroup>( child ) ) );

    axTransition->SetOperationGroup( *axOpGroup );
}

void TestFileBuilder::AddToSequence( boost::shared_ptr<AxComponent> parent, AxComponent& child )
{
    boost::shared_ptr<AxSequence> axSequence = boost::dynamic_pointer_cast<AxSequence>( parent );

    axSequence->AppendComponent( child );
}

void TestFileBuilder::AddToCommentMarker( boost::shared_ptr<AxComponent> parent, AxComponent& child )
{
    boost::shared_ptr<AxCommentMarker> axCommentMarker = boost::dynamic_pointer_cast<AxCommentMarker>( parent );

    boost::shared_ptr<AxSourceReference> axSrcRef( new AxSourceReference( AxQueryInterface<IAAFComponent, IAAFSourceReference>( child ) ) );

    axCommentMarker->SetAnnotation( *axSrcRef );
}

void TestFileBuilder::AddToOperationGroup( boost::shared_ptr<AxSegment> parent, AxSegment& child, int property )
{
    boost::shared_ptr<AxOperationGroup> axOpGroup = boost::dynamic_pointer_cast<AxOperationGroup>( parent );

    if ( property == INPUT_SEGMENT )
    {
        child.SetDataDef( axOpGroup->GetDataDef() );
        axOpGroup->AppendInputSegment( child );
    }
}

/*
 *
 * Annotations
 *
 */

void TestFileBuilder::AddComment( boost::shared_ptr<AxComponent> axComponent, const AxString& name, const AxString& value )
{
    axComponent->AppendComment( name, value );
}

void TestFileBuilder::AddKLVData( boost::shared_ptr<AxComponent> axComponent, const AxString& keyName, const AxString& value )
{

    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    //Find the key or create a new one.
    aafUID_t key;
    if ( _namedAUIDs.find( keyName ) == _namedAUIDs.end() )
    {
        //Allow a KLVData object to be added even if it hasn't been completly
        //registered.
        key = GenerateAUID();
        axDictionary.RegisterKLVDataKey( key, axDictionary.LookupTypeDef( kAAFTypeID_String ) );
    }
    else
    {
        key = _namedAUIDs[keyName];
    }

    //Set the length and value parameters.
    AxString internalValue = value;
    int length = sizeof( internalValue );
    aafUInt8* val = reinterpret_cast<aafUInt8*>( &internalValue );

    //Create and append the KLVData object.
    AxKLVData axKLVData( AxCreateInstance<IAAFKLVData>( axDictionary ) );
    axKLVData.Initialize( key, length, val );

    axComponent->AppendKLVData( axKLVData );
}

/*
 *
 * Mob Slots
 *
 */

void TestFileBuilder::AttachTimelineSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered, int markedIn, bool isMarkedIn, int markedOut, bool isMarkedOut, int userPos, bool isUserPos, int origin )
{

    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxTimelineMobSlot axTimelineMobSlot( AxCreateInstance<IAAFTimelineMobSlot>( axDictionary ) );

    axTimelineMobSlot.SetEditRate( editRate );
    axTimelineMobSlot.SetSegment( axSegment );
    axTimelineMobSlot.SetSlotID( parent.CountSlots() + 1 );
    axTimelineMobSlot.SetOrigin( origin );
    if ( isNamed )
    {
        axTimelineMobSlot.SetName( name );
    }
    if ( isNumbered )
    {
        axTimelineMobSlot.SetPhysicalNum( physicalTrackNum );
    }
    if ( isMarkedIn )
    {
        axTimelineMobSlot.SetMarkIn( markedIn );
    }
    if ( isMarkedOut )
    {
        axTimelineMobSlot.SetMarkOut( markedOut );
    }
    if ( isUserPos )
    {
      axTimelineMobSlot.SetUserPos( userPos );
    }

    parent.AppendSlot( axTimelineMobSlot );

}

void TestFileBuilder::AttachEventSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered, int intNothing1, bool boolNothing1, int intNothing2, bool boolNothing2, int intNothing3, bool boolNothing3, int intNothing4 )
{

    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxEventMobSlot axEventMobSlot( AxCreateInstance<IAAFEventMobSlot>( axDictionary ) );

    axEventMobSlot.SetEditRate( editRate );
    axEventMobSlot.SetSegment( axSegment );
    axEventMobSlot.SetSlotID( parent.CountSlots() + 1 );
    if ( isNamed )
    {
        axEventMobSlot.SetName( name );
    }
    if ( isNumbered )
    {
        axEventMobSlot.SetPhysicalNum( physicalTrackNum );
    }

    parent.AppendSlot( axEventMobSlot );

}

void TestFileBuilder::AttachStaticSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered, int intNothing1, bool boolNothing1, int intNothing2, bool boolNothing2,  int intNothing3, bool boolNothing3, int intNothing4 )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxStaticMobSlot axStaticMobSlot( AxCreateInstance<IAAFStaticMobSlot>( axDictionary ) );

    axStaticMobSlot.SetSegment( axSegment );
    axStaticMobSlot.SetSlotID( parent.CountSlots() + 1 );
    if ( isNamed )
    {
        axStaticMobSlot.SetName( name );
    }
    if ( isNumbered )
    {
        axStaticMobSlot.SetPhysicalNum( physicalTrackNum );
    }

    parent.AppendSlot( axStaticMobSlot );
}

/*
 *
 * Parameters
 *
 */

void TestFileBuilder::AttachConstantRationalParameter( AxOperationGroup& axOpGroup, const aafUID_t& paramDefId, aafUInt32 numerator, aafUInt32 denominator, const aafUID_t& uidNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxConstantValue axParam( AxCreateInstance<IAAFConstantValue>( axDictionary ) );

    aafRational_t param;
    param.numerator = numerator;
    param.denominator = denominator;

    axParam.Initialize( *(this->GetParameterDef( paramDefId, axDictionary )), sizeof(param), (aafDataBuffer_t)&param );

    axOpGroup.AddParameter( axParam );
}

void TestFileBuilder::AttachVaryingRationalParameter( AxOperationGroup& axOpGroup, const aafUID_t& paramDefId, aafUInt32 numerator, aafUInt32 denominator, const aafUID_t& interpolationDefId )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxVaryingValue axParam( AxCreateInstance<IAAFVaryingValue>( axDictionary ) );

    axParam.Initialize( *(this->GetParameterDef( paramDefId, axDictionary )), *(this->GetInterpolationDef( interpolationDefId, axDictionary ) ) );

    axOpGroup.AddParameter( axParam );
}

void TestFileBuilder::AttachDescriptiveFramework( boost::shared_ptr<AxDescriptiveMarker> axMarker )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxDescriptiveFramework axDesFwk( AxCreateInstance<IAAFDescriptiveFramework>( axDictionary ) );

    axMarker->SetDescriptiveFramework( axDesFwk );
}

void TestFileBuilder::SetOperationalPattern( aafUID_t pattern )
{
    AxHeader axHeader( _axFile.getHeader() );
    axHeader.SetOperationalPattern( pattern );
}

void TestFileBuilder::UseLegacyEffectDefinitions()
{
    _useLegacyEffectDefinitions = true;
}

/*
 *
 * Helper Functions
 *
 */

//Return a mob ID that is unique within this file.
const aafMobID_t TestFileBuilder::GenerateMobId(AxString mobName)
{
    _mobCount++;

	aafUInt8  j = static_cast<aafUInt8>( _mobCount );
	aafUInt16 k = static_cast<aafUInt16>( _mobCount );

	aafMobID_t id;
    id.SMPTELabel[0] = j % 233;
    id.SMPTELabel[1] = j % 234;
    id.SMPTELabel[2] = j % 235;
    id.SMPTELabel[3] = j % 236;
    id.SMPTELabel[4] = j % 237;
    id.SMPTELabel[5] = j % 238;
    id.SMPTELabel[6] = j % 239;
    id.SMPTELabel[7] = j % 240;
    id.SMPTELabel[8] = j % 241;
    id.SMPTELabel[9] = j % 242;
    id.SMPTELabel[10] = j % 243;
    id.SMPTELabel[11] = j % 244;
    id.length = j % 245;
    id.instanceHigh = j % 246;
    id.instanceMid = j % 247;
    id.instanceLow = j % 248;
    id.material.Data1 = _mobCount;
    id.material.Data2 = k % 65535;
    id.material.Data3 = k % 65536;
    id.material.Data4[0] = j % 249;
    id.material.Data4[1] = j % 250;
    id.material.Data4[2] = j % 251;
    id.material.Data4[3] = j % 252;
    id.material.Data4[4] = j % 253;
    id.material.Data4[5] = j % 254;
    id.material.Data4[6] = j % 255;
    id.material.Data4[7] = j % 256;
    
    if (mobIdMap.find(mobName)==mobIdMap.end())
    {
    	mobIdMap[mobName]=id;
    }
    
    return id;
}

//Return an AUID.  This AUID is not guaranteed to be globally unique, but,
//generating a random AUID would not be sufficient for regression testing
//purposes.
const aafUID_t TestFileBuilder::GenerateAUID()
{
    //Get a Mob ID and use the material portion as the AUID.
    aafMobID_t mobId = GenerateMobId(L"AUID");
    return mobId.material;
}

void TestFileBuilder::AddDataDef( AxComponent& axComponent, TrackType essenceType )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    switch ( essenceType )
    {
        case AUDIO:     axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Sound       ) ); break;
        case PICTURE:   axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture     ) ); break;
        case TIMECODE:  axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Timecode    ) ); break;
        case EDGECODE:  axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Edgecode    ) ); break;
        case AUXILIARY: axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Auxiliary   ) ); break;
        default:        axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Unknown     ) );
    }
}

boost::shared_ptr<AxOperationDef> TestFileBuilder::GetOperationDef( const AxString& opDef, AxDictionary& axDictionary )
{
    if ( _namedAUIDs.find( opDef ) == _namedAUIDs.end() )
    {
        AxOperationDef axOpDef( AxCreateInstance<IAAFOperationDef> ( axDictionary ) );
        aafUID_t opDefId;

        if ( opDef == L"Video Dissolve" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoDissolve;
            axOpDef.Initialize( opDefId,
                        L"Video Dissolve",
                        L"Video Dissolve" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"SMPTE Video Wipe" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_SMPTEVideoWipe;
            axOpDef.Initialize( opDefId,
                        L"SMPTE Video Wipe",
                        L"SMPTE Video Wipe" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Speed Control" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoSpeedControl;
            axOpDef.Initialize( opDefId,
                        L"Video Speed Control",
                        L"Video Speed Control" );
            axOpDef.SetIsTimeWarp( true );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Repeat" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoRepeat;
            axOpDef.Initialize( opDefId,
                        L"Video Repeat",
                        L"Video Repeat" );
            axOpDef.SetIsTimeWarp( true );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Flip" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_Flip;
            axOpDef.Initialize( opDefId,
                        L"Video Flip",
                        L"Video Flip" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Flop" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_Flop;
            axOpDef.Initialize( opDefId,
                        L"Video Flop",
                        L"Video Flop" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Flip Flop" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_FlipFlop;
            axOpDef.Initialize( opDefId,
                        L"Video Flip Flop",
                        L"Video Flip Flop" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Position" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoPosition;
            axOpDef.Initialize( opDefId,
                        L"Video Position",
                        L"Video Position" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Crop" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoCrop;
            axOpDef.Initialize( opDefId,
                        L"Video Crop",
                        L"Cideo Crop" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Scale" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoScale;
            axOpDef.Initialize( opDefId,
                        L"Video Scale",
                        L"Video Scale" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Rotate" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoRotate;
            axOpDef.Initialize( opDefId,
                        L"Video Rotate",
                        L"Video Rotate" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Video Corner Pinning" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoCornerPinning;
            axOpDef.Initialize( opDefId,
                        L"Video Corner Pinning",
                        L"Video Corner Pinning" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Alpha With Video Key effect" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoAlphaWithinVideoKey;
            axOpDef.Initialize( opDefId,
                        L"Alpha With Video Key effect",
                        L"Alpha With Video Key effect" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Separate Alpha Key effect" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoSeparateAlphaKey;
            axOpDef.Initialize( opDefId,
                        L"Separate Alpha Key effect",
                        L"Separate Alpha Key effect" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Luminance Key" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoLuminanceKey;
            axOpDef.Initialize( opDefId,
                        L"Luminance Key",
                        L"Luminance Key" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Chroma Key" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacyPicture;
            }
            else
            {
                dataDefId = kAAFDataDef_Picture;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_VideoChromaKey;
            axOpDef.Initialize( opDefId,
                        L"Chroma Key",
                        L"Chroma Key" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Mono Audio Gain" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacySound;
            }
            else
            {
                dataDefId = kAAFDataDef_Sound;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_MonoAudioGain;
            axOpDef.Initialize( opDefId,
                        L"Mono Audio Gain",
                        L"Mono Audio Gain" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Mono Audio Pan" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacySound;
            }
            else
            {
                dataDefId = kAAFDataDef_Sound;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_MonoAudioPan;
            axOpDef.Initialize( opDefId,
                        L"Mono Audio Pan",
                        L"Mono Audio Pan" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Mono Audio Dissolve" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacySound;
            }
            else
            {
                dataDefId = kAAFDataDef_Sound;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_MonoAudioDissolve,
            axOpDef.Initialize( opDefId,
                        L"Mono Audio Dissolve",
                        L"Mono Audio Dissolve" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDef == L"Two-Parameter Mono Audio Dissolve" )
        {
            aafUID_t dataDefId;
            if ( _useLegacyEffectDefinitions )
            {
                dataDefId = kAAFDataDef_LegacySound;
            }
            else
            {
                dataDefId = kAAFDataDef_Sound;
            }
            AxDataDef axDataDef( axDictionary.LookupDataDef( dataDefId ) );
            opDefId = kAAFOperationDef_TwoParameterMonoAudioDissolve;
            axOpDef.Initialize( opDefId,
                        L"Two-Parameter Mono Audio Dissolve",
                        L"Two-Parameter Mono Audio Dissolve" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }

        axDictionary.RegisterOperationDef( axOpDef );
        _namedAUIDs[opDef] = opDefId;
    }
    boost::shared_ptr<AxOperationDef> spOpDef( new AxOperationDef( axDictionary.LookupOperationDef( _namedAUIDs[opDef] ) ) );

    return spOpDef;
}

boost::shared_ptr<AxParameterDef> TestFileBuilder::GetParameterDef( const aafUID_t& paramDefId, AxDictionary& axDictionary )
{
    if ( !axDictionary.isKnownParameterDef( paramDefId ) )
    {

        AxParameterDef axParamDef( AxCreateInstance<IAAFParameterDef> ( axDictionary ) );
        AxTypeDef axTypeDef( axDictionary.LookupTypeDef( kAAFTypeID_Rational ) );

        if ( paramDefId == kAAFParameterDef_SpeedRatio )
        {
            axParamDef.Initialize(
                paramDefId,
                L"Speed Ratio",
                L"Speed Ratio",
                axTypeDef
            );
        }

        axDictionary.RegisterParameterDef( axParamDef );
    }
    boost::shared_ptr<AxParameterDef> spParamDef( new AxParameterDef( axDictionary.LookupParameterDef( paramDefId ) ) );

    return spParamDef;
}

boost::shared_ptr<AxInterpolationDef> TestFileBuilder::GetInterpolationDef( const aafUID_t& interpolationDefId, AxDictionary& axDictionary )
{
    if ( !axDictionary.isKnownInterpolationDef( interpolationDefId ) )
    {

        AxInterpolationDef axIntDef( AxCreateInstance<IAAFInterpolationDef> ( axDictionary ) );

        if ( interpolationDefId == kAAFInterpolationDef_BSpline )
        {
            axIntDef.Initialize(
                interpolationDefId,
                L"B Spline",
                L"B Spline"
            );
        }
        else if ( interpolationDefId == kAAFInterpolationDef_Constant )
        {
            axIntDef.Initialize(
                interpolationDefId,
                L"Constant",
                L"Constant"
            );
        }
        else if ( interpolationDefId == kAAFInterpolationDef_Linear )
        {
            axIntDef.Initialize(
                interpolationDefId,
                L"Linear",
                L"Linear"
            );
        }
        else if ( interpolationDefId == kAAFInterpolationDef_Log )
        {
            axIntDef.Initialize(
                interpolationDefId,
                L"Log",
                L"Log"
            );
        }
        else if ( interpolationDefId == kAAFInterpolationDef_None )
        {
            axIntDef.Initialize(
                interpolationDefId,
                L"None",
                L"None"
            );
        }
        else if ( interpolationDefId == kAAFInterpolationDef_Power )
        {
            axIntDef.Initialize(
                interpolationDefId,
                L"Power",
                L"Power"
            );
        }

        axDictionary.RegisterInterpolationDef( axIntDef );
    }
    boost::shared_ptr<AxInterpolationDef> spIntDef( new AxInterpolationDef( axDictionary.LookupInterpolationDef( interpolationDefId ) ) );

    return spIntDef;
}

} // end of namespace diskstream
