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
    if (0 == wcscmp(existingFilePath, pwcPath))
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
    CAAFEssenceFileContainer::GetPluginID (aafUID_t *uid)
{
	*uid = ContainerFile;		// UID of the "file" container definition
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::GetPluggableDefinition (IAAFDictionary *dict, IAAFPluggableDef **def)
{
	aafUID_t			uid;
	IAAFContainerDef	*container;
	IAAFDefObject		*obj;
	XPROTECT()
	{
		CHECK(dict->CreateInstance(&AUID_AAFContainerDef,
							IID_IAAFContainerDef, 
							(IUnknown **)&container));
		uid = ContainerFile;
		CHECK(container->SetEssenceIsIdentified(AAFFalse));
		CHECK(container->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		CHECK(obj->Init(&uid, L"Raw file Container", L"Essence is in a non-container file."));
		CHECK(container->QueryInterface(IID_IAAFPluggableDef, (void **)def));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFEssenceFileContainer::GetDescriptor (IAAFDictionary *dict, IAAFPluginDescriptor **desc)
{
	XPROTECT()
	{
		CHECK(dict->CreateInstance(&AUID_AAFPluginDescriptor,
							IID_IAAFPluginDescriptor, 
							(IUnknown **)desc));
	}
	XEXCEPT
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
