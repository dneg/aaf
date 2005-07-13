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

#ifndef __TESTVISITOR_h__
#define __TESTVISITOR_h__

//project files
#include "Visitor.h"

//STL files
#include <iostream>
#include <string>

namespace aafanalyzer {

class TestVisitor : public Visitor
{
 public:
  TestVisitor();
  virtual ~TestVisitor();
  
  virtual bool PreOrderVisit(Node& node);
  virtual bool PostOrderVisit(Node& node);
  virtual bool EdgeVisit(Edge& edge);

 private:

  std::basic_string<wchar_t> LevelToIndent(unsigned int l);
  
  unsigned int _level;

  // prohibited
  TestVisitor( const TestVisitor& );
  TestVisitor& operator=( const TestVisitor& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
