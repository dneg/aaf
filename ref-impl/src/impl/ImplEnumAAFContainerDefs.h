//@doc
//@class    EnumAAFContainerDefs | Implementation class for EnumAAFContainerDefs
#ifndef __ImplEnumAAFContainerDefs_h__
#define __ImplEnumAAFContainerDefs_h__

/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

class ImplAAFContainerDef;


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


typedef OMVariableSizeProperty<aafUID_t> containerDefWeakRefArrayProp_t;
typedef OMStrongReferenceVectorProperty<ImplAAFContainerDef> containerDefStrongRefArrayProp_t;

class ImplEnumAAFContainerDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFContainerDefs ();

protected:
  virtual ~ImplEnumAAFContainerDefs ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next container Definition
        (ImplAAFContainerDef ** ppPluggableDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of Pluggable definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive container definitions
         ImplAAFContainerDef ** ppContainerDefs,

         // @parm [out,ref] number of actual container definitions fetched into ppContainerDefs array
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
        (ImplEnumAAFContainerDefs ** ppEnum);

public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumProperty( ImplAAFObject *pObj, containerDefWeakRefArrayProp_t *pProp);
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, containerDefStrongRefArrayProp_t *pProp);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFContainerDefsTest.cpp.
  static AAFRESULT test();
private:
	aafUInt32							_current;
	ImplAAFObject						*_enumObj;
	containerDefWeakRefArrayProp_t		*_enumProp;
	containerDefStrongRefArrayProp_t	*_enumStrongProp;
};

#endif // ! __ImplEnumAAFContainerDefs_h__


