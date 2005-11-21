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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

//Regression Test Files files
#include "TestFileBuilder.h"

//Ax files
#include <AxEx.h>
#include <AxInit.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxDefObject.h>
#include <AxComponent.h>
#include <AxIterator.h>
#include <AxMobSlot.h>
#include <AxEssence.h>
#include <AxUtil.h>
#include <AxMob.h>
#include <AxParameter.h>
#include <AxMetaDef.h>

//AAF files
#include <AAFResult.h>
#include <AAFExtEnum.h>
#include <AAFDataDefs.h>
#include <AAFCodecDefs.h>
#include <AAFContainerDefs.h>
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
using namespace boost;

/*
 * 
 * Constructor/Destructor
 * 
 */

TestFileBuilder::TestFileBuilder( const char* outFile) : _mobCount( 0 )
{
    AxInit initObj;
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
    
    shared_ptr<AxDataDef> spAxDataDef( new AxDataDef( AxCreateInstance<IAAFDataDef>( axDictionary ) ) );
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
 * Mobs
 * 
 */

shared_ptr<AxMob> TestFileBuilder::AddTopLevel( const AxString& name, bool isNamed, aafRational_t ratNothing )
{           
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );
    
    shared_ptr<AxCompositionMob> spAxCompMob( new AxCompositionMob( AxCreateInstance<IAAFCompositionMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxCompMob->SetName( name );
    }
    spAxCompMob->SetUsageCode(kAAFUsage_TopLevel);
    spAxCompMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxCompMob );
    
    return spAxCompMob;    
}

shared_ptr<AxMob> TestFileBuilder::AddLowerLevel( const AxString& name, bool isNamed, aafRational_t ratNothing )
{           
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );
    
    shared_ptr<AxCompositionMob> spAxCompMob( new AxCompositionMob( AxCreateInstance<IAAFCompositionMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxCompMob->SetName( name );
    }
    spAxCompMob->SetUsageCode(kAAFUsage_LowerLevel);
    spAxCompMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxCompMob );
    
    return spAxCompMob;   
}

shared_ptr<AxMob> TestFileBuilder::AddSubClip( const AxString& name, bool isNamed, aafRational_t ratNothing )
{           
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );
    
    shared_ptr<AxCompositionMob> spAxCompMob( new AxCompositionMob( AxCreateInstance<IAAFCompositionMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxCompMob->SetName( name );
    }
    spAxCompMob->SetUsageCode(kAAFUsage_SubClip);
    spAxCompMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxCompMob );
    
    return spAxCompMob;   
}

shared_ptr<AxMob> TestFileBuilder::AddAdjustedClip( const AxString& name, bool isNamed, aafRational_t ratNothing )
{           
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );
    
    shared_ptr<AxCompositionMob> spAxCompMob( new AxCompositionMob( AxCreateInstance<IAAFCompositionMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxCompMob->SetName( name );
    }
    spAxCompMob->SetUsageCode(kAAFUsage_AdjustedClip);
    spAxCompMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxCompMob );
    
    return spAxCompMob;
}

shared_ptr<AxMob> TestFileBuilder::AddTemplateClip( const AxString& name, bool isNamed, aafRational_t ratNothing )
{ 
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxMasterMob> spAxMasterMob( new AxMasterMob( AxCreateInstance<IAAFMasterMob>( axDictionary ) ) );
    spAxMasterMob->Initialize();
    if ( isNamed )
    {
        spAxMasterMob->SetName( name );
    }
    spAxMasterMob->SetUsageCode( kAAFUsage_Template );
    spAxMasterMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxMasterMob );
    
    return spAxMasterMob;
}

shared_ptr<AxMob> TestFileBuilder::AddClip( const AxString& name, bool isNamed, aafRational_t ratNothing )
{ 
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxMasterMob> spAxMasterMob( new AxMasterMob( AxCreateInstance<IAAFMasterMob>( axDictionary ) ) );
    spAxMasterMob->Initialize();
    if ( isNamed )
    {
        spAxMasterMob->SetName( name );
    }
    spAxMasterMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxMasterMob );
    
    return spAxMasterMob;
}

shared_ptr<AxMob> TestFileBuilder::AddFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxCDCIDescriptor axFileDes( AxCreateInstance<IAAFCDCIDescriptor>( axDictionary ) );
    axFileDes.SetSampleRate( sampleRate );
    spAxSrcMob->SetEssenceDescriptor( axFileDes );
    spAxSrcMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxSrcMob );
    
    return spAxSrcMob;
}

shared_ptr<AxMob> TestFileBuilder::AddRecordingSource( const AxString& name, bool isNamed, aafRational_t ratNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }   

    AxRecordingDescriptor axRecording( AxCreateInstance<IAAFRecordingDescriptor>( axDictionary ) );
    axRecording.Initialize();
    spAxSrcMob->SetEssenceDescriptor( axRecording );
    spAxSrcMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxSrcMob );
    
    return spAxSrcMob;
}

shared_ptr<AxMob> TestFileBuilder::AddImportSource( const AxString& name, bool isNamed, aafRational_t ratNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxImportDescriptor axImport( AxCreateInstance<IAAFImportDescriptor>( axDictionary ) );
    axImport.Initialize();
    spAxSrcMob->SetEssenceDescriptor( axImport );
    spAxSrcMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxSrcMob );
    
    return spAxSrcMob;
}

shared_ptr<AxMob> TestFileBuilder::AddTapeSource( const AxString& name, bool isNamed, aafRational_t ratNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxTapeDescriptor axTape( AxCreateInstance<IAAFTapeDescriptor>( axDictionary ) );
    axTape.Initialize();
    spAxSrcMob->SetEssenceDescriptor( axTape );
    spAxSrcMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxSrcMob );
    
    return spAxSrcMob;  
}

shared_ptr<AxMob> TestFileBuilder::AddFilmSource( const AxString& name, bool isNamed, aafRational_t ratNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxFilmDescriptor axFilm( AxCreateInstance<IAAFFilmDescriptor>( axDictionary ) );
    spAxSrcMob->SetEssenceDescriptor( axFilm );
    spAxSrcMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxSrcMob );
    
    return spAxSrcMob;  
}

/*
 * 
 * Create Components
 * 
 */

shared_ptr<AxComponent> TestFileBuilder::CreateTimecode( TrackType essenceType, const aafUID_t& uidNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxTimecode> axTimecode( new AxTimecode( AxCreateInstance<IAAFTimecode>( axDictionary ) ) );

    aafTimecode_t tc;
    tc.startFrame = 1;
    tc.drop = 1;
    tc.fps = 1;
    
    axTimecode->Initialize( 1, tc );
    AddDataDef( *axTimecode, essenceType );
    
    return axTimecode;
    
}

shared_ptr<AxComponent> TestFileBuilder::CreateEdgecode( TrackType essenceType, const aafUID_t& uidNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxEdgecode> axEdgecode( new AxEdgecode( AxCreateInstance<IAAFEdgecode>( axDictionary ) ) );

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
    
    axEdgecode->Initialize( 1, ec );
    AddDataDef( *axEdgecode, essenceType );

    return axEdgecode;    

}

shared_ptr<AxComponent> TestFileBuilder::CreateEOC( TrackType essenceType, const aafUID_t& uidNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSourceClip> axSrcClip( new AxSourceClip( AxCreateInstance<IAAFSourceClip>( axDictionary ) ) );

    aafSourceRef_t srcRef;
    srcRef.sourceID     = AxConstants::NULL_MOBID;
    srcRef.sourceSlotID = 0;
    srcRef.startTime    = 0;
    
    AddDataDef( *axSrcClip, essenceType );
    axSrcClip->SetLength( 1 );
    axSrcClip->SetSourceReference( srcRef );
    
    return axSrcClip;
}

shared_ptr<AxComponent> TestFileBuilder::CreateOOF( TrackType essenceType, const aafUID_t& uidNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSourceClip> axSrcClip( new AxSourceClip( AxCreateInstance<IAAFSourceClip>( axDictionary ) ) );

    aafSourceRef_t srcRef;
    srcRef.sourceID     = this->GenerateMobId();
    srcRef.sourceSlotID = 1;
    srcRef.startTime    = 1;
    
    AddDataDef( *axSrcClip, essenceType );
    axSrcClip->SetLength( 1 );
    axSrcClip->SetSourceReference( srcRef );
    
    return axSrcClip;
}

shared_ptr<AxComponent> TestFileBuilder::CreateSourceClip( TrackType essenceType, const aafUID_t& uidNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSourceClip> axSrcClip( new AxSourceClip( AxCreateInstance<IAAFSourceClip>( axDictionary ) ) );
    
    AddDataDef( *axSrcClip, essenceType );
    
    return axSrcClip;   
}

shared_ptr<AxComponent> TestFileBuilder::CreateTransition( TrackType essenceType, const aafUID_t& uidNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxTransition> axTransition( new AxTransition( AxCreateInstance<IAAFTransition>( axDictionary ) ) );
    
    AddDataDef( *axTransition, essenceType );
    axTransition->SetLength( 1 );
    axTransition->SetCutPoint( 1 );
    
    return axTransition;
}

shared_ptr<AxComponent> TestFileBuilder::CreateSequence( TrackType essenceType, const aafUID_t& uidNothing )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSequence> axSequence( new AxSequence( AxCreateInstance<IAAFSequence>( axDictionary ) ) );
    
    AddDataDef( *axSequence, essenceType );
    
    return axSequence;
}

shared_ptr<AxComponent> TestFileBuilder::CreateOperationGroup( TrackType essenceType, const aafUID_t& opDefId )
{
    
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxOperationGroup> axOpGroup( new AxOperationGroup( AxCreateInstance<IAAFOperationGroup>( axDictionary ) ) );
    
    AddDataDef( *axOpGroup, essenceType );
    axOpGroup->SetLength( 1 );

    axOpGroup->SetOperationDef( *(this->GetOperationDef( opDefId, axDictionary )) );
    
    return axOpGroup;
}

/*
 * 
 * Fill Components
 * 
 */

void TestFileBuilder::InitializeSourceClip( shared_ptr<AxSegment> axSegment, AxMob& axMob )
{
    
    shared_ptr<AxSourceClip> axSrcClip = dynamic_pointer_cast<AxSourceClip>( axSegment );
    
    aafSourceRef_t srcRef;
    srcRef.sourceID     = axMob.GetMobID();
    srcRef.sourceSlotID = 1;
    srcRef.startTime    = 0;    
    
    axSrcClip->SetLength( 1 );
    axSrcClip->SetSourceReference( srcRef );
  
}

void TestFileBuilder::AddToTransition( shared_ptr<AxObject> axObject, AxObject& axObj )
{
    shared_ptr<AxTransition> axTransition = dynamic_pointer_cast<AxTransition>( axObject );
    
    shared_ptr<AxOperationGroup> axOpGroup( new AxOperationGroup( AxQueryInterface<IAAFObject, IAAFOperationGroup>( axObj ) ) );
    
    axTransition->SetOperationGroup( *axOpGroup );
}

void TestFileBuilder::AddToSequence( shared_ptr<AxObject> axObject, AxObject& axObj )
{
    shared_ptr<AxSequence> axSequence = dynamic_pointer_cast<AxSequence>( axObject );
    
    shared_ptr<AxComponent> axComponent( new AxComponent( AxQueryInterface<IAAFObject, IAAFComponent>( axObj ) ) );
    
    axSequence->AppendComponent( *axComponent );
}

void TestFileBuilder::AddToOperationGroup( shared_ptr<AxSegment> axSegment, AxObject& axObj, int property )
{
    shared_ptr<AxOperationGroup> axOpGroup = dynamic_pointer_cast<AxOperationGroup>( axSegment );

    shared_ptr<AxSegment> axChildSegment( new AxSegment( AxQueryInterface<IAAFObject, IAAFSegment>( axObj ) ) );
    axChildSegment->SetDataDef( axOpGroup->GetDataDef() );
    
    axOpGroup->AppendInputSegment( *axChildSegment );    
}

/*
 * 
 * Mob Slots
 * 
 */

void TestFileBuilder::AttachTimelineSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered )
{

    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );
    
    AxTimelineMobSlot axTimelineMobSlot( AxCreateInstance<IAAFTimelineMobSlot>( axDictionary ) );
    
    axTimelineMobSlot.SetEditRate( editRate );
    axTimelineMobSlot.SetSegment( axSegment );
    axTimelineMobSlot.SetSlotID( parent.CountSlots() + 1 );
    axTimelineMobSlot.SetOrigin( 0 );
    if ( isNamed )
    {
        axTimelineMobSlot.SetName( name );
    }
    if ( isNumbered )
    {
        axTimelineMobSlot.SetPhysicalNum( physicalTrackNum );
    }

    parent.AppendSlot( axTimelineMobSlot );

}

void TestFileBuilder::AttachEventSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered )
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

void TestFileBuilder::AttachStaticSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered  )
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

void TestFileBuilder::AttachConstantRationalParameter( AxOperationGroup& axOpGroup, const aafUID_t& paramDefId, aafUInt32 numerator, aafUInt32 denominator )
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

/*
 * 
 * Helper Functions
 * 
 */

//Return a mob ID that is unique within this file.
const aafMobID_t TestFileBuilder::GenerateMobId()
{
    _mobCount++;
    aafMobID_t id;
    id.SMPTELabel[0] = _mobCount % 233;
    id.SMPTELabel[1] = _mobCount % 234;
    id.SMPTELabel[2] = _mobCount % 235;
    id.SMPTELabel[3] = _mobCount % 236;
    id.SMPTELabel[4] = _mobCount % 237;
    id.SMPTELabel[5] = _mobCount % 238;
    id.SMPTELabel[6] = _mobCount % 239;
    id.SMPTELabel[7] = _mobCount % 240;
    id.SMPTELabel[8] = _mobCount % 241;
    id.SMPTELabel[9] = _mobCount % 242;
    id.SMPTELabel[10] = _mobCount % 243;
    id.SMPTELabel[11] = _mobCount % 244;
    id.length = _mobCount % 245;
    id.instanceHigh = _mobCount % 246;
    id.instanceMid = _mobCount % 247;
    id.instanceLow = _mobCount % 248;
    id.material.Data1 = _mobCount;
    id.material.Data2 = _mobCount % 65535;
    id.material.Data3 = _mobCount % 65536;
    id.material.Data4[0] = _mobCount % 249;
    id.material.Data4[1] = _mobCount % 250;
    id.material.Data4[2] = _mobCount % 251;
    id.material.Data4[3] = _mobCount % 252;
    id.material.Data4[4] = _mobCount % 253;
    id.material.Data4[5] = _mobCount % 254;
    id.material.Data4[6] = _mobCount % 255;
    id.material.Data4[7] = _mobCount % 256;
    return id;
}

void TestFileBuilder::AddDataDef( AxComponent& axComponent, TrackType essenceType )
{
    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );    

    switch ( essenceType )
    {
        case AUDIO: axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Sound ) ); break;
        case PICTURE: axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) ); break;
        case TIMECODE: axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Timecode ) ); break;
        case EDGECODE:axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Edgecode ) ); break;
        default: axComponent.SetDataDef( axDictionary.LookupDataDef( kAAFDataDef_Unknown ) );
    }
}

shared_ptr<AxOperationDef> TestFileBuilder::GetOperationDef( const aafUID_t& opDefId, AxDictionary& axDictionary )
{
    if ( !axDictionary.isKnownOperationDef( opDefId ) )
    {
        
        AxOperationDef axOpDef( AxCreateInstance<IAAFOperationDef> ( axDictionary ) );
        
        if ( opDefId == kAAFOperationDef_VideoDissolve )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );
            axOpDef.Initialize( opDefId,
                        L"Video Dissolve",
                        L"Video Dissolve" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_SMPTEVideoWipe )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"SMPTE Video Wipe",
                        L"SMPTE Video Wipe" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoSpeedControl )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );
            axOpDef.Initialize( opDefId,
                        L"Video Speed Control",
                        L"Video Speed Control" );
            axOpDef.SetIsTimeWarp( true );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoRepeat )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Video Repeat",
                        L"Video Repeat" );
            axOpDef.SetIsTimeWarp( true );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_Flip )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Video Flip",
                        L"Video Flip" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_Flop )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Video Flop",
                        L"Video Flop" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_FlipFlop )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Video Flip Flop",
                        L"Video Flip Flop" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoPosition )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Video Position",
                        L"Video Position" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoCrop )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Video Crop",
                        L"Cideo Crop" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoScale )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Video Scale",
                        L"Video Scale" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoRotate )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Video Rotate",
                        L"Video Rotate" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoCornerPinning )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Video Corner Pinning",
                        L"Video Corner Pinning" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoAlphaWithinVideoKey )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Alpha With Video Key effect",
                        L"Alpha With Video Key effect" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoSeparateAlphaKey )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Separate Alpha Key effect",
                        L"Separate Alpha Key effect" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoLuminanceKey )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Luminance Key",
                        L"Luminance Key" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_VideoChromaKey )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Picture ) );       
            axOpDef.Initialize( opDefId,
                        L"Chroma Key",
                        L"Chroma Key" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_MonoAudioGain )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Sound ) );       
            axOpDef.Initialize( opDefId,
                        L"Mono Audio Gain",
                        L"Mono Audio Gain" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_MonoAudioPan )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Sound ) );       
            axOpDef.Initialize( opDefId,
                        L"Mono Audio Pan",
                        L"Mono Audio Pan" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetBypass( 1 );
            axOpDef.SetNumberInputs( 1 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_MonoAudioDissolve )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Sound ) );       
            axOpDef.Initialize( kAAFEffectMonoAudioDissolve,
                        L"Mono Audio Dissolve",
                        L"Mono Audio Dissolve" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        else if ( opDefId == kAAFOperationDef_TwoParameterMonoAudioDissolve )
        {
            AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Sound ) );       
            axOpDef.Initialize( opDefId,
                        L"Two-Parameter Mono Audio Dissolve",
                        L"Two-Parameter Mono Audio Dissolve" );
            axOpDef.SetIsTimeWarp( false );
            axOpDef.SetNumberInputs( 2 );
            axOpDef.SetDataDef( axDataDef );
        }
        
        axDictionary.RegisterOperationDef( axOpDef );
    }
    shared_ptr<AxOperationDef> spOpDef( new AxOperationDef( axDictionary.LookupOperationDef( opDefId ) ) );

    return spOpDef;
}

shared_ptr<AxParameterDef> TestFileBuilder::GetParameterDef( const aafUID_t& paramDefId, AxDictionary& axDictionary )
{
    if ( !axDictionary.isKnownParameterDef( paramDefId ) )
    {
        
        AxParameterDef axParamDef( AxCreateInstance<IAAFParameterDef> ( axDictionary ) );
        
        if ( paramDefId == kAAFParameterDef_SpeedRatio )
        {
            AxTypeDef axTypeDef( axDictionary.LookupTypeDef( kAAFTypeID_Rational ) );
            axParamDef.Initialize(
                paramDefId,
                L"Speed Ratio",
                L"Speed Ratio",
                axTypeDef
            );
        }
        
        axDictionary.RegisterParameterDef( axParamDef );
    }
    shared_ptr<AxParameterDef> spParamDef( new AxParameterDef( axDictionary.LookupParameterDef( paramDefId ) ) );

    return spParamDef;
}

} // end of namespace diskstream
