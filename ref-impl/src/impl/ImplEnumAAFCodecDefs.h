//@doc
//@class    EnumAAFCodecDefs | Implementation class for EnumAAFCodecDefs
#ifndef __ImplEnumAAFCodecDefs_h__
#define __ImplEnumAAFCodecDefs_h__

/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/

class ImplAAFCodecDef;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMVariableSizeProperty<aafUID_t> codecDefWeakRefArrayProp_t;
typedef OMStrongReferenceVectorProperty<ImplAAFCodecDef> codecDefStrongRefArrayProp_t;



#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplEnumAAFCodecDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFCodecDefs ();

protected:
  virtual ~ImplEnumAAFCodecDefs ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next PluggableDefinition
        (ImplAAFCodecDef ** ppPluggableDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of Pluggable definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive Pluggable definitions
         ImplAAFCodecDef ** ppPluggableDefs,

         // @parm [out,ref] number of actual PluggableDefs fetched into ppPluggableDefs array
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
        (ImplEnumAAFCodecDefs ** ppEnum);


public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumProperty( ImplAAFObject *pObj, codecDefWeakRefArrayProp_t *pProp);
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, codecDefStrongRefArrayProp_t *pProp);

private:
	aafUInt32						_current;
	ImplAAFObject					*_enumObj;
	codecDefWeakRefArrayProp_t		*_enumProp;
	codecDefStrongRefArrayProp_t	*_enumStrongProp;
};

#endif // ! __ImplEnumAAFCodecDefs_h__


