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

#ifndef __EPTrackContentsTest_h_
#define __EPTrackContentsTest_h_

//Test/Result files
#include <Test.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

class TestLevelTestResult;

using namespace boost;

class EPTrackContentsTest : public Test
{
 public:

  EPTrackContentsTest( wostream& log,
                        shared_ptr<const TestGraph> spGraph );
  virtual ~EPTrackContentsTest();

  virtual shared_ptr<TestLevelTestResult> Execute();
  virtual AxString GetName() const;
  virtual AxString GetDescription() const;
  static const TestInfo GetTestInfo();

 private:

  // prohibited
  EPTrackContentsTest();
  EPTrackContentsTest( const EPTrackContentsTest& );
  EPTrackContentsTest& operator=( const EPTrackContentsTest& );

};

} // end of namespace diskstream

#endif /* __EPTrackContentsTest_h_*/
