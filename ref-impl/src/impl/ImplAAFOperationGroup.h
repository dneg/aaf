//@doc
//@class    AAFOperationGroup | Implementation class for AAFOperationGroup
#ifndef __ImplAAFOperationGroup_h__
#define __ImplAAFOperationGroup_h__


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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __ImplAAFOperationDef_h__
#include "ImplAAFOperationDef.h"
#endif

#include "OMStrongRefProperty.h"
#include "OMStrongRefVectorProperty.h"
#include "OMStrongRefSetProperty.h"
#include "OMWeakRefProperty.h"

class ImplAAFDataDef;
class ImplAAFParameter;
class ImplAAFSegment;
class ImplAAFSourceReference;

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFParameter> ImplEnumAAFParameters;

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFOperationGroup : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFOperationGroup ();

protected:
  virtual ~ImplAAFOperationGroup ();

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

         // @parm [in] Effect Definition object
         ImplAAFOperationDef * operationDef);
	//@comm  This function takes an already created effect definition object as an argument.
	//@comm  To add slots to the effect, call AddNewSlot.
	//@comm  To add renderings, call SetRender.

  //****************
  // GetOperationDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOperationDefinition
        // @parm [out] Effect definition object
        (ImplAAFOperationDef ** OperationDef);
	//@comm Replaces part of omfiEffectGetInfo

  //****************
  // SetOperationDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetOperationDefinition
        // @parm [in] Effect definition object
        (ImplAAFOperationDef * OperationDef);


  //****************
  // GetRender()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRender
        // @parm [out] Final rendering segment
        (ImplAAFSourceReference ** sourceRef);
	//@comm If this property does not exist the error
	// OM_ERR_PROP_NOT_PRESENT will be returned.
	//@comm Working and final renderings are handled by using
	// a media group as the segment.
	//@comm Replaces omfiEffectGetFinalRender and omfiEffectGetWorkingRender

  //****************
  // IsATimeWarp()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsATimeWarp
        // @parm [out] Working rendering source clip
        (aafBool *  sourceClip);
	//@comm Replaces omfiEffectIsATimeWarp


  //****************
  // GetBypassOverride()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBypassOverride
        // @parm [out] Bypass override property value
        (aafUInt32 *  bypassOverride);
	//@comm If the property does not exist, the error OM_ERR_PROP_NOT_PRESENT will be returned.)
	//@comm Replaces omfiEffectGetBypassOverride

  //****************
  // CountSourceSegments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountSourceSegments
        // @parm [out] Number of source media segments in the effect
        (aafUInt32 *  numSources);
	//@comm Replaces omfiEffectGetNumSlots


  //****************
  // CountParameters()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountParameters
        // @parm [out] Number of parameter slots in the effect
        (aafUInt32 *  numParameters);
	//@comm Replaces omfiEffectGetNumSlots

  //****************
  // IsValidTranEffect()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsValidTranOperation
        // @parm [out] TRUE if the effect is valid in a transition
        (aafBool *  validTransition);

  //****************
  // AddParameter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddParameter
        (// @parm [in] Parameter to place in effect slot
         ImplAAFParameter * value);
	//@comm Replaces part of omfiEffectAddNewSlot

  //****************
  // AppendInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendInputSegment
        (// @parm [in] Segment to place in effect
         ImplAAFSegment * value);
	//@comm Replaces part of omfiEffectAddNewSlot

  //****************
  // PrependInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependInputSegment
        (// @parm [in] Segment to place in effect
         ImplAAFSegment * value);
	//@comm Replaces part of omfiEffectAddNewSlot

  //****************
  // PrependInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertInputSegmentAt
        (// @parm [in] index to place segment
         aafUInt32 index,
	     // @parm [in] Segment to place in effect
         ImplAAFSegment * value);
	//@comm Replaces part of omfiEffectAddNewSlot

  //****************
  // SetRender()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetRender
        // @parm [in] A segment containing a representation of the rendering
        (ImplAAFSourceReference * sref);
	//@comm Replaces omfiEffectSetFinalRender and omfiEffectSetWorkingRender

  //****************
  // SetBypassOverride()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBypassOverride
        // @parm [in] Bypass override
        (aafUInt32  bypassOverride);
	//@comm Replaces omfiEffectSetBypassOverride

  //****************
  // GetParameterByArgID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    LookupParameter
        (// @parm [in] Arg ID
         aafArgIDType_t  argID,

         // @parm [out] Parameter object
         ImplAAFParameter ** parameter);

  //****************
  // GetParameters()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetParameters
        (// @parm [out] enumerator across parameters
         ImplEnumAAFParameters ** ppEnum);

  //****************
  // GetIndexedInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetInputSegmentAt
        (// @parm [in] 1-based index into the effet inputs
         aafUInt32  index,

         // @parm [out] Input segment
         ImplAAFSegment ** inputSegment);

  //****************
  // GetIndexedInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveInputSegmentAt
        (// @parm [in] 1-based index into the effet inputs
         aafUInt32  index);

public:

	 virtual AAFRESULT ChangeContainedReferences(aafMobID_constref from,
												aafMobID_constref to);
	 // Visit this component and its sub-components
	 // with the specified visitor.
	 virtual void Accept(AAFComponentVisitor& visitor);

private:
	OMWeakReferenceProperty<ImplAAFOperationDef>		_operationDefinition;
	OMStrongReferenceVectorProperty<ImplAAFSegment>		_inputSegments;
	OMStrongReferenceSetProperty<OMUniqueObjectIdentification, ImplAAFParameter>		_parameters;
	OMFixedSizeProperty<aafUInt32>						_bypassOverride;
	OMStrongReferenceProperty<ImplAAFSourceReference>	_rendering;
};

#endif // ! __ImplAAFOperationGroup_h__


