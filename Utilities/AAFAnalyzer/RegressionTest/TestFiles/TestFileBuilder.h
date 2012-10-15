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

#ifndef TESTFILE_H_
#define TESTFILE_H_

//Ax files
#include <AxFile.h>
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <map>

class AxObject;
class AxMob;
class AxComponent;
class AxSegment;
class AxOperationGroup;
class AxDictionary;
class AxOperationDef;
class AxParameterDef;
class AxConstantValue;
class AxDescriptiveMarker;
class AxSourceReference;
class AxInterpolationDef;

namespace aafanalyzer {

using namespace std;

class TestFileBuilder
{
    public:

        enum TrackType { NONE, AUDIO, PICTURE, TIMECODE, EDGECODE, AUXILIARY };
        static const int INPUT_SEGMENT = 1;
        static const int RENDERING = 2;

        TestFileBuilder( const char* outFile );
        ~TestFileBuilder();

        //Definition Objects:
        void CreateKLVDataDefinition( const AxString& name, const AxString& description );
        void CreateTaggedValueDefinition( const AxString& name, const AxString& description );
        void CreateOperationDefinition( const AxString& name, const AxString& description );

        //Mobs:
        //Composition:
        boost::shared_ptr<AxMob> AddTopLevel( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddLowerLevel( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddSubClip( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddAdjustedClip( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );

        //Master:
        boost::shared_ptr<AxMob> AddTemplateClip( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddClip( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );

        //Source:
        boost::shared_ptr<AxMob> AddFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddMonoAudioFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddMultiChannelAudioFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddRGBAFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 alphaVal, bool hasAlphaType );
        boost::shared_ptr<AxMob> AddCDCIFileSource( const AxString& name, bool isNamed, aafRational_t sampleRate, aafUInt32 alphaVal, bool hasAlphaType );
        boost::shared_ptr<AxMob> AddRecordingSource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddImportSource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddTapeSource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddFilmSource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );
        boost::shared_ptr<AxMob> AddAuxiliarySource( const AxString& name, bool isNamed, aafRational_t ratNothing, aafUInt32 intNothing, bool boolNothing );

        //Create Segments:
        //Type A:
        boost::shared_ptr<AxComponent> CreateTimecode( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing );
        boost::shared_ptr<AxComponent> CreateEdgecode( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing );
        boost::shared_ptr<AxComponent> CreateTimecodeStream12M( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing );
        boost::shared_ptr<AxComponent> CreateEOC( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing );
        boost::shared_ptr<AxComponent> CreateOOF( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int startTime, bool hasStartTime );

        //Type B:
        boost::shared_ptr<AxComponent> CreateSourceClip( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int startTime, bool hasStartTime );

        //Type C:
        boost::shared_ptr<AxComponent> CreateTransition( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing );
        boost::shared_ptr<AxComponent> CreateSequence( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int intNothing, bool boolNothing );
        boost::shared_ptr<AxComponent> CreateCommentMarker( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int position, bool hasPosition );
        boost::shared_ptr<AxComponent> CreateDescriptiveMarker( TrackType essenceType, const AxString& strNothing, aafLength_t length, bool hasLength, int position, bool hasPosition );

        //Type D:
        boost::shared_ptr<AxComponent> CreateOperationGroup( TrackType essenceType, const AxString& opDef, aafLength_t length, bool hasLength, int intNothing, bool boolNothing );

        //Fill Components:
        //Type B:
        void InitializeSourceClip( boost::shared_ptr<AxSourceReference> parent, AxMob& child, const AxString& source );

        //Type C:
        void AddToTransition( boost::shared_ptr<AxComponent> parent, AxComponent& child );
        void AddToSequence( boost::shared_ptr<AxComponent> parent, AxComponent& child );
        void AddToCommentMarker( boost::shared_ptr<AxComponent> parent, AxComponent& child );

        //Type D:
        void AddToOperationGroup( boost::shared_ptr<AxSegment> parent, AxSegment& child, int property );

        //Annotation:
        void AddComment( boost::shared_ptr<AxComponent> axComponent, const AxString& name, const AxString& value );
        void AddKLVData( boost::shared_ptr<AxComponent> axComponent, const AxString& keyName, const AxString& value );

        //Mob Slots:
        void AttachTimelineSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered, int markedIn, bool isMarkedIn, int markedOut, bool isMarkedOut, int userPos, bool isUserPos, int origin );
        void AttachEventSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered, int intNothing1, bool boolNothing1, int intNothing2, bool boolNothing2, int intNothing3, bool boolNothing3, int intNothing4 );
        void AttachStaticSlot( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered, int intNothing1, bool boolNothing1, int intNothing2, bool boolNothing2, int intNothing3, bool boolNothing3, int intNothing4 );

        //Parameters:
        void AttachConstantRationalParameter( AxOperationGroup& axOpGroup, const aafUID_t& paramDefId, aafUInt32 numerator, aafUInt32 denominator, const aafUID_t& uidNothing );
        void AttachVaryingRationalParameter( AxOperationGroup& axOpGroup, const aafUID_t& paramDefId, aafUInt32 numerator, aafUInt32 denominator, const aafUID_t& interpolationDefId );

        //Interchange Objects:
        void AttachDescriptiveFramework( boost::shared_ptr<AxDescriptiveMarker> axMarker );

        //Other:
        void SetOperationalPattern( aafUID_t pattern );
        void UseLegacyEffectDefinitions();

    private:

        AxFile _axFile;
        aafUInt32 _mobCount;
        map<AxString, aafUID_t> _namedAUIDs;
        bool _useLegacyEffectDefinitions;

        const aafMobID_t GenerateMobId(AxString mobName);
        const aafUID_t GenerateAUID();
        void AddDataDef( AxComponent& axComponent, TrackType essenceType );
        boost::shared_ptr<AxOperationDef> GetOperationDef( const AxString& opDef, AxDictionary& axDictionary );
        boost::shared_ptr<AxParameterDef> GetParameterDef( const aafUID_t& paramDefId, AxDictionary& axDictionary );
        boost::shared_ptr<AxInterpolationDef> GetInterpolationDef( const aafUID_t& interpolationDefId, AxDictionary& axDictionary );
		map<AxString, aafMobID_t> mobIdMap;
};

} // end of namespace diskstream

#endif /*TESTFILE_H_*/
