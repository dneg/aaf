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

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFSourceClip_h__
#include "ImplAAFSourceClip.h"
#endif

#include <assert.h>
#include <AAFResult.h>


ImplAAFSourceClip::ImplAAFSourceClip ():
	_fadeInLen(		PID_SOURCECLIP_FADEINLEN,		"fadeInLen"),
	_fadeInType(	PID_SOURCECLIP_FADEINTYPE,		"fadeInType"),
	_fadeInPresent( PID_SOURCECLIP_FADEINPRESENT,	"fadeInPresent"),
	_fadeOutLen(	PID_SOURCECLIP_FADEOUTLEN,		"fadeOutLen"),
	_fadeOutType(	PID_SOURCECLIP_FADEOUTTYPE,		"fadeOutType"),
	_fadeOutPresent(PID_SOURCECLIP_FADEOUTPRESENT,	"fadeOutPresent")
{
	_persistentProperties.put(		_fadeInLen.address());
	_persistentProperties.put(		_fadeInType.address());
	_persistentProperties.put(		_fadeInPresent.address());
	_persistentProperties.put(		_fadeOutLen.address());
	_persistentProperties.put(		_fadeOutType.address());
	_persistentProperties.put(		_fadeOutPresent.address());

}


ImplAAFSourceClip::~ImplAAFSourceClip ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::InitializeSourceClip (ImplAAFDataDef * /*datadef  */,
                           aafLength_t      /*length   */,
                           aafSourceRef_t   /*sourceRef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFSourceClip::GetFade (aafInt32 *       /*fadeInLen     */,
                           aafFadeType_t *  /*fadeInType    */,
                           aafBool *        /*fadeInPresent */,
                           aafInt32 *       /*fadeOutLen    */,
                           aafFadeType_t *  /*fadeOutType   */,
                           aafBool *        /*fadeOutPresent*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::ResolveRef (ImplAAFMob ** /*mob*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::GetRef (aafSourceRef_t *  /*sourceRef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


 AAFRESULT STDMETHODCALLTYPE
   ImplAAFSourceClip::SetFade (aafInt32       /*fadeInLen  */,
                           aafFadeType_t  /*fadeInType */,
                           aafInt32       /*fadeOutLen */,
                           aafFadeType_t  /*fadeOutType*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::SetRef (aafSourceRef_t  /*sourceRef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



  // Override from AAFSourceReference
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::GetSourceID (/*[retval][out]*/ aafUID_t *  /*pSourceID*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  // Override from AAFSourceReference
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::SetSourceID (/*[in]*/ aafUID_t   /*sourceID*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  // Override from AAFSourceReference
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::GetSourceMobSlotID (/*[retval][out]*/ aafSlotID_t *  /*pMobSlotID*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }

  // Override from AAFSourceReference
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceClip::SetSourceMobSlotID (/*[in]*/ aafSlotID_t   /*mobSlotID*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }




extern "C" const aafClassID_t CLSID_AAFSourceClip;

OMDEFINE_STORABLE(ImplAAFSourceClip, CLSID_AAFSourceClip);

