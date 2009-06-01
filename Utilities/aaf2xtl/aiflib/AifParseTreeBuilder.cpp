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

#include <AifParseTreeBuilder.h>

#include <AifParseTree.h>
#include <AifParseTreeNodeFactory.h>

#include <AxBaseObj.h>
#include <AxBaseObjIter.h>
#include <AxObject.h>

#include <assert.h>

#include <stack>
#include <memory>

#include <iostream>

// The newly built tree is a child of pRootParent.

void BuildTree( AxBaseObjRecIter& recIter,
		AifParseTreeNode* pRootParent,
		AifParseTreeNodeFactory& nodeFactory,
		AifParseTreeBuilderAcceptFunc& accept )
{
	using namespace std;

	bool nextExists;
	auto_ptr<AxBaseObj> nextPtr;
	
	AifParseTreeNode* pChild = 0;
	int level = 0;
	stack< pair<int,AifParseTreeNode*> > parentStack;

	parentStack.push( pair<int,AifParseTreeNode*>(level,pRootParent) );
	
	for( nextExists  = recIter.NextOne( nextPtr, level );
		 nextExists;
		 nextExists = recIter.NextOne( nextPtr, level ) ) {

		if ( dynamic_cast<AxObject*>( nextPtr.get() ) ) {

			auto_ptr<AxObject> obj( 
			 dynamic_cast<AxObject*>( nextPtr.release() ) );

			 // wcout << L"level = " << level << endl;
			 // wcout << L"parentStack.size() = " << parentStack.size() << endl ;
			 // wcout << L"parentStack.top().first = " << parentStack.top().first << endl;
			 // wcout << L"class name = " << obj->GetClassName() << endl << endl;

			if ( accept( static_cast<IAAFObjectSP>(*obj) ) ) {

				auto_ptr<AifParseTreeNode> childNode( nodeFactory.CreateNode( static_cast<IAAFObjectSP>(*obj) ) );
				pChild = childNode.get();
				parentStack.top().second->AddChild( childNode );

				parentStack.push( pair<int,AifParseTreeNode*>(level,pChild) );
			}
			else {
				recIter.PopStack();
			}
		}


		// These remain for reference.
		// else if ( dynamic_cast<AxProperty*>( nextPtr.get() ) ) {
		// }
		// else if ( dynamic_cast<AxPropertyValue*>( nextPtr.get() ) ) {
		// }


		// A new object never exists in a record subtree, so just stop the traveral.
		else if ( dynamic_cast< AxBaseObjAny<AxRecordIterator::Pair>* >( nextPtr.get() ) ) {
			recIter.PopStack();
		}

		// If we get an exception then log a message and keep on going in the 
		// belief that something is better than nothing for the user or developer
		// who is trying to sort out the problem (with the AAF file, or SDK).
		else if ( dynamic_cast<AxBaseObjAny<AxExHResult>*>( nextPtr.get() ) ) {

			auto_ptr< AxBaseObjAny<AxExHResult> > ex (
				dynamic_cast<AxBaseObjAny<AxExHResult>*>( nextPtr.release() ) );

			wcout << ex->get().what() << endl;
		}

		while ( level < parentStack.top().first ) {
			parentStack.pop();
		}

	 } // end for

}


void AifBuildParseTree( IAAFObjectSP spSubTreeRootObject,
			AifParseTreeNode* pParent,
			AifParseTreeNodeFactory& nodeFactory,
			AifParseTreeBuilderAcceptFunc& acceptFunc )
{
	assert( pParent );
	
	AxObject axRootObject( spSubTreeRootObject );

	std::auto_ptr< AxBaseObjIterPrtcl > axRootObjectIter(
			new AxBaseSolitaryObjIter<AxObject>(axRootObject) );

	AxBaseObjRecIter recIter( axRootObjectIter );

	BuildTree( recIter, pParent, nodeFactory, acceptFunc );
}

std::auto_ptr<AifParseTreeNode> AifBuildParseTree( IAAFObjectSP spSubTreeRootObject,
						   AifParseTreeNodeFactory& nodeFactory,
						   AifParseTreeBuilderAcceptFunc& acceptFunc )
{
	AifParseTreeNode fakeRoot;

	AifBuildParseTree( spSubTreeRootObject,
					   &fakeRoot,
					   nodeFactory,
					   acceptFunc );

	assert( fakeRoot.GetNumChildren() == 1 );

	return fakeRoot.ReleaseChild(0);
}
