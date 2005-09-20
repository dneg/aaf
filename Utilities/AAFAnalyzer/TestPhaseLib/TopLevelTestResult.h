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

#ifndef __TOPLEVELTESTRESULT_h__
#define __TOPLEVELTESTRESULT_h__

//Project files
#include "HighLevelTestResult.h"
#include "TestPhaseLevelTestResult.h"

namespace aafanalyzer {

using namespace std;

class TopLevelTestResult : public HighLevelTestResult
{
 public:

  TopLevelTestResult();
  TopLevelTestResult( const AxString& name, const AxString& desc,
                      const AxString& explain, const AxString& docRef,
                      Result defaultResult );
  ~TopLevelTestResult();

  void AppendSubtestResult( const boost::shared_ptr<const TestPhaseLevelTestResult>& subtestResult );
  const enum ResultLevel GetResultType() const;

  // prohibited
  
};

} // end of namespace diskstream

#endif/*__TOPLEVELTESTRESULT_h__*/
