/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif




#ifndef __ImplAAFFiller_h__
#include "ImplAAFFiller.h"
#endif

#include <assert.h>


ImplAAFFiller::ImplAAFFiller ()
{}


ImplAAFFiller::~ImplAAFFiller ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFFiller::Create (ImplAAFDataDef * /*datadef*/,
                           aafLength_t  /*length*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



extern "C" const aafClassID_t CLSID_AAFFiller;

OMDEFINE_STORABLE(ImplAAFFiller, CLSID_AAFFiller);

