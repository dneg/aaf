//@doc
//@class    AAFParameter | Implementation class for AAFParameter
#ifndef __ImplAAFParameter_h__
#define __ImplAAFParameter_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

class ImplAAFDataDef;


 



#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class ImplAAFParameter : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFParameter ();

protected:
  virtual ~ImplAAFParameter ();

public:


  //****************
  // SetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetLength
        // @parm [in] Length of this object
        (aafLength_t  length);


  //****************
  // GetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLength
        // @parm [out,retval] Length of this component
        (aafLength_t *  pLength);
	

  //****************
  // SetDataDefID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDataDefID
        // @parm [in] Data Definition of this object
        (aafUID_t  pDataDefID);

  //****************
  // GetDataDefID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDefID
        // @parm [out,retval] Data Definition of this object
        (aafUID_t *  pDataDefID);




public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFParameter)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFParameterTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFParameter_h__




