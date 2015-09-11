// BugTrapTest.h : main header file for the BugTrapTest application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// CBugTrapTestApp:
// See BugTrapTest.cpp for the implementation of this class
//

class CBugTrapTestApp : public CWinApp
{
public:
	CBugTrapTestApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	DECLARE_MESSAGE_MAP()

	afx_msg void OnAppAbout();
	afx_msg void OnTestAccessViolation();
	afx_msg void OnTestCppException();
};

extern CBugTrapTestApp theApp;
