//@doc
//@class    AAFDefObject | Implementation class for AAFDefObject
#ifndef __ImplAAFDefObject_h__
#define __ImplAAFDefObject_h__

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

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

class ImplAAFDictionary;

#include "ImplAAFPluginDescriptor.h"
#include "ImplEnumAAFPluginDescriptors.h"

class ImplAAFDefObject : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDefObject ();

protected:
  virtual ~ImplAAFDefObject ();

public:

  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
        // @parm [in] Pointer to an AUID reference
        (const aafUID_t & id,
		 const wchar_t *name,
		 const wchar_t *description);

  //****************
  // GetAUID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetAUID
        // @parm [retval,out] Pointer to an AUID reference
        (aafUID_t * pAuid) const;

  //****************
  // SetAUID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetAUID
        // @parm [in] AUID by which this object is to be identified
        (const aafUID_t & id);


  //****************
  // SetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetName
        (const aafCharacter *  name);  //@parm [in, ref] Definition Name


  //****************
  // GetName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetName
        (// @parm [out, string, size_is(bufSize)] buffer into which Name is to be written
         wchar_t *  pName,

         // @parm [in] size of *pName buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNameBufLen
        (aafUInt32 *  nameLen);  //@parm [in,out] Definition Name length


  //****************
  // SetDescription()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDescription
        (const aafCharacter * description);  //@parm [in, ref] Definition description


  //****************
  // GetDescription()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescription
        (aafWChar *  description,  //@parm [in] Definition Description
		 aafUInt32 bufSize);	  //@parm [in] size of the buffer required to hold Definition Description + terminator


  //****************
  // GetDescriptionBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescriptionBufLen
        (aafUInt32 *  descriptionLen);  //@parm [in,out] Definition description length

  //****************
  // AppendPluginDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendPluginDef
        // @parm [in] PluginDef to append
        (ImplAAFPluginDescriptor * pPluginDef);


  //****************
  // PrependPluginDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependPluginDef
        // @parm [in] PluginDef to append
        (ImplAAFPluginDescriptor * pPluginDef);

  //****************
  // GetPluginDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginDefs
        // @parm [out, retval] AAFPluginDef Enumeration
        (ImplEnumAAFPluginDescriptors ** ppEnum);

  //****************
  // CountPluginDefs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountPluginDefs
        // @parm [out, retval] Total number of plugin descriptor objects
        (aafUInt32 * pResult);

  //****************
  // InsertPluginDefAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertPluginDefAt
         // @parm [in] Index where plugin descriptor is to be inserted
        (aafUInt32 index,

		 // @parm [in] Plugin descriptor to insert
		 ImplAAFPluginDescriptor * pPluginDef);

  //****************
  // GetPluginDefAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPluginDefAt
         // @parm [in] Index of plugin descriptor to retrieve
        (aafUInt32 index,

		 // @parm [out] Retrieved plugin descriptor
		 ImplAAFPluginDescriptor ** ppPluginDef);

  //****************
  // RemovePluginDefAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemovePluginDefAt
         // @parm [in] Index of plugin descriptor to be removed
        (aafUInt32 index);

  virtual const OMUniqueObjectIdentification& identification(void) const;

private:
  // friendly name of this definition
  OMWideStringProperty          _name;

  // brief text description of this definition
  OMWideStringProperty          _description;

  // auid to be used to identify this definition
  OMFixedSizeProperty<aafUID_t> _identification;
  OMVariableSizeProperty<aafUID_t> _descriptors;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFDefObject> ImplAAFPropDefObjectSP;

#endif // ! __ImplAAFDefObject_h__
