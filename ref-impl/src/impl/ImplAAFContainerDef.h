//@doc
//@class    AAFContainerDef | Implementation class for AAFContainerDef
#ifndef __ImplAAFContainerDef_h__
#define __ImplAAFContainerDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFPluggableDef_h__
#include "ImplAAFPluggableDef.h"
#endif


class ImplAAFContainerDef : public ImplAAFPluggableDef
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

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFContainerDefTest.cpp.
  static AAFRESULT test();
private:
	OMFixedSizeProperty<aafBool>					_isIdentified;
};

#endif // ! __ImplAAFContainerDef_h__


