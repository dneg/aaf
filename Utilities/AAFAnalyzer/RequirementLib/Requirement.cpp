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

#include "Requirement.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

Requirement::Requirement(const AxString& id,
                         const RequirementType requirementType,
                         const Category category, const AxString& name,
                         const AxString& desc, const AxString& document,
                         const AxString& version, const AxString& section):
    _id( id ),
    _requirementType( requirementType ),
    _category( category ),
    _name( name ),
    _description( desc ),
    _document( document ),
    _version( version ),
    _section( section )
{}
    
Requirement::~Requirement()
{}

const AxString& Requirement::GetId() const
{
    return _id;
}

const enum Requirement::RequirementType Requirement::GetRequirementType() const
{
    return _requirementType;
}

const enum Requirement::Category Requirement::GetCategory() const
{
    return _category;
}

const AxString& Requirement::GetName() const
{
    return _name;
}

const AxString& Requirement::GetDescription() const
{
    return _description;
}

const AxString& Requirement::GetDocument() const
{
    return _document;
}

const AxString& Requirement::GetVersion() const
{
    return _version;
}

const AxString& Requirement::GetSection() const
{
    return _section;
}

} // end of namespace diskstream
