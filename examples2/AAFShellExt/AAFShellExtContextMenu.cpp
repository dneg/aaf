// AAFShellExtContextMenu.cpp : Implementation of CAAFShellExtContextMenu
#include "stdafx.h"
#include "AAFShellExt.h"
#include "AAFShellExtContextMenu.h"
#include <PidlMgr.h>

// We want the templatized max, not the macro.
#ifdef max
    #undef max  
#endif // max

#include <limits>

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtContextMenu
// Opens the folder.
const UINT OPEN_FOLDER = 0;

// Explores the folder
const UINT EXPLORE_FOLDER = 1;

// Indicates the number of commands.
const USHORT COMMAND_COUNT = 2;

// Help strings which get displayed in the explorer
// status bar.
const _bstr_t HELP_TEXT[] = {
    _T("Opens the folder."),
    _T("Explores the folder.")
};

// If the commands are executed by verb, these
// are the verbs the handler understands.
const _bstr_t VERB_TEXT[] = {
    _T("OpenFolder"),
    _T("ExploreFolder")
};

const _bstr_t MENU_TEXT[] = {
    _T("Open"),
    _T("Explore")
};

CAAFShellExtContextMenu::CAAFShellExtContextMenu() :
    m_pidl( NULL )
{
}

void CAAFShellExtContextMenu::SetPIDL( LPCITEMIDLIST pidl, IShellFolder* pFolder )
{
    CPidlMgr pidlMgr;
    m_pidl = pidlMgr.Copy( pidl );
    m_pFolder = pFolder;
}

STDMETHODIMP CAAFShellExtContextMenu::QueryContextMenu(
    HMENU hMenu, 
    UINT indexMenu, UINT idCmdFirst, 
    UINT idCmdLast, UINT uFlags)
{
    MENUITEMINFO mii;
    ZeroMemory( &mii, sizeof( mii ) );
    if(!(CMF_DEFAULTONLY & uFlags))
    {
        // This information will be the same
        // for both cases.
        mii.cbSize = sizeof(mii);
        mii.fMask = MIIM_ID | MIIM_TYPE | MIIM_STATE;
        mii.fType = MFT_STRING;
        mii.fState = MFS_ENABLED | MFS_DEFAULT;

        // If explore is specified first,
        // make it first and the default
        if ( CMF_EXPLORE & uFlags )
        {
            mii.wID = idCmdFirst + EXPLORE_FOLDER;
            mii.dwTypeData = MENU_TEXT[EXPLORE_FOLDER];
            InsertMenuItem( hMenu, 
                indexMenu++, 
                true, 
                &mii );

            InsertMenu( hMenu, 
                indexMenu++, 
                MF_STRING | MF_BYPOSITION, 
                idCmdFirst + OPEN_FOLDER, 
                MENU_TEXT[OPEN_FOLDER]);
        }
        else
        {
            // Otherwise, make Open the
            // default.
            mii.wID = idCmdFirst + OPEN_FOLDER;
            mii.dwTypeData = MENU_TEXT[OPEN_FOLDER];
            InsertMenuItem( hMenu, 
                indexMenu++, 
                true, 
                &mii );

            InsertMenu( hMenu, 
                indexMenu++, 
                MF_STRING | MF_BYPOSITION, 
                idCmdFirst + EXPLORE_FOLDER, 
                MENU_TEXT[EXPLORE_FOLDER]);
        }
        
        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, 
            COMMAND_COUNT );
    }
    
    return E_FAIL;
}

STDMETHODIMP CAAFShellExtContextMenu::InvokeCommand(
    LPCMINVOKECOMMANDINFO lpcmi)
{
    // We will allow the structure to grow, but we will
    // not accept versions smaller that the one we
    // are built for.
    if ( lpcmi->cbSize < sizeof( CMINVOKECOMMANDINFO ) )
    {
        return E_INVALIDARG;
    }

    // Set the command to an impossibly high value.
    // If this was actually a valid number, we added
    // far too many items to the menu.
    UINT unCommand = std::numeric_limits<UINT>::max(); 
    if ( HIWORD(lpcmi->lpVerb) != 0 )
    {
        for ( UINT i = 0; i < COMMAND_COUNT; ++i )
        {
            if ( VERB_TEXT[i] == _bstr_t(lpcmi->lpVerb) )
            {
                unCommand = i;
                break;
            }
        }
    }
    else
    {
        unCommand = LOWORD(lpcmi->lpVerb);
    }

    // In case the command was not found, tell the
    // caller that the argument is invalid.
    if ( unCommand == std::numeric_limits<UINT>::max() )
    {
        return E_INVALIDARG;
    }

    // We have what we think is a valid command.
    // Invoke it.
    switch (unCommand)
    {
    case OPEN_FOLDER:
        OpenFolder(lpcmi, _T("open"));
        break;
    case EXPLORE_FOLDER:
        OpenFolder(lpcmi, _T("explore") );
    default:
        // I include default for completeness.
        // I catch more bugs by always adding this
        // to a switch that I care to count.
        return E_INVALIDARG;
    }
    
    return NOERROR;
}

STDMETHODIMP CAAFShellExtContextMenu::GetCommandString(
    UINT idCmd, UINT uFlags, 
    UINT* pwReserved, LPSTR pszName, UINT cchMax)
{
    // Validate the command.
    if ( ( idCmd < OPEN_FOLDER ) || 
         ( idCmd > OPEN_FOLDER + COMMAND_COUNT ) )
    {
        return E_INVALIDARG;
    }
    
    _bstr_t theText;

    // If the command is not doing validation,
    // then we just need to lookup the string in
    // the correct table.  This implementation
    // may not be the easiest thing to internationalize.
    // Still, it shouldn't be hard either.
    switch( uFlags )
    {
    case GCS_HELPTEXT:
        theText = HELP_TEXT[idCmd];
        break;
    case GCS_VALIDATE:
        return S_OK;
        break;
    case GCS_VERB:
        theText = VERB_TEXT[idCmd];
        break;
    default:
        return E_INVALIDARG;
    }

    if ( theText.length() <= cchMax )
    {
        // Even building with UNICODE defined,
        // the compiler thinks LPSTR is a 
        // typedef for char*.  COM expects
        // unicode.  It gets unicode.
        unsigned short* pName = 
            reinterpret_cast<unsigned short*>( pszName );
        wcscpy(pName, theText);
    }
    else
    {
        // The storage handed to me was far too small.
        // I can't tell the caller how much to send.
        // Technically, pszName points to a bit of memory
        // that is too small.
        return E_POINTER;
    }
    return S_OK;
}

void CAAFShellExtContextMenu::OpenFolder(LPCMINVOKECOMMANDINFO lpcmi,
    LPTSTR verb)
{
    SHELLEXECUTEINFO sei;

    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_IDLIST | SEE_MASK_CLASSNAME;
    sei.lpIDList = m_pidl;
    sei.lpClass = TEXT("folder");
    sei.hwnd = lpcmi->hwnd;
    sei.nShow = SW_SHOWNORMAL;
    
    sei.lpVerb = verb;
    
    ShellExecuteEx(&sei);
}

