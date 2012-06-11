/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Describe Error dialog.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "resource.h"
#include "DescribeErrorDlg.h"
#include "BugTrapUI.h"
#include "BugTrapUtils.h"
#include "WaitCursor.h"
#include "LayoutManager.h"
#include "Globals.h"
#include "MemStream.h"
#include "Encoding.h"
#include "textFormat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @addtogroup BugTrapUI BugTrap Graphical User Interface
 * @{
 */

/// Control layouts for Send Mail dialog.
static LAYOUT_INFO g_arrDescribeErrorLayout[] =
{
	LAYOUT_INFO(IDC_DETAILS, ALIGN_LEFT,  ALIGN_TOP,    ALIGN_RIGHT, ALIGN_BOTTOM),
	LAYOUT_INFO(IDOK,        ALIGN_RIGHT, ALIGN_BOTTOM, ALIGN_RIGHT, ALIGN_BOTTOM),
	LAYOUT_INFO(IDCANCEL,    ALIGN_RIGHT, ALIGN_BOTTOM, ALIGN_RIGHT, ALIGN_BOTTOM)
};

/// Dialog layout manager.
static CLayoutManager g_LayoutMgr;

/**
 * @brief WM_COMMAND handler of Send Mail dialog.
 * @param hwnd - window handle.
 * @param id - control ID.
 * @param hwndCtl - control handle.
 * @param codeNotify - notification code.
 */
static void DescribeErrorDlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (codeNotify)
	{
	case BN_CLICKED:
		switch (id)
		{
		case IDOK:
			{
				CWaitCursor wait(true);
				hwndCtl = GetDlgItem(hwnd, IDC_DETAILS);
				int nTextLength = GetWindowTextLength(hwndCtl) + 1;
				PTSTR pszMessage = new TCHAR[nTextLength];
				if (! pszMessage)
					return;
				GetWindowText(hwndCtl, pszMessage, nTextLength);
				if (*pszMessage)
				{
					TCHAR szFileName[MAX_PATH];
					PathCombine(szFileName, g_szInternalReportFolder, _T("message.txt"));
					HANDLE hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hFile != INVALID_HANDLE_VALUE)
					{
						DWORD dwWritten;
						WriteFile(hFile, g_arrUTF8Preamble, sizeof(g_arrUTF8Preamble), &dwWritten, NULL);
						CMemStream MemStream(nTextLength);
						CUTF8EncStream EncStream(&MemStream);
						EncStream.WriteUTF8Bin(pszMessage);
						DWORD dwMessageLength = (DWORD)MemStream.GetLength();
						WriteFile(hFile, MemStream.GetBuffer(), dwMessageLength, &dwWritten, NULL);
						CloseHandle(hFile);
						g_pSymEngine->AppendFileToReport(g_szInternalReportFilePath, szFileName);
					}
				}
				delete[] pszMessage;
				EndDialog(hwnd, TRUE);
			}
			break;
		case IDCANCEL:
			EndDialog(hwnd, FALSE);
			break;
		}
		break;
	}
}

/**
 * @brief WM_INITDIALOG handler of Send Mail dialog.
 * @param hwnd - window handle.
 * @param hwndFocus - system-defined focus window.
 * @param lParam - user-defined parameter.
 * @return true to setup focus to system-defined control.
 */
static BOOL DescribeErrorDlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	lParam; hwndFocus;
	HWND hwndCtl;
	_ASSERTE(g_pResManager != NULL);
	if (g_pResManager->m_hBigAppIcon)
		SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)g_pResManager->m_hBigAppIcon);
	if (g_pResManager->m_hSmallAppIcon)
		SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)g_pResManager->m_hSmallAppIcon);
	CenterWindow(hwnd, GetParent(hwnd));
	g_LayoutMgr.InitLayout(hwnd, g_arrDescribeErrorLayout, countof(g_arrDescribeErrorLayout));

	hwndCtl = GetDlgItem(hwnd, IDC_DETAILS);
	SetFocus(hwndCtl);
	return FALSE;
}

/**
 * @brief WM_SIZE handler of Send Mail dialog.
 * @param hwnd - window handle.
 * @param state - window state.
 * @param cx - window width.
 * @param cy - window height.
 */
static void DescribeErrorDlg_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
	hwnd; cx; cy; state;
	g_LayoutMgr.ApplyLayout();
}

/**
 * @brief WM_GETMINMAXINFO handler of Send Mail State dialog.
 * @param hwnd - window handle.
 * @param pMinMaxInfo - window min-max info.
 */
static void DescribeErrorDlg_OnGetMinMaxInfo(HWND hwnd, PMINMAXINFO pMinMaxInfo)
{
	hwnd;
	pMinMaxInfo->ptMinTrackSize = g_LayoutMgr.GetMinTrackSize();
}

/**
 * @brief Dialog procedure of Send Mail dialog.
 * @param hwndDlg - window handle.
 * @param uMsg - message identifier.
 * @param wParam - 1st message parameter.
 * @param lParam - 2nd message parameter.
 * @return message result.
 */
INT_PTR CALLBACK DescribeErrorDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	HANDLE_MSG(hwndDlg, WM_INITDIALOG, DescribeErrorDlg_OnInitDialog);
	HANDLE_MSG(hwndDlg, WM_COMMAND, DescribeErrorDlg_OnCommand);
	HANDLE_MSG(hwndDlg, WM_SIZE, DescribeErrorDlg_OnSize);
	HANDLE_MSG(hwndDlg, WM_GETMINMAXINFO, DescribeErrorDlg_OnGetMinMaxInfo);
	default: return FALSE;
	}
}

/** @} */
