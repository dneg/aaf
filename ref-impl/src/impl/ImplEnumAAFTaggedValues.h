//@doc
//@class    EnumAAFTaggedValues | Implementation class for EnumAAFTaggedValues
#ifndef __ImplEnumAAFTaggedValues_h__
#define __ImplEnumAAFTaggedValues_h__

/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

class ImplAAFTaggedValue;


#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMStrongReferenceVectorProperty<ImplAAFTaggedValue> taggedValueStrongRefArrayProp_t;

class ImplEnumAAFTaggedValues : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFTaggedValues ();

protected:
  virtual ~ImplEnumAAFTaggedValues ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next Tagged Value
        (ImplAAFTaggedValue ** ppTaggedValue);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of Tagged Values requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive container definitions
         ImplAAFTaggedValue ** ppTaggedValues,

         // @parm [out,ref] number of actual Tagged Values fetched into ppTaggedValues array
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
        (ImplEnumAAFTaggedValues ** ppEnum);

public:
  // SDK Internal 
	virtual AAFRESULT STDMETHODCALLTYPE
		SetEnumStrongProperty( ImplAAFObject *pObj, taggedValueStrongRefArrayProp_t *pProp);


private:
	aafUInt32						_current;
	ImplAAFObject					*_enumObj;
	taggedValueStrongRefArrayProp_t	*_enumStrongProp;

};

#endif // ! __ImplEnumAAFTaggedValues_h__


