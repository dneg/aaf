//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=
#include "CAAFAdminMob.h"

#include <assert.h>
#include <string.h>
#include "AAFResult.h"

#include "AAF.h"

#include "extensionUtils.h"

#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"

#include "CAAFBuiltinDefs.h"




// Both plugins currently only support a single definition
const aafUInt32 kSupportedDefinitions = 1;


const wchar_t kAdminMobDisplayName[] = L"Example AAF Admin Mob Class Extension";
const wchar_t kAdminMobDescription[] = L"Handles Access to Personnel Mob AAF objects";

//
// Plugin Descriptor information
//
// {A9757BD4-A046-11d3-BFFA-00104BC9156D}
static const aafUID_t AVID_PERSONNELMOB_PLUGIN = 
{ 0xa9757bd5, 0xa046, 0x11d3, { 0xbf, 0xfa, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };


static wchar_t *kManufURL = L"http://www.avid.com";
static wchar_t *kDownloadURL = L"ftp://ftp.avid.com/pub/";

static wchar_t *kManufName = L"Avid Technology, Inc.";
static wchar_t *kManufRev = L"Rev 0.1";

// Should be shared by all built-in plugins created by AVID. /* TRR */
const aafUID_t MANUF_AVID_PLUGINS = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };


// local function for simplifying error handling.
inline void checkResult(AAFRESULT r)
{
  if (AAFRESULT_SUCCESS != r)
    throw HRESULT(r);
}

inline void checkExpression(bool test, AAFRESULT r)
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

CAAFAdminMob::CAAFAdminMob (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown),
  _bInitializedObject(false),
  _pObject(NULL),
  _pMob(NULL)
{
}


// Desctructor

CAAFAdminMob::~CAAFAdminMob ()
{
}



// This is an interface pointer back into the object that owns this extension.
// since extensions are implemented as an aggregate the object the object
// lifetimes are tied together. This means that we do not reference count
// this interface pointer. If we did then this extension as well as the 
// controlling/owning object will never be deleted.
HRESULT CAAFAdminMob::InitializeObject()
{
  HRESULT hr = S_OK;

  if (!_bInitializedObject)
  {
    if (NULL == _pObject)
    {
      // See if this objects controlling unknown supports the
      // IAAFObject interface. This should NOT fail!
      hr = QueryInterface(IID_IAAFObject, (void **)&_pObject);
      if (SUCCEEDED(hr))
      {
        // don't reference count this interface.
        _pObject->Release();
      }
    }

    if (NULL == _pMob)
    {
      // See if this objects controlling unknown supports the
      // IAAFMob interface. This should NOT fail!
      // Since we are using the extensionUtils routines we
      // also cache the Mob interface pointer.
      hr = QueryInterface(IID_IAAFMob, (void **)&_pMob);
      if (SUCCEEDED(hr))
      {
        // don't reference count this interface.
        _pMob->Release();
      }
    }

    if (SUCCEEDED(hr))
      _bInitializedObject = true;
  }

  return hr;
}


HRESULT STDMETHODCALLTYPE
CAAFAdminMob::Initialize
     (void)
{
  HRESULT hr = S_OK;

  try
  {
    hr = InitializeObject();
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
CAAFAdminMob::CountPersonnelResources
     (aafUInt32 * count)
{
  HRESULT hr = S_OK;

  if (NULL == count)
    return E_INVALIDARG;

  
  try
  {
    checkResult(InitializeObject());
    AdminMobGetNumPersonnel (NULL, _pMob, count);
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
CAAFAdminMob::AppendPersonnelResource
     (IAAFPersonnelResource * pPersonnelResource)
{
  HRESULT hr = S_OK;
  IAAFDictionary *pDictionary = NULL;
  IAAFObject *pPersonnelObject = NULL;

  if (NULL == pPersonnelResource)
    return E_INVALIDARG;

  try
  {
    checkResult(InitializeObject());
    checkResult(_pObject->GetDictionary(&pDictionary));
    checkResult(pPersonnelResource->QueryInterface(IID_IAAFObject, (void **)&pPersonnelObject));
    AdminMobAppendResource(pDictionary, _pMob, pPersonnelObject);
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

  // Cleanup
  if (pPersonnelObject)
    pPersonnelObject->Release();
  if (pDictionary)
    pDictionary->Release();

  return hr;
}



HRESULT STDMETHODCALLTYPE
CAAFAdminMob::GetPersonnelResourceAt
     (aafUInt32 index, IAAFPersonnelResource **ppPersonnelResource)
{
  HRESULT hr = S_OK;
  aafUInt32 count = 0;
  IAAFObject *pPersonnelResource = NULL;

  if (NULL == ppPersonnelResource)
    return E_INVALIDARG;

  
  try
  {
    checkResult(CountPersonnelResources(&count)); // calls InitializeObject
    checkExpression(index < count, E_INVALIDARG);

    AdminMobGetNthPersonnel(NULL, _pMob, index, &pPersonnelResource); 
    checkResult(pPersonnelResource->QueryInterface(IID_IAAFPersonnelResource, (void **)ppPersonnelResource));  
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

  // Cleanup
  if (pPersonnelResource)
    pPersonnelResource->Release();

  return hr;
}





HRESULT STDMETHODCALLTYPE
    CAAFAdminMob::CountDefinitions (aafUInt32 *pDefCount)
{
	if(NULL == pDefCount)
		return AAFRESULT_NULL_PARAM;

	*pDefCount = kSupportedDefinitions;

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFAdminMob::GetIndexedDefinitionID (aafUInt32 /* index */, aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = kClassID_AdminMob;		// UID of the AdminMob class definition

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFAdminMob::GetPluginDescriptorID (aafUID_t *uid)
{
	if(NULL == uid)
		return AAFRESULT_NULL_PARAM;

	*uid = AVID_PERSONNELMOB_PLUGIN;		// UID of the PluginDescriptor

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE CAAFAdminMob::GetIndexedDefinitionObject(
    aafUInt32 index, 
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
    // Make sure all of the definitions are registered.
		checkResult(RegisterDefinitions(pDictionary));

    checkResult(pDictionary->LookupClassDef(kClassID_AdminMob, &pClassDef));
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
    CAAFAdminMob::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDef **descPtr)
{
	HRESULT hr = S_OK;
	IAAFPluginDef			*desc = NULL;
	IAAFLocator				*pLoc = NULL;
 	IAAFNetworkLocator		*pNetLoc = NULL;
	
	if ((NULL == dict) || (NULL == descPtr))
		return AAFRESULT_NULL_PARAM;
	
	// Initialize return parameter.
	*descPtr = NULL;

	try
	{
	    CAAFBuiltinDefs defs (dict);

		checkResult(defs.cdPluginDef()->
					CreateInstance(IID_IAAFPluginDef, 
								   (IUnknown **)&desc));

		checkResult(desc->Initialize(AVID_PERSONNELMOB_PLUGIN,
		                       const_cast<wchar_t *>(kAdminMobDisplayName),
		                       const_cast<wchar_t *>(kAdminMobDescription)));
		checkResult(desc->SetCategoryClass(AUID_AAFDefObject));
		checkResult(desc->SetPluginVersionString(kManufRev));
		checkResult(desc->SetManufacturerID(MANUF_AVID_PLUGINS));
		checkResult(desc->SetPluginManufacturerName(kManufName));
		checkResult(desc->SetIsSoftwareOnly(kAAFTrue));
		checkResult(desc->SetIsAccelerated(kAAFFalse));
		checkResult(desc->SetSupportsAuthentication(kAAFFalse));

		// Create the network locator for the Manufacturer's web site: 
		checkResult(defs.cdNetworkLocator()->
					CreateInstance(IID_IAAFLocator, 
								   (IUnknown **)&pLoc));
		checkResult(pLoc->SetPath (kManufURL));
		checkResult(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		checkResult(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		
		// Create a Network locator to point to our default download site.
		checkResult(defs.cdNetworkLocator()->
					CreateInstance(IID_IAAFLocator, 
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


HRESULT STDMETHODCALLTYPE
    CAAFAdminMob::RegisterDefinitions (IAAFDictionary *pDictionary)
{
	HRESULT hr = S_OK;

  
	if (pDictionary == NULL)
		return AAFRESULT_NULL_PARAM;

	try
	{
		//!!!Later, add in dataDefs supported & filedescriptor class

    // Register the extensible enumeration describing Position in the
    // dictionary.
    CreateAndRegisterPositionEnum (pDictionary);

    // Create a class definition describing PesonnelResource objects and
    // register it in the dictionary.
    CreateAndRegisterPersonnelResource (pDictionary);

    // Create a type definition describing references to
    // PersonnelResource objects, and register it.
    CreateAndRegisterPersonnelResourceReference (pDictionary);

    // Create a type definition describing vectors of references to
    // PersonnelResource objects, and register it.
    CreateAndRegisterPersonnelResourceReferenceVector (pDictionary);

    // Create a class definition describing AdminMob objects, and
    // register it.
    CreateAndRegisterAdminMob (pDictionary);
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

	return hr;
}

//
// COM Infrastructure
// 

// What interfaces does this plugin support
// Override of CAAFUnknown method.
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFAdminMob::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We support the IAAFAdminMob interface 
    if (EQUAL_UID(riid,IID_IAAFAdminMob)) 
    { 
        *ppvObj = (IAAFAdminMob *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
		// and the IAAFPlugin interface.
    else if (EQUAL_UID(riid,IID_IAAFPlugin)) 
    { 
        *ppvObj = (IAAFPlugin *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
		// and the IAAFClassExtension interface.
    else if (EQUAL_UID(riid,IID_IAAFClassExtension)) 
    { 
        *ppvObj = (IAAFClassExtension *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFAdminMob)
