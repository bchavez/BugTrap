/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Manual mode dialog.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "BaseTabItem.h"
#include "BaseProcessor.h"
#include "LayoutManager.h"
#include "RadixEdit.h"

/// Manual map mode dialog.
class CManualModeDlg :
	public CDialogImpl<CManualModeDlg, CBaseTabItem>,
	public CScrollImpl<CManualModeDlg>,
	public CLayoutManager<CManualModeDlg>
{
	DECLARE_RUNTIME_CLASS(CManualModeDlg)
public:
	/// Dialog resource identifier.
	enum { IDD = IDD_MANUAL_MODE };

	BEGIN_MSG_MAP_EX(CManualModeDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestroy)
		COMMAND_ID_HANDLER_EX(IDOK, OnCalculate)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER_EX(IDC_MAPPDBFILE_BROWSE, BN_CLICKED, OnMapPdbFileBrowse)
		COMMAND_HANDLER_EX(IDC_MAPPDBFILE, EN_CHANGE, OnChangeMapPdbFile)
		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CScrollImpl<CManualModeDlg>)
		CHAIN_MSG_MAP(CLayoutManager<CManualModeDlg>)
	END_MSG_MAP()

	/// Create tab item window.
	virtual BOOL CreateItem(HWND hwndParent);
	/// Get page title.
	virtual void GetItemTitle(CString& strTitle);
	/// Save tab contents.
	virtual void SaveState(void);
	/// Restore tab contents.
	virtual void LoadState(void);
	/// WM_PAINT event handler.
	void DoPaint(CDCHandle dc);

protected:
	/// WM_INITDIALOG event handler.
	LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);
	/// WM_DESTROY event handler.
	void OnDestroy();
	/// Browse button command handler.
	void OnMapPdbFileBrowse(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// Calculate button command handler.
	void OnCalculate(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// Close active dialog.
	void OnCloseCmd(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// MAP/PDB file change handler.
	void OnChangeMapPdbFile(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// Clear processor.
	void ClearProcessor();

	/// MAP/PDB file edit box.
	CEdit m_txtMapPdbFile;
	/// Base address edit box.
	CRadixEdit m_txtBaseAddress;
	/// Module size edit box.
	CRadixEdit m_txtModuleSize;
	/// Crash address edit box.
	CRadixEdit m_txtCrashAddress;
	/// Function name edit box.
	CEdit m_txtFunctionName;
	/// Source file name edit box.
	CEdit m_txtSourceFile;
	/// Line number edit box.
	CEdit m_txtLineNumber;
	/// MAP/PDB file processor.
	boost::shared_ptr<CBaseProcessor> m_pProcessor;
	/// Type of processed file.
	CBaseProcessor::PROCESSED_FILE_TYPE m_eProcessedFileType;
};

/**
 * @param hwndParent - parent window handle.
 */
inline BOOL CManualModeDlg::CreateItem(HWND hwndParent)
{
	return (Create(hwndParent) != NULL);
}

/**
 * @param strTitle - page title buffer.
 */
inline void CManualModeDlg::GetItemTitle(CString& strTitle)
{
	strTitle.LoadString(IDS_MANUALMODE);
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
inline void CManualModeDlg::OnCloseCmd(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	GetParent().PostMessage(WM_CLOSE);
}

inline void CManualModeDlg::DoPaint(CDCHandle /*dc*/)
{
}
