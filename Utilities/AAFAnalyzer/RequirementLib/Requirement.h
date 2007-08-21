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

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <string>
#include <map>

namespace aafanalyzer {

using namespace std;
using namespace boost;

class Requirement
{
public:

    typedef map< const wstring, shared_ptr<const Requirement> > RequirementMap;
    typedef shared_ptr<RequirementMap> RequirementMapSP;

    enum RequirementType {REQ_TYPE_UNDEFINED, FILE, APP, DEFINITION};

    enum Category        {CAT_UNDEFINED, GENERAL, IMPORT_EXPORT, COMPOSITIONAL, METADATA,
                          MIXDOWN, AUXILIARY_FILE, ANNOTATIONS, EFFECT,
                          OPTIONAL_PROPERTIES, STRUCTURED_STORAGE, PROTOCOL,
                          ADHOC};

    Requirement(const wstring& id,
                const RequirementType requirementType,
		const wstring& requirementTypeAsString,
                const Category category,
		const wstring& categoryAsString,
                const wstring& name,
                const wstring& desc,
                const wstring& document,
                const wstring& version,
                const wstring& section);
    ~Requirement();

    // Primary accessors
    const wstring& GetId() const;  
    const enum RequirementType GetRequirementType() const;
    const enum Category GetCategory() const;
    const wstring& GetName() const;
    const wstring& GetDescription() const;
    const wstring& GetDocument() const;
    const wstring& GetVersion() const;
    const wstring& GetSection() const;

    // Convenience accessors
    const wstring& GetRequirementTypeAsString() const;
    const wstring& GetCategoryAsString() const;

 private:

    const wstring _id;
    const enum RequirementType _requirementType;
    wstring _requirementTypeAsString;
    const enum Category _category;
    wstring _categoryAsString;
    const wstring _name;
    const wstring _description;
    const wstring _document;
    const wstring _version;
    const wstring _section;

    // prohibited
    Requirement();
    Requirement(const Requirement&);
    Requirement& operator=( const Requirement& );
};

} // end of namespace diskstream

#endif /*__REQUIREMENT_H_*/
