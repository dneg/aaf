//@doc
//@class    AAFCodecDef | Implementation class for AAFCodecDef
#ifndef __ImplAAFCodecDef_h__
#define __ImplAAFCodecDef_h__


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



class ImplAAFClassDef;

class ImplEnumAAFCodecFlavours;



#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#include "ImplEnumAAFDataDefs.h"

class ImplAAFCodecDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFCodecDef ();

protected:
  virtual ~ImplAAFCodecDef ();

public:


  //****************
  // IsEssenceKindSupported()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsEssenceKindSupported
        (// @parm [in,ref] The essence kind
         const aafUID_t & essenceKind,

         // @parm [out,retval] Is this type supported
         aafBool*  pIsSupported);
 

  //****************
  // AddEssenceKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddEssenceKind
        // @parm [in] The essence kind
        (const aafUID_t & essenceKind);



  //****************
  // RemoveEssenceKind()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveEssenceKind
        // @parm [in] The essence kind to remove
        (const aafUID_t & essenceKind);



  //****************
  // CountEssenceKinds()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountEssenceKinds
        // @parm [out, retval] The returned number of essece kinds
        (aafUInt32 * pResult);


  //****************
  // GetEssenceKinds()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEssenceKinds
        // @parm [out, retval] The enumerator over essence kinds
        (ImplEnumAAFDataDefs ** ppEnum);


  //****************
  // AreThereFlavours()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AreThereFlavours
        // @parm [out,retval] True if there are flavours of this codec
        (aafBool *  pResult);


  //****************
  // GetFileDescriptorClass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileDescriptorClass
        // @parm [out] Returned file descriptor class object
        (ImplAAFClassDef ** ppClass);


  //****************
  // SetFileDescriptorClass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFileDescriptorClass
        // @parm [in] File descriptor class object
        (ImplAAFClassDef * pClass);


  //****************
  // EnumCodecFlavours()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnumCodecFlavours
        // @parm [out,retval] Codec flavour Enumeration
        (ImplEnumAAFCodecFlavours ** ppEnum);
  

private:
	DataDefWeakRefArrayProp_t			_dataDefs;
	OMFixedSizeProperty<aafUID_t>		_fileDescClass;
};

#endif // ! __ImplAAFCodecDef_h__


