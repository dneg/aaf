//@doc
//@class    AAFLocator | Implementation class for AAFLocator
#ifndef __ImplAAFLocator_h__
#define __ImplAAFLocator_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/






#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class ImplAAFLocator : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFLocator ();

protected:
  virtual ~ImplAAFLocator ();

public:

  //****************
  // GetPath()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPath
        (// @parm [out, size_is(bufSize), string] buffer into which path is written
         aafCharacter *  pPathBuf,

         // @parm [in] The size of the pPathBuf buffer
         aafInt32  bufSize);


  //****************
  // GetPathBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPathBufLen
        // @parm [out] required buffer length
        (aafInt32 *  pLen);


  //****************
  // SetPath()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPath
        // @parm [in, string] the new path
        (aafCharacter *  pPathBuf);
};

#endif // ! __ImplAAFLocator_h__


