/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/

#include "OMDictionary.h"
#include "OMPropertyDefinition.h"

OMDictionary::PropertyDefinitionSet OMDictionary::_propertyDefinitions;

static
OMBuiltinPropertyDefinition dictionary(0, L"MetaDictionary", 0x0001, false);
static
OMBuiltinPropertyDefinition clientRoot(0, L"Header",         0x0002, false);

OMPropertyDefinition* OMDictionary::find(const OMPropertyId propertyId)
{
  TRACE("OMDictionary::find");

  PRECONDITION("Valid property id", propertyId != 0);

  OMPropertyDefinition* result = 0;
  bool status = _propertyDefinitions.find(propertyId, result);

  POSTCONDITION("Property definition found", status);
  POSTCONDITION("Valid result", result != 0);
  return result;
}

void OMDictionary::insert(const OMPropertyId propertyId,
                          const OMPropertyDefinition* definition)
                          
{
  TRACE("OMDictionary::insert");

  PRECONDITION("Valid property id", propertyId != 0);
  PRECONDITION("Valid property definition", definition != 0);
  PRECONDITION("Definition not already present",
                                   !_propertyDefinitions.contains(propertyId));

  bool status = _propertyDefinitions.insert(
                                propertyId,
                                const_cast<OMPropertyDefinition*>(definition));

  POSTCONDITION("Definition not previously present", status);
  POSTCONDITION("Definition present",
                                    _propertyDefinitions.contains(propertyId));
}


