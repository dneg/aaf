//@doc
//@class    AAFEssenceCodec | Implementation class for basic interpolators plugin
#ifndef __CAAFBasicInterp_h__
#define __CAAFBasicInterp_h__

/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/


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
  CAAFBasicInterp (IUnknown * pControllingUnknown, aafBool doInit = AAFTrue);
  virtual ~CAAFBasicInterp ();

public:


  // Set up a codec.
  STDMETHOD (Start)
     (void);

  // Tear down a codec.
  STDMETHOD (Finish)
     (void);

  STDMETHOD (GetNumDefinitions)(aafInt32 *pDefCount);
  STDMETHOD (GetIndexedDefinitionID)(aafInt32 index, aafUID_t *result);
  STDMETHOD (GetPluginDescriptorID)(aafUID_t *result);
  STDMETHOD (GetIndexedDefinitionObject)(aafInt32 index, IAAFDictionary *dict, IAAFDefObject **def);
  STDMETHOD (CreateDescriptor)(IAAFDictionary *dict, IAAFPluginDescriptor **desc);

  STDMETHOD (GetNumTypesSupported)(
    /* [out] */aafInt32*  pCount);
  STDMETHOD (GetIndexedSupportedType)(
    /* [in] */ aafInt32  index,
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
    /* [in] */ aafInt32  pGenerateCount,
    /* [out] */aafMemPtr_t pOutputValue,
    /* [out] */aafUInt32 *  ppResultCount);

protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  virtual HRESULT InternalQueryInterface(REFIID riid, void **ppvObjOut);

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

