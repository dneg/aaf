/*****************************************************************************\
*                                                                             *
* macdef.h - 	MAC specific definitions. All grouped here for reference.	  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1996, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/


#if !defined(__MACDEF__) && !defined(_MACDEF_H_)
#define __MACDEF__
#define _MACDEF_H_

#ifdef __cplusplus
extern "C" {
#endif


#if defined(_WIN32)
#include <limits.h>
#if UINT_MAX != ULONG_MAX
#pragma message("### warning: sizeof(int) != sizeof(long)!")
#endif
typedef int OLEBOOL;
#else
typedef unsigned long OLEBOOL;
#endif


/* Predefined Clipboard Formats */


#define cfPict          'PICT'
#define cfText          'TEXT'
#define cfOwnerLink     'OLNK'          /* ole clip formats */
#define cfObjectLink    'OJLK'
#define cfLink          'LINK'
#define cfNative        'NATV'
#define cfBinary        'BINA'
#define cfFileName      'FNAM'
#define cfNetworkName   'NETN'
#define cfClap          'CLAP'
#define	cfTransferDesc	'TNAM'
#ifdef __powerc
#define	cfDataObject	'MURF'
#else
#define	cfDataObject	'DOBJ'
#endif

#define cfDataObjectSig 'DSIG'
#define	cfEmbeddedObject 'EMBO'
#define	cfEmbedSource 	'EMBS'
#define	cfLinkSource 	'LNKS'
#define	cfOleDraw	 	'DRAW'
#define	cfLinkSrcDescriptor	 	'LKSD'
#define	cfObjectDescriptor	 	'OBJD'
#define cfOle10Native   'OLE1'          /* Mac format tag of IStorages with "\1Ole10Native" streams */

#define typeDDE 		'DDE '
#define typeRNCF		'RNCF'
#define typeEMBD		'EMBD'


typedef struct tagColorEntry {
    Byte	red;
    Byte	green;
    Byte	blue;
    Byte	flags;
} ColorEntry;
typedef ColorEntry  *LPColorEntry;

// Color scheme table
typedef struct tagOLECOLORSCHEME {
    short		version;
    short		numEntries;
    ColorEntry	oleColor[1];
} OLECOLORSCHEME;
typedef OLECOLORSCHEME	    *LPOLECOLORSCHEME;

#if !defined(LOGPALETTE) && !defined(_WIN32)

#define LOGPALETTE			OLECOLORSCHEME
#define LPLOGPALETTE		LPOLECOLORSCHEME

#endif	// !LOGPALETTE

// Registration database API's etc.


/* return codes from win 3.1 Registration functions */
/* Retained for compatibility reasons */

#ifndef  ERROR_SUCCESS
#define ERROR_SUCCESS           0L
#endif

#ifndef  ERROR_BADDB
#define ERROR_BADDB             1L
#endif

#ifndef  ERROR_BADKEY
#define ERROR_BADKEY            2L
#endif

#ifndef  ERROR_CANTOPEN
#define ERROR_CANTOPEN          3L
#endif

#ifndef  ERROR_CANTREAD
#define ERROR_CANTREAD          4L
#endif

#ifndef  ERROR_CANTWRITE
#define ERROR_CANTWRITE         5L
#endif

#ifndef  ERROR_OUTOFMEMORY
#define ERROR_OUTOFMEMORY       6L
#endif

#ifndef  ERROR_INVALID_PARAMETER
#define ERROR_INVALID_PARAMETER 7L
#endif

#ifndef  ERROR_ACCESS_DENIED
#define ERROR_ACCESS_DENIED     8L
#endif


/* new  error codes / or new error code names */

#ifndef  ERROR_INSUFFICIENT_MEMORY	
#define ERROR_INSUFFICIENT_MEMORY	10L
#endif


#ifndef  ERROR_CANNOT_OPEN_REGFILE	
#define ERROR_VALUE_NOT_FOUND		9L

#define ERROR_CANNOT_OPEN_REGFILE	11L
#define ERROR_REGFILE_READERROR		12L
#define ERROR_REGFILE_WRITEERROR	13L
#define ERROR_REGKEY_NOTFOUND		14L
#define ERROR_REGKEY_INVALIDKEY		15L
#define	ERROR_REG_BUFFER_OVERFLOW	16L
		// same as insufficient memory
#define	ERROR_REGKEY_CANNOT_DELETE	17L

#endif



// REGENUMPROGIDF; flags indicating the status of a subkey returned
// from RegEnumProgID.
typedef enum tagREGENUMPROGIDF
{
    REGENUMPROGIDF_ISPROGID		= 1,   // RegEnumProgID returned subkey that is a ProgID
    REGENUMPROGIDF_EMBPREF 	 	= 2,   // RegEnumProgID returned subkey from Embedding Pref file
    REGENUMPROGIDF_INSERTABLE	= 4    // RegEnumProgID returned a ProgID that is Insertable
	,REGENUMPROGIDF_FORCELONG	=	2147483647
} REGENUMPROGIDF;

typedef long    OLELONG;

// Server Registration API

typedef enum {
    OLEREG_OK,  					// Function operated correctly
	OLEREG_ERROR_SUCCESS = OLEREG_OK,
	OLEREG_INVALID_KEY,				// invalid reg db key to close reg
	OLEREG_INVALID_KEYWORD,			// invalid reg db keyword to query/set reg
	
	OLEREG_ITEM_NOT_FOUND,		
	OLEREG_INDEX_NOT_FOUND,			// index in item list not exist
	OLEREG_ENUM_DONE = OLEREG_INDEX_NOT_FOUND,		// enumeration done	
	
	OLEREG_REG_NOT_OPEN,			// reg not open when calling close reg
	OLEREG_DB_NOT_FOUND,			// emb pref file not found
	OLEREG_DB_LOCKED,				// emb pref file locked by another app
	OLEREG_DB_ERROR, 				// emb pref file error while opening/reading/writing
	OLEREG_ERR_MEMORY,				// insuff. or bad memory operation
	OLEREG_DB_BADFORM,				// reg db has invalid entries
	OLEREG_FILE_NOT_FOUND,			// specified file not found

    //......more codes here,      	//


	OLEREG_ERR_GENERIC,				// some general error
	OLEREG_ERR_BUFFER_SIZE,			// buffer to set data illegitimate size
	OLEREG_ERR_NO_INIT,			// buffer to set data illegitimate size
	OLEREG_ERROR_BADKEY	= 2,
	OLEREGSTATUS_FORCELONG	=	2147483647
	
} OLEREGSTATUS;



typedef char *  OLEREG_VALUE;
typedef long    OLEREG_HKEY;

typedef enum {
	OLEREG_ROOT,
	OLEREG_GENERIC,

	OLEREGSVR_CLASS,
	OLEREGSVR_HUMAN_READABLE,
	OLEREGSVR_EXTENSION,                   		// win
	OLEREGSVR_FILE_TYPE,						// mac
//	OLEREGSVR_FILE_TYPE = OLEREGSVR_EXTENSION,	// mac
	OLEREGSVR_PROTOCOL,
	OLEREGSVR_SERVER,                       	// win
	OLEREGSVR_SIGNATURE = OLEREGSVR_SERVER,		// mac
	OLEREGSVR_HANDLER,
	OLEREGSVR_VERB,
	OLEREGSVR_NET_DRIVE,    	// network volume for server


                	            // The following keywords can only
	OLEREG_QUERY_ONLY = 2000, // be used with OleregGetValue()
	OLEREGSVR_CLASS_FROM_HUMAN_READABLE,
	OLEREGSVR_CLASS_FROM_EXTENSION,           // win
	OLEREGSVR_CLASS_FROM_SIG = OLEREGSVR_CLASS_FROM_EXTENSION	// mac
	,OLEREG_KEYWORD_FORCELONG	=	2147483647
} OLEREG_KEYWORD;

// this const used by OleregRemoveKey to delete all items of a list.
#define	REG_ALL_INDEXES	0xFFFFFFFF

typedef enum {
	REG_APPEND,					// 0 add to end of list
	REG_IGNORE = REG_APPEND,	// 0 parameter not used
	REG_INSERT,					// 1 insert at front of list
	REG_REPLACE,				// 2 replace all previous entries w/ new one
	REG_DELETE = REG_REPLACE	// 2 replace all previous entries w/ new one
	,OLEREG_ORDER_FORCELONG	=	2147483647
} OLEREG_ORDER;


#define OLEICON_CCHKEYMAX		256
#define OLEICON_CCHPATHMAX		256
#define OLEICON_CCHLABELMAX		40
#define OLEICON_CCHDBENTRYMAX	256

// This is the OleIconSource structure returned by OleUIPictExtractIconSource
//	For an explanation of its use, consult {stdimpl}IconSrc.h
typedef enum { FileAndResIDMethod,
				CreatorAndIndexMethod,
				CreatorAndTypeMethod,
				SystemResIDMethod
} IconRetrievalMethod ;
	
typedef struct {
	IconRetrievalMethod method;

	union {
		struct {
			char 	szPathname[OLEICON_CCHPATHMAX];
			long 	lFileID;
			short	sResID;
		} FileAndResIDStruct;
		
		struct {
			OSType	lCreator;
			short	sIndex;
		} CreatorAndIndexStruct;
		
		struct {
			OSType lCreator;
			OSType lType;
		} CreatorAndTypeStruct;
		
		struct {
			short sResID;
		} SystemResIDStruct;
		
	} IconSourceUnion;
} OleIconSource;

#ifdef __cplusplus
}
#endif

#endif /* __MACDEF__ */

