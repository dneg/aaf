//@doc
//@class    AAFEssenceFileContainer | Implementation class for AAFEssenceFileContainer
#ifndef __CAAFEssenceFileContainer_h__
#define __CAAFEssenceFileContainer_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#ifndef __CAAFUnknown_h__
#include "CAAFUnknown.h"
#endif

#ifndef __CAAFEssenceFileStream_h__
#include "CAAFEssenceFileStream.h"
#endif

//
// Include the standard plugin interfaces.
//
#include "AAFPlugin.h"




class CAAFEssenceFileContainer
  : public IAAFEssenceContainer,
    public IAAFPlugin,
    public CAAFUnknown
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFEssenceFileContainer (IUnknown * pControllingUnknown);
  virtual ~CAAFEssenceFileContainer ();


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  //
  // Macro also declares the IUnknown methods
  // 
  AAF_DECLARE_CONCRETE();


  //
  // IAAFPlugin interface methods.
  //

  // Set up the plugin.
  STDMETHOD (Start)(void);

  // Tear down the plugin.
  STDMETHOD (Finish)(void);
 
  STDMETHOD (GetPluginID)(aafUID_t *result);

  STDMETHOD (GetPluggableDefinition)(IAAFDictionary *dict, IAAFPluggableDef **def);

  STDMETHOD (GetDescriptor)(IAAFDictionary *dict, IAAFPluginDescriptor **desc);

  //
  // IAAFEssenceFileContainer interface methods.
  //

  // Attempt to create an essence stream in the container 
  // for reading and writing.
  STDMETHOD (CreateEssenceStream)
    (/*[in,string]*/ wchar_t *  pName, // String key to identify the new essence within container
     /*[in]*/ aafUID_t *  pMobID, // Optional mobID identifying the essence
     /*[out,retval]*/ IAAFEssenceStream ** ppEssenceStream); // address of local variable to contain the essence stream pointer. 

  // Attempt to create an essence stream in the container 
  // for writing.
  STDMETHOD (CreateEssenceStreamWriteOnly)
    (/*[in,string]*/ wchar_t *  pPath, // String key to identify the new essence within container
     /*[in]*/ aafUID_t *  pMobID, // Optional mobID identifying the essence
     /*[out,retval]*/ IAAFEssenceStream ** ppEssenceStream); // address of local variable to contain the essence stream pointer. 

  // Attempt to open an essence stream for reading.
  // Returns one of the following:
  STDMETHOD (OpenEssenceStreamReadOnly)
    (/*[in,string]*/ wchar_t *  pFilePath, // String key to identify the existing essence within container
     /*[in]*/ aafUID_t *  pMobID, // Optional mobID identifying the essence
     /*[out,retval]*/ IAAFEssenceStream ** ppEssenceStream); // address of local variable to contain the essence stream pointer. 

  // Attempt to open an essence stream for appending.
  // Returns one of the following:
  STDMETHOD (OpenEssenceStreamAppend)
    (/*[in,string]*/ wchar_t *  pFilePath, // String key to identify the existing essence within container
     /*[in]*/ aafUID_t *  pMobID, // Optional mobID identifying the essence
     /*[out,retval]*/ IAAFEssenceStream ** ppEssenceStream); // address of local variable to contain the essence stream pointer. 



protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  virtual HRESULT InternalQueryInterface(REFIID riid, void **ppvObjOut);

public:
  // Accessors
  CAAFEssenceFileStream* LastFileStream() const;
  void SetLastFileStream(CAAFEssenceFileStream *pLastFileStream);
  
  // Check to see if it is ok to create of open the stream with given
  // path and mode.
  HRESULT CheckExistingStreams(const wchar_t *pwcPath, FileStreamMode streamMode);


private:
  // List of all open file streams...
  CAAFEssenceFileStream* _pLastFileStream;
};


EXTERN_C const CLSID CLSID_AAFEssenceFileContainer;


#endif // ! __CAAFEssenceFileContainer_h__



