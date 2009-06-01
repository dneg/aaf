//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Siemens SBS Media
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef _AifParseTreeNode_h_
#define _AifParseTreeNode_h_

#include <AifParseTreeVisitor.h>

#include <AxSmartPointer.h>

#include <stack>
#include <map>
#include <vector>
#include <typeinfo>

#include <assert.h>

class AifParseTreeNodeDecoration {
public:
	AifParseTreeNodeDecoration()
	{}
	virtual ~AifParseTreeNodeDecoration()
	{}
};

class AifParseTreeNode {

private:                                                 
	typedef std::stack<AifParseTreeNodeDecoration*>	     DecorationStackType;
	typedef std::map<const char*, DecorationStackType* > DecorationStackMapType;

public:
	
	AifParseTreeNode();
	AifParseTreeNode(IAAFObjectSP sp);
	virtual ~AifParseTreeNode();

	void AddChild( std::auto_ptr<AifParseTreeNode> pChild );

	AifParseTreeNode* GetParent();

	int  GetNumChildren();
	std::auto_ptr<AifParseTreeNode> ReleaseChild( int i );
	AifParseTreeNode& GetChild( int i );

	virtual void Visit( AifParseTreeVisitor* visitor );

	void SetVisitChildren( bool visitChildren );

	bool isAAFObjectPresent();
	IAAFObjectSP GetAAFObject();

	unsigned int GetLuid();

	void SetExceptionFlag();
	bool GetExceptionFlag();

	template <class Type>
	bool IsDecorated( Type*& pDecoration )
	{
		const type_info& info = typeid( Type );

		if ( IsStackPresent( info.raw_name() ) ) {
			GetDecoration( pDecoration );
			return true;
		}
		else {
			return false;
		}
	}
		
	template <class Type>
	void GetDecoration( Type*& pDecoration )
	{
		const type_info& info = typeid( Type );

		DecorationStackType& decStack = GetDecorationStack( info.raw_name(), false );

		AifParseTreeNodeDecoration* pType2 = decStack.top();

		Type* pType = dynamic_cast<Type*>( decStack.top() );

		assert( pType );

		pDecoration =  pType;
	}

	// Type must be a subclass of AifParseTreeNodeDecoration
	template <class Type>
	void PushDecoration( std::auto_ptr<Type> pDecoration )
	{
		const type_info& info = typeid( Type );

		std::auto_ptr<AifParseTreeNodeDecoration> pAifNodeDec( pDecoration.release() );
			
		PushDecoration( info.raw_name(), pAifNodeDec );
	}

	template <class Type>
	void PopDecoration( std::auto_ptr<Type>& pDecoration )
	{
		const type_info& info = typeid( Type );

		std::auto_ptr<AifParseTreeNodeDecoration> pAifNodeDec = PopDecoration( info.raw_name() );

		Type* pType = dynamic_cast<Type*>( pAifNodeDec.release() );
		
		assert( pType );
		
		std::auto_ptr<Type> pDecorationRet( pType );

		pDecoration = pDecorationRet;
	}

protected:

	void VisitChildren( AifParseTreeVisitor* visitor );
	void VisitChild( AifParseTreeVisitor* visitor, int i );

	void SetParent( AifParseTreeNode* pParent );

private:

	// Does a stack for a particular type exists?
	bool IsStackPresent( const char* typeName );
	
	// Get stack that maps to typeName.
	DecorationStackType& GetDecorationStack( const char* typeName, bool create_if_not_present );

	// Push decoration onto stack that maps to typeName.
	void PushDecoration( const char* typeName, std::auto_ptr<AifParseTreeNodeDecoration> pDecoration  );

	// Pop decoration off stack that maps to typeName.
	std::auto_ptr<AifParseTreeNodeDecoration> PopDecoration( const char* typeName );


	AifParseTreeNode*			   _parent;
	std::vector<AifParseTreeNode*> _children;
	bool _visitChildren;

	bool _isAAFObjPresent;
	IAAFObjectSP _sp;

	DecorationStackMapType _decorationStackMap;

	static unsigned int _luidCounter;
	unsigned int _luid;

	bool _exceptionFlag;
};


#endif
