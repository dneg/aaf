//@doc
//@class    AAFOperationGroup | Implementation class for AAFOperationGroup
#ifndef __ImplAAFOperationGroup_h__
#define __ImplAAFOperationGroup_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif


class ImplAAFDataDef;

class ImplAAFOperationDef;

class ImplAAFParameter;

class ImplEnumAAFOperationDefs;

class ImplEnumAAFParameterDefs;

class ImplAAFSegment;

class ImplAAFSourceReference;

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
         aafUID_t * pDatadef,


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
  // GetNumSourceSegments()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumSourceSegments
        // @parm [out] Number of source media segments in the effect
        (aafInt32 *  numSources);
	//@comm Replaces omfiEffectGetNumSlots


  //****************
  // GetNumParameters()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumParameters
        // @parm [out] Number of parameter slots in the effect
        (aafInt32 *  numParameters);
	//@comm Replaces omfiEffectGetNumSlots

  //****************
  // IsValidTranEffect()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsValidTranOperation
        // @parm [out] TRUE if the effect is valid in a transition
        (aafBool *  validTransition);

  //****************
  // AddNewParameter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddNewParameter
        (// @parm [in] Parameter to place in effect slot
         ImplAAFParameter * value);
	//@comm Replaces part of omfiEffectAddNewSlot

  //****************
  // AddNewInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendNewInputSegment
        (// @parm [in] Segment to place in effect
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
    GetParameterByArgID
        (// @parm [in] Arg ID
         aafArgIDType_t  argID,

         // @parm [out] Parameter object
         ImplAAFParameter ** parameter);

  //****************
  // GetIndexedInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIndexedInputSegment
        (// @parm [in] 1-based index into the effet inputs
         aafInt32  index,

         // @parm [out] Input segment
         ImplAAFSegment ** inputSegment);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFOperationGroup)

private:
	OMFixedSizeProperty<aafUID_t>						_operationDefinition;
	OMStrongReferenceVectorProperty<ImplAAFSegment>		_inputSegments;
	OMStrongReferenceVectorProperty<ImplAAFParameter>	_parameters;
	OMFixedSizeProperty<aafUInt32>						_bypassOverride;
	OMStrongReferenceProperty<ImplAAFSourceReference>	_rendering;
};

#endif // ! __ImplAAFOperationGroup_h__


