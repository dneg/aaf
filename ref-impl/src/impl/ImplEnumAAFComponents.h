//@doc
//@class    EnumAAFComponents | Implementation class for EnumAAFComponents
#ifndef __ImplEnumAAFComponents_h__
#define __ImplEnumAAFComponents_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


class ImplAAFComponent;
class ImplAAFSequence;







#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMStrongReferenceVectorProperty<ImplAAFComponent> ComponentStrongRefArrayProp_t;

class ImplEnumAAFComponents : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFComponents ();
  virtual ~ImplEnumAAFComponents ();



  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
		// @parm [out,retval] The Next Component
        (ImplAAFComponent ** ppComponent);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of components requested
		 aafUInt32  count,

		 // @parm [out, size_is(count), length_is(*pFetched)] array to receive components
		 ImplAAFComponent ** ppComponents,

		 // @parm [out,ref] number of actual Components fetched into ppComponents array
         aafUInt32 *  pFetched);

  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
		// @parm [in] Number of elements to skip
        (aafUInt32  count);

  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();


  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
		// @parm [out,retval] new enumeration
        (ImplEnumAAFComponents ** ppEnum);


public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, ComponentStrongRefArrayProp_t *pProp);

private:
	aafUInt32							_current;
	ImplAAFObject						*_enumObj;
	ComponentStrongRefArrayProp_t	*_enumStrongProp;
};

#endif // ! __ImplEnumAAFComponents_h__

