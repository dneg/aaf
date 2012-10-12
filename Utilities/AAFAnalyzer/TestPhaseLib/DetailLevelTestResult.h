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

#ifndef __DETAILLEVELTESTRESULT_h__
#define __DETAILLEVELTESTRESULT_h__

//Test/Result files
#include "LowLevelTestResult.h"

//Base files
#include <Node.h>

namespace aafanalyzer {

using namespace std;

// A detail level test result stores a result for a single requirement
// (All others store a value that is aggregate result determine by
// take the worst sub result at consolidation time.) The requirement
// identfied by reqId must be present.  Note, while
// DetailLevelTestResult inherits pass/warn/fail result maps from the
// TestReult parent class only a single result is in those maps
// by. This is an important invariant in the DetailLevelTestResult
// implemenation.  As such, it cannot have chld results (i.e. is
// always a leaf node in the result tree).
//
// reqId must identify one of the requirements registered against the
// associatedTest.
//
// JPT REVIEW - This should be renamed to SingleTestResult so that its
// name more accurately represents it role.

class DetailLevelTestResult : public LowLevelTestResult
{
 public:

  // Result name and description are initialized to those of the
  // requirment.  Call must identify the requirment, provide a result,
  // and an explanation.
  // Pass a null spNode if this result cannot be associated with a
  // single node.
  DetailLevelTestResult( const boost::shared_ptr<const Test> associatedTest,
                         const wstring& explain,
			 const wstring& reqId,
                         Result result,
			 boost::shared_ptr<Node> _spNode );

  virtual ~DetailLevelTestResult();

  // Sets the result for the requirement identified by reqId. reqId
  // must be the same value passed to the constructor. This condition
  // is asserted. (This may seem redundant. It is done because passing
  // this value forces test implementors to be certain the correct
  // result is being updated and ensures complete clarity in the
  // code.)
  void SetResult( const wstring& reqId, Result result );

  virtual const enum ResultLevel GetResultType() const;

  // Returns ~0 if this result is not associated with any particular
  // node.
  boost::shared_ptr<Node> GetAssociatedNode() const;

  // Get the require id against which this result is being reported.
  const wstring& GetId() const;

 private:

  // prohibited
  DetailLevelTestResult( const DetailLevelTestResult& );
  DetailLevelTestResult& operator=( const DetailLevelTestResult& );

  boost::shared_ptr<const Requirement> GetRequirement() const;

  // This detail result is for the following requirement.
  // This is here primarily to aid debugging. We add test results
  // on at a time against requirements, but they are accumulated
  // in sets in the TestResult base class. This is less obtuse.
  const wstring _reqId;

  // The node associated with the AAF object that caused the error.
  boost::shared_ptr<Node> _spNode;
};

} // end of namespace diskstream

#endif/*__DETAILLEVELTESTRESULT_h__*/
