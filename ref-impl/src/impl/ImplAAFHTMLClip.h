//@doc
//@class    AAFHTMLClip | Implementation class for AAFHTMLClip
#ifndef __ImplAAFHTMLClip_h__
#define __ImplAAFHTMLClip_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#ifndef __ImplAAFTextClip_h__
#include "ImplAAFTextClip.h"
#endif


class ImplAAFHTMLClip : public ImplAAFTextClip
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFHTMLClip ();

protected:
  virtual ~ImplAAFHTMLClip ();

public:


  //****************
  // GetBeginAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBeginAnchor
        (// @parm [in,string] Begin Anchor Name
         wchar_t *  pName,

         // @parm [in] length of the buffer to hold Begin Anchor Name
         aafInt32  bufSize);

  //****************
  // GetBeginAnchorBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBeginAnchorBufLen
        // @parm [out] Length in bytes
        (aafUInt32 *  pLen);

  //****************
  // SetBeginAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBeginAnchor
        // @parm [in, string] Specifies the HTML tag that defines the start of the text
        (wchar_t *  pName);

  //****************
  // GetEndAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEndAnchor
        (// @parm [in,string] End Anchor Name
         wchar_t *  pName,

         // @parm [in] length of the buffer to hold End Anchor Name
         aafInt32  bufSize);

  //****************
  // GetEndAnchorBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEndAnchorBufLen
        // @parm [out] Length in bytes
        (aafUInt32 *  pLen);

  //****************
  // SetEndAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEndAnchor
        // @parm [in, string] Specifies the HTML tag that defines the end of the text
        (wchar_t *  pName);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFHTMLClip)

protected:
  // Persistent properties...
  OMWideStringProperty _beginAnchor;
  OMWideStringProperty _endAnchor;
};

#endif // ! __ImplAAFHTMLClip_h__


