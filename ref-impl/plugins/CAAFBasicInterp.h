//@doc
//@class    AAFEssenceCodec | Implementation class for basic interpolators plugin
#ifndef __CAAFBasicInterp_h__
#define __CAAFBasicInterp_h__

/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


#include "AAFUtils.h"
#include "AAF.h"
#include "AAFPlugin.h"
#include "CAAFUnknown.h"
#include "AAFRational.h"

EXTERN_C const CLSID CLSID_AAFBasicInterp;

class CAAFBasicInterp
  : public IAAFInterpolator,
	public IAAFPlugin,
    public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFBasicInterp (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFBasicInterp ();

public:

  STDMETHOD (CountDefinitions)(aafUInt32 *pDefCount);
  STDMETHOD (GetIndexedDefinitionID)(aafUInt32 index, aafUID_t *result);
  STDMETHOD (GetPluginDescriptorID)(aafUID_t *result);
  STDMETHOD (GetIndexedDefinitionObject)(aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def);
  STDMETHOD (CreateDescriptor)(IAAFDictionary *dict, IAAFPluginDef **desc);

  STDMETHOD (GetNumTypesSupported)(
    /* [out] */aafUInt32*  pCount);
  STDMETHOD (GetIndexedSupportedType)(
    /* [in] */ aafUInt32  index,
    /* [out] */IAAFTypeDef ** ppType);
  STDMETHOD (GetTypeDefinition)(
    /* [out] */IAAFTypeDef ** ppTypeDef);
  STDMETHOD (SetTypeDefinition)(
    /* [in] */ IAAFTypeDef * pTypeDef);
  STDMETHOD (GetParameter)(
    /* [out] */IAAFParameter ** ppParameter);
  STDMETHOD (SetParameter)(
    /* [in] */ IAAFParameter * pParameter);
  STDMETHOD (InterpolateOne)(
    /* [in] */ aafRational_t *  pInputValue,
	/* [in] */aafUInt32	bufSize,
    /* [out] */aafMemPtr_t pOutputValue,
	/* [in] */aafUInt32	*bytesRead);
  STDMETHOD (InterpolateMany)(
	/* [in] */ aafRational_t *  pStartInputValue,
    /* [in] */ aafRational_t *  pInputStep,
    /* [in] */ aafUInt32  pGenerateCount,
    /* [out] */aafMemPtr_t pOutputValue,
    /* [out] */aafUInt32 *  ppResultCount);

protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);

  HRESULT FindBoundValues(aafRational_t point, aafUInt32 valueSize,
						AAFRational *lowerBoundTime, aafMemPtr_t lowerBoundValue,
						AAFRational *upperBoundTime, aafMemPtr_t upperBoundValue);
public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********

  // Declare the module test method. The implementation of the will be be
  // in /test/CAAFEssenceCodecTest.cpp.
  static HRESULT test();

private:
	IAAFTypeDef		*_typeDef;
	IAAFParameter	*_parameter;
};

#endif // ! __CAAFBasicInterp_h__

