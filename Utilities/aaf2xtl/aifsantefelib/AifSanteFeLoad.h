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
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Siemens SBS Media
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <AxTypes.h>
#include <AxFile.h>

#include <memory>

class AxInit;
class AifParseTree;
class AifParseTreeNodeFactory;

class AifSanteFeLoader {
public:
	AifSanteFeLoader( const AxString& fileName,
					  const char* pAafComLib,
					  bool initAAF );

	~AifSanteFeLoader();

	std::auto_ptr<AifParseTree> ParseFile( AifParseTreeNodeFactory& factory );
	IAAFHeaderSP GetHeader();

private:

	IAAFMobSP GetPrimaryMob();

	std::string _comLibPath;
	std::auto_ptr<AxInit> _initObj;
	AxString _fileName;
	AxFile   _axFile;
	static int _instanceCount;
};
