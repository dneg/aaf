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
#include "SegmentInfo.h"

//Ax files
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <stack>
#include <map>

class AxMob;
class AxSegment;

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
 
    typedef shared_ptr<AxMob>(TestFileBuilder::*ptrToAddFunction)( const AxString& name, bool isNamed );
    typedef shared_ptr<AxSegment>(TestFileBuilder::*ptrToCreateFunction)( AxMob& axMob, TestFileBuilder::TrackType essenceType );
    typedef void(TestFileBuilder::*ptrToFillFunction) ( shared_ptr<AxSegment> axSegment, AxMob& axMob );
    typedef void(TestFileBuilder::*ptrToAttachSlotFunction)( AxMob& parent, AxSegment& axSegment, aafRational_t editRate );
    typedef pair<const AxString, const bool> OptionalStringAttrib;
    typedef pair<const int, const bool> OptionalIntAttrib;

    stack<shared_ptr<AxMob> > _mobStack;
    stack<SegmentInfo> _segmentStack;
    map<AxString, ptrToAddFunction> _materialTypeMap;
    map<AxString, ptrToCreateFunction> _createSegmentMap;
    map<AxString, ptrToFillFunction> _fillSegmentMap;
    map<AxString, ptrToAttachSlotFunction> _attachSlotMap;
    map<AxString, TestFileBuilder::TrackType> _essenceMap;
    TestFileBuilder _testFile;
    
    const AxString GetStringAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const AxString& default_val ) const;
    const int GetIntAtribValue( const AxString& attrib, const char** attribs, const unsigned int size, const int default_val ) const;
    const OptionalStringAttrib GetOptionalStringAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const AxString& default_val ) const;
    const OptionalIntAttrib GetOptionalIntAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const int default_val ) const;

};

} // end of namespace diskstream

#endif /*__INPUTPARSER_H_*/
