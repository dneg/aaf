//@doc
//@class    AAFDefObject | Implementation class for AAFDefObject
#ifndef __ImplAAFDefObject_h__
#define __ImplAAFDefObject_h__


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


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


/*************************************************************************
 * 
 * @module AAFDefObject | AAFDefObject is an abstract class
 *		  that defines an item to be referenced in the AAF file. 
 *		  AAFDefObject specifies the AUID unique identifier.
 *
 * @base public | AAFObject
 *
 *************************************************************************/





#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class ImplAAFDefObject : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDefObject ();

protected:
  virtual ~ImplAAFDefObject ();

public:



  //****************
  // GetAUID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAUID
        // @parm [retval,out] Pointer to an AUID reference
        (aafUID_t *  pAuid);

  //****************
  // SetAUID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAUID
        // @parm [in] Pointer to an AUID reference
        (aafUID_t *  pAuid);


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (aafWChar *  name,  //@parm [in] Definition Name
		aafInt32 bufSize);	  //@parm [in] size of the buffer required to hold Definition Name + terminator
  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        (aafInt32 *  nameLen);  //@parm [in,out] Definition Name length
  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        (aafWChar *  name);  //@parm [in, ref] Definition Name

  //****************
  // GetDescription()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescription
        (aafWChar *  description,  //@parm [in] Definition Description
		aafInt32 bufSize);	  //@parm [in] size of the buffer required to hold Definition Description + terminator
  //****************
  // GetDescriptionBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescriptionBufLen
        (aafInt32 *  descriptionLen);  //@parm [in,out] Definition description length
  //****************
  // SetDescription()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDescription
        (aafWChar *  description);  //@parm [in, ref] Definition description

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFDefObject)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFDefObjectTest.cpp.
  static AAFRESULT test();

private:
	OMFixedSizeProperty<aafUID_t>		_ID;
	OMWideStringProperty				_name;
	OMWideStringProperty				_description;
};

#endif // ! __ImplAAFDefObject_h__


