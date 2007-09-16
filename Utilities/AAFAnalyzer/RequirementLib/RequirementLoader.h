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

#ifndef __REQUIREMENTLOADER_H_
#define __REQUIREMENTLOADER_H_

//Requirement files
#include "Requirement.h"

//STL files
#include <string>
#include <map>

namespace aafanalyzer {

class RequirementLoader
{
public:

    RequirementLoader();
    ~RequirementLoader();

    static void __StartElement(void* userData, const char* name, const char** atts);
    static void __EndElement(void* userData, const char* name);
    static void __EndData(void* userData, const char* s, int len);

    void StartElement(const wstring& name, const char** attribs);
    void EndElement(const wstring& name);
    void EndData(const wstring& contents);

    void ParseXML( const char* filename ) const;

 private:

    struct Current
    {
      Current()
      : Data(),
        Id(),
        Type(Requirement::REQ_TYPE_UNDEFINED),
        TypeAsString(),
        Category(Requirement::CAT_UNDEFINED),
        CategoryAsString(),
        Action(),
        Name(),
        Desc(),
        Annotate(),
        Note(),
        Document(),
        Version(),
        Section()
      {}
      
      wstring Data;
      
      wstring Id;
      Requirement::RequirementType Type;
      wstring TypeAsString;
      Requirement::Category Category;
      wstring CategoryAsString;
      wstring Action;
      wstring Name;
      wstring Desc;
      wstring Annotate;
      wstring Note;
      wstring Document;
      wstring Version;
      wstring Section;
    };

    Current _current;

    map<wstring, Requirement::Category> _categoryMap;

    // prohibited
    RequirementLoader( const RequirementLoader& );
    RequirementLoader& operator=( const RequirementLoader& );

};

} // end of namespace diskstream

#endif /*__REQUIREMENTLOADER_H_*/
