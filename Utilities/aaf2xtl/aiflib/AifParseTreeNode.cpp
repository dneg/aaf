// Copyright 2003 BBC Technology

// $Id$
// $Name$

#include <AifParseTreeNode.h>

#include <assert.h>

unsigned int AifParseTreeNode::_luidCounter = 0;

AifParseTreeNode::AifParseTreeNode()
:	_parent(0),
	_isAAFObjPresent(false),
	_visitChildren(true),
	_luid(_luidCounter++),
	_exceptionFlag(false)
{}

AifParseTreeNode::AifParseTreeNode(IAAFObjectSP sp)
:	_parent(0),
	_sp(sp),
	_isAAFObjPresent(true),
	_visitChildren(true),
	_luid(_luidCounter++),
	_exceptionFlag(false)
{}

AifParseTreeNode::~AifParseTreeNode()
{
	// delete children 
	std::vector<AifParseTreeNode*>::iterator iter;
	for( iter = _children.begin();
		 iter != _children.end();
		 ++iter ) {
		delete *iter;
	}

	// delete decorations
	DecorationStackMapType::iterator mapIter;
	for( mapIter  = _decorationStackMap.begin();
		 mapIter != _decorationStackMap.end();
		 ++mapIter ) {
			 while ( !mapIter->second->empty()  ) {
				delete mapIter->second->top();
				mapIter->second->pop();
			 }
	}
}

void  AifParseTreeNode::SetParent( AifParseTreeNode* pParent )
{
	_parent = pParent;
}

void AifParseTreeNode::AddChild( std::auto_ptr<AifParseTreeNode> pChild )
{
	pChild->SetParent( this );
	_children.push_back( pChild.release() );	
}

int AifParseTreeNode::GetNumChildren()
{
	return _children.size();
}

std::auto_ptr<AifParseTreeNode> AifParseTreeNode::ReleaseChild( int i )
{
	// ... vector<> will erase an element, but only if we supply an iterator,
	// not an index.
	int j;
	std::vector<AifParseTreeNode*>::iterator iter;
	for( j = 0, iter = _children.begin();
		 j != i && iter != _children.end();
		 ++j, ++iter ) {
		// noop
	}

	assert( iter != _children.end() );

	std::auto_ptr<AifParseTreeNode> node( _children[i] );

	_children.erase( iter );

	// Node is now unrooted.  Clear parent.
	node->SetParent(0);

	return node;
}

AifParseTreeNode* AifParseTreeNode::GetParent()
{
	return _parent;
}

AifParseTreeNode& AifParseTreeNode::GetChild( int i )
{
	assert( i < GetNumChildren() );
	return *_children[i];
}

void AifParseTreeNode::Visit( AifParseTreeVisitor* visitor )
{
	visitor->PreOrderVisit( *this );

	VisitChildren( visitor );

	visitor->PostOrderVisit( *this );
}

void AifParseTreeNode::VisitChildren( AifParseTreeVisitor* visitor )
{
	if ( !_visitChildren ) {
		return;
	}

	std::vector<AifParseTreeNode*>::iterator iter;
	for( iter = _children.begin();
		 iter != _children.end();
		 ++iter ) {

		(*iter)->Visit( visitor );
	}
}

void AifParseTreeNode::VisitChild( AifParseTreeVisitor* visitor, int i )
{
	assert( i < GetNumChildren() );
	_children[i]->Visit( visitor );
}


bool AifParseTreeNode::isAAFObjectPresent()
{
	return _isAAFObjPresent;
}

IAAFObjectSP AifParseTreeNode::GetAAFObject()
{
	// Fixme - thow exception.
	// user user call isAAFObjectPresent first
	assert( _isAAFObjPresent );

	return _sp;
}

unsigned int AifParseTreeNode::GetLuid()
{
	return _luid;
}

void AifParseTreeNode::SetExceptionFlag()
{
	_exceptionFlag = true;
}

bool AifParseTreeNode::GetExceptionFlag()
{
	return _exceptionFlag;
}

void AifParseTreeNode::SetVisitChildren( bool visitChildren )
{
	_visitChildren = visitChildren;
}


bool AifParseTreeNode::IsStackPresent( const char* typeName )
{
	DecorationStackMapType::iterator iter;

	iter = _decorationStackMap.find( typeName );
	if ( iter == _decorationStackMap.end() ) {
		return false;
	}
	else {
		return true;
	}
}

AifParseTreeNode::DecorationStackType& AifParseTreeNode::GetDecorationStack( const char* typeName, bool create_if_not_exists )
{
	DecorationStackMapType::iterator iter;

	iter = _decorationStackMap.find( typeName );
	if ( iter == _decorationStackMap.end() ) {
		if ( create_if_not_exists ) {
			_decorationStackMap[typeName] = new DecorationStackType;
		}
		else {
			assert(0);
		}
	}

	return *_decorationStackMap[typeName];

}

// Push decoration onto stack the maps to typeName.
void AifParseTreeNode::PushDecoration( const char* typeName,
				  std::auto_ptr<AifParseTreeNodeDecoration> pDecoration )
{
	GetDecorationStack(typeName, true).push( pDecoration.release() );
}

// Pop decoration off of stack that maps to typeName.
std::auto_ptr<AifParseTreeNodeDecoration> AifParseTreeNode::PopDecoration( const char* typeName )
{
	DecorationStackType& stack = GetDecorationStack( typeName, false );
	
	std::auto_ptr<AifParseTreeNodeDecoration> pDecoration( stack.top() );
	stack.pop();

	return pDecoration;
}
