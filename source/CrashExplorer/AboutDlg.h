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

#pragma once

/// About class.
class CAboutDlg : public CDialogImpl<CAboutDlg>
{
public:
	/// Dialog resource identifier.
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP_EX(CAboutDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_ID_HANDLER_EX(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

protected:
	/// WM_INITDIALOG event handler.
	LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);
	/// Close button command handler.
	void OnCloseCmd(UINT uNotifyCode, int nID, HWND hWndCtl);

	/// Web-site hyper-link.
	CHyperLink m_lblURL;
};

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
inline void CAboutDlg::OnCloseCmd(UINT /*uNotifyCode*/, int nID, HWND /*hWndCtl*/)
{
	EndDialog(nID);
}
