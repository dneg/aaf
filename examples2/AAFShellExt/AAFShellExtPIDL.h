#ifndef AAFSHELLEXTPIDL_H
#define AAFSHELLEXTPIDL_H


enum AAFShellExtPidlType{
    PIDL_FOLDER,
    PIDL_DATAITEM,
    PIDL_UNKNOWN
};


// A magic number (actually, a uuid) that lets us uniquely identify our own pidls.

const GUID AAFSHELL_PIDL_ID = 
{ 0xdc03ca04, 0x4740, 0x47d9, {0xbe, 0xdb, 0x00, 0xe6, 0xfe, 0xf7, 0xc4, 0x88} };

#define AAFSHELL_MAX_DISPLAY_TEST_SIZE 256

struct AAFShellExtPidl
{
	GUID				id;
	int					lid; // level id  (ie. (level<<16)|_levelCounter[level])
    int					nVersion;  
	// FIXME !  Saving a pointer in a pidl is not a good idea.  It
	// cannot be made persistant.  Find a better way to do this.
	LPCITEMIDLIST		absPidl;
	// FIXME - Be more flexible.  Dynamially allocate string and
	// store at the end of the strucutre (same goes for above pidl).
    wchar_t				pDisplayText[AAFSHELL_MAX_DISPLAY_TEST_SIZE];
    AAFShellExtPidlType eType;
};


#endif // AAFSHELLEXTPIDL_H
