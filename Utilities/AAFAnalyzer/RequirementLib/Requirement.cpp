//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
//=---------------------------------------------------------------------=

//Requirement files
#include "Requirement.h"

#include <assert.h>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

Requirement::Requirement(const wstring& id,
                         const RequirementType requirementType,
			 const wstring& requirementTypeAsString,
                         const Category category,
			 const wstring& categoryAsString,
			 const wstring& action,
			 const wstring& name,
                         const wstring& desc,
			 const wstring& annotation,
			 const wstring& note,
			 const wstring& document,
                         const wstring& version,
			 const wstring& section):
    _id( id ),
    _requirementType( requirementType ),
    _requirementTypeAsString( requirementTypeAsString ),
    _category( category ),
    _categoryAsString( categoryAsString ),
    _action( action ),
    _name( name ),
    _description( desc ),
    _annotation( annotation ),
    _note( note ),
    _document( document ),
    _version( version ),
    _section( section )
{}
    
Requirement::~Requirement()
{}

const wstring& Requirement::GetId() const
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

const wstring& Requirement::GetAction() const
{
    return _action;
}

const wstring& Requirement::GetName() const
{
    return _name;
}

const wstring& Requirement::GetDescription() const
{
    return _description;
}

const wstring& Requirement::GetAnnotation() const
{
    return _annotation;
}

const wstring& Requirement::GetNote() const
{
    return _note;
}

const wstring& Requirement::GetDocument() const
{
    return _document;
}

const wstring& Requirement::GetVersion() const
{
    return _version;
}

const wstring& Requirement::GetSection() const
{
    return _section;
}

const wstring& Requirement::GetRequirementTypeAsString() const
{
  return _requirementTypeAsString;
}

const wstring& Requirement::GetCategoryAsString() const
{
  return _categoryAsString;
}

} // end of namespace diskstream
