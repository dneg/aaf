// Copyright 2003 BBC Technology

// $Id$
// $Name$

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
