#include "stdafx.h"
#include "resource.h"
#include "Summary Hook_idl.h"
#include "Summary HookApp.h"
#include "SummaryHook.h"
#include "StockIDispatch.h"
#include "SummaryHookSettings.h"
#include <StockIDispatch.h>

///////////////////////////////////////////////////////////////////////
//
// If a function comment has an initial untitled section, it is, by 
// default, the remarks section. This hook takes the first sentence or
// paragraph in this default section and repeats it as the summary.
//
// We do this by modifying the comment block we get when OnObjectFound is
// called for a function object. We extract the initial default section (if
// there is one) and repeat either the first sentence or paragraph of this
// as a summary section. For example:
//
//    // Mangoes the rhubarb in this orange.
//    //
//    // Parameters:
//    // nbr - The number of times to perform this operation
//    void MangoIt(int nbr)
//
// Will be replaced with
//
//    // Summary:
//    // Mangoes the rhubarb in this orange.
//    //
//    // Remarks:
//    // Mangoes the rhubarb in this orange.
//    //
//    // Parameters:
//    // nbr - The number of times to perform this operation
//    void MangoIt(int nbr)
//
// The first sentence is defined to end with a full stop and a new line.
// This is so that "e.g. example" and "apple.Orange()" don't get included
// as sentence end marks.
// The first paragraph is defined to end with a blank line or a section marker.
//
// There are times when you don't want a summary auto-generated. For example,
// in a derived version of a virtual function, the remarks (or default remarks)
// section is appended to the base class's remarks section as an "Implementation
// Notes:" section. The summary is also automatically picked up from the base
// class. To prevent the summary from being generated, simply have no initial
// default section. For example:
//
//    // Remarks:
//    // This version always returns NULL.
//    virtual char* Apricot() const;
//
// If you wish to write a summary that isn't repeated in the remarks then you
// need to write both sections. For example:
//
//    // Summary:
//    // Returns the total length.
//    //
//    // Remarks:
//    // Adds up all the lengths of the sub-components and returns this total.
//    // int TotalLength() const;
//
// This hook implements a settings dialog. The dialog itself is pretty
// basic (implemented in "Settings.cpp"). We use the settings to store
// the names of the keywords that introduce sections in the comments
// and whether to choose the first sentence or first paragraph.
//
///////////////////////////////////////////////////////////////////////


// All this hash table stuff was purloined from Standard Scanners

const wchar_t *HashTable::Search( const wchar_t *start, const wchar_t *& end )
{
	unsigned int h = ComputeHash( start );
	while ( hashTable[h].introducer != 0 ) {
		if ( Compare( hashTable[h].introducer, start, end ) )
		  return hashTable[h].sectionName;
		++h;
		if ( h == hashTableSize )
		  h = 0;
	}
	return 0;
}

HashTable::HashTable()
{
	hashTable = 0;
	hashTableSize = 0;
}

void HashTable::Close()
{
	if ( hashTable != 0 ) {
		for ( unsigned int i = 0; i < hashTableSize; ++i ) {
			if ( hashTable[i].introducer != 0 ) {
				free( hashTable[i].introducer );
				free( hashTable[i].sectionName );
			}
		}
		delete[hashTableSize] hashTable;
		hashTable = 0;
		hashTableSize = 0;
	}
}

HashTable::~HashTable()
{
	Close();
}

void HashTable::Add( const wchar_t *introducer, const wchar_t *sectionName )
{
	unsigned int h = ComputeHash( introducer );
	while ( hashTable[h].introducer != 0 ) {
		++h;
		if ( h == hashTableSize )
		  h = 0;
	}
	hashTable[h].introducer = wcsdup( introducer );
	hashTable[h].sectionName = wcsdup( sectionName );
}

static inline void mash( unsigned int &h, wchar_t c )
{
	h = (h << 1) ^ c ^ (h >> 31);
}

static inline void skipspaces( wchar_t const *&s )
{
	while ( *s == ' ' ) ++s;
}

unsigned int HashTable::ComputeHash( const wchar_t *text )
{
	unsigned int hash = 0;

	// hash it such that:
	//   one or more spaces count as one space
	//   uppercase==lowercase

	const wchar_t *s = text;
	skipspaces( s );
	while ( *s != '\0' && *s != ':' && *s != '\n' ) {
		if ( *s == ' ' ) {
			++s;
			skipspaces( s );

			if ( *s != '\0' && *s != ':' && *s != '\n' )
			  mash( hash, ' ' );
		}
		else if ( iswlower( *s ) )
		  mash( hash, towupper(*s++) );
		else
		  mash( hash, *s++ );
	}

	return hash % hashTableSize;
}

bool HashTable::Compare( const wchar_t *introducer, const wchar_t *text,
			 wchar_t const *& end )
{
	const wchar_t *ip = introducer;
	const wchar_t *tp = text;

	// Same as with the hash:
	//   spaces=space
	//   uppercase=lowercase
	// Also, at the end, verify that block is trailed by a newline or colon.

	skipspaces(ip);
	skipspaces(tp);
	while ( *ip != '\0' ) {
		if ( *ip == ' ' && *tp == ' ' ) {
			++ip;
			skipspaces(ip);
			++tp;
			skipspaces(tp);
		}
		else {
			wchar_t cip = *ip;
			if ( iswlower(cip) )
			  cip = towupper(cip);
			wchar_t ctp = *tp;
			if ( iswlower(ctp) )
			  ctp = towupper(ctp);
			if ( cip != ctp )
			  return false;
			++ip;
			++tp;
		}
	}

	skipspaces(tp);
	end = tp;
	return ( *tp == '\n' ) || ( *tp == ':' ) || ( *tp == '\0' );
}

#include <math.h>
static bool prime( int c )
{
	int end = (int)sqrt( c );
	// Kindof stupid, but it'll be speedy enough...
	for ( int d = 3; d <= end; ++d )
	  if ( c % d == 0 )
	    return false;

	return true;
}

void HashTable::Initialize( IProjectConfiguration *config )
{
	ASSERT( hashTable == 0 );

	SAFEARRAY *allCommentSections = 0;
	config->GetAllCommentSectionNames( &allCommentSections );
	ASSERT( allCommentSections->cDims == 1 &&
		allCommentSections->cbElements == sizeof(BSTR) );

	SafeArrayLock( allCommentSections );
	int c = 0;
	for ( unsigned long i = 0; i < allCommentSections->rgsabound[0].cElements; ++i ) {
		void *ppvData;
		VERIFY( S_OK == SafeArrayPtrOfIndex( allCommentSections, (long *)&i, &ppvData ) );
		BSTR &section = *(BSTR *)ppvData;
		int numIntros = 0;
		SAFEARRAY *intros;
		config->GetCommentSectionIntroducers( section, &intros );
		ASSERT( intros->cDims == 1 && intros->cbElements == sizeof(BSTR) );
		c += intros->rgsabound[0].cElements;
		SafeArrayDestroy( intros );
	}

	// Now find a prime number that's more than 2c+1...
	c = 2*c + 1;
	while ( !prime( c ) )
	  c += 2; // skip even numbers
	hashTableSize = c;

	// Okay, now we know how big the table will be, so let's create it.
	hashTable = new HashTableEntry[hashTableSize];

	for ( i = 0; i < hashTableSize; ++i ) {
		hashTable[i].introducer = 0;
		hashTable[i].sectionName = 0;
	}

	// Now fill it in...
	for ( i = 0; i < allCommentSections->rgsabound[0].cElements; ++i ) {
		void *ppvData;
		VERIFY( S_OK == SafeArrayPtrOfIndex( allCommentSections, (long *)&i, &ppvData ) );
		BSTR &section = *(BSTR *)ppvData;

		SAFEARRAY *intros = 0;
		config->GetCommentSectionIntroducers( section, &intros );
		ASSERT( intros->cDims == 1 && intros->cbElements == sizeof(BSTR) );
		SafeArrayLock( intros );
		for ( unsigned long j = 0; j < intros->rgsabound[0].cElements; ++j ) {
			VERIFY( S_OK == SafeArrayPtrOfIndex( intros, (long *)&j, &ppvData ) );
			BSTR &intro = *(BSTR *)ppvData;
			Add( intro, section );
		}
		SafeArrayUnlock( intros );
		SafeArrayDestroy( intros );
	}
	SafeArrayUnlock( allCommentSections );
	SafeArrayDestroy( allCommentSections );
}

void HashTable::InitializeJustDescription( IProjectConfiguration *config )
{
	ASSERT( hashTable == 0 );

	int numIntros = 0;
	SAFEARRAY *intros = 0;
	config->GetCommentSectionIntroducers( L"Description", &intros );
	if ( intros == 0 ) {
		// This is a wierd kind of situation, so it doesn't really require
		//  a sensible reply...
		hashTableSize = 1;
		hashTable = new HashTableEntry[1];
		hashTable[0].introducer = 0;
		hashTable[0].sectionName = 0;
	}
	else {
		ASSERT( intros->cDims == 1 && intros->cbElements == sizeof(BSTR) );
		int c = intros->rgsabound[0].cElements;

		// Now find a prime number that's more than 2c+1...
		c = 2*c + 1;
		while ( !prime( c ) )
		  c += 2; // skip even numbers
		hashTableSize = c;

		// Okay, now we know how big the table will be, so let's create it.
		hashTable = new HashTableEntry[hashTableSize];

		for ( unsigned int i = 0; i < hashTableSize; ++i ) {
			hashTable[i].introducer = 0;
			hashTable[i].sectionName = 0;
		}

		// Now fill it in...
		SafeArrayLock( intros );
		for ( unsigned long j = 0; j < intros->rgsabound[0].cElements; ++j ) {
			void *ppvData;
			VERIFY( S_OK == SafeArrayPtrOfIndex( intros, (long *)&j, &ppvData ) );
			BSTR &intro = *(BSTR *)ppvData;
			Add( intro, L"Description" );
		}
		SafeArrayUnlock( intros );
		SafeArrayDestroy( intros );
	}
}

const wchar_t *HashTable::FindIntroducerFor( const wchar_t *logicalName )
{
	for ( unsigned int i = 0; i < hashTableSize; ++i ) {
		if ( hashTable[i].sectionName != 0 &&
		     0 == wcscmp( hashTable[i].sectionName, logicalName ) )
		  return hashTable[i].introducer;
	}
	return 0;
}





STDMETHODIMP CSummaryHook::GetLongDescription( BSTR *out )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	CString d;
	VERIFY( d.LoadString( IDS_SummaryHook_DESCRIPTION ) );
	USES_CONVERSION;
	*out = SysAllocString( A2W(d) );
	return S_OK;
}


STDMETHODIMP CSummaryHook::TestOutputFormat( IOutputFormat *o, BOOL *isComplete )
{
	*isComplete = TRUE;
	return S_OK;
}

CSummaryHook::CSummaryHook()
{
	summaryIntro = 0;
	remarksIntro = 0;
}


STDMETHODIMP CSummaryHook::Initialize( IProjectConfiguration *p )
{
	CComBSTR s;
	VERIFY( S_OK == p->GetAttribute( L"Usage", &s ) );
	if ( s != 0 && 0 == wcsicmp( s, L"paragraph" ) )
	  usage = PARAGRAPH;
	else
	  usage = SENTENCE;

	if ( summaryIntro == 0 ) {
		sections.Initialize( p );
		summaryIntro = sections.FindIntroducerFor( L"Description" );
		remarksIntro = sections.FindIntroducerFor( L"Remarks" );
	}

	return S_OK;
}

void CSummaryHook::StoreSettings( IProjectConfiguration *p )
{
	VERIFY( S_OK == p->SetAttribute( L"Usage",
		(usage == SENTENCE ? L"Sentence" : L"Paragraph" ) ) );
}


// IDocJetAddInSettings

STDMETHODIMP CSummaryHook::GetAXControl( IDocJetAdvancedSettingsControl **ppbg )
{
	if ( *ppbg != 0 )
	  (*ppbg)->Release();
	CComObject<CSummaryHookSettingsControl> *x = new CComObject<CSummaryHookSettingsControl>;
	x->owner = this;
	*ppbg = x;
	(*ppbg)->AddRef();
	return S_OK;
}



class CSummaryHookObjectIterator :
	public StockIDispatch<IHookIterator,&IID_IHookIterator,
			      &LIBID_DocJetInterfaceLib>
{
	CSummaryHook *hook;
	IObjectCollection *allObjects;
      public:
	CSummaryHookObjectIterator( CSummaryHook *hook, IObjectCollection *allObjects );
	~CSummaryHookObjectIterator();

	STDMETHOD(DoIt)( IObject *obj,
			 BSTR name,
			 ISubtype *subtype,
			 BSTR inheritor,
			 BSTR sourceFileName,
			 ULONG sourceFileLine,
			 BSTR comment,
			 BSTR declaration,
			 BSTR body,
			 BSTR attr1,
			 BSTR attr2,
			 BSTR attr3 );
};

CSummaryHookObjectIterator::CSummaryHookObjectIterator
	( CSummaryHook *p_hook, IObjectCollection *p_allObjects )
{
	hook = p_hook;
	((IDocJetAddIn *)hook)->AddRef();
	allObjects = p_allObjects;
	allObjects->AddRef();
}

CSummaryHookObjectIterator::~CSummaryHookObjectIterator()
{
	((IDocJetAddIn *)hook)->Release();
	allObjects->Release();
}

STDMETHODIMP CSummaryHookObjectIterator::DoIt( IObject *obj,
					BSTR name,
					ISubtype *subtype,
					BSTR inheritor,
					BSTR sourceFileName,
					ULONG sourceFileLine,
					BSTR comment,
					BSTR declaration,
					BSTR body,
					BSTR attr1,
					BSTR attr2,
					BSTR attr3 )
{
	if ( comment == 0 )
	  return S_OK; // <<--  Quickly dispense with the null comment case.

	RoughType rt;
	VERIFY( S_OK == subtype->get_Classification( &rt ) );
	if ( rt != TYPEDEF_ID && rt != FUNCTION_ID && rt != CLASS_ID )
	  return S_OK;

	// Start at the beginning of the comment and look for the 
	// start of a section.
	const wchar_t *s = comment;
	bool foundAnyTextYet = false;
	const wchar_t *sectionStart = 0;

	while (*s != '\0' && sectionStart == 0 ) {
		while ( *s == ' ' )
		  ++s;
		if ( *s == '\n' )
		  ++s;
		else {
			const wchar_t* nss;
			const wchar_t *section = hook->sections.Search( s, nss );
			if ( section == 0 ) {
				foundAnyTextYet = true;
				const wchar_t* newLine = wcschr(s, '\n');
				if (newLine == 0)
				  s += wcslen(s);
				else
				  s = newLine + 1;
			}
			else {
				if ( !foundAnyTextYet )
				  return S_OK; // <<-- No need to do anything, this
				               //  comment starts with a section.
				sectionStart = s;
			}
		}
	}
	if ( sectionStart == 0 )
	  sectionStart = s;

	// do we have a sentence ?
	
	const wchar_t* period = comment;
	
	while(period = wcschr(period, '.'))
		{
		// Found a period, is it followed by whitespace?
		if (*period && *(period + 1) && ((*(period + 1) == ' ')||(*(period + 1) == '\n'))){
			// Found the end of the sentence!
			period++;
			break;
			}
		if(*period) period++;
		}

    if ((hook->usage == CSummaryHook::SENTENCE)&& period)
        {
		// Start at the beginning of the comment and look for a newline
		// preceeded with a dot, before the start of a section.
		// <SMC> changed to search for period followed by whitespace
		s = period;
        }
    else if (hook->usage == CSummaryHook::PARAGRAPH) {
		// Start at the beginning of the comment and look for a 
		// blank line, before the start of a section.
		s = comment;
		while (s < sectionStart) {
			const wchar_t *pastSpaces = s;
			while ( *pastSpaces == ' ' )
			  ++pastSpaces;
			if ( *pastSpaces == '\n' )
			  break;

			const wchar_t* newLine = wcschr(s, '\n');
			if (newLine == 0)
			  s = sectionStart;
			else
			  s = newLine+1;
			}
        }
	else {
		ASSERT(0);
		return S_OK;
	}
        
   	if((sectionStart-s)<0)
		s = sectionStart;

        // Create a new comment with the summary prefixed.
        // Sometimes the summary section doesn't end with a new line
        // (e.g. the whole comment is just one line) so append
        // a new line to make sure the Summary and Remarks sections
        // are separated by a blank line.
        int newSize = 3+wcslen(hook->summaryIntro) +
                      (s - comment) + 
                      ((*(s - 1) == '\n') ? 0 : 1) + 
                      3+wcslen(hook->remarksIntro) + 
                      wcslen(comment);
        wchar_t *newComment = (wchar_t*)_alloca( sizeof(wchar_t)*(newSize + 1) );

	wcscpy(newComment, L" " );
	wcscat(newComment, hook->summaryIntro);
	wcscat(newComment, L":\n" );
	wcsncat(newComment, comment, (s - comment));
	wcscat(newComment, (*(s - 1) == '\n') ? L"" : L"\n");
	if(sectionStart-s) { // else remarks must be empty
		wcscat(newComment, L" " );
		wcscat(newComment, hook->remarksIntro); // <SMC> only place remainder in remarks
		wcscat(newComment, L":\n" );
		wcsncat(newComment, s, (sectionStart-s));
		wcscat(newComment, L"\n" );
		}
	wcscat(newComment, sectionStart );

	obj->put_Comment( newComment );

	return S_OK;
}



// IObjectMapper
STDMETHODIMP CSummaryHook::RewriteObjects( IObjectCollection *allObjects,
						 IErrors *errors )
{
	if ( summaryIntro == 0 || remarksIntro == 0 )
	  return S_OK;

	CSummaryHookObjectIterator allObjectsIterator( this, allObjects );
	allObjectsIterator.OnStack();
	allObjects->Foreach( &allObjectsIterator, 0, 0, 0 );
	return S_OK;
}


STDMETHODIMP CSummaryHook::AugmentSubtypes
	( ISourceFileScanner *filetype )
{
	return S_OK;
}







