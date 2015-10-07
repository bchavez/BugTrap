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

#include "stdafx.h"
#include "resource.h"

#include "WaitDlg.h"
#include "CustomDialogs.h"
#include "ManualModeDlg.h"
#include "MessageTip.h"
#include "MapProcessor.h"
#include "PdbProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// Tab settings registry key.
const TCHAR g_szManualModeSettingsRegKey[] = _T("SOFTWARE\\IntelleSoft\\CrashExplorer\\Manual Mode Settings");
/// Map/pdb file name.
const TCHAR g_szMapPdbFile[] = _T("Map or Pdb File");
/// Base address.
const TCHAR g_szBaseAddress[] = _T("Base Address");
/// Crash address.
const TCHAR g_szCrashAddress[] = _T("Crash Explorer");
/// Module size.
const TCHAR g_szModuleSize[] = _T("Module Size");

IMPLEMENT_RUNTIME_CLASS(CManualModeDlg, CBaseTabItem)

/// Dialog layout information.
static const LAYOUT_INFO g_arrLayout[] =
{
	LAYOUT_INFO(IDC_MAPPDBFILE,         ALIGN_LEFT,   ALIGN_TOP, ALIGN_RIGHT,  ALIGN_TOP),
	LAYOUT_INFO(IDC_MAPPDBFILE_BROWSE,  ALIGN_RIGHT,  ALIGN_TOP, ALIGN_RIGHT,  ALIGN_TOP),
	LAYOUT_INFO(IDC_BASEADDRESS,        ALIGN_LEFT,   ALIGN_TOP, ALIGN_CENTER, ALIGN_TOP),
	LAYOUT_INFO(IDC_BASEADDRESS_RADIX,  ALIGN_CENTER, ALIGN_TOP, ALIGN_CENTER, ALIGN_TOP),
	LAYOUT_INFO(IDC_CRASHADDRESS,       ALIGN_LEFT,   ALIGN_TOP, ALIGN_CENTER, ALIGN_TOP),
	LAYOUT_INFO(IDC_CRASHADDRESS_RADIX, ALIGN_CENTER,   ALIGN_TOP, ALIGN_CENTER, ALIGN_TOP),
	LAYOUT_INFO(IDC_MODULESIZE_LABEL,   ALIGN_CENTER, ALIGN_TOP, ALIGN_CENTER, ALIGN_TOP),
	LAYOUT_INFO(IDC_MODULESIZE,         ALIGN_CENTER, ALIGN_TOP, ALIGN_RIGHT,  ALIGN_TOP),
	LAYOUT_INFO(IDC_MODULESIZE_RADIX,   ALIGN_RIGHT,  ALIGN_TOP, ALIGN_RIGHT,  ALIGN_TOP),
	LAYOUT_INFO(IDC_FUNCTIONNAME,       ALIGN_LEFT,   ALIGN_TOP, ALIGN_RIGHT,  ALIGN_TOP),
	LAYOUT_INFO(IDC_SOURCEFILE,         ALIGN_LEFT,   ALIGN_TOP, ALIGN_RIGHT,  ALIGN_TOP),
	LAYOUT_INFO(IDC_LINENUMBER,         ALIGN_LEFT,   ALIGN_TOP, ALIGN_RIGHT,  ALIGN_TOP),
	LAYOUT_INFO(IDOK,                   ALIGN_CENTER, ALIGN_TOP, ALIGN_CENTER, ALIGN_TOP),
	LAYOUT_INFO(IDCANCEL,               ALIGN_CENTER, ALIGN_TOP, ALIGN_CENTER, ALIGN_TOP)
};

/**
 * @param hWnd - handle to the control to receive the default keyboard focus.
 * @param lParam - specifies additional initialization data.
 * @return message result code.
 */
LRESULT CManualModeDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
	m_txtMapPdbFile.Attach(GetDlgItem(IDC_MAPPDBFILE));
	m_txtMapPdbFile.SetLimitText(MAX_PATH - 1);
	m_txtBaseAddress.Attach(GetDlgItem(IDC_BASEADDRESS), GetDlgItem(IDC_BASEADDRESS_RADIX));
	CNumEdit& txtBaseAddress = m_txtBaseAddress.GetNumEdit();
	txtBaseAddress.SetLimitText(8);
	txtBaseAddress.SetNumFormat(CNumEdit::NF_HEX);
	CNumEdit& txtModuleSize = m_txtModuleSize.GetNumEdit();
	m_txtModuleSize.Attach(GetDlgItem(IDC_MODULESIZE), GetDlgItem(IDC_MODULESIZE_RADIX));
	txtModuleSize.SetLimitText(8);
	txtModuleSize.SetNumFormat(CNumEdit::NF_HEX);
	txtModuleSize.EnableWindow(FALSE);
	CNumEdit& txtCrashAddress = m_txtCrashAddress.GetNumEdit();
	m_txtCrashAddress.Attach(GetDlgItem(IDC_CRASHADDRESS), GetDlgItem(IDC_CRASHADDRESS_RADIX));
	txtCrashAddress.SetLimitText(8);
	txtCrashAddress.SetNumFormat(CNumEdit::NF_HEX);
	m_txtFunctionName.Attach(GetDlgItem(IDC_FUNCTIONNAME));
	m_txtSourceFile.Attach(GetDlgItem(IDC_SOURCEFILE));
	m_txtLineNumber.Attach(GetDlgItem(IDC_LINENUMBER));

	//////////////////////////////////////////////////////////////
	// workaround for WinXP bug when scrollbars are not skinned //
	LONG lStyle = GetWindowLong(GWL_STYLE);                     //
	SetWindowLong(GWL_STYLE, lStyle | WS_HSCROLL | WS_VSCROLL); //
	SetWindowLong(GWL_STYLE, lStyle);                           //
	//////////////////////////////////////////////////////////////

	InitLayout(false, true);
	AddControlsToLayout(g_arrLayout, countof(g_arrLayout));

	CRect rcClient;
	GetClientRect(&rcClient);
	SetScrollSize(rcClient.right, rcClient.bottom);
	DWORD dwDlgUnits = GetDialogBaseUnits();
	int nLineUnit = HIWORD(dwDlgUnits);
	SetScrollLine(nLineUnit, nLineUnit);

	ClearProcessor();
	return TRUE;
}

void CManualModeDlg::OnDestroy()
{
	SetMsgHandled(FALSE);
	ClearProcessor();
}

void CManualModeDlg::ClearProcessor()
{
	m_eProcessedFileType = CBaseProcessor::PFT_UNDEFINED;
	m_pProcessor.reset();
	CNumEdit& txtModuleSize = m_txtModuleSize.GetNumEdit();
	txtModuleSize.SetWindowText(NULL);
	txtModuleSize.EnableWindow(FALSE);
	CRadixMenu& btnModuleSize = m_txtModuleSize.GetRadixMenu();
	btnModuleSize.EnableWindow(FALSE);
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
void CManualModeDlg::OnMapPdbFileBrowse(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	MsgTip::HideMessage();
	CString strMapPdbFile;
	m_txtMapPdbFile.GetWindowText(strMapPdbFile);
	CCustomFileDialog dlgSelectFile(TRUE,
		NULL,
		strMapPdbFile,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("All Supported Files\0*.map;*.pdb;*.exe;*.dll\0")
		_T("Map Files\0*.map\0")
		_T("Pdb Files\0*.pdb\0")
		_T("Executable Files\0*.exe;*.dll\0")
		_T("All Files\0*.*\0"),
		m_hWnd);
	if (dlgSelectFile.DoModal(m_hWnd) == IDOK)
		m_txtMapPdbFile.SetWindowText(dlgSelectFile.m_szFileName);
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
void CManualModeDlg::OnChangeMapPdbFile(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	try
	{
		ClearProcessor();
		CString strMapPdbFile;
		m_txtMapPdbFile.GetWindowText(strMapPdbFile);
		if (strMapPdbFile.IsEmpty())
			return;
		if (GetFileAttributes(strMapPdbFile) != INVALID_FILE_ATTRIBUTES)
		{
			CWaitCursor wait;
			CBaseProcessor::PROCESSED_FILE_TYPE eProcessedFileType = CBaseProcessor::GetFileType(strMapPdbFile);
			switch (eProcessedFileType)
			{
			case CBaseProcessor::PFT_MAP:
				{
					CMapProcessor* pMapProcessor = new CMapProcessor();
					m_pProcessor.reset(pMapProcessor);
					m_eProcessedFileType = CBaseProcessor::PFT_MAP;
					pMapProcessor->LoadMapText(strMapPdbFile);
					CNumEdit& txtBaseAddress = m_txtBaseAddress.GetNumEdit();
					PVOID pPreferredBaseAddress = pMapProcessor->GetBaseAddress();
					if (pPreferredBaseAddress)
					{
						CString strPreferredBaseAddress;
						strPreferredBaseAddress.Format(_T("%08X"), pPreferredBaseAddress);
						txtBaseAddress.SetWindowText(strPreferredBaseAddress);
					}
					else
						txtBaseAddress.SetWindowText(NULL);
				}
				break;
			case CBaseProcessor::PFT_PDB:
				{
					m_eProcessedFileType = CBaseProcessor::PFT_PDB;
					CNumEdit& txtModuleSize = m_txtModuleSize.GetNumEdit();
					txtModuleSize.EnableWindow(TRUE);
					CRadixMenu& btnModuleSize = m_txtModuleSize.GetRadixMenu();
					btnModuleSize.EnableWindow(TRUE);
				}
				break;
			default:
				MsgTip::ShowMessage(m_txtMapPdbFile, IDS_INVALIDMAPPDBFILE);
				return;
			}
		}
	}
	catch (std::exception& error)
	{
		CString strProjectTitle;
		strProjectTitle.LoadString(IDS_PROJECTTITLE);
		MessageBox(CA2CT(error.what()), strProjectTitle, MB_OK | MB_ICONERROR);
	}
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
void CManualModeDlg::OnCalculate(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	try
	{
		CString strMapPdbFile;
		m_txtMapPdbFile.GetWindowText(strMapPdbFile);
		if (strMapPdbFile.IsEmpty())
		{
			MsgTip::ShowMessage(m_txtMapPdbFile, IDS_INVALIDMAPPDBFILE);
			return;
		}

		CNumEdit& txtBaseAddress = m_txtBaseAddress.GetNumEdit();
		DWORD64 dwBaseAddress64;
		txtBaseAddress.GetValue(dwBaseAddress64);
		PVOID ptrBaseAddress = (PVOID)dwBaseAddress64;
		if (! ptrBaseAddress)
		{
			MsgTip::ShowMessage(txtBaseAddress, IDS_INVALIDBASEADDRESS);
			return;
		}

		CNumEdit& txtCrashAddress = m_txtCrashAddress.GetNumEdit();
		DWORD64 dwCrashAddress64;
		txtCrashAddress.GetValue(dwCrashAddress64);
		PVOID ptrCrashAddress = (PVOID)dwCrashAddress64;
		if (! ptrCrashAddress)
		{
			MsgTip::ShowMessage(txtCrashAddress, IDS_INVALIDCRASHADDRESS);
			return;
		}

		CWaitDialog wait(m_hWnd);
		switch (m_eProcessedFileType)
		{
		case CBaseProcessor::PFT_MAP:
			{
				CBaseProcessor* pBaseProcessor = m_pProcessor.get();
				if (! pBaseProcessor || pBaseProcessor->GetRuntimeClass() != RUNTIME_CLASS(CMapProcessor))
				{
					MsgTip::ShowMessage(m_txtMapPdbFile, IDS_INVALIDMAPPDBFILE);
					return;
				}
				CMapProcessor* pMapProcessor = STATIC_DOWNCAST(CMapProcessor, pBaseProcessor);
				pMapProcessor->SetBaseAddress(ptrBaseAddress);
			}
			break;
		case CBaseProcessor::PFT_PDB:
			{
				CNumEdit& txtModuleSize = m_txtModuleSize.GetNumEdit();
				DWORD32 dwModuleSize;
				txtModuleSize.GetValue(dwModuleSize);
				if (! dwModuleSize)
				{
					MsgTip::ShowMessage(txtModuleSize, IDS_INVALIDMODULESIZE);
					return;
				}
				m_pProcessor.reset(); // unload old modules
				CPdbProcessor* pPdbProcessor = new CPdbProcessor();
				m_pProcessor.reset(pPdbProcessor);
				pPdbProcessor->LoadModule(strMapPdbFile, ptrBaseAddress, dwModuleSize);
			}
			break;
		default:
			MsgTip::ShowMessage(m_txtMapPdbFile, IDS_INVALIDMAPPDBFILE);
			return;
		}

		CString strFunctionInfo;
		boost::shared_ptr<CBaseFnInfo> pFnInfo;
		DWORD64 dwDisplacement64;
		if (m_pProcessor->FindFunctionInfo(ptrCrashAddress, pFnInfo, dwDisplacement64))
		{
			CString strFunctionName(CA2CT(pFnInfo->GetName().c_str()));
			if (dwDisplacement64 != 0)
				strFunctionInfo.Format(_T("%s+%I64u byte(s)"), strFunctionName, dwDisplacement64);
			else
				strFunctionInfo = strFunctionName;
		}
		else
			strFunctionInfo.LoadString(IDS_NOFUNCTIONINFO);
		m_txtFunctionName.SetWindowText(strFunctionInfo);

		CString strSourceFile, strLineInfo;
		boost::shared_ptr<CBaseFileInfo> pFileInfo;
		boost::shared_ptr<CBaseLineInfo> pLineInfo;
		DWORD dwDisplacement32;
		if (m_pProcessor->FindLineInfo(ptrCrashAddress, pFileInfo, pLineInfo, dwDisplacement32))
		{
			strSourceFile = CA2CT(pFileInfo->GetFileName().c_str());
			UINT uLineNumber = pLineInfo->GetNumber();
			if (dwDisplacement32 != 0)
				strLineInfo.Format(_T("line %u+%I32u byte(s)"), uLineNumber, dwDisplacement32);
			else
				strLineInfo.Format(_T("line %u"), uLineNumber);
		}
		else
		{
			strSourceFile.LoadString(IDS_NOSOURCEFILEINFO);
			strLineInfo.LoadString(IDS_NOLINEINFO);
		}
		m_txtSourceFile.SetWindowText(strSourceFile);
		m_txtLineNumber.SetWindowText(strLineInfo);
	}
	catch (std::exception& error)
	{
		CString strProjectTitle;
		strProjectTitle.LoadString(IDS_PROJECTTITLE);
		MessageBox(CA2CT(error.what()), strProjectTitle, MB_OK | MB_ICONERROR);
	}
}

void CManualModeDlg::SaveState(void)
{
	CRegKey reg;
	if (reg.Create(HKEY_CURRENT_USER, g_szManualModeSettingsRegKey) == ERROR_SUCCESS)
	{
		CString strText;

		m_txtMapPdbFile.GetWindowText(strText);
		reg.SetStringValue(g_szMapPdbFile, strText);

		m_txtBaseAddress.GetNumEdit().GetWindowText(strText);
		reg.SetStringValue(g_szBaseAddress, strText);

		m_txtCrashAddress.GetNumEdit().GetWindowText(strText);
		reg.SetStringValue(g_szCrashAddress, strText);

		m_txtModuleSize.GetNumEdit().GetWindowText(strText);
		reg.SetStringValue(g_szModuleSize, strText);

		reg.Close();
	}
}

void CManualModeDlg::LoadState(void)
{
	CRegKey reg;
	if (reg.Open(HKEY_CURRENT_USER, g_szManualModeSettingsRegKey) == ERROR_SUCCESS)
	{
		TCHAR szText[MAX_PATH];

		ULONG ulNumChars = countof(szText);
		reg.QueryStringValue(g_szMapPdbFile, szText, &ulNumChars);
		m_txtMapPdbFile.SetWindowText(szText);

		ulNumChars = countof(szText);
		reg.QueryStringValue(g_szBaseAddress, szText, &ulNumChars);
		m_txtBaseAddress.GetNumEdit().SetWindowText(szText);

		ulNumChars = countof(szText);
		reg.QueryStringValue(g_szCrashAddress, szText, &ulNumChars);
		m_txtCrashAddress.GetNumEdit().SetWindowText(szText);

		ulNumChars = countof(szText);
		reg.QueryStringValue(g_szModuleSize, szText, &ulNumChars);
		m_txtModuleSize.GetNumEdit().SetWindowText(szText);

		reg.Close();
	}
}
