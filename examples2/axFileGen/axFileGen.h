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
//=---------------------------------------------------------------------=

#include <AxTypes.h>
#include <AxSmartPointer.h>

#include <memory>
#include <map>
#include <vector>

//=---------------------------------------------------------------------=

class AxFGOp {

public:

	// Use this to get existing AxFGOp instances out of the registry.
	// This ensures the instance name is set properly so that GetName()
	// calls work.
	static AxFGOp& GetInstance( const AxString& instanceName );
	static void RemoveInstance( const AxString& instanceName );
	
	AxFGOp( const AxString& operationName );

	// Return the name of this instance (a unique name).
	AxString GetName() const;

	// Return the name of this operation.  This must match the name of
	// the factory that created it.
	AxString GetOpName() const;

	// argv[0] is the operation name.
	virtual void Execute( const std::vector<AxString>& args ) = 0;

	// Access the COM object created by this object.  Query to ComType is
	// automatic.
	template <class ComType>
	GetCOM( IAAFSmartPointer<ComType>& dstPtr )
	{
		AxQueryInterface( _spIUnknown, dstPtr );
	}
	
	// Register under the name instanceName.  A reference is added, since
	// the register is given a pointer.
	void RegisterInstance( const AxString& instanceName ); 

	// FIXME - It should be possible to keep these private.
	// Currently not due use in the command processing code
	// in this file.  Maybe use freind classes to work around
	// issue?
	void AddRef();
	void DecRef();
	int  GetRef() const;

	// The following are provided as a convienience.
	AxString GetDesc() const;
	AxString GetUsage() const;
	AxString GetNotes() const;
	int GetMinArgC() const;
	int GetMaxArgC() const;

protected:

    virtual ~AxFGOp();

	template <class ComType>
	SetCOM( IAAFSmartPointer<ComType> spComType ) 
	{
		AxQueryInterface( spComType, _spIUnknown ); 
	}

private:

	// prohibited
	AxFGOp();
	AxFGOp( const AxFGOp& );
	AxFGOp& operator&( const AxFGOp& );

	AxString _instanceName;
	AxString _operationName;

	// Com interface created when this object is executed.
	IUnknownSP _spIUnknown;

	int _refCount;
};

//=---------------------------------------------------------------------=

class AxFGOpFactory {

public:
	AxFGOpFactory( const AxString& name,
				   const AxString& desc,
				   const AxString& usage,
				   const AxString& notes,
				   int   minArgc,
				   int   maxArgc );

	virtual ~AxFGOpFactory();

	// Create an AxFGOp*.  The reference count of the object should be
	// incremented by the CreateInstance implementation.  The calls 
	// takes owner ship and should decrement it.  Dang... should be a
	// smart pointer.
	virtual AxFGOp* CreateInstance() const = 0;

	AxString GetName() const;
	AxString GetDesc() const;
	AxString GetUsage() const;
	AxString GetNotes() const;
	int GetMinArgC() const;
	int GetMaxArgC() const;

private:
	// prohibited
	AxFGOpFactory();
	AxFGOpFactory( const AxFGOpFactory& );
	AxFGOpFactory& operator&( const AxFGOpFactory& );

	AxString _name;
	AxString _desc;
	AxString _usage;
	AxString _notes;
	int _minArgc;
	int _maxArgc;

};

//=---------------------------------------------------------------------=

template <class OpType>
class AxFGStandardOpFactory : AxFGOpFactory {
public:

	AxFGStandardOpFactory( const AxString& name,
				           const AxString& desc,
				           const AxString& usage,
				           const AxString& notes,
				           int   minArgc,
				           int   maxArgc )
	: AxFGOpFactory( name, desc, usage, notes, minArgc, maxArgc )
	{}

	virtual ~AxFGStandardOpFactory()
	{}

	virtual AxFGOp* CreateInstance() const
	{
		AxFGOp* op = new OpType ( GetName() );
		op->AddRef();
		return op;
	}
};


//=---------------------------------------------------------------------=

// This could be done as a template.. but the macro will suffice and result
// in a slight simpler definition of the Execute method for AxFGOp implementations.

// FIXME - Perhaps a template is better... it could provide a default
// destructor implementation that a user could override if they wanted to
// use their own destructor.

#define AXFG_OP_DECLARATION( TYPE ) \
class TYPE : public AxFGOp { \
public: \
  TYPE ( const AxString& opName ) \
    : AxFGOp( opName ) \
	{} \
  virtual ~TYPE(); \
  virtual void Execute( const std::vector<AxString>& args ); \
};


// Global factory variable is prefixed by the type name.
#define AXFG_OP_FACTORY_DECLARATION( TYPE, NAME, DESC, USAGE, NOTES, MINARGC, MAXARGC ) \
AxFGStandardOpFactory<TYPE> TYPE##_Factory( \
	NAME, DESC, USAGE, NOTES, MINARGC, MAXARGC ); \

// User gets to pick the factory vaiable name prefix.
#define AXFG_OP_FACTORY_DECLARATION_B( TYPE, PREFIX, NAME, DESC, USAGE, NOTES, MINARGC, MAXARGC ) \
AxFGStandardOpFactory<TYPE> PREFIX##_Factory( \
	NAME, DESC, USAGE, NOTES, MINARGC, MAXARGC ); \


// This will declare both.  The user must then simply add the implementation of:
// void TYPE::Execute( const std::vector<AxString>& args ) { your code here }
#define AXFG_OP( TYPE, NAME, DESC, USAGE, NOTES, MINARGC, MAXARGC ) \
	AXFG_OP_DECLARATION( TYPE ) \
	AXFG_OP_FACTORY_DECLARATION( TYPE, NAME, DESC, USAGE, NOTES, MINARGC, MAXARGC )

//=---------------------------------------------------------------------=

// AxFGOpRegistry - Maps a name to pointer to a class instance.
// The class must implement the following:
//		AxString GetName() const

// FIXME - Remoe the GetName() requirement.  It is not necessary... just burdensom for
// AxFGOp.

template <class T>
class AxFGRegistry {

   typedef std::map<AxString,T*> MapType;
   typedef std::map<AxString,T*>::iterator MapIterType;

public:

	static AxFGRegistry& GetInstance();

	~AxFGRegistry();

	// Registry does not take ownership of the pointer!
	// It will never delete it, you must arrange to delete the object.
	void Register( T* object );

    void Remove( const AxString& name );

    T& Get( const AxString& name );
	
	// Null return pointer indicates end of list.
	// Iterator is invalidated when objects are add or removed.
	std::pair<AxString, const T*> Next();
	void ResetIter();

	bool IsKnown( const AxString& name );

private:
	
	AxFGRegistry();
	static AxFGRegistry* _singleInstance;

	MapType _map;
	MapIterType _mapIter;
};

typedef AxFGRegistry<AxFGOp>        AxFGOpRegistry;
typedef AxFGRegistry<AxFGOpFactory> AxFGOpFactoryRegistry;

//=---------------------------------------------------------------------=

// Utilty routines
IAAFFileSP FileFromFileOp( AxFGOp& op) ;
IAAFHeaderSP HeaderFromFileOp( AxFGOp& op );
IAAFDictionarySP DictionaryFromFileOp( AxFGOp& op );

IAAFFileSP FileFromFileOp( const AxString& name ) ;
IAAFHeaderSP HeaderFromFileOp( const AxString& name );
IAAFDictionarySP DictionaryFromFileOp( const AxString& name );

//=---------------------------------------------------------------------=

class AxFGEx {
public:
	AxFGEx()
		: _what( L"" )
	{}

	AxFGEx( const AxString& what )
		: _what( what )
	{}

	virtual ~AxFGEx()
	{}

	virtual const AxString what() const
	{ return _what; }

private:
	AxString _what;

};

class AxFGUsageEx : public AxFGEx {
public:

	AxFGUsageEx()
		: AxFGEx( L"Usage error." )
	{}

	AxFGUsageEx( const AxString& what )
		: AxFGEx( L"Usage: " + what )
	{}
};

class AxFGOpUsageEx : public AxFGUsageEx {
public:

	AxFGOpUsageEx( const AxFGOp& op );

	AxFGOpUsageEx( const AxFGOp& op, const AxString msg );

	virtual const AxString what() const;

private:

	void opUseMessage( 	std::wostringstream& os, const AxFGOp& op );

	AxString _what;
};
