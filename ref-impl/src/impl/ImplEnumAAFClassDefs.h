//@doc
//@class    EnumAAFClassDefs | Implementation class for EnumAAFClassDefs
#ifndef __ImplEnumAAFClassDefs_h__
#define __ImplEnumAAFClassDefs_h__
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

class ImplAAFMetaDictionary;

#include "ImplAAFClassDef.h"
#include "OMReferenceContainerIter.h"



class ImplEnumAAFClassDefs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFClassDefs ();

protected:
  virtual ~ImplEnumAAFClassDefs ();

public:

  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next ClassDefinition
        (ImplAAFClassDef ** ppClassDef);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of class definitions requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive class definitions
         ImplAAFClassDef ** ppClassDefs,

         // @parm [out,ref] number of actual ClassDefinitions fetched into ppClassDefs array
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
        (ImplEnumAAFClassDefs ** ppEnum);


public:
  // SDK Internal 
  virtual AAFRESULT STDMETHODCALLTYPE
	  SetIterator(ImplAAFMetaDictionary *pObj,
				OMReferenceContainerIterator<ImplAAFClassDef>* iterator);

private:
	ImplAAFMetaDictionary*	_enumObj;
	OMReferenceContainerIterator<ImplAAFClassDef>*	_iterator;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplEnumAAFClassDefs> ImplEnumAAFClassDefsSP;

#endif // ! __ImplEnumAAFClassDefs_h__
