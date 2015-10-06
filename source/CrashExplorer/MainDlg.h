/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Main dialog.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "BaseTabItem.h"

/// Forward un-handled command messages to the active tab item
#define FORWARD_TAB_MESSAGES() \
	if (uMsg == WM_COMMAND && m_nSelTabItem >= 0) \
	{ \
		CBaseTabItem* pTabItem = m_arrTabs[m_nSelTabItem].m_pItem; \
		if (pTabItem != NULL && pTabItem->m_hWnd != NULL) \
			pTabItem->SendMessage(uMsg, wParam, lParam); \
	}

/// Main dialog class.
class CMainDlg : public CDialogImpl<CMainDlg>,
				 public CMessageFilter
{
public:
	/// Dialog resource identifier.
	enum { IDD = IDD_MAIN };

	/// Override this function to filter window messages before they are dispatched to the Windows functions TranslateMessage() and DispatchMessage().
	virtual BOOL PreTranslateMessage(PMSG pMsg);

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SYSCOMMAND(OnSysCommand)
		MSG_WM_DESTROY(OnDestroy)
		MSG_WM_SIZE(OnSize)
		MSG_WM_ERASEBKGND(OnEraseBackground)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCloseCmd)
		NOTIFY_CODE_HANDLER_EX(TCN_SELCHANGE, OnSelChange)
		FORWARD_TAB_MESSAGES()
	END_MSG_MAP()

	/// Initialize member variables.
	CMainDlg(void);
	/// Restore window appearance.
	void RestoreWindowSettings(int nCmdShow = SW_SHOWDEFAULT);
	/// Close dialog and terminate application.
	void CloseDialog(int nVal);

protected:
	/// Window settings.
	struct CWindowSettings
	{
		/// Index of selected tab item.
		int m_nSelTabItem;
		/// Save form?
		bool m_bSaveForm;
	};

	/// Tab item data.
	struct CTabItemData
	{
		/// Item runtime class.
		const CBaseRuntimeClass* m_pRuntimeClass;
		/// Pointer to item object.
		CBaseTabItem* m_pItem;
	};

	/// WM_INITDIALOG event handler.
	LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);
	/// WM_DESTROY event handler.
	void OnDestroy(void);
	/// WM_SIZE event handler.
	void OnSize(UINT uType, CSize size);
	/// Close button command handler.
	void OnCloseCmd(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// WM_SYSCOMMAND event handler.
	void OnSysCommand(UINT uID, CPoint point);
	/// WM_ERASEBKGND event handler.
	BOOL OnEraseBackground(HDC hdc);
	/// TCN_SELCHANGE event handler.
	LRESULT OnSelChange(LPNMHDR pnmh);
	/// WM_KEYDOWN event handler.
	void OnKeyDown(TCHAR chCode, UINT uRepCount, UINT uFlags);
	/// Select new mode.
	void SelectMode(int nMode);
	/// Set auto-save mode.
	void SetAutoSaveMode(bool bSaveForm);
	/// Save settings.
	void SaveSettings(void);
	/// Restore settings.
	void LoadSettings(CWindowSettings& rWndSettings);

	/// Index of selected tab item.
	int m_nSelTabItem;
	/// Save form?
	bool m_bSaveForm;
	/// Modes tab control.
	CTabCtrl m_tcModes;
	/// Array of tab items info.
	static CTabItemData m_arrTabs[];
};

/**
 * @param hdc - handle to the device context.
 * @return nonzero if application erases the background.
 */
inline BOOL CMainDlg::OnEraseBackground(HDC /*hdc*/)
{
	return TRUE;
}
