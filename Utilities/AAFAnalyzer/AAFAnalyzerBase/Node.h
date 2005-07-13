//=---------------------------------------------------------------------=
//
// $Id$
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

#ifndef __NODE_h__
#define __NODE_h__

//boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <string>

namespace aafanalyzer {

class Visitor;

class Node
{
 public:
  typedef unsigned int LID;

  Node();
  Node( const std::basic_string<wchar_t>& name );
  virtual ~Node();

  virtual bool PreOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  virtual bool PostOrderVisit(boost::shared_ptr<Visitor> spVisitor);
  LID GetLID() const;
  const std::basic_string<wchar_t>& GetName() const;

 private:
  static LID _counter;
  std::basic_string<wchar_t> _name;
  LID _lid;

  // prohibited
  Node( const Node& );
  Node& operator=( const Node& );
};

} // end of namespace diskstream

#endif
