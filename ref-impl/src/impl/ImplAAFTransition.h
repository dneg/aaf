//@doc
//@class    AAFTransition | Implementation class for AAFTransition
#ifndef __ImplAAFTransition_h__
#define __ImplAAFTransition_h__


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

#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

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
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Create
        (// @parm [in] Data Definition Object
         const aafUID_t & datadef,

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



private:
	OMStrongReferenceProperty<ImplAAFOperationGroup>	_operationGroup;
	OMFixedSizeProperty<aafPosition_t>					_cutPoint;

};

#endif // ! __ImplAAFTransition_h__


