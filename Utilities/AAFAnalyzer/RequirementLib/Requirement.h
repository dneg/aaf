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

#ifndef __REQUIREMENT_H_
#define __REQUIREMENT_H_

//Ax files
#include <AxTypes.h>

//stl files
#include <map>

//Boost Files
#include <boost/shared_ptr.hpp>

namespace aafanalyzer {

class Requirement
{
public:
    enum RequirementType {FILE, APP};
    enum Category        {COMPOSITIONAL, ANNOTATIONS, ADHOC};

    Requirement(const AxString& id,
                const RequirementType requirementType,
                const Category category,
                const AxString& name,
                const AxString& desc,
                const AxString& document,
                const AxString& version,
                const AxString& section);
    ~Requirement();
    
    const AxString& GetId() const;
    const enum RequirementType GetRequirementType() const;
    const enum Category GetCategory() const;
    const AxString& GetName() const;
    const AxString& GetDescription() const;
    const AxString& GetDocument() const;
    const AxString& GetVersion() const;
    const AxString& GetSection() const;
    
    typedef std::map< const AxString, boost::shared_ptr<const Requirement> > RequirementMap;
    typedef boost::shared_ptr<RequirementMap> RequirementMapSP;

 private:
    const AxString _id;
    const enum RequirementType _requirementType;
    const enum Category _category;
    const AxString _name;
    const AxString _description;
    const AxString _document;
    const AxString _version;
    const AxString _section;

  // prohibited
  Requirement(const Requirement&);
  Requirement& operator=( const Requirement& );
};

} // end of namespace diskstream

#endif /*__REQUIREMENT_H_*/
