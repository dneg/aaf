//@doc
//@class    AAFParameter | Implementation class for AAFParameter
#ifndef __ImplAAFParameter_h__
#define __ImplAAFParameter_h__

/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

class ImplAAFParameterDef;
class ImplAAFTypeDef;

#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif


class ImplAAFParameter : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFParameter ();

protected:
  virtual ~ImplAAFParameter ();

public:
  virtual AAFRESULT STDMETHODCALLTYPE
    SetParameterDefinition (
      ImplAAFParameterDef *pParmDef);

  virtual AAFRESULT STDMETHODCALLTYPE
    GetParameterDefinition (
      ImplAAFParameterDef **ppParmDef);	

  virtual AAFRESULT STDMETHODCALLTYPE
    SetTypeDefinition (
      ImplAAFTypeDef*  pTypeDef);

  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef);

private:

	OMFixedSizeProperty<aafUID_t>				_parmDef;
	OMFixedSizeProperty<aafUID_t>				_typeDef;
};

#endif // ! __ImplAAFParameter_h__




