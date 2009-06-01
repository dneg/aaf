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

#ifndef __LOADPHASE_h__
#define __LOADPHASE_h__

//Base Test files (included so the application only needs to include the Test
//Phase and not individual tests in order to register tests).
#include <FileLoad.h>
#include <RefResolver.h>
#include <AcyclicAnalysis.h>
#include <CompMobDependency.h>

//Test/Result files
#include <TestPhase.h>

//Base files
#include <Node.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class AAFGraphInfo;

class LoadPhase : public TestPhase
{
 public:
  LoadPhase(wostream& os, const basic_string<wchar_t> AAFFile);
  virtual ~LoadPhase();

  shared_ptr<const AAFGraphInfo> GetTestGraphInfo();
  virtual shared_ptr<TestPhaseLevelTestResult> Execute();  

  // Get the (unreferenced) root nodes identified by the dependendency
  // analysis that is executed at the start of this test phase.
  vector<shared_ptr<Node> > GetRoots() const;

  // The same set of roots, but returned as as ComMobNodeVector (to
  // pass on to the EPMobDepPhase).  Returns and empty vector
  // if IsCyclic() returns true;
  CompMobDependency::CompMobNodeVectorSP GetCompMobRoots();

  // True if the graph is determined to by cyclic after resolving
  // references. The dependency analysis is not executed if this
  // is true.
  bool IsCyclic() const;

 private:

  // prohibited
  LoadPhase();
  LoadPhase( const LoadPhase& );
  LoadPhase& operator=( const LoadPhase& );

  shared_ptr<const AAFGraphInfo> _spGraphInfo;
  const basic_string<wchar_t> _FileName;

  CompMobDependency::CompMobNodeVectorSP _spRootsVector;
  bool _isCyclic;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
