#if !defined(AAFSHELLEXTLISTVIEW_H)
#define AAFSHELLEXTLISTVIEW_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AAFShellExtListView.h : header file
//
#ifndef COMMANDHANDLER_H
    #include <CommandHandler.h>
#endif // COMMANDHANDLER_H

/////////////////////////////////////////////////////////////////////////////
// CAAFShellExtListView window

class CAAFShellExtListView : public CommandHandler<CWnd>
{
// Construction
public:
	CAAFShellExtListView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAAFShellExtListView)
	public:
	virtual BOOL DestroyWindow();
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void Refresh();
	bool Init( LPSV2CVW2_PARAMS lpParams );
	virtual ~CAAFShellExtListView();
    FOLDERVIEWMODE GetViewMode();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAAFShellExtListView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHelpAbout();
	afx_msg void OnUpdateViewDetails(CCmdUI* pCmdUI);
	afx_msg void OnViewDetails();
	afx_msg void OnUpdateViewLargeicons(CCmdUI* pCmdUI);
	afx_msg void OnViewLargeicons();
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnViewList();
	afx_msg void OnUpdateViewSmallicons(CCmdUI* pCmdUI);
	afx_msg void OnViewSmallicons();
	//}}AFX_MSG
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()

private:
	void RemoveCachedPIDLs();
	void SetupReportView();

    CListCtrl m_list;
    CImageList m_smallImageList;
    CImageList m_largeImageList;
    static const UINT m_iconIDs[];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AAFSHELLEXTLISTVIEW_H)
