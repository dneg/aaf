//@doc
//@class    AAFNetworkLocator | Implementation class for AAFNetworkLocator
#ifndef __ImplAAFNetworkLocator_h__
#define __ImplAAFNetworkLocator_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif


#include "OMProperty.h"

const int PID_NETWORKLOCATOR_URLSTRING          = 0;

class ImplAAFNetworkLocator : public ImplAAFLocator
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFNetworkLocator ();
  virtual ~ImplAAFNetworkLocator ();


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


  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFNetworkLocator)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFNetworkLocatorTest.cpp.
  static AAFRESULT test();

private:
  OMWideStringProperty                             _path;
};

#endif // ! __ImplAAFNetworkLocator_h__

