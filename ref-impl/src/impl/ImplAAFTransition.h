//@doc
//@class    AAFTransition | Implementation class for AAFTransition
#ifndef __ImplAAFTransition_h__
#define __ImplAAFTransition_h__


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


class ImplAAFDataDef;

#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

#include "OMStrongRefProperty.h"

#ifndef __ImplAAFOperationGroup_h__
#include "ImplAAFOperationGroup.h"
#endif

class ImplAAFTransition : public ImplAAFComponent
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTransition ();

protected:
  virtual ~ImplAAFTransition ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] Data Definition Object
         ImplAAFDataDef * pDataDef,

		 // @parm [in] Length property value
         aafLength_t  length,

         // @parm [in] The point at which a cut would be inserted if the transition were removed
         aafPosition_t  cutPoint,

         // @parm [in] A reference to an OperationGroup object
         ImplAAFOperationGroup * group);	 

  //****************
  // GetCutPoint()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCutPoint
        // @parm [out] Cut Point
        (aafPosition_t *  cutPoint);
	//@comm Replaces part of omfsTransitionGetInfo

  //****************
  // GetEffect()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOperationGroup
        // @parm [out] Effect used by transition
        (ImplAAFOperationGroup ** effObj);
	//@comm Replaces part of omfsTransitionGetInfo

  //****************
  // SetCutPoint()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCutPoint
        // @parm [in] Cut Point
        (aafPosition_t  cutPoint);

  //****************
  // SetEffect()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetOperationGroup
        // @parm [in] Effect used by transition
        (ImplAAFOperationGroup * group);


  	virtual AAFRESULT ChangeContainedReferences(aafMobID_constref from,
												aafMobID_constref to);

	virtual AAFRESULT GetComponentType(implCompType_t* pType) {*pType = kTransition; return AAFRESULT_SUCCESS;}


private:
	OMStrongReferenceProperty<ImplAAFOperationGroup>	_operationGroup;
	OMFixedSizeProperty<aafPosition_t>					_cutPoint;

};

#endif // ! __ImplAAFTransition_h__


