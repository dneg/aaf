//@doc
//@class    EnumAAFDataDefs | Implementation class for EnumAAFDataDefs
#ifndef __ImplEnumAAFDataDefs_h__
#define __ImplEnumAAFDataDefs_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


class ImplAAFDataDef;








#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMVariableSizeProperty<aafUID_t> DataDefWeakRefArrayProp_t;
typedef OMStrongReferenceVectorProperty<ImplAAFDataDef> DataDefStrongRefArrayProp_t;

class ImplEnumAAFDataDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFDataDefs ();

protected:
  virtual ~ImplEnumAAFDataDefs ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next DataDefinition
        (ImplAAFDataDef ** ppDataDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of data definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive data definitions
         ImplAAFDataDef ** ppDataDef,

         // @parm [out,ref] number of actual DataDefinition fetched into ppDataDefinition array
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
        (ImplEnumAAFDataDefs ** ppEnum);



public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumProperty( ImplAAFObject *pObj, DataDefWeakRefArrayProp_t *pProp);
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, DataDefStrongRefArrayProp_t *pProp);

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFDataDefsTest.cpp.
  static AAFRESULT test();
private:
		aafUInt32						_current;
		ImplAAFObject					*_enumObj;
	DataDefWeakRefArrayProp_t		*_enumProp;
	DataDefStrongRefArrayProp_t		*_enumStrongProp;
};

#endif // ! __ImplEnumAAFDataDefs_h__


