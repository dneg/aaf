//@doc
//@class    AAFOperationGroup | Implementation class for AAFOperationGroup
#ifndef __ImplAAFOperationGroup_h__
#define __ImplAAFOperationGroup_h__


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
 *  prior written permission of Avid Technology, Inc.
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

private:
	OMFixedSizeProperty<aafUID_t>						_operationDefinition;
	OMStrongReferenceVectorProperty<ImplAAFSegment>		_inputSegments;
	OMStrongReferenceVectorProperty<ImplAAFParameter>	_parameters;
	OMFixedSizeProperty<aafUInt32>						_bypassOverride;
	OMStrongReferenceProperty<ImplAAFSourceReference>	_rendering;
};

#endif // ! __ImplAAFOperationGroup_h__


