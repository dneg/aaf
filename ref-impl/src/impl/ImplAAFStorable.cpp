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
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
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

/*************************************************************************
 * 
 * @module ImplAAFStorable | ImplAAFStorable is an abstract class
 *		  that defines a base class of all persistant objects in
 *      an AAF file. All of its methods are inherited and then implemented
 *      by a subclass.
 *
 * @base public | ImplAAFRoot | OMStorable
 *
 *************************************************************************/



#include "ImplAAFStorable.h"
#include "ImplAAFClassDef.h"
#include "assert.h"

ImplAAFStorable::ImplAAFStorable ()
{
}


ImplAAFStorable::~ImplAAFStorable ()
{
}


// Associate OMClassDefinition and OMPropertyDefinitions with this object.
void ImplAAFStorable::InitializeOMStorable(ImplAAFClassDef * pClassDef)
{
  assert (NULL != pClassDef);
  
  // Install the class definition for this storable.
  setDefinition(pClassDef);
  
  // Make sure all of the properties exist and have property definitions.
  InitOMProperties(pClassDef);
}

  
AAFRESULT ImplAAFStorable::GetDefinition(ImplAAFClassDef ** ppClassDef)
{
  if (NULL == ppClassDef)
    return AAFRESULT_NULL_PARAM;
  *ppClassDef = NULL;
  
  OMClassDefinition * classDefinition = const_cast<OMClassDefinition *>(definition());
  ImplAAFClassDef * pClassDef = dynamic_cast<ImplAAFClassDef *>(classDefinition);
  assert (NULL != pClassDef);
  if (!pClassDef)
    return AAFRESULT_NOT_INITIALIZED;
  
  *ppClassDef = pClassDef;
  pClassDef->AcquireReference();
  return AAFRESULT_SUCCESS;
}
