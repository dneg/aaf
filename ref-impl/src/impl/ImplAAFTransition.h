//@doc
//@class    AAFTransition | Implementation class for AAFTransition
#ifndef __ImplAAFTransition_h__
#define __ImplAAFTransition_h__


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

#ifndef __ImplAAFComponent_h__
#include "ImplAAFComponent.h"
#endif

#ifndef __ImplAAFGroup_h__
#include "ImplAAFGroup.h"
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
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTransition)


  //****************
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Create
        (// @parm [in] Data Definition Object
         aafUID_t * pDatadef,

		 // @parm [in] Length property value
         aafLength_t  length,

         // @parm [in] The point at which a cut would be inserted if the transition were removed
         aafPosition_t  cutPoint,

         // @parm [in] A reference to an effect invocation object
         ImplAAFGroup * effect);	 

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
    GetEffect
        // @parm [out] Effect used by transition
        (ImplAAFGroup ** effObj);
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
    SetEffect
        // @parm [in] Effect used by transition
        (ImplAAFGroup * effObj);



public:

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTransitionTest.cpp.
  static AAFRESULT test();

private:
	OMStrongReferenceProperty<ImplAAFGroup>	_effect;
	OMFixedSizeProperty<aafPosition_t>		_cutPoint;

};

#endif // ! __ImplAAFTransition_h__


