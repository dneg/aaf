//@doc
//@class    EnumAAFSegments | Implementation class for EnumAAFSegments
#ifndef __ImplEnumAAFSegments_h__
#define __ImplEnumAAFSegments_h__


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

class ImplAAFSegment;
class ImplAAFSelector;


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

typedef OMStrongReferenceVectorProperty<ImplAAFSegment> SegmentStrongRefArrayProp_t;


class ImplEnumAAFSegments : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFSegments ();

protected:
  virtual ~ImplEnumAAFSegments ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next Segment
        (ImplAAFSegment ** ppSegment);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of segments requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive segments
         ImplAAFSegment ** ppSegments,

         // @parm [out,ref] number of actual Segments fetched into ppSegments array
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
        (ImplEnumAAFSegments ** ppEnum);


public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEnumStrongProperty( ImplAAFObject *pObj, SegmentStrongRefArrayProp_t *pProp);

private:
	aafUInt32					_current;
	ImplAAFObject				*_enumObj;
	SegmentStrongRefArrayProp_t	*_enumStrongProp;
};

#endif // ! __ImplEnumAAFSegments_h__


