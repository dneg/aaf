//@doc
//@class    MetaDefinition | Implementation class for MetaDefinition
#ifndef __ImplAAFStorable_h__
#define __ImplAAFStorable_h__

//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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
