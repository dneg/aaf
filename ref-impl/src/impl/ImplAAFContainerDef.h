//@doc
//@class    AAFContainerDef | Implementation class for AAFContainerDef
#ifndef __ImplAAFContainerDef_h__
#define __ImplAAFContainerDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif


class ImplAAFContainerDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFContainerDef ();

protected:
  virtual ~ImplAAFContainerDef ();

public:


  //****************
  // EssenceIsIdentified()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EssenceIsIdentified
        // @parm [out] The EssenceIsIdentified
        (aafBool *  pEssenceIsIdentified);


  //****************
  // SetEssenceIsIdentified()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEssenceIsIdentified
        // @parm [in] The EssenceIsIdentified flag
        (aafBool  EssenceIsIdentified);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFContainerDef)

private:
	OMFixedSizeProperty<aafBool>					_isIdentified;
};

#endif // ! __ImplAAFContainerDef_h__


