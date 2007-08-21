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

//Test/Result files
#include <LowLevelTestResult.h>
#include <Test.h>

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

LowLevelTestResult::LowLevelTestResult( const shared_ptr<const Test> associatedTest )
  : TestResult(),
    _spAssociatedTest( associatedTest )
{}

LowLevelTestResult::LowLevelTestResult( const shared_ptr<const Test> associatedTest,
                                        const wstring& name, const wstring& desc,
                                        const wstring& explain )
  : TestResult( name, desc, explain ),
    _spAssociatedTest( associatedTest )
{}

LowLevelTestResult::~LowLevelTestResult()
{}

const shared_ptr<const Test> LowLevelTestResult::GetAssociatedTest() const
{
  return _spAssociatedTest;
}

} // end of namespace diskstream
