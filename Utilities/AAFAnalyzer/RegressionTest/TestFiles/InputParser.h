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

#ifndef __INPUTPARSER_H_
#define __INPUTPARSER_H_

//Regression Test Files files
#include "TestFileBuilder.h"
#include "SlotInfo.h"

//Ax files
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <stack>
#include <map>

class AxObject;
class AxMob;
class AxComponent;
class AxSegment;
class AxSourceReference;

namespace aafanalyzer {

using namespace std;
using namespace boost;

class InputParser
{
public:

    InputParser( const char* outFile );
    ~InputParser();

    static void __StartElement(void* userData, const char* name, const char** atts);
    static void __EndElement(void* userData, const char* name);
    static void __EndData(void* userData, const char* s, int len);

    void StartElement(const AxString& name, const char** attribs);
    void EndElement(const AxString& name);
    void EndData(const AxString& contents);

    void ParseXML( const char* filename ) const;

 private:

    typedef shared_ptr<AxMob>(TestFileBuilder::*ptrToAddFunction)( const AxString& name, bool isNamed, aafRational_t rationalParam, aafUInt32 optIntParam, bool hasOptIntParam );
    typedef shared_ptr<AxComponent>(TestFileBuilder::*ptrToCreateFunction)( TestFileBuilder::TrackType essenceType, const AxString& strParam, aafLength_t length, bool hasLength, int intParam, bool hasIntParam );
    typedef void(TestFileBuilder::*ptrToAttachSlotFunction)( AxMob& parent, AxSegment& axSegment, aafRational_t editRate, const AxString& name, bool isNamed, int physicalTrackNum, bool isNumbered, int markedIn, bool isMarkedIn, int markedOut, bool isMarkedOut, int orgin );
    typedef void(TestFileBuilder::*ptrToAttachParameterFunction)( AxOperationGroup& axOpGroup, const aafUID_t& paramDefId, aafUInt32 intParam1, aafUInt32 intParam2, const aafUID_t& interpolationDefId );
    typedef void(TestFileBuilder::*ptrToCreateDefFunction)( const AxString& name, const AxString& description );
    typedef void(TestFileBuilder::*ptrToAddAnnotationFunction)( shared_ptr<AxComponent> axComponent, const AxString& keyName, const AxString& value );

    //Components can be divided into 4 distinct classes based upon the types of
    //objects that may be attached.  Each class has its own signature for attaching:

    //Type A (No objects can be attached)
    //Filler, Event, GPITrigger, Timecode, TimecodeStream, TimecodeStream12M, Edgecode, ScopeReference
    //NOTE: No fill function is needed because no objects are attached.
    //NOTE: Event and TimecodeStream are abstract classes, no special
    //      if functions are needed to create descendant classes, they will be
    //      no different from a normal create function.  Fill functions for
    //      these descendant classes will be classified as needed, but, in many
    //      cases will be Type A.

    //Type B (One Mob Referenced)
    //SourceReference, SourceClip
    typedef void(TestFileBuilder::*ptrToFillFunctionB) ( shared_ptr<AxSourceReference> parent, AxMob& child, const AxString& source );
    //NOTE: SourceReference is an abstract class.  Although the general create
    //      function will be sufficient to create new descendant classes, a
    //      specialized fill function will be needed.  In many cases, that
    //      fill function will be classified as Type B, therefore, a map is
    //      still being used although at the present it contains only one
    //      element.

    //Type C (Interchange Objects can be attached (one or many in an array) )
    //Transition, Sequence, CommentMarker, DescriptiveMarker, Pulldown
    typedef void(TestFileBuilder::*ptrToFillFunctionC) ( shared_ptr<AxComponent> parent, AxComponent& child );

    //Type D (Multiple Segments assigned to different properties)
    //OperationGroup, Selector, EssenceGroups
    typedef void(TestFileBuilder::*ptrToFillFunctionD) ( shared_ptr<AxSegment> parent, AxSegment& child, int property );

    typedef pair<AxString, shared_ptr<AxComponent> > ComponentPair;
    typedef pair<AxString, AxString> TypeDPair;

    stack<shared_ptr<AxMob> > _mobStack;
    stack<ComponentPair> _componentStack;
    stack<SlotInfo> _slotStack;

    map<AxString, ptrToCreateDefFunction> _definitionMap;
    map<AxString, ptrToAddFunction> _materialTypeMap;
    map<AxString, ptrToCreateFunction> _createSegmentMap;
    map<AxString, ptrToAttachSlotFunction> _attachSlotMap;

    map<AxString, ptrToFillFunctionB> _fillSegmentMapB;
    map<AxString, ptrToFillFunctionC> _fillSegmentMapC;
    map<AxString, ptrToFillFunctionD> _fillSegmentMapD;

    map<AxString, ptrToAttachParameterFunction> _attachParameterMap;
    map<AxString, ptrToAddAnnotationFunction> _annotationMap;

    map<AxString, TestFileBuilder::TrackType> _essenceMap;
    map<AxString, AxString> _optRationalParam;
    map<AxString, AxString> _effectMap;
    map<AxString, aafUID_t> _parameterTypeMap;
    map<AxString, aafUID_t> _interpolationTypeMap;
    map<TypeDPair, int> _typeDPropertyMap;
    map<AxString, aafUID_t> _operationalPatternMap;
    map<AxString, AxString> _annotationIds;

    TestFileBuilder _testFile;

    const AxString GetStringAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const AxString& default_val ) const;
    const int GetIntAtribValue( const AxString& attrib, const char** attribs, const unsigned int size, const int default_val ) const;
    const int GetBoolAtribValue( const AxString& attrib, const char** attribs, const unsigned int size, const int default_val ) const;
	const SlotInfo::OptionalStringAttrib GetOptionalStringAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const AxString& default_val ) const;
    const SlotInfo::OptionalIntAttrib GetOptionalIntAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const int default_val ) const;

	//These private functions are needed to compile under VC++.
	void SetupDefinitionMap();
	void SetupMaterialTypeMap();
	void SetupCreateSegmentMap();
	void SetupFillSegmentMap();
	void SetupAttachSlotMap();
	void SetupAttachParameterMap();
	void SetupAnnotationMap();
	void SetupAnnotationIDMap();
	void SetupOptionalRationalParamMap();
	void SetupEffectMap();
	void SetupParameterTypeMap();
	void SetupInterpolationTypeMap();
	void SetupOperationalPatternMap();

};

} // end of namespace diskstream

#endif /*__INPUTPARSER_H_*/
