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

#ifndef __TESTINFO_H_
#define __TESTINFO_H_

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <string>
#include <vector>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class TestInfo
{
 public:

  TestInfo(const wstring& name, const shared_ptr<const vector<wstring> >& requirements);
  TestInfo(const TestInfo& other);
  ~TestInfo();
  const wstring GetName() const;
  const shared_ptr<const vector<wstring> > GetRequirementIds() const;

 private:

  const wstring _name;
  const shared_ptr<const vector<wstring> > _spRequirementIds;

   // prohibited

};

} // end of namespace diskstream

#endif /*__TESTINFO_H_*/
