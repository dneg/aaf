//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

// AxShellExtFolder.h

#ifndef AXSHELLEXTFOLDER_H
#define AXSHELLEXTFOLDER_H

#include "StdAfx.h"

#include <AxTypes.h>
#include <AxBaseObjIter.h>
#include <AxInit.h>
#include <AxFile.h>

#include <map>

bool FilePathFromRootPidl( LPCITEMIDLIST rootPidl, AxString& retFileName );
bool ObjectPathFromRootPidl( LPCITEMIDLIST rootPidl, vector<AxString>& objPath );
LPCITEMIDLIST GetFirstAAFPidl( LPCITEMIDLIST rootPidl );
AxString ObjPathStringFromRootPidl( LPCITEMIDLIST rootPidl );
AxString CompletePathFromPidl( LPCITEMIDLIST rootPidl );
void DumpPidlDebugInfo( LPCITEMIDLIST pidl );
LPCITEMIDLIST GetAbsolutePidl( LPCITEMIDLIST pidl );

// Generate an ID list for single level of the OPV tree.
// (OPV = Object, Property, Value).
class AxShellExtIDList {
public:
	AxShellExtIDList( LPCITEMIDLIST _rootPidl );
	~AxShellExtIDList();

	bool Next( AxString& desc, bool& isFolder, int& lid );

	// AxShellExt keeps ownership of rootPidl.
	LPCITEMIDLIST GetRootPidl();

	void AxShellExtIDList::Initialize();

	void AxShellExtIDList::Finalize();


private:
	bool RealNext( AxString& desc, bool& isFolder, int& level, int& lid );
	void DescendObjectPath();

	LPCITEMIDLIST _rootPidl;
	AxString _fileName;
	std::auto_ptr<AxFile> _axFile;
	std::auto_ptr<AxBaseObjRecIter> _recIter;
	std::auto_ptr<AxInit> _initObj;

	std::vector<AxString> _objPath;

	int _levelToIterateOver;

	int _startLid;

	// Count the number of items retrieved at each level.  This
	// forms a unique identifier for the item.
	// map<level,count>
	map<int,int> _levelCounters;
};

#endif
