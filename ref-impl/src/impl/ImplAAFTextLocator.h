/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/
//@doc
//@class    AAFTextLocator | Implementation class for AAFTextLocator
#ifndef __ImplAAFTextLocator_h__
#define __ImplAAFTextLocator_h__

#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif


class ImplAAFTextLocator : public ImplAAFLocator
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTextLocator ();

protected:
  virtual ~ImplAAFTextLocator ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (// @parm [out, size_is(bufSize), string] buffer into which Name is written
         wchar_t *  pNameBuf,

         // @parm [in] The size of the pNameBuf buffer
         aafInt32  bufSize);


  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        // @parm [out] required buffer length
        (aafInt32 *  pLen);


  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        // @parm [in, string] the new Name
        (wchar_t *  pNameBuf);


  // Persistent data
private:
	OMWideStringProperty                      _name;
};

#endif // ! __ImplAAFTextLocator_h__


