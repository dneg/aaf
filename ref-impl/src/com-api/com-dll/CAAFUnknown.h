#ifndef __CAAFUnknown_h__
#define __CAAFUnknown_h__

//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

//=--------------------------------------------------------------------------=
// @doc
//
// @module AAFUnknown.h |
//
// Class declaration for COM objects that need to inherit an implementation of
// IUnknown super-class that will support aggregation.
// @end
//
//  Modified 1998-06-30 by TRR for use by AAF.
//
//=--------------------------------------------------------------------------=


#ifndef __AAFCOMPlatform_h__
#include "AAFCOMPlatform.h"
#endif


//=--------------------------------------------------------------------------=
// AAF_DECLARE_STANDARD_UNKNOWN
//=--------------------------------------------------------------------------=
// @func
//  Macro to insert standard implementation if delegating <i IUnknown> interface.
//
// @comm
// All objects that are going to inherit from CAAFUnknown for their IUnknown
// implementation should put this in their class declaration instead of the
// three IUnknown methods. This macro is usually embedded within the <f AAF_DECLARE_CONCRETE>
// macro. (See the example for <f AAF_DECLARE_FACTORY>)
//
#define AAF_DECLARE_STANDARD_UNKNOWN() \
  STDMETHOD(QueryInterface)(REFIID riid, void **ppvObjOut) \
  { \
    return ExternalQueryInterface(riid, ppvObjOut); \
  } \
  STDMETHOD_(ULONG, AddRef)(void) \
  { \
    return ExternalAddRef(); \
  } \
  STDMETHOD_(ULONG, Release)(void) \
  { \
    return ExternalRelease(); \
  } 



//=--------------------------------------------------------------------------=
// AAF_DECLARE_FACTORY
//=--------------------------------------------------------------------------=
// @func
//  Macro to declare the interface for object class factory.
//
// @comm
//  All COM objects will need to use this macro within their class declaration
// so that <f DllGetClassObject> can correctly instantiate an appropriate 
// <c CAAFClassFactory> that will use the defined factory method to implement
// the <i IClassFactory> interface. This macro is usually embedded within the 
// <f AAF_DECLARE_CONCRETE> macro.
//
// @ex The Following example will declare the factory method of Foo |
// // module CFoo.h
// class CFoo : 
//   public IFoo,
//   public CAAFUnknown
// {
// public:
//   CFoo(IUnknown *pUnkOuter);
//   virtual ~CFoo();
// 
//   // Declare the factory for this class.
//   AAF_DECLARE_FACTORY();
//   
//   // Declare the standard delegating unknown methods
//   AAF_DECLARE_STANDARD_UNKNOWN();
//   
//   // IFoo methods
//   //...
// };
//
// 
// // will generate the following method declarations 
// class CFoo : 
//   public IFoo,
//   public CAAFUnknown
// {
// public:
//   CFoo(IUnknown *pUnkOuter);
//   virtual ~CFoo();
// 
//   // Declare the factory for this class.
//   static HRESULT AAFCreate(IUnknown *pUnkOuter, void **ppvObjOut);
//
//   // Declare the standard delegating unknown methods
//   STDMETHOD(QueryInterface)(REFIID riid, void **ppvObjOut)
//   {
//     return ExternalQueryInterface(riid, ppvObjOut);
//   }
//   STDMETHOD_(ULONG, AddRef)(void)
//   {
//     return ExternalAddRef();
//   }
//   STDMETHOD_(ULONG, Release)(void)
//   {
//     return ExternalRelease();
//   } 
//   
//   // IFoo methods
//   //...
// };
// 
#define AAF_DECLARE_FACTORY() \
    static HRESULT COMCreate(IUnknown *pUnkOuter, void **ppvObjOut)

//=--------------------------------------------------------------------------=
// AAF_DEFINE_FACTORY
//=--------------------------------------------------------------------------=
// @func
//  Macro to define the implementation for object class factory.
//
// @parm 
//  Base name of the com class implemenation. This should be the expected
// COM co-class name. 
//
// @comm
// All COM objects will need to use this macro within their class definition
// so that <f DllGetClassObject> can correctly instantiate an appropriate 
// <c CAAFClassFactory> that will use the defined factory method to implement
// the <i IClassFactory> interface. This macro is usually embedded within the 
// <f AAF_DECLARE_CONCRETE> macro.
//
// @ex The Following example will define the factory method of Foo |
// // module Foo.cpp
// AAF_DEFINE_FACTORY(Foo)
// 
// // will generate the following method definition
// HRESULT CFoo::AAFCreate(IUnknown *pUnkOuter, void **ppvObjOut)
// {
//   *ppvObjOut = NULL;
//   CFoo *pFoo new CFoo(pUnkOuter);
//   if (NULL == pFoo)
//     return E_OUTOFMEMORY;
//   *ppvObjOut = static_cast<IFoo *>(pFoo);
//   ((IUnknown *)(*ppvObjOut))->AddRef();
//   return S_OK;
// }
//
#define AAF_DEFINE_FACTORY(xclass) \
  HRESULT C##xclass::COMCreate(IUnknown *pUnkOuter, void **ppvObjOut) \
  { \
    *ppvObjOut = NULL; \
    C##xclass *p##xclass = new C##xclass(pUnkOuter); \
    if (NULL == p##xclass) \
      return E_OUTOFMEMORY; \
    *ppvObjOut = static_cast<CAAFUnknown *>(p##xclass); \
    return S_OK; \
  }

 
//=--------------------------------------------------------------------------=
// AAF_DECLARE_CONCRETE
//=--------------------------------------------------------------------------=
// @func
//  Macro to declare the interface for object class factory.
//
// @comm
//  All COM objects will need to use this macro within their class declaration
// so that <f DllGetClassObject> can correctly instantiate an appropriate 
// <c CAAFClassFactory> that will use the defined factory method to implement
// the <i IClassFactory> interface. Note: this is just a short cut for calling
// <f AAF_DECLARE_STANDARD_UNKNOWN>(); followed by <f AAF_DECLARE_FACTORY>();
//
#define AAF_DECLARE_CONCRETE() \
  AAF_DECLARE_STANDARD_UNKNOWN() \
  AAF_DECLARE_FACTORY() 

//=--------------------------------------------------------------------------=
// AAF_DEFINE_CONCRETE
//=--------------------------------------------------------------------------=
// @func
//  Macro to define the implementation for object class factory.
//
// @comm
//  All COM objects will need to use this macro within their class definition
// so that <f DllGetClassObject> can correctly instantiate an appropriate 
// <c CAAFClassFactory> that will use the defined factory method to implement
// the <i IClassFactory> interface.Note: this is just a alias for calling
// <f AAF_DEFINE_FACTORY>(xclass); .
//
#define AAF_DEFINE_CONCRETE(xclass) \
  AAF_DEFINE_FACTORY(xclass) 


//=--------------------------------------------------------------------------=
// @class CAAFUnknown | 
// Virtual base class that provides a default implementation if <i IUnknown>
// which also allows subclasses to be aggregated.
//
// @comm
// This class doesn't inherit from IUnknown since people inheriting from it
// are going to do so, and just delegate their IUnknown calls to the External*
// member functions on this object. The internal private unknown object does
// need to inherit from IUnknown, since it will be used directly as an IUnknown
// object.
//

class CAAFUnknown
{
//@access Public Members
public:
  //@cmember Constructor, create with controlling unknown.
  CAAFUnknown(IUnknown *pUnkOuter);
  
  //@cmember Destructor
  virtual ~CAAFUnknown();

//@access Protected Members
protected:
  // @cmember,mfunc Delagates <om IUnknown.QueryInterface> to controlling unknown.
  HRESULT ExternalQueryInterface(REFIID riid, void **ppvObjOut)
  {
    return m_pUnkOuter->QueryInterface(riid, ppvObjOut);
  }

  // @cmember,mfunc Delagates <om IUnknown.AddRef> to controlling unknown.
  ULONG ExternalAddRef(void)
  {
    return m_pUnkOuter->AddRef();
  }

  // @cmember,mfunc Delagates <om IUnknown.Release> to controlling unknown.
  ULONG ExternalRelease(void)
  {
    return m_pUnkOuter->Release();
  }


  // @cmember,mfunc People should use this during creation to return their private
  // unknown
  //
  inline IUnknown *GetPrivateUnknown (void)
  {
    return &m_UnkPrivate;
  }

  // @cmember Called by <om CAAFUnknown::QueryInterface>.
  // @comm This method is overridden by all derived classes to check for 
  // specific inteface implmentations.
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);
  
  // @cmember,mfunc
  // @comm Allows access to the non-aggregating IUnknown implementation
  // for this class.
  STDMETHOD_(ULONG, InternalAddRef)(void);

  // @cmember,mfunc
  // @comm Allows access to the non-aggregating IUnknown implementation
  // for this class.
  STDMETHOD_(ULONG, InternalRelease)(void);

  //@access Private Members
private:

  // @class CAAFPrivateUnknown |
  // The inner, private unknown implementation is for the aggregator
  // to control the lifetime of this object, and for those cases where
  // this object isn't aggregated.
  //
  class CAAFPrivateUnknown : public IUnknown
  {
    //@access Public Members
  public:
    //@cmember Implements non-delegating <om IUnknown.QueryInterface>.
    STDMETHOD(QueryInterface)(REFIID riid, void **ppvObjOut)
		{
			return This()->InternalQueryInterface(riid, ppvObjOut);
		}

    //@cmember Implements non-delegating <om IUnknown.AddRef>.
    STDMETHOD_(ULONG, AddRef)(void)
		{
			return This()->InternalAddRef();
		}

    //@cmember Implements non-delegating <om IUnknown.Release>.
    STDMETHOD_(ULONG, Release)(void)
		{
			return This()->InternalRelease();
		}


    //@access Private Members
    private:
      //@cmember Return pointer to outer object's this pointer.
      CAAFUnknown *This();
  };

  // so they can reference themselves in CAAFUnknown from pMainUnknown()
  //
  friend class CAAFPrivateUnknown;

  // so the class factory's CreateInstance method can call the private
  // nondelegating methods.
  //
  friend class CAAFClassFactory;


	// Member data:

	//@cmember Current reference count for this object.
  ULONG m_cRef;

  //@cmember Outer controlling Unknown
  IUnknown *m_pUnkOuter;

  //@cmember Nested class instance that implements the non-delegating <i IUnknown> interface.
  CAAFPrivateUnknown m_UnkPrivate;
};

#endif // __CAAFUnknown_h__


