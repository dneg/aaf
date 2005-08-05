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

#ifndef __ACYCLICVISITOR_h__
#define __ACYCLICVISITOR_h__

#include "Visitor.h"

//stl files
#include <vector>

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

class Node;

class AcyclicVisitor : public Visitor
{
 public:

  typedef std::vector<unsigned int> Vector;

  AcyclicVisitor();
  virtual ~AcyclicVisitor();

  virtual bool PreOrderVisit(Node& node);
  virtual bool PostOrderVisit(Node& node);

 private:
  bool IsPresent(unsigned int lid);
  void Erase(unsigned int lid);

  Vector _Vector;
  

  // prohibited
  AcyclicVisitor( const AcyclicVisitor& );
  AcyclicVisitor& operator=( const AcyclicVisitor& );
};

} // end of namespace diskstream

#endif
