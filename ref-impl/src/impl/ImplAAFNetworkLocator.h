//@doc
//@class    AAFNetworkLocator | Implementation class for AAFNetworkLocator
#ifndef __ImplAAFNetworkLocator_h__
#define __ImplAAFNetworkLocator_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/





#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif




class ImplAAFNetworkLocator : public ImplAAFLocator
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFNetworkLocator ();
  virtual ~ImplAAFNetworkLocator ();


  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  // Override from AAFLocator
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPath (/*[out, size_is(bufSize), string]*/ aafWChar *  pPathBuf,
	  /*[in]*/ aafInt32  bufSize);



  // Override from AAFLocator
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPathBufLen (/*[out]*/ aafInt32 *  pLen);



  // Override from AAFLocator
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPath (/*[in, string]*/ aafWChar *  pPathBuf);

private:
  OMWideStringProperty                             _path;
};

#endif // ! __ImplAAFNetworkLocator_h__

