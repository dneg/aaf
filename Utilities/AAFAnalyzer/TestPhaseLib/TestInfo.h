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

//Ax Files
#include <AxTypes.h>

//Boost Files
#include <boost/shared_ptr.hpp>

//Project Files
#include <Requirement.h>

//STL files
#include <vector>

namespace aafanalyzer {

class TestInfo
{
 public:

  TestInfo(const AxString& name, const boost::shared_ptr<const std::vector<AxString> >& requirements);
  TestInfo(const TestInfo& other);
  ~TestInfo();
  const AxString GetName() const;
  const boost::shared_ptr<const std::vector<AxString> > GetRequirementIds() const;

 private:

  const AxString _name;
  const boost::shared_ptr<const std::vector<AxString> > _spRequirementIds;

   // prohibited

};

} // end of namespace diskstream

#endif /*__TESTINFO_H_*/
