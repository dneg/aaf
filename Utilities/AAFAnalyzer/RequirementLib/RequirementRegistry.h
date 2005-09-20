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

#ifndef __REQUIREMENTREGISTRY_H_
#define __REQUIREMENTREGISTRY_H_

//Ax Files
#include <AxTypes.h>

//Boost Files
#include <boost/shared_ptr.hpp>

//Project Files
#include "Requirement.h"

namespace aafanalyzer {

class RequirementRegistry
{
 public:

  // dtor must be public so that shared_ptr class can access it
  ~RequirementRegistry();

  static RequirementRegistry& GetInstance();
  void Register( const boost::shared_ptr<const Requirement>& req );
  const boost::shared_ptr<const Requirement> GetRequirement( const AxString& id ) const;
  const Requirement::RequirementMap& GetAllRequirements() const;

 private:

  RequirementRegistry();//using Singleton pattern to allow only one object

  static RequirementRegistry* _pRequirementRegistry;
  Requirement::RequirementMap _requirementSet;

  // prohibited

};

} // end of namespace diskstream

#endif /*__REQUIREMENTREGISTRY_H_*/
