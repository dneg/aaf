#ifndef __AxBaseObjIter_h__
#define __AxBaseObjIter_h__

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

#include "AxBaseObj.h"

// Required for AxRecordIterator.
#include "AxIterator.h"

#include <memory>
#include <utility>
#include <deque>

class AxObject;
class AxProperty;
class AxPropertyValue;

// AxBaseObjIterPrtcl defines the interface of AxBaseObj interators.
class AxBaseObjIterPrtcl {
public:
	typedef std::pair<bool,std::auto_ptr<AxBaseObj> > Pair;
	
	virtual ~AxBaseObjIterPrtcl() = 0;

	virtual Pair NextOne() = 0;

	virtual void Reset() = 0;

	virtual std::auto_ptr<AxBaseObjIterPrtcl> Clone() = 0;
};


// Implementation of AxBaseObjIterPrtcl that simply forwards calls to
// an iterator of type IterType that produces objects of type ObjType.
// ObjType is in instance of a class derived from AxBaseObj.
//
// IterType must have the following attributes:
// - A TypeDef called TypeSP that is the same type used as an argument
// to an ObjType constructor.
// - A method NextOne( IterType::TypeSP& );

template <class IterType, class ObjType>
class AxBaseObjIter : public AxBaseObjIterPrtcl {
public:
	AxBaseObjIter( const IterType& iter )
	: _iter( iter )
	{}

	virtual ~AxBaseObjIter()
	{}

	virtual Pair NextOne()
	{
		IterType::Pair pairRet = _iter.NextOne();
		if ( pairRet.first ) {
			auto_ptr<AxBaseObj> apObj( new ObjType ( pairRet.second ) );
			return Pair(true, apObj);
		}
		else {
			return Pair(false, auto_ptr<AxBaseObj>(0) );
		}
	}

	virtual void Reset()
	{
		_iter.Reset();
	}

	virtual std::auto_ptr<AxBaseObjIterPrtcl> Clone()
	{
		auto_ptr< IterType > iter( _iter.Clone() );
		return auto_ptr<AxBaseObjIterPrtcl>(
			new AxBaseObjIter<IterType,ObjType>( *iter ) );
	}

private:
	IterType _iter;
};


template < class ObjType >
class AxBaseSolitaryObjIter : public AxBaseObjIterPrtcl {
public:

	AxBaseSolitaryObjIter( const ObjType& obj )
		: _obj( obj ),
		  _done( false )
	{}

	~AxBaseSolitaryObjIter()
	{}

	virtual Pair NextOne()
	{
		if ( _done ) {
			auto_ptr<AxBaseObj> apObj(0);
			return Pair( false, apObj );
		}
		else {
			auto_ptr<AxBaseObj> apObj( new ObjType(_obj) );
			_done = true;
			return Pair( true, apObj );
		}
	}

	virtual void Reset()
	{
		_done = false;
	}

	virtual std::auto_ptr<AxBaseObjIterPrtcl> Clone()
	{
		auto_ptr<AxBaseObjIterPrtcl> iter (
			new AxBaseSolitaryObjIter( _obj ) );
		return iter;
	}

private:
	ObjType _obj;
	bool _done;
};


// AxBaseObjIterPrtcl for odd ball records

class AxBaseRecordObjIter : public AxBaseObjIterPrtcl {
public:
	AxBaseRecordObjIter( auto_ptr< AxRecordIterator > );
	~AxBaseRecordObjIter();

	virtual Pair NextOne();

	virtual void Reset();

	virtual auto_ptr<AxBaseObjIterPrtcl> Clone();

private:
	AxBaseRecordObjIter();
	AxBaseRecordObjIter( const AxBaseRecordObjIter& );
	AxBaseRecordObjIter& operator=( const AxBaseRecordObjIter& );

	auto_ptr<AxRecordIterator> _axRecordIter;
};

// AxBaseObjIterPrtcl for odd ball arrays

template <class TypeDef>
class AxBaseArrayObjIter : public AxBaseObjIterPrtcl {
public:
	AxBaseArrayObjIter( auto_ptr< AxArrayIterator<TypeDef> > );
	~AxBaseArrayObjIter();

	virtual Pair NextOne();

	virtual void Reset();

	virtual auto_ptr<AxBaseObjIterPrtcl> Clone();

private:
	AxBaseArrayObjIter();
	AxBaseArrayObjIter( const AxBaseArrayObjIter& );
	AxBaseArrayObjIter& operator=( const AxBaseArrayObjIter& );

	auto_ptr<AxArrayIterator<TypeDef> > _axArrayIter;
};

//=---------------------------------------------------------------------=

// Ax Interchange Object Recursive Iterator
// Presents the objects as a tree of AAF Objects, Properties,
// and Property Values.
//
// The implementatin of NextOne() examines the object about
// to be returned and, if necessary, creates an iterator to
// represent the next level of recursion and pushes it on
// the internal stack.  You can detect that this has occurred
// as follows:
//
//		NextOne( &level )
//		if ( level != GetLevel() ) {recursed to next level};

// FIXME - Should consolidate the Pair and level returned by
// NextOne() into an instance of a single class.

// TODO - Implement Clone and Reset methods.

class AxBaseObjRecIter {
public:
	
	typedef AxBaseObjIterPrtcl::Pair Pair;

	AxBaseObjRecIter( std::auto_ptr< AxBaseObjIterPrtcl > root );

	virtual ~AxBaseObjRecIter();

	Pair NextOne( int& level );

	void PopStack();

	int GetLevel();

private:

	AxBaseObjRecIter();
	AxBaseObjRecIter( const AxBaseObjRecIter& );
	AxBaseObjRecIter& operator=( const AxBaseObjRecIter& );

	void Push( std::auto_ptr< AxBaseObjIterPrtcl > );
	void Pop();
	const std::auto_ptr< AxBaseObjIterPrtcl >& Top();
	bool Empty();
	int  Size();

	void HandleObjectRecursion( AxObject& obj );
	void HandlePropertyRecursion( AxProperty& prop );
	void HandlePropertyValueRecursion( AxPropertyValue& propVal );
	void HandleRecordPropertyValueRecursion( AxRecordIterator::Pair& recPair );

	std::auto_ptr< AxBaseObjIterPrtcl > _root;
	std::deque< std::auto_ptr< AxBaseObjIterPrtcl > > _deque;
};

#endif
