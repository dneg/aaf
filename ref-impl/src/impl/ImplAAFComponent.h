//@doc
//@class    AAFComponent | Implementation class for AAFComponent
#ifndef __ImplAAFComponent_h__
#define __ImplAAFComponent_h__


class ImplAAFDataDef;


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "OMProperty.h"

const int CLSID_AAFCOMPONENT = 45;

const int PID_IDENTIFICATION_DATADEF           = 0;
const int PID_IDENTIFICATION_LENGTH            = 1;

class ImplAAFComponent : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFComponent ();
  ~ImplAAFComponent ();


  //****************
  // SetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetLength
        (aafLength_t *  length);  //@parm [in] Length of this object


  //****************
  // GetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLength
        (aafLength_t *  length);  //@parm [retval][out] Length of this component

	
  //****************
  // SetDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDataDef
        (aafUID_t *  datadef);  //@parm [in] DataDef of this object


  //****************
  // GetDataDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDataDef
        (aafUID_t *  datadef);  //@parm [retval][out] DataDef of this object


public:
	AAFRESULT SetNewProps(
				aafLength_t length,		// IN - Length  property value
				aafUID_t *dataDef);		// IN - DataDef property value

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFComponentTest.cpp.
  static AAFRESULT test();

private:
	OMFixedSizeProperty<aafUID_t>		_dataDef;
	OMFixedSizeProperty<aafInt64>		_length;
};

#endif // ! __ImplAAFComponent_h__

