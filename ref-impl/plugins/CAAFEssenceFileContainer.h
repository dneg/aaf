//@doc
//@class    AAFEssenceFileContainer | Implementation class for AAFEssenceFileContainer
#ifndef __CAAFEssenceFileContainer_h__
#define __CAAFEssenceFileContainer_h__

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
 * prior written permission of Avid Technology, Inc.
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

  STDMETHOD (CountDefinitions)(aafUInt32 *pDefCount);
  STDMETHOD (GetIndexedDefinitionID)(aafUInt32 index, aafUID_t *result);
  STDMETHOD (GetPluginDescriptorID)(aafUID_t *result);

  STDMETHOD (GetIndexedDefinitionObject)(aafUInt32 index, IAAFDictionary *dict, IAAFDefObject **def);

  STDMETHOD (CreateDescriptor)(IAAFDictionary *dict, IAAFPluginDescriptor **desc);

  //
  // IAAFEssenceFileContainer interface methods.
  //

  // Attempt to create an essence stream in the container 
  // for reading and writing.
  STDMETHOD (CreateEssenceStream)
    (/*[in,string]*/ const aafCharacter * pName, // String key to identify the new essence within container
     /*[in]*/ aafMobID_constptr pMobID, // Optional mobID identifying the essence
     /*[out,retval]*/ IAAFEssenceStream ** ppEssenceStream); // address of local variable to contain the essence stream pointer. 

  // Attempt to create an essence stream in the container 
  // for writing.
  STDMETHOD (CreateEssenceStreamWriteOnly)
    (/*[in,string]*/ const aafCharacter * pPath, // String key to identify the new essence within container
     /*[in]*/ aafMobID_constptr pMobID, // Optional mobID identifying the essence
     /*[out,retval]*/ IAAFEssenceStream ** ppEssenceStream); // address of local variable to contain the essence stream pointer. 

  // Attempt to open an essence stream for reading.
  // Returns one of the following:
  STDMETHOD (OpenEssenceStreamReadOnly)
    (/*[in,string]*/ const aafCharacter * pFilePath, // String key to identify the existing essence within container
     /*[in]*/ aafMobID_constptr pMobID, // Optional mobID identifying the essence
     /*[out,retval]*/ IAAFEssenceStream ** ppEssenceStream); // address of local variable to contain the essence stream pointer. 

  // Attempt to open an essence stream for appending.
  // Returns one of the following:
  STDMETHOD (OpenEssenceStreamAppend)
    (/*[in,string]*/ const aafCharacter * pFilePath, // String key to identify the existing essence within container
     /*[in]*/ aafMobID_constptr pMobID, // Optional mobID identifying the essence
     /*[out,retval]*/ IAAFEssenceStream ** ppEssenceStream); // address of local variable to contain the essence stream pointer. 



protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);

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



