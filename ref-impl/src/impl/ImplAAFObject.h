//@doc
//@class    AAFObject | Implementation class for AAFObject
#ifndef __ImplAAFObject_h__
#define __ImplAAFObject_h__

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


//
// Forward declarations
//
class ImplAAFClassDef;
class ImplAAFIdentification;
class ImplAAFProperty;
class ImplAAFPropertyDef;
class ImplAAFPropertyValue;
class ImplPropertyCollection;
class ImplAAFDictionary;

template <class T> 
class ImplAAFEnumerator;
typedef ImplAAFEnumerator<ImplAAFProperty> ImplEnumAAFProperties;


#include "AAFTypes.h"
#include "OMStorable.h"
#include "OMFixedSizeProperty.h"
#include "ImplAAFStorable.h"

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

class ImplAAFObject : public ImplAAFStorable
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFObject ();
  virtual ~ImplAAFObject ();


  //****************
  // EnableGenerationTracking()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    EnableGenerationTracking ();


  //****************
  // DisableGenerationTracking()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    DisableGenerationTracking ();


  //****************
  // IsGenerationTracked()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsGenerationTracked
        // @parm [out] set to true if tracked, false otherwise.
        (aafBoolean_t * pResult) const;


  //****************
  // GetGeneration()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetGeneration
		// @parm [out] Identification corresponding to this object's
		// generation
        (ImplAAFIdentification **  ppResult);


  //****************
  // GetGenerationAUID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetGenerationAUID
		// @parm [out] Generation ID into which this object's generation is to be written
        (aafUID_t *  pGeneration);


  //****************
  // GetObjectClass()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	GetObjectClass
		(aafUID_t * pClass);


  //****************
  // GetProperties()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	GetProperties
		(ImplEnumAAFProperties ** ppEnum);


  //****************
  // CountProperties()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	CountProperties
		(aafUInt32 * pCount);


  //****************
  // GetPropertyValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	GetPropertyValue
		(ImplAAFPropertyDef * pPropDef,
		 ImplAAFPropertyValue ** ppPropVal);


  //****************
  // SetPropertyValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	SetPropertyValue
		(ImplAAFPropertyDef * pPropDef,
		 ImplAAFPropertyValue * pPropVal);


  //****************
  // IsPropertyPresent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
	IsPropertyPresent
		(ImplAAFPropertyDef * pPropDef,
		 aafBool * pResult);


  //****************
  // RemoveOptionalProperty()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveOptionalProperty
        // @parm [in] property definition indentifying desired property
        (ImplAAFPropertyDef * pPropDef);
        

  //****************
  // CreatePropertyValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateOptionalPropertyValue
        (// @parm [in] property definition indentifying desired property
         ImplAAFPropertyDef * pPropDef,

         // @parm [out] returned AAFPropertyValue
         ImplAAFPropertyValue ** ppPropVal);



  // Interfaces ivisible inside the toolkit, but not exposed through the API
protected:

  // Associate the existing OMProperties with corresponding property definitions from
  // the given class definition. NOTE: This call is recursive, it calls itself again
  // for the parent class of the given class until current class is a "root" class.
  virtual void InitOMProperties (ImplAAFClassDef * pClassDef);
public:
  
  // Same as above for a single property (not recursive).
  virtual OMProperty * InitOMProperty(ImplAAFPropertyDef * pPropertyDef, OMPropertySet * ps);


  // Gets the head object of the file containing this object.
  // This function is used to maintain MOB and Definition tables in the
  // head object.
  virtual AAFRESULT MyHeadObject
    (class ImplAAFHeader **header) const;

  // Gets the dictionary used to create this instance.
  virtual AAFRESULT STDMETHODCALLTYPE 
    GetDictionary(ImplAAFDictionary **ppDictionary) const;


  void pvtSetSoid (const aafUID_t & id);

  virtual const OMClassId& classId(void) const;

  // Create and intialize associated external extensions.
  AAFRESULT InitializeExtensions(void);

  // Remembers the given property so it will be deleted with this
  // object.
  void RememberAddedProp (OMProperty * pProp);

  aafBoolean_t pvtIsGenerationTracked() const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

  
  // Overrides of ImplAAFStorable.
  // Return true if this is a meta object
  // NOTE: These objects will eventually owned by the Object Manager.
  virtual bool metaObject(void) const;
  
  // Return true is this is a data object (Interchange object).
  virtual bool dataObject(void) const;

private:

  OMFixedSizeProperty<aafUID_t> _generation;

  // private method
  AAFRESULT InitProperties ();

  ImplPropertyCollection * _pProperties;

  // stored object ID
  aafUID_t                 _soid;

  // 
  // The following section is intended only to delete added properties
  // when this object goes away.
  // 
  // Private class to manage saved properties
  class SavedProp
  {
  public:
	SavedProp (OMProperty * p);
	~SavedProp ();

  private:
	// Disallow copy and assignment, so declare them private and don't
	// implement!
	SavedProp (const SavedProp&);
	SavedProp& operator= (const SavedProp&);

	OMProperty * _p;
  };
  //
  // pointer to array of pointers to saved properties.
  SavedProp ** _apSavedProps;
  //
  // size of _pSavedProps array.
  aafUInt32 _savedPropsSize;
  //
  // number of those props that are actually used
  aafUInt32 _savedPropsCount;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFObject> ImplAAFObjectSP;


#endif // ! __ImplAAFObject_h__
