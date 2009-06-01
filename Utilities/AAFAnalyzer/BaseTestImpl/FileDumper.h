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

#ifndef __FILEDUMPER_h__
#define __FILEDUMPER_h__

//Test/Result files
#include <Test.h>

//Ax files
#include <AxTypes.h>

namespace aafanalyzer {
    
using namespace std;
using namespace boost;

class TestLevelTestResult;
class TestGraph;
class Node;

class FileDumper : public Test
{
 public:

  FileDumper(wostream& os, shared_ptr<const TestGraph> spGraph, shared_ptr<Node> spRoot, bool followReferences );

  ~FileDumper();

  virtual shared_ptr<TestLevelTestResult> Execute();
  virtual AxString GetName() const;
  virtual AxString GetDescription() const;
  static const TestInfo GetTestInfo();

 private:

  // prohibited
  FileDumper();
  FileDumper(const FileDumper&);
  FileDumper& operator=( const FileDumper& );

  shared_ptr<Node> _spRoot;
  bool _followReferences;
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
