//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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

#ifndef __REFRESOLVER_h__
#define __REFRESOLVER_h__

//project files
#include <Test.h>

namespace aafanalyzer {

class RefResolver : public Test
{
 public:
  RefResolver(std::ostream& os, boost::shared_ptr<TestGraph> spTestGraph);
  ~RefResolver();

  TestResult Execute();
  std::string GetName() const;
  std::string GetDescription() const;

 private:

  // prohibited
  RefResolver(const RefResolver&);
  RefResolver& operator=( const RefResolver& );
};

} // end of namespace diskstream

#endif/*__TEMPLATE_h__*/
