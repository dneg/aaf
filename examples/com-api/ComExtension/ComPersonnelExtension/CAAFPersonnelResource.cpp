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
#include "CAAFPersonnelResource.h"

#include <assert.h>
#include "AAFResult.h"

#include "AAF.h"

#include "extensionUtils.h"

#include "aafDataDefs.h"
#include "aafDefUIDs.h"
#include "AAFStoredObjectIDs.h"

#include "CAAFBuiltinDefs.h"




// Both plugins currently only support a single definition
const aafInt32 kSupportedDefinitions = 1;


const wchar_t kPersonnelResourceDisplayName[] = L"Example AAF Personel Resource Class Extension";
const wchar_t kPersonnelResourceDescription[] = L"Handles Access to Personnel Resource AAF objects";

const wchar_t kPersonnelMobDisplayName[] = L"Example AAF Personel Mob Class Extension";
const wchar_t kPersonnelMobDescription[] = L"Handles Access to Personnel Mob AAF objects";

const aafProductVersion_t kAAFPluginVersion = {1, 0, 0, 1, kVersionBeta};

//
// Plugin Descriptor information
//
// {A9757BD4-A046-11d3-BFFA-00104BC9156D}
static const aafUID_t AVID_PERSONNELRESOURCE_PLUGIN = 
{ 0xa9757bd4, 0xa046, 0x11d3, { 0xbf, 0xfa, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };


static wchar_t *kManufURL = L"http://www.avid.com";
static wchar_t *kDownloadURL = L"ftp://ftp.avid.com/pub/";
static aafVersionType_t samplePluginVersion = { 0, 1 };

static wchar_t *kManufName = L"Avid Technology, Inc.";
static wchar_t *kManufRev = L"Rev 0.1";

// Should be shared by all built-in plugins created by AVID. /* TRR */
const aafUID_t MANUF_AVID_PLUGINS = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };


const aafUID_t NULL_ID = {0};

// local function for simplifying error handling.
inline void checkResult(HRESULT r)
{
  if (S_OK != r)
    throw r;
}

inline void checkExpression(bool test, HRESULT r)
{
  if (!test)
    throw r;
}

inline void checkAssertion(bool test)
{
  if (!test)
    throw HRESULT(AAFRESULT_ASSERTION_VIOLATION);
}




// Constructor

CAAFPersonnelResource::CAAFPersonnelResource (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown),
  _bInitializedObject(false),
  _pObject(NULL)
{
}


// Desctructor

CAAFPersonnelResource::~CAAFPersonnelResource ()
{
}



// This is an interface pointer back into the object that owns this extension.
// since extensions are implemented as an aggregate the object the object
// lifetimes are tied together. This means that we do not reference count
// this interface pointer. If we did then this extension as well as the 
// controlling/owning object will never be deleted.
HRESULT CAAFPersonnelResource::InitializeObject()
{
  HRESULT hr = S_OK;

  if (!_bInitializedObject)
  {
    // See if this objects controlling unknown supports the
    // IAAFObject interface. This should NOT fail!
    hr = QueryInterface(IID_IAAFObject, (void **)&_pObject);
    if (SUCCEEDED(hr))
    {
      // don't reference count this interface.
      _pObject->Release();

      _bInitializedObject = true;
    }
  }

  return hr;
}


HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::Initialize
     (aafCharacter_constptr pFamilyName,
      aafCharacter_constptr pGivenName,
      ePosition_constref position)
{
  HRESULT hr = S_OK;

  if (NULL == pGivenName || NULL == pFamilyName)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceInitialize(
      _pObject, 
      const_cast<aafCharacter *>(pFamilyName), 
      const_cast<aafCharacter *>(pGivenName), 
      position);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}


HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::SetGivenName
     (aafCharacter_constptr pGivenName)
{
  HRESULT hr = S_OK;

  if (NULL == pGivenName)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceSetGivenName(_pObject, const_cast<aafCharacter *>(pGivenName));
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::GetGivenNameBufLen
     (aafUInt32 *pBufSize)
{
  HRESULT hr = S_OK;

  if (NULL == pBufSize)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceGetGivenNameBufLen(_pObject, pBufSize);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::GetGivenName
     (aafCharacter* pGivenName, aafUInt32 bufSize)
{
  HRESULT hr = S_OK;
  if (NULL == pGivenName)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceGetGivenName(_pObject, pGivenName, bufSize);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::SetFamilyName
     (aafCharacter_constptr pFamilyName)
{
  HRESULT hr = S_OK;
  if (NULL == pFamilyName)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceSetFamilyName(_pObject, const_cast<aafCharacter *>(pFamilyName));
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::GetFamilyNameBufLen
     (aafUInt32 *pBufSize)
{
  HRESULT hr = S_OK;
  if (NULL == pBufSize)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceGetFamilyNameBufLen(_pObject, pBufSize);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::GetFamilyName
     (aafCharacter* pFamilyName, aafUInt32 bufSize)
{
  HRESULT hr = S_OK;
  if (NULL == pFamilyName)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceGetFamilyName(_pObject, pFamilyName, bufSize);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::SetPosition
     (ePosition_constref position)
{
  HRESULT hr = S_OK;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceSetPosition(_pObject, position);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::GetPosition
     (ePosition_t *        position)
{
  HRESULT hr = S_OK;
  if (NULL == position)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceGetPosition(_pObject, position);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::SetContractID
     (contractID_constref contractID)
{
  HRESULT hr = S_OK;

  try
  {
    checkResult(InitializeObject());

    PersonnelResourceSetContractID(_pObject, contractID);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::GetContractID
     (contractID_t *      contractID)
{
  HRESULT hr = S_OK;
  if (NULL == contractID)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());

    hr = PersonnelResourceGetContractID(_pObject, contractID);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::SetPart
     (aafCharacter_constptr pPart)
{
  HRESULT hr = S_OK;
  if (NULL == pPart)
    return E_INVALIDARG;


  try
  {
    checkResult(InitializeObject());

    PersonnelResourceSetActorRole(_pObject, const_cast<aafCharacter *>(pPart));
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::GetPartBufLen
     (aafUInt32 *pBufSize)
{
  HRESULT hr = S_OK;
  if (NULL == pBufSize)
    return E_INVALIDARG;


  try
  {
    checkResult(InitializeObject());

    hr = PersonnelResourceGetActorNameBufLen(_pObject, pBufSize);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFPersonnelResource::GetPart
     (aafCharacter* pPart, aafUInt32 bufSize)
{
  HRESULT hr = S_OK;
  if (NULL == pPart)
    return E_INVALIDARG;


  try
  {
    checkResult(InitializeObject());

    hr = PersonnelResourceGetActorName(_pObject, pPart, bufSize);
  }
  catch (HRESULT &rhr)
  {
    // Catch any explicitly thrown by this plugin or utility function.
    hr = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
    //
    hr = AAFRESULT_UNEXPECTED_EXCEPTION;
  }

  return hr;
}





HRESULT STDMETHODCALLTYPE
    CAAFPersonnelResource::Start (void)
{
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPersonnelResource::Finish (void)
{
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPersonnelResource::GetNumDefinitions (aafInt32 *pDefCount)
{
	if(NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = kSupportedDefinitions;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPersonnelResource::GetIndexedDefinitionID (aafInt32 index, aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = kClassID_PersonnelResource;		// UID of the JPEG codec definition

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFPersonnelResource::GetPluginDescriptorID (aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AVID_PERSONNELRESOURCE_PLUGIN;		// UID of the PluginDescriptor

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE CAAFPersonnelResource::GetIndexedDefinitionObject(
    aafInt32 index, 
    IAAFDictionary *pDictionary, 
    IAAFDefObject **pDef)
{
	HRESULT hr = S_OK;
	IAAFClassDef	*pClassDef = NULL;

  
	if((pDictionary == NULL) || (pDef == NULL))
		return AAFRESULT_NULL_PARAM;

	// Don't we need to validate the index as well? /* TRR??? */
	if (0 > kSupportedDefinitions || kSupportedDefinitions <= index)
		return AAFRESULT_BADINDEX;

	try
	{
		//!!!Later, add in dataDefs supported & filedescriptor class

    // Register the extensible enumeration describing Position in the
    // dictionary.
    CreateAndRegisterPositionEnum (pDictionary);

    // Create a class definition describing PesonnelResource objects and
    // register it in the dictionary.
    CreateAndRegisterPersonnelResource (pDictionary);

    checkResult(pDictionary->LookupClassDef(kClassID_PersonnelResource, &pClassDef));
    checkResult(pClassDef->QueryInterface(IID_IAAFDefObject, (void **)pDef));
  }
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

  // Cleanup
  if (pClassDef)
    pClassDef->Release();

	return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFPersonnelResource::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDescriptor **descPtr)
{
	HRESULT hr = S_OK;
	IAAFPluginDescriptor	*desc = NULL;
	IAAFLocator				*pLoc = NULL;
 	IAAFNetworkLocator		*pNetLoc = NULL;
	
	if ((NULL == dict) || (NULL == descPtr))
		return AAFRESULT_NULL_PARAM;
	
	// Initialize return parameter.
	*descPtr = NULL;

	try
	{
	    CAAFBuiltinDefs defs (dict);

		checkResult(dict->CreateInstance(defs.cdPluginDescriptor(),
			IID_IAAFPluginDescriptor, 
			(IUnknown **)&desc));

		checkResult(desc->Initialize(AVID_PERSONNELRESOURCE_PLUGIN,
		                       const_cast<wchar_t *>(kPersonnelResourceDisplayName),
		                       const_cast<wchar_t *>(kPersonnelResourceDescription)));
		checkResult(desc->SetCategoryClass(AUID_AAFDefObject));
		checkResult(desc->SetPluginVersionString(kManufRev));
		checkResult(desc->SetManufacturerID(MANUF_AVID_PLUGINS));
		checkResult(desc->SetPluginManufacturerName(kManufName));
		checkResult(desc->SetIsSoftwareOnly(AAFTrue));
		checkResult(desc->SetIsAccelerated(AAFFalse));
		checkResult(desc->SetSupportsAuthentication(AAFFalse));

		// Create the network locator for the Manufacturer's web site: 
		checkResult(dict->CreateInstance(defs.cdNetworkLocator(),
			IID_IAAFLocator, 
			(IUnknown **)&pLoc));
		checkResult(pLoc->SetPath (kManufURL));
		checkResult(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		checkResult(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		
		// Create a Network locator to point to our default download site.
		checkResult(dict->CreateInstance(defs.cdNetworkLocator(),
			IID_IAAFLocator, 
			(IUnknown **)&pLoc));
		checkResult(pLoc->SetPath (kDownloadURL));
		checkResult(desc->AppendLocator(pLoc));
	
		pLoc->Release();
		pLoc = NULL;

		*descPtr = desc; // reference count already incremented, we do not need to call AddRef()
		desc = NULL;
	}
	catch (HRESULT& rhr)
	{
		hr = rhr; // return thrown error code.
	}
	catch (...)
	{
		// We CANNOT throw an exception out of a COM interface method!
		// Return a reasonable exception code.
		hr = AAFRESULT_UNEXPECTED_EXCEPTION;
	}

	// Cleanup...
	if (NULL != desc)
		desc->Release();
	if (NULL != pLoc)
		pLoc->Release();
	if (NULL != pNetLoc)
		pNetLoc->Release();


	return hr;
}


//
// COM Infrastructure
// 

// What interfaces does this plugin support
// Override of CAAFUnknown method.
HRESULT CAAFPersonnelResource::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We support the IAAFPersonnelResource interface 
    if (riid == IID_IAAFPersonnelResource) 
    { 
        *ppvObj = (IAAFPersonnelResource *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
		// and the IAAFPlugin interface.
    else if (riid == IID_IAAFPlugin) 
    { 
        *ppvObj = (IAAFPlugin *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFPersonnelResource)
