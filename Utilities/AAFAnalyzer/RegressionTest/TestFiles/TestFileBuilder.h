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

#ifndef TESTFILE_H_
#define TESTFILE_H_

//Ax files
#include <AxFile.h>
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

class AxObject;
class AxMob;
class AxComponent;
class AxSegment;
class AxOperationGroup;
class AxDictionary;
class AxOperationDef;
class AxParameterDef;
class AxConstantValue;

namespace aafanalyzer {

using namespace boost;

class TestFileBuilder
{
    public:
    
        enum TrackType { NONE, AUDIO, PICTURE, TIMECODE, EDGECODE, AUXILIARY };
    
        TestFileBuilder( const char* outFile );
        ~TestFileBuilder();

        //Mobs:
        //Composition:
        shared_ptr<AxMob> AddTopLevel( const AxString& name, bool isNamed, aafRational_t ratNothing );
        shared_ptr<AxMob> AddLowerLevel( const AxString& name, bool isNamed, aafRational_t ratNothing );
        shared_ptr<AxMob> AddSubClip( const AxString& name, bool isNamed, aafRational_t ratNothing );
        shared_ptr<AxMob> AddAdjustedClip( const AxString& name, bool isNamed, aafRational_t ratNothing );

        //Master:
        shared_ptr<AxMob> AddTemplateClip( const AxString& name, bool isNamed, aafRational_t ratNothing );
        shared_ptr<AxMob> AddClip( const AxString& name, bool isNamed, aafRational_t ratNothing );

        //Source:
        shared_ptr<AxMob> AddFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate );
        shared_ptr<AxMob> AddRecordingSource( const AxString& name, bool isNamed, aafRational_t ratNothing );
        shared_ptr<AxMob> AddImportSource( const AxString& name, bool isNamed, aafRational_t ratNothing );
        shared_ptr<AxMob> AddTapeSource( const AxString& name, bool isNamed, aafRational_t ratNothing );
        shared_ptr<AxMob> AddFilmSource( const AxString& name, bool isNamed, aafRational_t ratNothing );
        shared_ptr<AxMob> AddAuxiliarySource( const AxString& name, bool isNamed, aafRational_t ratNothing );

        //Create Segments:
        //Type A:
        shared_ptr<AxComponent> CreateTimecode( TrackType essenceType, const aafUID_t& uidNothing );
        shared_ptr<AxComponent> CreateEdgecode( TrackType essenceType, const aafUID_t& uidNothing );
        shared_ptr<AxComponent> CreateTimecodeStream12M( TrackType essenceType, const aafUID_t& uidNothing );
        shared_ptr<AxComponent> CreateEOC( TrackType essenceType, const aafUID_t& uidNothing );
        shared_ptr<AxComponent> CreateOOF( TrackType essenceType, const aafUID_t& uidNothing );

        //Type B:
        shared_ptr<AxComponent> CreateSourceClip( TrackType essenceType, const aafUID_t& uidNothing );

        //Type C:
        shared_ptr<AxComponent> CreateTransition( TrackType essenceType, const aafUID_t& uidNothing );
        shared_ptr<AxComponent> CreateSequence( TrackType essenceType, const aafUID_t& uidNothing );
        
        //Type D:
        shared_ptr<AxComponent> CreateOperationGroup( TrackType essenceType, const aafUID_t& opDefId );
       
        //Fill Components:
        //Type B:
        void InitializeSourceClip( shared_ptr<AxSegment> axSegment, AxMob& axMob );
        
        //Type C:
        void AddToTransition( shared_ptr<AxObject> axObject, AxObject& axObj );
        void AddToSequence( shared_ptr<AxObject> axObject, AxObject& axObj );

        //Type D:
        void AddToOperationGroup( shared_ptr<AxSegment> axSegment, AxObject& axObj, int property );

        //Mob Slots:
        void AttachTimelineSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered );
        void AttachEventSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered );
        void AttachStaticSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered );
        
        //Parameters:
        void AttachConstantRationalParameter( AxOperationGroup& axOpGroup, const aafUID_t& paramDefId, aafUInt32 numerator, aafUInt32 denominator );
        

    private:
    
        AxFile _axFile;
        aafUInt32 _mobCount;
        
        const aafMobID_t GenerateMobId();
        void AddDataDef( AxComponent& axComponent, TrackType essenceType );
        shared_ptr<AxOperationDef> GetOperationDef( const aafUID_t& opDefId, AxDictionary& axDictionary );
        shared_ptr<AxParameterDef> GetParameterDef( const aafUID_t& paramDefId, AxDictionary& axDictionary );

};

} // end of namespace diskstream

#endif /*TESTFILE_H_*/
