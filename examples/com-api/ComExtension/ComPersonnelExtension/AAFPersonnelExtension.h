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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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
#ifndef __AAFPersonnelExtension_h__
#define __AAFPersonnelExtension_h__

#ifndef __AAFCOMPlatform_h__
#include "AAFCOMPlatform.h"
#endif

// Include our common aaf data types
#include "AAFTypes.h"


// For local in-memory use, we'll define the following typedef for
// handy manipulation of ePosition extensible enumerations.
//
typedef aafUID_t ePosition_t;  

// Contract ID type
typedef aafUInt32 contractID_t;

// Define the reference and pointer types.
#ifdef __cplusplus
typedef const contractID_t * contractID_constptr;
typedef const contractID_t & contractID_constref;
typedef const ePosition_t & ePosition_constref;
#else
typedef contractID_t * contractID_constptr;
typedef contractID_constptr contractID_constref;
typedef ePosition_constptr ePosition_constref;
#endif


// Forward declaration of interface. This should make sure that for any "circular" 
// imports a given iterface will always have a forward declaration.
interface IAAFPersonnelResource;
interface IAAFAdminMob;



EXTERN_C const IID IID_IAAFPersonnelResource;

#undef  INTERFACE
#define INTERFACE   IAAFPersonnelResource

DECLARE_INTERFACE_(IAAFPersonnelResource, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;


  /* *** IAAFPersonnelResource methods *** */
  //
  // After creating a new instance of a PersonnelResource call
  // this method to completely initialize all of the required 
  // properties and optional properties with validation.
  //
  STDMETHOD(Initialize)(THIS_ 
                        aafCharacter_constptr pFamilyName,
                        aafCharacter_constptr pGivenName,
                        ePosition_constref position) PURE;

  //
  // Set/Get the value of the FamilyName property.
  //
  STDMETHOD(SetFamilyName)(THIS_ aafCharacter_constptr pFamilyName) PURE;
  STDMETHOD(GetFamilyNameBufLen)(THIS_ aafUInt32 *pBufSize) PURE;
  STDMETHOD(GetFamilyName)(THIS_ aafCharacter* pFamilyName, aafUInt32 bufSize) PURE;

  //
  // Set/Get the value of the GivenName property.
  //
  STDMETHOD(SetGivenName)(THIS_ aafCharacter_constptr pGivenName) PURE;
  STDMETHOD(GetGivenNameBufLen)(THIS_ aafUInt32 *pBufSize) PURE;
  STDMETHOD(GetGivenName)(THIS_ aafCharacter* pGivenName, aafUInt32 bufSize) PURE;

  //
  // Set/Get the value of the Position property.
  //
  STDMETHOD(SetPosition)(THIS_ ePosition_constref position) PURE;
  STDMETHOD(GetPosition)(THIS_ ePosition_t *      position) PURE;

  //
  // Set/Get the value of the optional contractID property.
  //
  STDMETHOD(SetContractID)(THIS_ contractID_constref contractID) PURE;
  STDMETHOD(GetContractID)(THIS_ contractID_t *      contractID) PURE;

  //
  // Set/Get the value of the optional Part property.
  //
  STDMETHOD(SetPart)(THIS_ aafCharacter_constptr pPart) PURE;
  STDMETHOD(GetPartBufLen)(THIS_ aafUInt32 *pBufSize) PURE;
  STDMETHOD(GetPart)(THIS_ aafCharacter* pPart, aafUInt32 bufSize) PURE;
 
  END_INTERFACE
};


EXTERN_C const IID IID_IAAFAdminMob;

#undef  INTERFACE
#define INTERFACE   IAAFAdminMob

DECLARE_INTERFACE_(IAAFAdminMob, IUnknown)
{
  BEGIN_INTERFACE

  /* *** IUnknown methods *** */
  STDMETHOD(QueryInterface) (THIS_ REFIID riid, void **ppvObj) PURE;
  STDMETHOD_(ULONG,AddRef) (THIS)  PURE;
  STDMETHOD_(ULONG,Release) (THIS) PURE;


  /* *** IAAFAdminMob methods *** */
  //
  // After creating a new instance of a AdminMob call
  // this method to completely initialize all of the properties.
  //
  STDMETHOD(Initialize)(THIS_ ) PURE;

  //
  // Get the current number of contained PersonnelResource objects.
  //
  STDMETHOD(CountPersonnelResources)(THIS_ aafUInt32 * count) PURE;

  //
  // Append a PersonnelResource to this object.
  //
  STDMETHOD(AppendPersonnelResource)(THIS_ IAAFPersonnelResource * pPersonnelResource) PURE;

  //
  // Get the personnel resource at the given index.
  //
  STDMETHOD(GetPersonnelResourceAt)(THIS_ 
                                    aafUInt32 index, 
                                    IAAFPersonnelResource **ppPersonnelResource) PURE;
 
  END_INTERFACE
};



/* *** Plugin code class identifiers for our two plugins. *** */
//
EXTERN_C const CLSID CLSID_AAFPersonnelResource;

EXTERN_C const CLSID CLSID_AAFAdminMob;


#endif // #ifndef __AAFPersonnelExtension_h__

