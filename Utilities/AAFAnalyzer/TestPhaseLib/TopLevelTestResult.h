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

#ifndef __TOPLEVELTESTRESULT_h__
#define __TOPLEVELTESTRESULT_h__

//Test/Result files
#include "HighLevelTestResult.h"

namespace aafanalyzer {

using namespace std;
using namespace boost;

class TestPhaseLevelTestResult;

class TopLevelTestResult : public HighLevelTestResult
{
 public:

  TopLevelTestResult();

  TopLevelTestResult( const wstring& name, const wstring& desc,
                      const wstring& explain );

  virtual ~TopLevelTestResult();

  void AppendSubtestResult( shared_ptr<TestPhaseLevelTestResult> subtestResult );

  const enum ResultLevel GetResultType() const;

 private:
  // prohibited
  TopLevelTestResult( const TopLevelTestResult& );
  TopLevelTestResult& operator=( const TopLevelTestResult& );
};

} // end of namespace diskstream

#endif/*__TOPLEVELTESTRESULT_h__*/
