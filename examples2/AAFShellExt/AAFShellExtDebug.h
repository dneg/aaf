// AAFShellExtDebug.h

#include <PidlMgr.h>

void _AAFShellExtLog( const char* msg );
void _AAFShellExtLog( const wchar_t* msg );
void _AAFShellExtLog( int line );
void _AAFShellExtPidlLog( LPCITEMIDLIST pidl );

#if 0
#define AAFShellExtLog( msg ) \
{_AAFShellExtLog( __FILE__ );  _AAFShellExtLog( __LINE__); _AAFShellExtLog(msg);}

#define AAFShellExtPidlLog( pidl) \
{_AAFShellExtLog( __FILE__ ); _AAFShellExtLog( __LINE__); _AAFShellExtPidlLog( pidl );}
#elif 0
#define AAFShellExtLog( msg ) _AAFShellExtLog( msg )
#define AAFShellExtPidlLog( pidl) _AAFShellExtPidlLog( pidl )
#else
#define AAFShellExtLog( msg ) 
#define AAFShellExtPidlLog( pidl) 
#endif

