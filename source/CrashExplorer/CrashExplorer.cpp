/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Application entry point.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "stdafx.h"
#include "resource.h"

#include "AboutDlg.h"
#include "MainDlg.h"
#include "RegSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// Application object.
CAppModule _Module;

/**
 * @brief Displays main dialog and starts main event loop.
 * @param pszCmdLine - application command line.
 * @param nCmdShow - specifies how the window is to be shown.
 */
int Run(PTSTR pszCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	pszCmdLine;
	nCmdShow;

	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if (dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}
	dlgMain.RestoreWindowSettings(nCmdShow);

	int nRet = theLoop.Run();
	_Module.RemoveMessageLoop();

	return nRet;
}

/**
 * @brief Application entry point.
 * @param hInstance - Handle to the current instance of the application.
 * @param hPrevInstance - Handle to the previous instance of the application. This parameter is always NULL.
 * @param pszCmdLine - Pointer to a null-terminated string specifying the command line for the application, excluding the program name.
 * @param nCmdShow - Specifies how the window is to be shown.
 */
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, PTSTR pszCmdLine, int nCmdShow)
{
#ifdef _CRTDBG_MAP_ALLOC
	// Watch for memory leaks.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	HRESULT hRes = OleInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	// This resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used.
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES);	// Add flags to support other controls.

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(pszCmdLine, nCmdShow);

	_Module.Term();
	OleUninitialize();

	return nRet;
}
