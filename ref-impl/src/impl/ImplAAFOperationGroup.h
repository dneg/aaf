//@doc
//@class    AAFGroup | Implementation class for AAFGroup
#ifndef __ImplAAFGroup_h__
#define __ImplAAFGroup_h__


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

class ImplAAFEffectDef;

class ImplAAFParameter;

class ImplEnumAAFEffectDefs;

class ImplEnumAAFParameterDefs;

class ImplAAFSegment;

class ImplAAFSourceReference;

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFGroup : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFGroup ();

protected:
  virtual ~ImplAAFGroup ();

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
         ImplAAFEffectDef * effectDef);
	//@comm  This function takes an already created effect definition object as an argument.
	//@comm  To add slots to the effect, call AddNewSlot.
	//@comm  To add renderings, call SetRender.

  //****************
  // GetEffectDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEffectDefinition
        // @parm [out] Effect definition object
        (ImplAAFEffectDef ** effectDef);
	//@comm Replaces part of omfiEffectGetInfo


  //****************
  // GetRender()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRender
        // @parm [out] Final rendering segment
        (ImplAAFSegment ** segment);
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
        (aafBool **  sourceClip);
	//@comm Replaces omfiEffectIsATimeWarp


  //****************
  // GetBypassOverride()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBypassOverride
        // @parm [out] Bypass override property value
        (aafArgIDType_t *  bypassOverride);
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
    IsValidTranEffect
        // @parm [out] TRUE if the effect is valid in a transition
        (aafBool *  validTransition);

  //****************
  // AddNewParameter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddNewParameter
        (// @parm [in] Argument ID for the slot
         aafArgIDType_t  argID,

         // @parm [in] Parameter to place in effect slot
         ImplAAFParameter * value);
	//@comm Replaces part of omfiEffectAddNewSlot

  //****************
  // AddNewInputSegment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddNewInputSegment
        (// @parm [in] Index (1-based) of the input segment
         aafInt32  index,

         // @parm [in] Segment to place in effect
         ImplAAFSegment * value);
	//@comm Replaces part of omfiEffectAddNewSlot

  //****************
  // SetRender()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetRender
        // @parm [in] A segment containing a representation of the rendering
        (ImplAAFSegment * segment);
	//@comm Replaces omfiEffectSetFinalRender and omfiEffectSetWorkingRender

  //****************
  // SetBypassOverride()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBypassOverride
        // @parm [in] Bypass override
        (aafArgIDType_t  bypassOverride);
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
  OMDECLARE_STORABLE(ImplAAFGroup)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFGroupTest.cpp.
  static AAFRESULT test();

private:
//	OMFixedSizeProperty<aafUID_t>						_effectDefinition;
//	OMStrongReferenceVectorProperty<ImplAAFSegment>		_inputSegments;
//	OMStrongReferenceVectorProperty<ImplAAFParameter>	_parameters;
//	OMFixedSizeProperty<aafUInt32>						_bypassOverride;
//	OMStrongReferenceProperty<ImplAAFSourceReference>	_rendering;
};

#endif // ! __ImplAAFGroup_h__


