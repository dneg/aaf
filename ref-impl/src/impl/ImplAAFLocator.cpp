/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFLocator_h__
#include "ImplAAFLocator.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFLocator::ImplAAFLocator ()
{}


ImplAAFLocator::~ImplAAFLocator ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFLocator::GetPath (
      aafCharacter *  /*pPathBuf*/,
      aafInt32  /*bufSize*/)
{
  return AAFRESULT_ABSTRACT_CLASS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFLocator::GetPathBufLen (
      aafInt32 *  /*pLen*/)
{
  return AAFRESULT_ABSTRACT_CLASS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFLocator::SetPath (
      aafCharacter *  /*pPathBuf*/)
{
  return AAFRESULT_ABSTRACT_CLASS;
}

OMDEFINE_STORABLE(ImplAAFLocator, AUID_AAFLocator);


