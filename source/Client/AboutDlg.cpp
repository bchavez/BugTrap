/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: About dialog.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "resource.h"
#include "AboutDlg.h"
#include "BugTrapUI.h"
#include "BugTrapUtils.h"
#include "Globals.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @addtogroup BugTrapUI BugTrap Graphical User Interface
 * @{
 */

/// E-mail hyper-link control.
static CHyperLink g_hlURL;

/**
 * @brief WM_COMMAND handler of About dialog.
 * @param hwnd - window handle.
 * @param id - control ID.
 * @param hwndCtl - control handle.
 * @param codeNotify - notification code.
 */
static void AboutDlg_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    codeNotify; hwndCtl;
    switch (id)
    {
    case IDOK:
    case IDCANCEL:
        EndDialog(hwnd, FALSE);
        break;
    }
}

/**
 * @brief WM_INITDIALOG handler of About dialog.
 * @param hwnd - window handle.
 * @param hwndFocus - system-defined focus window.
 * @param lParam - user-defined parameter.
 * @return true to setup focus to system-defined control.
 */
static BOOL AboutDlg_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    lParam; hwndFocus;

    CenterWindow(hwnd, GetParent(hwnd));
    HWND hwndCtl = GetDlgItem(hwnd, IDCANCEL);
    SetFocus(hwndCtl);

    hwndCtl = GetDlgItem(hwnd, IDC_URL);
    TCHAR szLinkURL[MAX_PATH];
    GetWindowText(hwndCtl, szLinkURL, countof(szLinkURL));
    g_hlURL.SetLinkURL(szLinkURL);
    g_hlURL.Attach(hwndCtl);

    WORD MajorVersion = 0;
    WORD MinorVersion = 0;
    WORD BuildNumber = 0;
    WORD RevisionNumber = 0;
    if (GetProductVersion(&MajorVersion, &MinorVersion, &BuildNumber, &RevisionNumber))
    {
        TCHAR buffer[MAX_PATH];
        _stprintf_s(buffer, countof(buffer), _T("Version %d.%d.%d"), MajorVersion, MinorVersion, BuildNumber);
        SetDlgItemText(hwnd, IDC_STATIC_VERSION, buffer);
    }
    return FALSE;
}

/**
* @brief Get major, minor, build and revision numbers from module
* @param MajorVersion major version number
* @param MinorVersion minor version number
* @param BuildNumber build number
* @param RevisionNumber revision number
* @return true if version info could be read
*/
bool GetProductVersion(WORD *MajorVersion, WORD *MinorVersion, WORD *BuildNumber, WORD *RevisionNumber)
{
    // get the filename of the executable containing the version resource
    TCHAR szFilename[MAX_PATH + 1] = { 0 };
    if (GetModuleFileName(g_hInstance, szFilename, MAX_PATH) == 0)
    {
        return false;
    }

    // allocate a block of memory for the version info
    DWORD dummy;
    DWORD dwSize = GetFileVersionInfoSize(szFilename, &dummy);
    if (dwSize == 0)
    {
        return false;
    }
    LPTSTR lpData = (LPTSTR)malloc(dwSize);

    // load the version info
    if (!GetFileVersionInfo(szFilename, NULL, dwSize, lpData))
    {
        free(lpData);
        return false;
    }

    UINT                uiVerLen = 0;
    VS_FIXEDFILEINFO*   pFixedInfo = 0;     // pointer to fixed file info structure
                                            // get the fixed file info (language-independend) 
    if (VerQueryValue(lpData, TEXT("\\"), (void**)&pFixedInfo, (UINT *)&uiVerLen) == 0)
    {
        free(lpData);
        return false;
    }

    *MajorVersion = HIWORD(pFixedInfo->dwFileVersionMS);
    *MinorVersion = LOWORD(pFixedInfo->dwFileVersionMS);
    *BuildNumber = HIWORD(pFixedInfo->dwFileVersionLS);
    *RevisionNumber = LOWORD(pFixedInfo->dwFileVersionLS);

    free(lpData);
    return true;
}

/**
 * @brief WM_DESTROY handler of About dialog.
 * @param hwnd - window handle.
 */
static void AboutDlg_OnDestroy(HWND hwnd)
{
    hwnd;
    g_hlURL.Detach();
}

/**
 * @brief Dialog procedure of About dialog.
 * @param hwndDlg - window handle.
 * @param uMsg - message identifier.
 * @param wParam - 1st message parameter.
 * @param lParam - 2nd message parameter.
 * @return message result.
 */
INT_PTR CALLBACK AboutDlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    HANDLE_MSG(hwndDlg, WM_INITDIALOG, AboutDlg_OnInitDialog);
    HANDLE_MSG(hwndDlg, WM_COMMAND, AboutDlg_OnCommand);
    HANDLE_MSG(hwndDlg, WM_DESTROY, AboutDlg_OnDestroy);
    default: return FALSE;
    }
}

/** @} */
