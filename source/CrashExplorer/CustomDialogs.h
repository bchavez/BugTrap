/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: File dialog helper classes.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

/// File-open/file-save dialog class that can be automatically centered on the screen.
class CCustomFileDialog : public CFileDialogImpl<CCustomFileDialog>
{
public:
	/// Initialize new object.
	CCustomFileDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		HWND hWndParent = NULL)
		: CFileDialogImpl<CCustomFileDialog>(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, hWndParent)
	{ }

	BEGIN_MSG_MAP_EX(CCustomFileDialog)
		CHAIN_MSG_MAP(CFileDialogImpl<CCustomFileDialog>)
	END_MSG_MAP()

	/// Center dialog on the screen.
	void OnInitDone(LPOFNOTIFY /*lpon*/)
	{
		GetParent().CenterWindow(m_ofn.hwndOwner);
	}
};

/// Browse-for-folder dialog that can automatically highlight previous user selection.
class CCustomFolderDialog : public CFolderDialogImpl<CCustomFolderDialog>
{
public:
	/// Initialize the object.
	CCustomFolderDialog(HWND hWndParent = NULL, LPCTSTR lpstrTitle = NULL, UINT uFlags = BIF_RETURNONLYFSDIRS) :
		CFolderDialogImpl<CCustomFolderDialog>(hWndParent, lpstrTitle, uFlags)
	{
	}

	/// Set default folder path.
	void SetFolderPath(PCTSTR pszFolderPath)
	{
		_tcscpy_s(m_szFolderPath, countof(m_szFolderPath), pszFolderPath);
	}

	/// Highlight default folder path.
	void OnInitialized()
	{
		if (*m_szFolderPath != _T('\0'))
			SetSelection(m_szFolderPath);
		CWindow(m_hWnd).CenterWindow(m_bi.hwndOwner);
	}
};
