// Copyright 2003 BBC Technology

// $Id$
// $Name$

#include <AifSanteFeLoad.h>

#include <AifVersions.h>
#include <AifParseTree.h>
#include <AifParseTreeNodeFactory.h>
#include <AifParseTreeBuilder.h>

#include <AxInit.h>
#include <AxSmartPointer.h>
#include <AxFile.h>
#include <AxHeader.h>
#include <AxMob.h>
#include <AxIterator.h>

#include <AAFExtEnum.h>
#include <AAFSDKBuild.h>
#include <AAFResult.h>

#include <assert.h>

int AifSanteFeLoader::_instanceCount = 0;

AifSanteFeLoader::AifSanteFeLoader(const AxString& fileName,
								   const char* pAafComLib,
								   bool initAAF )
: _fileName( fileName )
{
	if ( initAAF && _instanceCount == 0) {
		std::auto_ptr<AxInit> initObj( new AxInit( pAafComLib ) );
		_initObj = initObj;
	}

	_instanceCount++;

#if AAF2XTL_MODIFY_FILE
	_axFile.OpenExistingModify( _fileName );
#else
	_axFile.OpenExistingRead( _fileName );
#endif
}

AifSanteFeLoader::~AifSanteFeLoader()
{
	_instanceCount--;

#if AAF2XTL_MODIFY_FILE
	_axFile.Save();
	_axFile.Close();
#else
	_axFile.Close();
#endif
}

std::auto_ptr<AifParseTree> AifSanteFeLoader::ParseFile( AifParseTreeNodeFactory& nodeFactory )
{
	AxMob axPrimaryMob( GetPrimaryMob() );
	
	return std::auto_ptr<AifParseTree>( 
		new AifParseTree( AifBuildParseTree( axPrimaryMob, nodeFactory ) ) );
}

IAAFMobSP AifSanteFeLoader::GetPrimaryMob()
{
	const wchar_t* errorMsg = L"No suitable composition mob found.";

	AxHeader axHeader( _axFile.getHeader() );

#if defined(AIF_V110)
	// First, search for the TopLevel composition mob.
	{
		aafSearchCrit_t crit;
		crit.searchTag = kAAFByCompositionMobUsageCode;
		crit.tags.usageCode = kAAFUsage_TopLevel;
		AxMobIter axCompMobIter( axHeader.GetMobs( crit ) );
		IAAFMobSP spMob;
		bool found = axCompMobIter.NextOne( spMob );
		if ( found ) {
			return spMob;
		}
	}
#else
	// Search for a mob named "TopLevel" until V1.1 support is folded in.
	{
		aafSearchCrit_t crit;
		crit.searchTag = kAAFByMobKind;
		crit.tags.mobKind = kAAFCompMob;
		AxMobIter axCompMobIter( axHeader.GetMobs( crit ) );
		IAAFMobSP spMob;
		bool found;
		for ( found = axCompMobIter.NextOne( spMob );
			  found;
			  found = axCompMobIter.NextOne( spMob ) ) {
            AxMob axMob( spMob );
  		    AxString topLevel( L"TopLevel" );
			try {
				if ( topLevel == axMob.GetName() ) {
					return spMob;
				}
			}
			catch( const AxExHResult& ex ) {
				if ( AAFRESULT_PROP_NOT_PRESENT != ex.getHResult() ) {
					throw;
				}
			}
		}
	}


#endif

	// There is no TopLevel composition mob. Check if there is
	// a single composition mob, and if so use it.
	{
		aafSearchCrit_t crit;
		crit.searchTag = kAAFByMobKind;
		crit.tags.mobKind = kAAFCompMob;
		AxMobIter axCompMobIter( axHeader.GetMobs( crit ) );
		IAAFMobSP spMob;
		bool found = axCompMobIter.NextOne( spMob );
		if ( found ) {
			// There should not be another.  If there is, throw an
			// exception because we have no way of determining which,
			// if any, is top level without a complex analysis.
			IAAFMobSP spUnused;
			bool nextFound = axCompMobIter.NextOne( spUnused );
			if ( nextFound ) {
				throw AxEx( errorMsg );
			}
			return spMob;
		}
	}

	// No suitable composition mob was found.  Log an error.
	throw AxEx( errorMsg );
}

IAAFHeaderSP AifSanteFeLoader::GetHeader()
{
	return _axFile.getHeader();
}

