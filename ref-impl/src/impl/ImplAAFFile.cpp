
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#ifndef __ImplAAFFile_h__
#include "ImplAAFFile.h"
#endif

#include <assert.h>


ImplAAFFile::ImplAAFFile ()
: _pContainer (0)
{}


ImplAAFFile::~ImplAAFFile ()
{}


void ImplAAFFile::InitContainer (void * pContainer)
{
  assert (! _pContainer);
  _pContainer = pContainer;
  assert (_pContainer);
}


class IAAFObject;

void * ImplAAFFile::GetContainer ()
{
  assert (_pContainer);
  return _pContainer;
}

