//@doc
//@class    EnumAAFOperationDefs | Implementation class for EnumAAFOperationDefs
#ifndef __ImplEnumAAFOperationDefs_h__
#define __ImplEnumAAFOperationDefs_h__


/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/

class ImplAAFOperationDef;







#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMVariableSizeProperty<aafUID_t> OperationDefWeakRefArrayProp_t;
typedef OMStrongReferenceVectorProperty<ImplAAFOperationDef> OperationDefStrongRefArrayProp_t;


class ImplEnumAAFOperationDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFOperationDefs ();

protected:
  virtual ~ImplEnumAAFOperationDefs ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next OperationDefinition
        (ImplAAFOperationDef ** ppOperationDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of effect definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive operation definitions
         ImplAAFOperationDef ** ppOperationDefs,

         // @parm [out,ref] number of actual OperationDefs fetched into ppOperationDefs array
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
        (ImplEnumAAFOperationDefs ** ppEnum);


public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumProperty( ImplAAFObject *pObj, OperationDefWeakRefArrayProp_t *pProp);
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, OperationDefStrongRefArrayProp_t *pProp);

private:
	aafUInt32						_current;
	ImplAAFObject					*_enumObj;
	OperationDefWeakRefArrayProp_t		*_enumProp;
	OperationDefStrongRefArrayProp_t	*_enumStrongProp;
};

#endif // ! __ImplEnumAAFOperationDefs_h__


