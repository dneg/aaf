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

//AAF files
#include <AAFResult.h>
#include <AAFExtEnum.h>
#include <AAFDataDefs.h>
#include <AAFCodecDefs.h>
#include <AAFContainerDefs.h>

//STL files
#include <iostream>
#include <sstream>

using namespace std;
using namespace boost;

TestFileBuilder::TestFileBuilder( const char* outFile) : _mobCount( 0 )
{

    AxInit initObj;
    wostringstream ssOutFile;
    ssOutFile << outFile;
    AxString filename( ssOutFile.str().c_str() );
    bool openExisting = false;
        
    try {
        if ( AxFile::isAAFFile( filename ) ) {
            wcout << L"Existing file will be modified: ";
            openExisting = true;
        }
    }
    catch( const AxExHResult& ex ) {
        //  AAFRESULT_FILE_NOT_FOUND is okay.  The file does
        // not exist.
        if ( ex.getHResult() != AAFRESULT_FILE_NOT_FOUND ) {
            throw;
        }
    }

    if ( openExisting ) {
        wcout << L"Open existing AAF File" << endl;
        _axFile.OpenExistingModify( filename );
    }
    else {
        wcout << L"Create new AAF File"<< endl;
        _axFile.OpenNewModify( filename );
    }
        
}

TestFileBuilder::~TestFileBuilder()
{

    _axFile.Save();
    _axFile.Close();
    
}

shared_ptr<AxCompositionMob> TestFileBuilder::AddTopLevel( const AxString& name, bool isNamed )
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

shared_ptr<AxCompositionMob> TestFileBuilder::AddLowerLevel( const AxString& name, bool isNamed )
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

shared_ptr<AxCompositionMob> TestFileBuilder::AddSubClip( const AxString& name, bool isNamed )
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

shared_ptr<AxCompositionMob> TestFileBuilder::AddAdjustedClip( const AxString& name, bool isNamed )
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

shared_ptr<AxMasterMob> TestFileBuilder::AddTemplateClip( const AxString& name, bool isNamed )
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

shared_ptr<AxMasterMob> TestFileBuilder::AddClip( const AxString& name, bool isNamed )
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

shared_ptr<AxSourceMob> TestFileBuilder::AddFileSource( const AxString& name, bool isNamed )
{

    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    shared_ptr<AxSourceMob> spAxSrcMob( new AxSourceMob( AxCreateInstance<IAAFSourceMob>( axDictionary ) ) );
    if ( isNamed )
    {
        spAxSrcMob->SetName( name );
    }

    AxCDCIDescriptor axFileDes( AxCreateInstance<IAAFCDCIDescriptor>( axDictionary ) );
    spAxSrcMob->SetEssenceDescriptor( axFileDes );
    spAxSrcMob->SetMobID( this->GenerateMobId() );

    axHeader.AddMob( *spAxSrcMob );
    
    return spAxSrcMob;  

}

shared_ptr<AxSourceMob> TestFileBuilder::AddRecordingSource( const AxString& name, bool isNamed )
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

shared_ptr<AxSourceMob> TestFileBuilder::AddImportSource( const AxString& name, bool isNamed )
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

shared_ptr<AxSourceMob> TestFileBuilder::AddTapeSource( const AxString& name, bool isNamed )
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

shared_ptr<AxSourceMob> TestFileBuilder::AddFilmSource( const AxString& name, bool isNamed )
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

void TestFileBuilder::Attach( AxMob& parent, AxMob& child )
{

    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );
    
    AxSourceClip axSrcClip( AxCreateInstance<IAAFSourceClip>( axDictionary ) );
    AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Auxiliary ) );
    aafSourceRef_t srcRef;
    srcRef.sourceID     = child.GetMobID();
    srcRef.sourceSlotID = 1;
    srcRef.startTime    = 0;    
    axSrcClip.Initialize( axDataDef, 1, srcRef);

    aafRational_t editRate = {1, 1};
    AxTimelineMobSlot axSlot( parent.AppendNewTimelineSlot(editRate, axSrcClip, parent.CountSlots() + 1, L"Unnamed", 0) );

}

void TestFileBuilder::AttachEOC( AxMob& parent )
{

    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxSourceClip axSrcClip( AxCreateInstance<IAAFSourceClip>( axDictionary ) );
    AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Auxiliary ) );
    aafSourceRef_t srcRef;
    srcRef.sourceID     = AxConstants::NULL_MOBID;
    srcRef.sourceSlotID = 0;
    srcRef.startTime    = 0;
    axSrcClip.Initialize( axDataDef, 0, srcRef);
    
    aafRational_t editRate = {1, 1};
    AxTimelineMobSlot axSlot( parent.AppendNewTimelineSlot(editRate, axSrcClip, parent.CountSlots() + 1, L"End of Chain", 0) );
    
}

void TestFileBuilder::AttachOOF( AxMob& parent )
{

    AxHeader axHeader( _axFile.getHeader() );
    AxDictionary axDictionary( axHeader.GetDictionary() );

    AxSourceClip axSrcClip( AxCreateInstance<IAAFSourceClip>( axDictionary ) );
    AxDataDef axDataDef( axDictionary.LookupDataDef( kAAFDataDef_Auxiliary ) );
    aafSourceRef_t srcRef;

    srcRef.sourceID     = this->GenerateMobId();
    srcRef.sourceSlotID = 1;
    srcRef.startTime    = 1;
    axSrcClip.Initialize( axDataDef, 1, srcRef);
    
    aafRational_t editRate = {1, 1};
    AxTimelineMobSlot axSlot( parent.AppendNewTimelineSlot(editRate, axSrcClip, parent.CountSlots() + 1, L"End of Chain", 0) );

}

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
