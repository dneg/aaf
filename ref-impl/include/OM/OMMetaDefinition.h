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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __OMMETADEFINITION_H__
#define __OMMETADEFINITION_H__

#include "OMDataTypes.h"

class OMMetaDefinition
{
public:
    enum Category
    {
        META, // used by ImplAAFMetaDefinition so that ImplAAFMetaDefinition can be allocated  
        CLASS,
        PROPERTY,
        TYPE, // used by ImplAAFTypeDefinition so that ImplAAFTypeDefinition can be allocated
        CHARACTER_TYPE,
        ENUMERATED_TYPE,
        EXT_ENUMERATED_TYPE,
        FIXED_ARRAY_TYPE,
        INDIRECT_TYPE,
        INTEGER_TYPE,
        OPAQUE_TYPE,
        RECORD_TYPE,
        RENAMED_TYPE,
        SET_TYPE,
        STREAM_TYPE,
        STRING_TYPE,
        STRONG_REF_TYPE,
        VARIABLE_ARRAY_TYPE,
        WEAK_REF_TYPE,
        INTERNAL_TYPE // for OMInternalType
    };
    virtual Category category(void) const = 0;
    
    virtual const OMUniqueObjectIdentification& identification() const = 0;
    virtual const wchar_t* name() const = 0;
    virtual const wchar_t* description() const = 0;
};

#endif

