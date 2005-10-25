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

//Ax files
#include <AxTypes.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <stack>
#include <map>

class AxMob;

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
 
    stack<shared_ptr<AxMob> > _mobStack;
    map<AxString, ptrToAddFunction> _test;
    TestFileBuilder _testFile;

};

} // end of namespace diskstream

#endif /*__INPUTPARSER_H_*/
