//@doc
//@class    EnumAAFPropertyDefs | Implementation class for EnumAAFPropertyDefs
#ifndef __ImplEnumAAFPropertyDefs_h__
#define __ImplEnumAAFPropertyDefs_h__

/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/

class ImplAAFPropertyDef;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


typedef OMStrongReferenceVectorProperty<ImplAAFPropertyDef>
  propertyDefStrongRefArrayProp_t; 


class ImplEnumAAFPropertyDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFPropertyDefs ();

protected:
  virtual ~ImplEnumAAFPropertyDefs ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next PropertyDefinition
        (ImplAAFPropertyDef ** ppPropertyDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of property definition definitions
		 // requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to
		 // receive property definitions
         ImplAAFPropertyDef ** ppPropertyDefs,

         // @parm [out,ref] number of actual PropertyDefs fetched into
		 // ppPropertyDefs array
         aafUInt32 *  pFetched);

  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
        // @parm [in] Number of elements to skip
        (aafUInt32  count);

  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();


  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
        // @parm [out,retval] new enumeration
        (ImplEnumAAFPropertyDefs ** ppEnum);



public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty (ImplAAFObject *pObj,
						   propertyDefStrongRefArrayProp_t *pProp);

private:
	aafUInt32                         _current;
	ImplAAFObject                   * _enumObj;
	propertyDefStrongRefArrayProp_t * _enumStrongProp;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplEnumAAFPropertyDefs> ImplEnumAAFPropertyDefsSP;

#endif // ! __ImplEnumAAFPropertyDefs_h__
