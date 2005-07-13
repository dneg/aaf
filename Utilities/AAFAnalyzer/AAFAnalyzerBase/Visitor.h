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

#ifndef __VISITOR_h__
#define __VISITOR_h__

//boost files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

class Node;
class Edge;

class Visitor
{
 public:
  Visitor();
  virtual ~Visitor();

  virtual bool PreOrderVisit(Node& node);
  virtual bool PostOrderVisit(Node& node);
  virtual bool EdgeVisit(Edge& edge);

 private:

  // prohibited
  Visitor( const Visitor& );
  Visitor& operator=( const Visitor& );
};

} // end of namespace diskstream

#endif
