//@doc
//@class    EnumAAFLoadedPlugins | Implementation class for EnumAAFLoadedPlugins
#ifndef __ImplEnumAAFLoadedPlugins_h__
#define __ImplEnumAAFLoadedPlugins_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplAAFPluginDescriptor;

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "aafTable.h"

class ImplAAFPluginManager;

class ImplEnumAAFLoadedPlugins : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFLoadedPlugins ();

protected:
  virtual ~ImplEnumAAFLoadedPlugins ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next AAFPluginDescriptor
        (ImplAAFPluginDescriptor ** ppAAFPluginDescriptor);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of AAFPluginDescriptor requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive AAFPluginDescriptors
         ImplAAFPluginDescriptor ** ppAAFPluginDesc,

         // @parm [out,ref] number of actual AAFPluginDescriptor fetched into ppAAFPluginDesc array
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
        (ImplEnumAAFLoadedPlugins ** ppEnum);


public:
// Private to the SDK
	virtual AAFRESULT SetCategory(aafUID_t *category);

private:
	aafUID_t				_category;
	ImplAAFPluginManager	*_manager;
	aafTableIterate_t		_tableIter;
	aafBool					_isFirst;
};

#endif // ! __ImplEnumAAFLoadedPlugins_h__


