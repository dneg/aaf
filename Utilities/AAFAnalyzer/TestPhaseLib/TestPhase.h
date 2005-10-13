//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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

#ifndef __TESTPHASE_h__
#define __TESTPHASE_h__

//Boost files
#include <boost/shared_ptr.hpp>

//STL file
#include <string>
#include <iostream>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class TestPhaseLevelTestResult;

class TestPhase
{
 public:
  TestPhase(wostream& os);
  virtual ~TestPhase();

  virtual wstring GetDescription() const;
  virtual wstring GetName() const;
  virtual shared_ptr<TestPhaseLevelTestResult> Execute() = 0; 
  wostream& GetOutStream() const; 

 private:
  wostream& _os;

  // prohibited
  TestPhase();
  TestPhase( const TestPhase& );
  TestPhase& operator=( const TestPhase& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
