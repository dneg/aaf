//@doc
//@class    EnumAAFTypeDefs | Implementation class for EnumAAFTypeDefs
#ifndef __ImplEnumAAFTypeDefs_h__
#define __ImplEnumAAFTypeDefs_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

class ImplAAFTypeDef;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


typedef OMVariableSizeProperty<aafUID_t> typeDefWeakRefArrayProp_t;
typedef OMStrongReferenceVectorProperty<ImplAAFTypeDef> typeDefStrongRefArrayProp_t;


class ImplEnumAAFTypeDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFTypeDefs ();

protected:
  virtual ~ImplEnumAAFTypeDefs ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next TypeDefinition
        (ImplAAFTypeDef ** ppTypeDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of type definition definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive type definition definitions
         ImplAAFTypeDef ** ppTypeDefs,

         // @parm [out,ref] number of actual TypeDefs fetched into ppTypeDefs array
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
        (ImplEnumAAFTypeDefs ** ppEnum);



public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumProperty( ImplAAFObject *pObj, typeDefWeakRefArrayProp_t *pProp);
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, typeDefStrongRefArrayProp_t *pProp);

private:
	aafUInt32						_current;
	ImplAAFObject					*_enumObj;
	typeDefWeakRefArrayProp_t		*_enumProp;
	typeDefStrongRefArrayProp_t		*_enumStrongProp;
};

#endif // ! __ImplEnumAAFTypeDefs_h__


