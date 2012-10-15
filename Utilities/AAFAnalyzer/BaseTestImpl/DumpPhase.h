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

#ifndef __DUMPPHASE_h__
#define __DUMPPHASE_h__

//Base Test files (included so the application only needs to include the Test
//Phase and not individual tests in order to register tests).
#include "FileDumper.h"

//Test/Result files
#include <TestPhase.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {
    
using namespace std;

class TestGraph;
class Node;

class DumpPhase : public TestPhase
{
 public:

  // Use the root node contained by the TestGraph. Only follow edges
  // that model contaiment. (i.e. for AAF file this dumps the header
  // and all contained objects)
  DumpPhase(wostream& os, boost::shared_ptr<const TestGraph> spGraph);

  // Use alternate root node.
  // This is used to dump the top level composition in an aaf file. It
  // will configure the visitor to follow references such that the
  // entire composition graph, including resolved mob references, is
  // dumped.
  DumpPhase(wostream& os, boost::shared_ptr<const TestGraph> spGraph, boost::shared_ptr<Node> spRoot);

  ~DumpPhase();

  boost::shared_ptr<const TestGraph> GetTestGraph();
  boost::shared_ptr<TestPhaseLevelTestResult> Execute();

 private:
  // prohibited
  DumpPhase();
  DumpPhase( const DumpPhase& );
  DumpPhase& operator=( const DumpPhase& );

  boost::shared_ptr<const TestGraph> _spGraph;
  boost::shared_ptr<Node> _spRoot;

  bool _followReferences;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
