//@doc
//@class    MetaDefinition | Implementation class for MetaDefinition
#ifndef __ImplAAFStorable_h__
#define __ImplAAFStorable_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
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

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#include "ImplAAFRoot.h"
#include "OMStorable.h"

class ImplAAFClassDef;

class ImplAAFStorable : 
  public OMStorable,
  public ImplAAFRoot
{
protected:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFStorable ();

  virtual ~ImplAAFStorable ();

public:
  // Private SDK methods.
  
  // Associate OMClassDefinition and OMPropertyDefinitions with this object.
  virtual void InitializeOMStorable(ImplAAFClassDef * pClassDef);
  
  virtual AAFRESULT GetDefinition(ImplAAFClassDef ** ppClassDef);

  // Return true if this is a meta object
  // NOTE: These objects will eventually owned by the Object Manager.
  virtual bool metaObject(void) const = 0;
  
  // Return true is this is a data object (Interchange object).
  virtual bool dataObject(void) const = 0;

protected:
  // Associate the existing OMProperties with corresponding property definitions from
  // the given class definition. NOTE: This call is recursive, it calls itself again
  // for the parent class of the given class until current class is a "root" class.
  virtual void InitOMProperties (ImplAAFClassDef * pClassDef) = 0;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFStorable> ImplAAFStorableSP;


//
// Inlude the generic enumerator for storables.
//
#ifndef __ImplAAFEnumerator_h__
#include "ImplAAFEnumerator.h"
#endif
typedef ImplAAFEnumerator<ImplAAFStorable> ImplEnumAAFStorables;

#endif // ! __ImplAAFStorable_h__
