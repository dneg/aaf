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
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        (aafWChar *  name);  //@parm [in, ref] Definition Name


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (// @parm [out, string, size_is(bufSize)] buffer into which Name is to be written
         wchar_t *  pName,

         // @parm [in] size of *pName buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        (aafInt32 *  nameLen);  //@parm [in,out] Definition Name length


  //****************
  // SetDescription()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDescription
        (aafWChar *  description);  //@parm [in, ref] Definition description


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


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFDefObject)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFDefObjectTest.cpp.
  static AAFRESULT test();

private:
  // friendly name of this definition
  OMWideStringProperty          _name;

  // brief text description of this definition
  OMWideStringProperty          _description;

  // auid to be used to identify this definition
  OMFixedSizeProperty<aafUID_t> _identification;
};

#endif // ! __ImplAAFDefObject_h__
