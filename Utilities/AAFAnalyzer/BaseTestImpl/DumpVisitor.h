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

#ifndef __DUMPVISITOR_h__
#define __DUMPVISITOR_h__

#include <Visitor.h>
#include <Edge.h>

//STL files
#include <string>
#include <deque>

namespace aafanalyzer {

using namespace std;

class DumpVisitor : public Visitor
{
 public:

  // Uses Visitor::FOLLOW_ALL by default
  DumpVisitor();

  DumpVisitor( Visitor::Follow_e follow );

  virtual ~DumpVisitor();
  
  virtual bool PreOrderVisit(Node& node);
  virtual bool PostOrderVisit(Node& node);

 private:
  
  std::wstring LevelToIndent(size_t l);

  // prohibited
  DumpVisitor( const DumpVisitor& );
  DumpVisitor& operator=( const DumpVisitor& );
};

} // end of namespace diskstream

#endif/*__DumpVisitor_h__*/
