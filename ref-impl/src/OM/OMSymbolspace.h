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

#ifndef __OMSYMBOLSPACE_H__
#define __OMSYMBOLSPACE_H__

#include "OMDataTypes.h"
#include "OMWString.h"
#include "OMSet.h"


class OMSymbolspace
{
public:
    OMSymbolspace(OMClassId id, const wchar_t* uri, const wchar_t* preferredPrefix, 
        const wchar_t* description);
    ~OMSymbolspace();
    
    OMClassId getId() const;
    const wchar_t* getURI() const;
    const wchar_t* getPreferredPrefix() const;
    const wchar_t* getDescription() const;
    
    const wchar_t* getSymbol(OMClassId id) const;
    const wchar_t* createSymbolForClass(OMClassId id, const wchar_t* name);
    const wchar_t* createSymbolForProperty(OMClassId id, const wchar_t* name);
    const wchar_t* createSymbolForType(OMClassId id, const wchar_t* name);
        
    void addSymbol(OMClassId id, const wchar_t* symbol);
    
    
    static OMSymbolspace* createDefaultExtSymbolspace(OMClassId id);
    static OMSymbolspace* createV11Symbolspace();
    
private:
    OMClassId   _id;
    wchar_t*    _uri;
    wchar_t*    _preferredPrefix;
    wchar_t*    _description;
    OMSet<OMClassId, OMWString>  _idToSymbol; 
    OMSet<OMWString, OMClassId>  _symbolToId; 
};


#endif
