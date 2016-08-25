// cssample5.h : main header file for the CSsample5 application
//

#if !defined(AFX_CSsample5_H__0F66B0C4_0D16_4778_AC04_BDC10627944E__INCLUDED_)
#define AFX_CSsample5_H__0F66B0C4_0D16_4778_AC04_BDC10627944E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCssample5App:
// See cssample5.cpp for the implementation of this class
//

class CCssample5App : public CWinApp
{
public:
	CCssample5App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCssample5App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CCssample5App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSsample5_H__0F66B0C4_0D16_4778_AC04_BDC10627944E__INCLUDED_)
