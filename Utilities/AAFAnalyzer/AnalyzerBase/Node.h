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

#ifndef __NODE_h__
#define __NODE_h__

//Boost files
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

//STL files
#include <string>

namespace aafanalyzer {

using namespace std;

class Visitor;

class Node : public boost::enable_shared_from_this<Node>
{
 public:
  typedef unsigned int LID;

  Node();
  Node( const basic_string<wchar_t>& name );
  virtual ~Node();

  virtual bool PreOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  virtual bool PostOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  LID GetLID() const;
  boost::shared_ptr<Node> GetSharedPointerToNode();
  const basic_string<wchar_t>& GetName() const;
  virtual void Decorate( boost::shared_ptr<Node> decoratedNode );

 protected:
  Node( const basic_string<wchar_t>& name, LID lid );

 private:
  static LID _counter;
  basic_string<wchar_t> _name;
  LID _lid;

  // prohibited
  Node( const Node& );
  Node& operator=( const Node& );
};

} // end of namespace diskstream

#endif
