/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "CAAFEssenceFileContainer.h"
#include "CAAFEssenceFileStream.h"

#include <assert.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "aafErr.h"
#include "AAFStoredObjectIDs.h"

#include <errno.h>

#if defined(_MAC) || defined(macintosh)
#include <wstring.h>
#endif

const aafUID_t  EXAMPLE_FILE_PLUGIN =	{ 0x914B3AD1, 0xEDE7, 0x11d2, { 0x80, 0x9F, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };


// CLSID for AAFEssenceFileStream 
// {a7337030-c103-11d2-8089-006008143e6f}
const CLSID CLSID_AAFEssenceFileContainer = { 0xa7337030, 0xc103, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}




//
// Constructor 
//
CAAFEssenceFileContainer::CAAFEssenceFileContainer (IUnknown * pControllingUnknown)
  : CAAFUnknown(pControllingUnknown),
  _pLastFileStream(NULL)
{
}


CAAFEssenceFileContainer::~CAAFEssenceFileContainer ()
{
}


// Accessors
CAAFEssenceFileStream* CAAFEssenceFileContainer::LastFileStream() const
{ 
  return _pLastFileStream; 
}

void CAAFEssenceFileContainer::SetLastFileStream(CAAFEssenceFileStream *pLastFileStream)
{ 
  _pLastFileStream = pLastFileStream; 
}

static bool PathsAreEqual(const wchar_t *path1, const wchar_t *path2)
{
	assert(NULL != path1 && NULL != path2);
	
	if (NULL == path1 || NULL == path2)
		return false;
	
	int i = 0;
	while ((path1[i] && path2[i]) && (path1[i] == path2[i]))
		++i;
			
	return (path1[i] == path2[i]);
}

HRESULT CAAFEssenceFileContainer::CheckExistingStreams(
  const wchar_t *pwcPath,
  FileStreamMode streamMode)
{
  CAAFEssenceFileStream *pCurrentFileStream = LastFileStream();
  CAAFEssenceFileStream *pPrevFileStream = NULL;
  while (pCurrentFileStream)
  {
    // Is the file stream already open?
    const wchar_t *existingFilePath = pCurrentFileStream->FilePath();
    if (PathsAreEqual(existingFilePath, pwcPath))
    {
      // Check for invalid mode combinations.
      FileStreamMode existingFileMode = pCurrentFileStream->StreamMode();

      if (openNew == streamMode || openAppend == streamMode)
      { // The file stream is already opened for writing.
        return AAFRESULT_ALREADY_OPEN; 
      }
      else if (openRead == streamMode && openRead == existingFileMode)
      { // Continue just in case there is another file  was not opened
        // for writing. This should never happen...but I'm paranoid...
      }
      else
      {
        return AAFRESULT_ALREADY_OPEN;
      }
    }

    pPrevFileStream = pCurrentFileStream;
    pCurrentFileStream = pCurrentFileStream->PrevFileStream();
  }

  return S_OK;
}


// Set up the plugin.
HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::Start(void)
{
  return S_OK;
}

// Tear down the plugin.
HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::Finish(void)
{
  return S_OK;
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::GetNumDefinitions (aafInt32 *pDefCount)
{
	if(pDefCount == NULL)
		return AAFRESULT_NULL_PARAM;
	
	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::GetIndexedDefinitionID (aafInt32 index, aafUID_t *uid)
{
	if(uid == NULL)
		return AAFRESULT_NULL_PARAM;

	*uid = ContainerFile;		// UID of the DefObject
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::GetPluginDescriptorID (aafUID_t *uid)
{
	*uid = EXAMPLE_FILE_PLUGIN;		// UID of the PluginDescriptor
	return AAFRESULT_SUCCESS;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::GetIndexedDefinitionObject (aafInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
	aafUID_t			uid;
	IAAFContainerDef	*container = NULL;
	IAAFDefObject		*obj = NULL;
	if((dict == NULL) || (def == NULL))
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(dict->CreateInstance(&AUID_AAFContainerDef,
							IID_IAAFContainerDef, 
							(IUnknown **)&container));
		uid = ContainerFile;
		CHECK(container->SetEssenceIsIdentified(AAFFalse));
		CHECK(container->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		CHECK(obj->Init(&uid, L"Raw file Container", L"Essence is in a non-container file."));
		obj->Release();
		obj = NULL;
		CHECK(container->QueryInterface(IID_IAAFDefObject, (void **)def));
		container->Release();
		container = NULL;
	}
	XEXCEPT
	{
		if(container != NULL)
			container->Release();
		if(obj != NULL)
			obj->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;
}


static wchar_t *manufURL = L"http://www.avid.com";
static wchar_t *downloadURL = L"ftp://ftp.avid.com/pub/";
const aafUID_t MANUF_JEFFS_PLUGINS = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
static aafVersionType_t samplePluginVersion = { 0, 1 };

static wchar_t *manufName = L"Avid Technology, Inc.";
static wchar_t *manufRev = L"Rev 0.1";

HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDescriptor **descPtr)
{
	IAAFPluginDescriptor	*desc = NULL;
	IAAFLocator				*pLoc = NULL;
	IAAFNetworkLocator		*pNetLoc = NULL;
	aafUID_t				category = AUID_AAFDefObject, manufacturer = MANUF_JEFFS_PLUGINS;
	aafUID_t				plugID = EXAMPLE_FILE_PLUGIN;
	
	XPROTECT()
	{
		CHECK(dict->CreateInstance(&AUID_AAFPluginDescriptor,
			IID_IAAFPluginDescriptor, 
			(IUnknown **)&desc));
		*descPtr = desc;
		CHECK(desc->Init(&plugID, L"Essence File Container", L"Handles non-container files."));

		CHECK(desc->SetCategoryClass(&category));
		CHECK(desc->SetPluginVersionString(manufRev));
		CHECK(dict->CreateInstance(&AUID_AAFNetworkLocator,
			IID_IAAFLocator, 
			(IUnknown **)&pLoc));
		CHECK(pLoc->SetPath (manufURL));
		CHECK(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		CHECK(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		CHECK(desc->SetManufacturerID(&manufacturer));
		CHECK(desc->SetPluginManufacturerName(manufName));
		CHECK(desc->SetIsSoftwareOnly(AAFTrue));
		CHECK(desc->SetIsAccelerated(AAFFalse));
		CHECK(desc->SetSupportsAuthentication(AAFFalse));
		
		/**/
		CHECK(dict->CreateInstance(&AUID_AAFNetworkLocator,
			IID_IAAFLocator, 
			(IUnknown **)&pLoc));
		CHECK(pLoc->SetPath (downloadURL));
		CHECK(desc->AppendLocator(pLoc));
		desc->Release();
		desc = NULL;
		pLoc->Release();
		pLoc = NULL;
	}
	XEXCEPT
	{
		if(desc != NULL)
			desc->Release();
		if(pLoc != NULL)
			pLoc->Release();
		if(pNetLoc != NULL)
			pNetLoc->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::CreateEssenceStream (wchar_t *  pName,
        aafUID_t *  pMobID,
        IAAFEssenceStream ** ppEssenceStream)
{
  HRESULT hr = S_OK;
  CAAFEssenceFileStream *pEssenceFileStream = NULL;


  // Validate return argument.
  if (NULL == ppEssenceStream)
    return E_INVALIDARG;

  try
  {
    // First see if the stream has already been opened.
    checkResult(CheckExistingStreams(pName, openNew));


    // Create file stream object.
    pEssenceFileStream = CAAFEssenceFileStream::CreateFileStream(this);
 	  checkExpression(NULL != pEssenceFileStream, E_OUTOFMEMORY);
    
    // Temporarily reuse code for obsolete CAAFEssenceFileScream
    checkResult(pEssenceFileStream->Create(pName, pMobID));
    
    // Return the interface to the stream to the caller.
    checkResult(pEssenceFileStream->QueryInterface(IID_IAAFEssenceStream, (void **)ppEssenceStream));
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

  //
  // If an error occurs the following release will delete the object.
  if (pEssenceFileStream)
    pEssenceFileStream->Release();

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::CreateEssenceStreamWriteOnly (wchar_t *  pName,
        aafUID_t *  pMobID,
        IAAFEssenceStream ** ppEssenceStream)
{
  return HRESULT_NOT_IMPLEMENTED;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::OpenEssenceStreamReadOnly (wchar_t *  pName,
        aafUID_t *  pMobID,
        IAAFEssenceStream ** ppEssenceStream)
{
  HRESULT hr = S_OK;
  CAAFEssenceFileStream *pEssenceFileStream = NULL;


  // Validate return argument.
  if (NULL == ppEssenceStream)
    return E_INVALIDARG;

  try
  {
    // First see if the stream has already been opened.
    checkResult(CheckExistingStreams(pName, openRead));

    // Create file stream object.
    pEssenceFileStream = CAAFEssenceFileStream::CreateFileStream(this);
 	  checkExpression(NULL != pEssenceFileStream, E_OUTOFMEMORY);
    
    // Temporarily reuse code for obsolete CAAFEssenceFileScream
    checkResult(pEssenceFileStream->OpenRead(pName, pMobID));
    
    // Return the interface to the stream to the caller.
    checkResult(pEssenceFileStream->QueryInterface(IID_IAAFEssenceStream, (void **)ppEssenceStream));
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

  //
  // If an error occurs the following release will delete the object.
  if (pEssenceFileStream)
    pEssenceFileStream->Release();

  return hr;
}


HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::OpenEssenceStreamAppend (wchar_t *  pName,
        aafUID_t *  pMobID,
        IAAFEssenceStream ** ppEssenceStream)
{
  HRESULT hr = S_OK;
  CAAFEssenceFileStream *pEssenceFileStream = NULL;


  // Validate return argument.
  if (NULL == ppEssenceStream)
    return E_INVALIDARG;

  try
  {
    // First see if the stream has already been opened.
    checkResult(CheckExistingStreams(pName, openAppend));

    // Create file stream object.
    pEssenceFileStream = CAAFEssenceFileStream::CreateFileStream(this);
 	  checkExpression(NULL != pEssenceFileStream, E_OUTOFMEMORY);
    
    // Temporarily reuse code for obsolete CAAFEssenceFileScream
    checkResult(pEssenceFileStream->OpenAppend(pName, pMobID));
    
    // Return the interface to the stream to the caller.
    checkResult(pEssenceFileStream->QueryInterface(IID_IAAFEssenceStream, (void **)ppEssenceStream));
  }
  catch (HRESULT& rResult)
  {
    hr = rResult;
  }

  //
  // If an error occurs the following release will delete the object.
  if (pEssenceFileStream)
    pEssenceFileStream->Release();

  return hr;
}


//
// 
// 
HRESULT CAAFEssenceFileContainer::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We support the IAAFEssenceContainer interface 
    if (riid == IID_IAAFEssenceContainer) 
    { 
        *ppvObj = (IAAFEssenceContainer *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    // and the IAAFPlugin interface
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
HRESULT CAAFEssenceFileContainer::COMCreate(IUnknown *pUnkOuter, void **ppvObjOut)
{
	*ppvObjOut = NULL;
 	CAAFEssenceFileContainer *pAAFEssenceFileContainer = new CAAFEssenceFileContainer(pUnkOuter);
 	if (NULL == pAAFEssenceFileContainer)
 		return E_OUTOFMEMORY;
 	*ppvObjOut = static_cast<IAAFEssenceContainer *>(pAAFEssenceFileContainer);
 	((IUnknown *)(*ppvObjOut))->AddRef();
 	return S_OK;
}
