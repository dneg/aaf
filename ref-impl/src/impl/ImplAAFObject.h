//@doc
//@class    AAFObject | Implementation class for AAFObject
#ifndef __ImplAAFObject_h__
#define __ImplAAFObject_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


//
// Forward declarations
//
class ImplEnumAAFProperties;
class ImplAAFClassDef;
class ImplAAFProperty;
class ImplAAFPropertyDef;
class ImplAAFPropertyValue;
class ImplPropertyCollection;
class ImplAAFDictionary;

#include "AAFTypes.h"
#include "OMStorable.h"
#include "OMProperty.h"
#include "ImplAAFRoot.h"



class ImplAAFObject : public OMStorable, public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFObject ();
  virtual ~ImplAAFObject ();


  //****************
  // SetGeneration()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetGeneration
		// @parm [in] Generation ID to which this object is to be set
        (aafUID_t *  pGeneration);


  //****************
  // GetGeneration()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetGeneration
		// @parm [out] Generation ID into which this object's generation is to be written
        (aafUID_t *  pGeneration);


  //****************
  // GetDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDefinition
		// @parm [out] class definition of which this object is an instance.
        (ImplAAFClassDef ** ppClassDef);


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


  //***********************************************************
  // METHOD NAME: GetStoredByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetStoredByteOrder |
  // Returns the "Endian-ness" in which the current object was or will
  // be stored.  If this is a transient object (i.e., one which has
  // not been persisted) then it will return the native byte order of
  // the platform on which this is running.
  //
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetStoredByteOrder (
    // @parm [out] eAAFByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    eAAFByteOrder_t *  pOrder
  );



  //***********************************************************
  // METHOD NAME: GetNativeByteOrder()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFEndian | GetNativeByteOrder |
  // Returns the native "Endian-ness" of the platform on which this is
  // running.
  //
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetNativeByteOrder (
    // @parm [out] eAAFByteOrder_t * | pOrder | Pointer to place where byte order is to be put
    eAAFByteOrder_t *  pOrder
  );


public:
  // Interfaces ivisible inside the toolkit, but not exposed through the API

  // Gets the head object of the file containing this object.
  // This function is used to maintain MOB and Definition tables in the
  // head object.
  virtual AAFRESULT MyHeadObject
    (class ImplAAFHeader **header) const;

  // Gets the dictionary used to create this instance.
  virtual AAFRESULT STDMETHODCALLTYPE 
    GetDictionary(ImplAAFDictionary **ppDictionary);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFObjectTest.cpp.
  static AAFRESULT test();

  OMDECLARE_STORABLE(ImplAAFObject)

private:
  // private method
  AAFRESULT InitProperties ();

  ImplPropertyCollection * _pProperties;
};


#endif // ! __ImplAAFObject_h__
