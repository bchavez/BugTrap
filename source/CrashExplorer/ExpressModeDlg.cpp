/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Express mode dialog.
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
#include "ExpressModeDlg.h"
#include "MapProcessor.h"
#include "PdbProcessor.h"
#include "ComError.h"
#include "MessageTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// Tab settings registry key.
const TCHAR g_szExpressModeSettingsRegKey[] = _T("SOFTWARE\\IntelleSoft\\CrashExplorer\\Express Mode Settings");
/// Log file name.
const TCHAR g_szLogFile[] = _T("Log File");
/// Map-pdb folder name.
const TCHAR g_szMapPdbFolder[] = _T("Map or Pdb Folder");

IMPLEMENT_RUNTIME_CLASS(CExpressModeDlg, CBaseTabItem)

/// Stack trace list column identifiers.
enum STACK_COLUMN_ID
{
	/// Stack frame address.
	CID_ENTRY_ADDRESS,
	/// Function name.
	CID_ENTRY_FUNCTION,
	/// Source file name.
	CID_ENTRY_FILE,
	/// Entry line number.
	CID_ENTRY_LINE,
	/// Module file name.
	CID_ENTRY_MODULE
};

/// Dialog layout information.
static const LAYOUT_INFO g_arrLayout[] =
{
	LAYOUT_INFO( IDC_LOGFILE,             ALIGN_LEFT,  ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    ),
	LAYOUT_INFO( IDC_LOGFILE_BROWSE,      ALIGN_RIGHT, ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    ),
	LAYOUT_INFO( IDC_MAPPDBFOLDER,        ALIGN_LEFT,  ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    ),
	LAYOUT_INFO( IDC_MAPPDBFOLDER_BROWSE, ALIGN_RIGHT, ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    ),
	LAYOUT_INFO( IDC_ERROR_REASON_GROUP,  ALIGN_LEFT,  ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    ),
	LAYOUT_INFO( IDC_ERROR_REASON,        ALIGN_LEFT,  ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    ),
	LAYOUT_INFO( IDC_STACK_TRACE_GROUP,   ALIGN_LEFT,  ALIGN_TOP, ALIGN_RIGHT, ALIGN_BOTTOM ),
	LAYOUT_INFO( IDC_STACK_TRACE,         ALIGN_LEFT,  ALIGN_TOP, ALIGN_RIGHT, ALIGN_BOTTOM ),
	LAYOUT_INFO( IDOK,                    ALIGN_RIGHT, ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    ),
	LAYOUT_INFO( IDC_COPY,                ALIGN_RIGHT, ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    ),
	LAYOUT_INFO( IDC_SAVE,                ALIGN_RIGHT, ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    ),
	LAYOUT_INFO( IDCANCEL,                ALIGN_RIGHT, ALIGN_TOP, ALIGN_RIGHT, ALIGN_TOP    )
};

/**
 * @param hWnd - handle to the control to receive the default keyboard focus.
 * @param lParam - specifies additional initialization data.
 * @return message result code.
 */
LRESULT CExpressModeDlg::OnInitDialog(HWND /*hWnd*/, LPARAM /*lParam*/)
{
	m_rxFunctionName.assign("([0-9A-Za-z_:]+)\\(.*\\)");

	m_txtLogFile.Attach(GetDlgItem(IDC_LOGFILE));
	m_txtLogFile.SetLimitText(MAX_PATH - 1);
	m_txtMapPdbFolder.Attach(GetDlgItem(IDC_MAPPDBFOLDER));
	m_txtMapPdbFolder.SetLimitText(MAX_PATH - 1);
	m_txtErrorReason.Attach(GetDlgItem(IDC_ERROR_REASON));
	m_lstStackTrace.Attach(GetDlgItem(IDC_STACK_TRACE));
	m_lstStackTrace.SetExtendedListViewStyle(LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP);

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

	TCHAR szColumnTitle[64];
	HINSTANCE hInstance = _Module.GetResourceInstance();

	CRect rcList;
	m_lstStackTrace.GetClientRect(&rcList);

	LVCOLUMN lvc;
	ZeroMemory(&lvc, sizeof(lvc));
	lvc.mask = LVCF_TEXT;
	lvc.pszText = szColumnTitle;

	LoadString(hInstance, IDS_COLUMN_ADDRESS, szColumnTitle, countof(szColumnTitle));
	m_lstStackTrace.InsertColumn(CID_ENTRY_ADDRESS, &lvc);
	m_lstStackTrace.SetColumnWidth(CID_ENTRY_ADDRESS, rcList.right / 6);

	LoadString(hInstance, IDS_COLUMN_FUNCTION, szColumnTitle, countof(szColumnTitle));
	m_lstStackTrace.InsertColumn(CID_ENTRY_FUNCTION, &lvc);
	m_lstStackTrace.SetColumnWidth(CID_ENTRY_FUNCTION, rcList.right / 4);

	LoadString(hInstance, IDS_COLUMN_FILE, szColumnTitle, countof(szColumnTitle));
	m_lstStackTrace.InsertColumn(CID_ENTRY_FILE, &lvc);
	m_lstStackTrace.SetColumnWidth(CID_ENTRY_FILE, rcList.right / 4);

	LoadString(hInstance, IDS_COLUMN_LINE, szColumnTitle, countof(szColumnTitle));
	m_lstStackTrace.InsertColumn(CID_ENTRY_LINE, &lvc);
	m_lstStackTrace.SetColumnWidth(CID_ENTRY_LINE, rcList.right / 6);

	LoadString(hInstance, IDS_COLUMN_MODULE, szColumnTitle, countof(szColumnTitle));
	m_lstStackTrace.InsertColumn(CID_ENTRY_MODULE, &lvc);
	m_lstStackTrace.SetColumnWidth(CID_ENTRY_MODULE, rcList.right / 4);

	return TRUE;
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
void CExpressModeDlg::OnLogFileBrowse(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	MsgTip::HideMessage();
	CString strLogFile;
	m_txtLogFile.GetWindowText(strLogFile);
	CCustomFileDialog dlgSelectFile(TRUE,
		_T("xml"),
		strLogFile,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("XML Log Files\0*.xml\0All Files\0*.*\0"),
		m_hWnd);
	if (dlgSelectFile.DoModal(m_hWnd) == IDOK)
		m_txtLogFile.SetWindowText(dlgSelectFile.m_szFileName);
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
void CExpressModeDlg::OnMapPdbFolderBrowse(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	MsgTip::HideMessage();
	CString strMapPdbFolder;
	m_txtMapPdbFolder.GetWindowText(strMapPdbFolder);
	CString strTitle;
	strTitle.LoadString(IDS_SELECTMAPPDBFOLDER);
	CCustomFolderDialog dlgSelectFolder(m_hWnd, strTitle, BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE | BIF_EDITBOX | BIF_NONEWFOLDERBUTTON | BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS);
	dlgSelectFolder.SetFolderPath(strMapPdbFolder);
	if (dlgSelectFolder.DoModal(m_hWnd) == IDOK)
		m_txtMapPdbFolder.SetWindowText(dlgSelectFolder.GetFolderPath());
}

void CExpressModeDlg::ClearData()
{
	m_pXMLElementDocument.Release();
	m_pXMLDocument.Release();
	m_pXMLNodeError.Release();
	m_pXMLNodeProcess.Release();
	m_mapModules.clear();
	m_pPdbProcessor.reset();
}

void CExpressModeDlg::OnDestroy()
{
	SetMsgHandled(FALSE);
	ClearData();
}

/**
 * @param strFilePath - document file path.
 */
void CExpressModeDlg::LoadXMLDocument(const CString& strFilePath)
{
	ClearData();
	HRESULT hRes = m_pXMLDocument.CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER);
	CHECK_HRESULT(hRes);
	m_pXMLDocument->setProperty(CComBSTR(OLESTR("SelectionLanguage")), CComVariant(OLESTR("XPath")));
	m_pXMLDocument->put_async(VARIANT_FALSE);
	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	hRes = m_pXMLDocument->load(CComVariant(strFilePath), &bSuccess);
	CHECK_HRESULT(hRes);
	if (! bSuccess)
	{
		CComPtr<IXMLDOMParseError> pXMLError;
		hRes = m_pXMLDocument->get_parseError(&pXMLError);
		if (hRes == S_OK)
		{
			long lErrorCode = 0;
			pXMLError->get_errorCode(&lErrorCode);
			if (lErrorCode != 0)
			{
				CComBSTR bstrError;
				pXMLError->get_reason(&bstrError);
				throw com_error(lErrorCode, CW2A(bstrError));
			}
		}
		CStringA strError;
		strError.LoadString(IDS_ERROR_XMLLOADERROR);
		throw com_error(strError);
	}
	hRes = m_pXMLDocument->get_documentElement(&m_pXMLElementDocument);
	CHECK_HRESULT(hRes);

	CComVariant varReportVersion;
	hRes = m_pXMLElementDocument->getAttribute(CComBSTR(OLESTR("version")), &varReportVersion);
	CHECK_HRESULT(hRes);
	if (hRes == S_OK)
	{
		hRes = varReportVersion.ChangeType(VT_I4);
		CHECK_HRESULT(hRes);
		if (V_I4(&varReportVersion) != 1)
		{
			CStringA strErrorMessage;
			strErrorMessage.LoadString(IDS_ERROR_UNSUPPORTEDREPORTVERSION);
			throw com_error(strErrorMessage);
		}
	}

	if (GetXMLNodeText(m_pXMLElementDocument, OLESTR("./platform"), m_strPlatform))
	{
		// validate platform
		GetPlatform();
	}

	if (SelectXMLNode(m_pXMLElementDocument, OLESTR("./error"), m_pXMLNodeError))
	{
		CString strProcessID;
		GetXMLNodeText(m_pXMLNodeError, OLESTR("./process/id"), strProcessID);
		if (! strProcessID.IsEmpty())
		{
			CString strExpression;
			strExpression.Format(_T("./processes/process[id=%s]"), strProcessID);
			SelectXMLNode(m_pXMLElementDocument, CT2W(strExpression), m_pXMLNodeProcess);
		}
	}
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
void CExpressModeDlg::OnCalculate(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	try
	{
		CString strLogFile;
		m_txtLogFile.GetWindowText(strLogFile);
		if (GetFileAttributes(strLogFile) == INVALID_FILE_ATTRIBUTES)
		{
			MsgTip::ShowMessage(m_txtLogFile, IDS_INVALIDLOGFILE);
			return;
		}

		CString strMapPdbFolder;
		m_txtMapPdbFolder.GetWindowText(strMapPdbFolder);
		if (GetFileAttributes(strMapPdbFolder) == INVALID_FILE_ATTRIBUTES)
		{
			MsgTip::ShowMessage(m_txtMapPdbFolder, IDS_INVALIDMAPPDBFOLDER);
			return;
		}

		CWaitDialog wait(m_hWnd);
		LoadXMLDocument(strLogFile);
		SetErrorReasonText();
		FillStackTraceList();
	}
	catch (std::exception& error)
	{
		HandleException(error);
	}
}

/**
 * @param pXMLNodeParent - parent XML node.
 * @param pszExpression - XPath expression.
 * @param pXMLNodeSelection - selected XML node.
 * @return true if selection is not empty.
 */
bool CExpressModeDlg::SelectXMLNode(const CComPtr<IXMLDOMNode>& pXMLNodeParent, LPCOLESTR pszExpression, CComPtr<IXMLDOMNode>& pXMLNodeSelection)
{
	HRESULT hRes = pXMLNodeParent->selectSingleNode(CComBSTR(pszExpression), &pXMLNodeSelection);
	CHECK_HRESULT(hRes);
	return (hRes == S_OK);
}

/**
 * @param pXMLNodeParent - parent XML node.
 * @param pszExpression - XPath expression.
 * @param pXMLNodeListSelection - selected XML nodes.
 * @return true if selection is not empty.
 */
bool CExpressModeDlg::SelectXMLNodes(const CComPtr<IXMLDOMNode>& pXMLNodeParent, LPCOLESTR pszExpression, CComPtr<IXMLDOMNodeList>& pXMLNodeListSelection)
{
	HRESULT hRes = pXMLNodeParent->selectNodes(CComBSTR(pszExpression), &pXMLNodeListSelection);
	CHECK_HRESULT(hRes);
	return (hRes == S_OK);
}

/**
 * @param pXMLNodeParent - parent XML node.
 * @param rStackEntry - stack entry information.
 */
void CExpressModeDlg::GetStackEntry(const CComPtr<IXMLDOMNode>& pXMLNodeParent, CStackEntry& rStackEntry)
{
	GetXMLNodeText(pXMLNodeParent, OLESTR("./module"), rStackEntry.m_strModule);
	GetXMLNodeText(pXMLNodeParent, OLESTR("./address"), rStackEntry.m_strAddress);
	GetXMLNodeText(pXMLNodeParent, OLESTR("./function/name"), rStackEntry.m_strFunctionName);
	GetXMLNodeText(pXMLNodeParent, OLESTR("./function/offset"), rStackEntry.m_strFunctionOffset);
	GetXMLNodeText(pXMLNodeParent, OLESTR("./file"), rStackEntry.m_strSourceFile);
	GetXMLNodeText(pXMLNodeParent, OLESTR("./line/number"), rStackEntry.m_strLineNumber);
	GetXMLNodeText(pXMLNodeParent, OLESTR("./line/offset"), rStackEntry.m_strLineOffset);

	UpdateStackEntryFromMap(rStackEntry);

	if (! rStackEntry.m_strFunctionName.IsEmpty())
	{
		rStackEntry.m_strFunctionInfo = rStackEntry.m_strFunctionName;
		rStackEntry.m_strFunctionInfo += _T("()");
		if (! rStackEntry.m_strFunctionOffset.IsEmpty() && _tcstoui64(rStackEntry.m_strFunctionOffset, NULL, 0) != 0)
		{
			rStackEntry.m_strFunctionInfo += _T('+');
			rStackEntry.m_strFunctionInfo += rStackEntry.m_strFunctionOffset;
			rStackEntry.m_strFunctionInfo += _T(" byte(s)");
		}
	}
	else
		rStackEntry.m_strFunctionInfo.Empty();

	if (! rStackEntry.m_strLineNumber.IsEmpty())
	{
		rStackEntry.m_strLineInfo = _T("line ");
		rStackEntry.m_strLineInfo += rStackEntry.m_strLineNumber;
		if (! rStackEntry.m_strLineOffset.IsEmpty() && _tcstoui64(rStackEntry.m_strLineOffset, NULL, 0) != 0)
		{
			rStackEntry.m_strLineInfo += _T('+');
			rStackEntry.m_strLineInfo += rStackEntry.m_strLineOffset;
			rStackEntry.m_strLineInfo += _T(" byte(s)");
		}
	}
	else
		rStackEntry.m_strLineInfo.Empty();
}

/**
 * @param pszMapPdbFolder - folder with MAP/PDB files.
 * @param pszModuleName - base module name.
 * @param pszFileExt - requested file extension.
 * @param pszFileName - combined file name.
 * @return true if requested file exist.
 */
bool CExpressModeDlg::FindFileByPattern(PCTSTR pszMapPdbFolder, PCTSTR pszModuleName, PCTSTR pszFileExt, PTSTR pszFileName)
{
	PathCombine(pszFileName, pszMapPdbFolder, pszModuleName);
	PathRenameExtension(pszFileName, pszFileExt);
	return (GetFileAttributes(pszFileName) != INVALID_FILE_ATTRIBUTES);
}

/**
 * @param strModule - module file name.
 * @return pointer to module processor.
 */
boost::shared_ptr<CBaseProcessor> CExpressModeDlg::GetModuleInfo(const CString& strModule)
{
	boost::shared_ptr<CBaseProcessor> pBaseProcessor;
	if (strModule.IsEmpty())
		return pBaseProcessor;
	CModuleMap::iterator itModule = m_mapModules.find(strModule);
	if (itModule == m_mapModules.end())
	{
		if (m_pXMLNodeProcess != NULL)
		{
			CString strExpression;
			strExpression.Format(_T("./modules/module[name=\"%s\"]"), strModule);
			CComPtr<IXMLDOMNode> pXMLNodeModule;
			if (SelectXMLNode(m_pXMLNodeProcess, CT2CW(strExpression), pXMLNodeModule))
			{
				CString strBaseAddress;
				GetXMLNodeText(pXMLNodeModule, OLESTR("./base"), strBaseAddress);
				PVOID ptrBaseAddress = (PVOID)_tcstoui64(strBaseAddress, NULL, 0);
				CString strModuleSize;
				GetXMLNodeText(pXMLNodeModule, OLESTR("./size"), strModuleSize);
				DWORD dwModuleSize = _tcstoul(strModuleSize, NULL, 0);
				if (ptrBaseAddress != NULL && dwModuleSize != 0)
				{
					TCHAR szModuleName[MAX_PATH];
					_tcscpy_s(szModuleName, countof(szModuleName), PathFindFileName(strModule));
					TCHAR szMapPdbFolder[MAX_PATH];
					m_txtMapPdbFolder.GetWindowText(szMapPdbFolder, countof(szMapPdbFolder));
					TCHAR szSystemFolder[MAX_PATH];
					GetSystemDirectory(szSystemFolder, countof(szSystemFolder));
					TCHAR szMapPdbFile[MAX_PATH];
					if (! FindFileByPattern(szMapPdbFolder, szModuleName, _T(".map"), szMapPdbFile) &&
						! FindFileByPattern(szMapPdbFolder, szModuleName, _T(".pdb"), szMapPdbFile) &&
						! FindFileByPattern(szSystemFolder, szModuleName, _T(".map"), szMapPdbFile) &&
						! FindFileByPattern(szSystemFolder, szModuleName, _T(".pdb"), szMapPdbFile))
					{
						return pBaseProcessor;
					}
					CBaseProcessor::PROCESSED_FILE_TYPE eProcessedFileType = CBaseProcessor::GetFileType(szMapPdbFile);
					switch (eProcessedFileType)
					{
					case CBaseProcessor::PFT_MAP:
						{
							CMapProcessor* pMapProcessor = STATIC_DOWNCAST(CMapProcessor, m_pMapProcessor.get());
							if (pMapProcessor == NULL)
							{
								pMapProcessor = new CMapProcessor();
								m_pMapProcessor.reset(pMapProcessor);
							}
							pBaseProcessor = m_pMapProcessor;
							pMapProcessor->LoadMapText(szMapPdbFile);
							pMapProcessor->SetBaseAddress(ptrBaseAddress);
						}
						break;
					case CBaseProcessor::PFT_PDB:
						{
							CPdbProcessor* pPdbProcessor = STATIC_DOWNCAST(CPdbProcessor, m_pPdbProcessor.get());
							if (pPdbProcessor == NULL)
							{
								pPdbProcessor = new CPdbProcessor();
								m_pPdbProcessor.reset(pPdbProcessor);
							}
							pBaseProcessor = m_pPdbProcessor;
							pPdbProcessor->LoadModule(szMapPdbFile, ptrBaseAddress, dwModuleSize);
						}
						break;
					}
					if (pBaseProcessor.get() != NULL)
						m_mapModules.insert(CModuleMap::value_type(strModule, pBaseProcessor));
				}
			}
		}
	}
	else
		pBaseProcessor = itModule->second;
	return pBaseProcessor;
}

/**
 * @param rStackEntry - stack entry information.
 */
void CExpressModeDlg::UpdateStackEntryFromMap(CStackEntry& rStackEntry)
{
	if (! rStackEntry.m_strAddress.IsEmpty() &&
		(
			rStackEntry.m_strFunctionName.IsEmpty() ||
			rStackEntry.m_strSourceFile.IsEmpty() ||
			rStackEntry.m_strLineNumber.IsEmpty()
		))
	{
		int nAddrPos = rStackEntry.m_strAddress.Find(_T(':'));
		if (nAddrPos >= 0)
			++nAddrPos;
		else
			nAddrPos = 0;
		PVOID ptrAddress = (PVOID)_tcstoui64((PCTSTR)rStackEntry.m_strAddress + nAddrPos, NULL, 16);
		if (ptrAddress != NULL)
		{
			boost::shared_ptr<CBaseProcessor> pBaseProcessor(GetModuleInfo(rStackEntry.m_strModule));
			if (pBaseProcessor.get() != NULL)
			{
				boost::shared_ptr<CBaseFnInfo> pFnInfo;
				DWORD64 dwDisplacement64;
				if (pBaseProcessor->FindFunctionInfo(ptrAddress, pFnInfo, dwDisplacement64))
				{
					std::string strFunctionName(pFnInfo->GetName());
					boost::match_results<std::string::const_iterator> what;
					if (boost::regex_search(strFunctionName, what, m_rxFunctionName))
						rStackEntry.m_strFunctionName = CA2CT(what[1].str().c_str());
					else
						rStackEntry.m_strFunctionName = CA2CT(strFunctionName.c_str());
					if (dwDisplacement64 != 0)
						rStackEntry.m_strFunctionOffset.Format(_T("%I64u"), dwDisplacement64);
					else
						rStackEntry.m_strFunctionOffset.Empty();
				}
				boost::shared_ptr<CBaseFileInfo> pFileInfo;
				boost::shared_ptr<CBaseLineInfo> pLineInfo;
				DWORD dwDisplacement32;
				if (pBaseProcessor->FindLineInfo(ptrAddress, pFileInfo, pLineInfo, dwDisplacement32))
				{
					rStackEntry.m_strSourceFile = CA2CT(pFileInfo->GetFileName().c_str());
					rStackEntry.m_strLineNumber.Format(_T("%u"), pLineInfo->GetNumber());
					if (dwDisplacement32 != 0)
						rStackEntry.m_strLineOffset.Format(_T("%I32u"), dwDisplacement32);
					else
						rStackEntry.m_strLineOffset.Empty();
				}
			}
		}
	}
}

/**
 * @param strErrorReason - buffer for error reason.
 */
void CExpressModeDlg::GetErrorReason(CString& strErrorReason)
{
	strErrorReason.Empty();
	if (! m_pXMLNodeError)
		return;

	CString strProcessName;
	GetXMLNodeText(m_pXMLNodeError, OLESTR("./process/name"), strProcessName, _T("UNKNOWN PROCESS"));
	CString strWhat;
	GetXMLNodeText(m_pXMLNodeError, OLESTR("./what"), strWhat, _T("UNKNOWN ERROR"));
	CStackEntry StackEntry;
	GetStackEntry(m_pXMLNodeError, StackEntry);

	strErrorReason = strProcessName;
	strErrorReason += _T(" caused ");
	strErrorReason += strWhat;

	if (! StackEntry.m_strModule.IsEmpty())
	{
		strErrorReason += _T(" in module \"");
		strErrorReason += StackEntry.m_strModule;
		strErrorReason += _T('\"');
	}

	if (! StackEntry.m_strAddress.IsEmpty())
	{
		strErrorReason += _T(" at ");
		strErrorReason += StackEntry.m_strAddress;
	}

	if (! StackEntry.m_strFunctionInfo.IsEmpty())
	{
		strErrorReason += _T(", ");
		strErrorReason += StackEntry.m_strFunctionInfo;
	}

	if (! StackEntry.m_strSourceFile.IsEmpty())
	{
		strErrorReason += _T(" in \"");
		strErrorReason += StackEntry.m_strSourceFile;
		strErrorReason += _T('\"');
	}

	if (! StackEntry.m_strLineInfo.IsEmpty())
	{
		strErrorReason += _T(", ");
		strErrorReason += StackEntry.m_strLineInfo;
	}
}

void CExpressModeDlg::SetErrorReasonText()
{
	CString strErrorReason;
	GetErrorReason(strErrorReason);
	m_txtErrorReason.SetWindowText(strErrorReason);
}

void CExpressModeDlg::FillStackTraceList()
{
	m_lstStackTrace.DeleteAllItems();
	if (! m_pXMLNodeProcess)
		return;
	CComPtr<IXMLDOMNodeList> pXMLNodeListStackFrames;
	if (! SelectXMLNodes(m_pXMLElementDocument, OLESTR("./threads/thread[status=\"interrupted\"]/stack/frame"), pXMLNodeListStackFrames))
		return;

	LVITEM lvi;
	ZeroMemory(&lvi, sizeof(lvi));
	lvi.mask = LVIF_TEXT;
	CStackEntry StackEntry;

	for (int iItemPos = 0; ; ++iItemPos)
	{
		CComPtr<IXMLDOMNode> pXMLNodeStackFrame;
		HRESULT hRes = pXMLNodeListStackFrames->nextNode(&pXMLNodeStackFrame);
		CHECK_HRESULT(hRes);
		if (hRes != S_OK)
			break;
		GetStackEntry(pXMLNodeStackFrame, StackEntry);

		lvi.iItem = iItemPos;
		lvi.pszText = (PTSTR)(PCTSTR)StackEntry.m_strAddress;
		m_lstStackTrace.InsertItem(&lvi);
		m_lstStackTrace.SetItemText(iItemPos, CID_ENTRY_FUNCTION, StackEntry.m_strFunctionInfo);
		m_lstStackTrace.SetItemText(iItemPos, CID_ENTRY_FILE, StackEntry.m_strSourceFile);
		m_lstStackTrace.SetItemText(iItemPos, CID_ENTRY_LINE, StackEntry.m_strLineInfo);
		m_lstStackTrace.SetItemText(iItemPos, CID_ENTRY_MODULE, StackEntry.m_strModule);
	}

	m_lstStackTrace.SetColumnWidth(CID_ENTRY_ADDRESS, LVSCW_AUTOSIZE_USEHEADER);
	m_lstStackTrace.SetColumnWidth(CID_ENTRY_FUNCTION, LVSCW_AUTOSIZE_USEHEADER);
	m_lstStackTrace.SetColumnWidth(CID_ENTRY_FILE, LVSCW_AUTOSIZE_USEHEADER);
	m_lstStackTrace.SetColumnWidth(CID_ENTRY_LINE, LVSCW_AUTOSIZE_USEHEADER);
	m_lstStackTrace.SetColumnWidth(CID_ENTRY_MODULE, LVSCW_AUTOSIZE_USEHEADER);
}

/**
 * @param pXMLNodeParent - parent XML node.
 * @param pszExpression - XPath expression.
 * @param strNodeText - buffer that receives node text.
 * @param pszDefaultText - default node text used if node is unavailable.
 * @return true if selected node is present.
 */
bool CExpressModeDlg::GetXMLNodeText(const CComPtr<IXMLDOMNode>& pXMLNodeParent, LPCOLESTR pszExpression, CString& strNodeText, PCTSTR pszDefaultText)
{
	CComPtr<IXMLDOMNode> pXMLNodeSelection;
	if (SelectXMLNode(pXMLNodeParent, pszExpression, pXMLNodeSelection))
	{
		CComBSTR bstrNodeValue;
		pXMLNodeSelection->get_text(&bstrNodeValue);
		strNodeText = bstrNodeValue;
		return true;
	}
	else
	{
		if (pszDefaultText != NULL)
			strNodeText = pszDefaultText;
		else
			strNodeText.Empty();
		return false;
	}
}

/**
 * @param strLogText - error log text.
 */
void CExpressModeDlg::GetErrorLog(CString& strLogText)
{
	strLogText.Empty();
	if (! m_pXMLElementDocument)
		return;
	HRESULT hRes;

	static const TCHAR szDividerMsg[] = _T("----------------------------------------\r\n");
	static const TCHAR szNewLine[] = _T("\r\n");

	CString strApplication;
	GetXMLNodeText(m_pXMLElementDocument, OLESTR("./application"), strApplication);
	if (! strApplication.IsEmpty())
	{
		static const TCHAR szAppMsg[] = _T("Application: ");

		strLogText += szAppMsg;
		strLogText += strApplication;
		strLogText += szNewLine;
	}

	CString strVersion;
	GetXMLNodeText(m_pXMLElementDocument, OLESTR("./version"), strVersion);
	if (! strVersion.IsEmpty())
	{
		static const TCHAR szVersionMsg[] = _T("Version: ");

		strLogText += szVersionMsg;
		strLogText += strVersion;
		strLogText += szNewLine;
	}

	CString strComputer;
	GetXMLNodeText(m_pXMLElementDocument, OLESTR("./computer"), strComputer);
	if (! strComputer.IsEmpty())
	{
		static const TCHAR szComputerNameMsg[] = _T("Computer: ");

		strLogText += szComputerNameMsg;
		strLogText += strComputer;
		strLogText += szNewLine;
	}

	CString strUser;
	GetXMLNodeText(m_pXMLElementDocument, OLESTR("./user"), strUser);
	if (! strUser.IsEmpty())
	{
		static const TCHAR szUserNameMsg[] = _T("User: ");

		strLogText += szUserNameMsg;
		strLogText += strUser;
		strLogText += szNewLine;
	}

	CString strTimeStamp;
	GetXMLNodeText(m_pXMLElementDocument, OLESTR("./timestamp"), strTimeStamp);
	if (! strTimeStamp.IsEmpty())
	{
		ULONGLONG uiTimeStamp = _tcstoui64(strTimeStamp, NULL, 0);
		if (uiTimeStamp != 0)
		{
			GetDateTimeString(uiTimeStamp, strTimeStamp);
			if (! strTimeStamp.IsEmpty())
			{
				static const TCHAR szDateTimeMsg[] = _T("Date: ");

				strLogText += szDateTimeMsg;
				strLogText += strTimeStamp;
				strLogText += szNewLine;
			}
		}
	}

	CString strErrorReason;
	GetErrorReason(strErrorReason);
	if (! strErrorReason.IsEmpty())
	{
		static const TCHAR szErrorMsg[] = _T("\r\nError Reason:\r\n");

		strLogText += szErrorMsg;
		strLogText += szDividerMsg;
		strLogText += strErrorReason;
	}

	CString strUserMessage;
	if (GetXMLNodeText(m_pXMLElementDocument, OLESTR("./usermsg"), strUserMessage) &&
		! strUserMessage.IsEmpty())
	{
		static const TCHAR szUserMsg[] = _T("\r\n\r\nUser Message:\r\n");

		strLogText += szUserMsg;
		strLogText += szDividerMsg;
		strLogText += strUserMessage;
	}

	CComPtr<IXMLDOMNode> pXMLNodeSysError;
	if (SelectXMLNode(m_pXMLElementDocument, OLESTR("./syserror"), pXMLNodeSysError))
	{
		CString strSysErrorCode;
		GetXMLNodeText(pXMLNodeSysError, OLESTR("./code"), strSysErrorCode);
		if (! strSysErrorCode.IsEmpty())
		{
			static const TCHAR szSysErrorMsg[] = _T("\r\n\r\nSystem Error:\r\n");

			strLogText += szSysErrorMsg;
			strLogText += szDividerMsg;
			strLogText += strSysErrorCode;

			CString strSysErrorDescription;
			GetXMLNodeText(pXMLNodeSysError, OLESTR("./description"), strSysErrorDescription);
			if (! strSysErrorDescription.IsEmpty())
			{
				strLogText += _T(" - ");
				strLogText += strSysErrorDescription;
			}
		}
	}

	CComPtr<IXMLDOMNode> pXMLNodeComError;
	if (SelectXMLNode(m_pXMLElementDocument, OLESTR("./comerror"), pXMLNodeComError))
	{
		CString strComErrorDescription;
		GetXMLNodeText(pXMLNodeComError, OLESTR("./description"), strComErrorDescription);
		CString strComErrorHelpFile;
		GetXMLNodeText(pXMLNodeComError, OLESTR("./helpfile"), strComErrorHelpFile);
		CString strComErrorSource;
		GetXMLNodeText(pXMLNodeComError, OLESTR("./source"), strComErrorSource);
		CString strComErrorGuid;
		GetXMLNodeText(pXMLNodeComError, OLESTR("./guid"), strComErrorGuid);

		if (! strComErrorDescription.IsEmpty() ||
			! strComErrorHelpFile.IsEmpty() ||
			! strComErrorSource.IsEmpty() ||
			! strComErrorGuid.IsEmpty())
		{
			static const TCHAR szCOMErrorMsg[] = _T("\r\n\r\nCOM Error:\r\n");
			static const TCHAR szDescriptionMsg[] = _T("Description: ");
			static const TCHAR szHelpFileMsg[] = _T("Help File:   ");
			static const TCHAR szSourceMsg[] = _T("Source:      ");
			static const TCHAR szGuidMsg[] = _T("GUID:        ");

			strLogText += szCOMErrorMsg;
			strLogText += szDividerMsg;

			BOOL bNotEmpty = FALSE;

			if (! strComErrorDescription.IsEmpty())
			{
				bNotEmpty = TRUE;
				strLogText += szDescriptionMsg;
				strLogText += strComErrorDescription;
			}

			if (! strComErrorHelpFile.IsEmpty())
			{
				if (bNotEmpty)
					strLogText += szNewLine;
				else
					bNotEmpty = TRUE;
				strLogText += szHelpFileMsg;
				strLogText += strComErrorHelpFile;
			}

			if (! strComErrorSource.IsEmpty())
			{
				if (bNotEmpty)
					strLogText += szNewLine;
				else
					bNotEmpty = TRUE;
				strLogText += szSourceMsg;
				strLogText += strComErrorSource;
			}

			if (! strComErrorGuid.IsEmpty())
			{
				if (bNotEmpty)
					strLogText += szNewLine;
				else
					bNotEmpty = TRUE;
				strLogText += szGuidMsg;
				strLogText += strComErrorGuid;
			}
		}
	}

	CComPtr<IXMLDOMNode> pXMLNodeRegisters;
	if (SelectXMLNode(m_pXMLElementDocument, OLESTR("./registers"), pXMLNodeRegisters))
	{
		static const TCHAR szRegistersMsg[] = _T("\r\n\r\nRegisters:\r\n");

		CString strRegValue;

		switch (GetPlatform())
		{
		case TPLAT_X86:
			{
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./eax"), strRegValue);
				DWORD dwEAX = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./ebx"), strRegValue);
				DWORD dwEBX = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./ecx"), strRegValue);
				DWORD dwECX = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./edx"), strRegValue);
				DWORD dwEDX = _tcstoul(strRegValue, NULL, 0);

				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./esi"), strRegValue);
				DWORD dwESI = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./edi"), strRegValue);
				DWORD dwEDI = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./esp"), strRegValue);
				DWORD dwESP = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./ebp"), strRegValue);
				DWORD dwEBP = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./eip"), strRegValue);
				DWORD dwEIP = _tcstoul(strRegValue, NULL, 0);

				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./cs"), strRegValue);
				DWORD dwCS = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./ds"), strRegValue);
				DWORD dwDS = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./ss"), strRegValue);
				DWORD dwSS = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./es"), strRegValue);
				DWORD dwES = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./fs"), strRegValue);
				DWORD dwFS = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./gs"), strRegValue);
				DWORD dwGS = _tcstoul(strRegValue, NULL, 0);

				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./eflags"), strRegValue);
				DWORD dwEFLAGS = _tcstoul(strRegValue, NULL, 0);

				strRegValue.Format(
					_T("EAX=%08X  EBX=%08X  ECX=%08X  EDX=%08X\r\n")
					_T("ESI=%08X  EDI=%08X  FLG=%08X\r\n")
					_T("EBP=%08X  ESP=%08X  EIP=%08X\r\n")
					_T("CS=%04X  DS=%04X  SS=%04X  ES=%04X  FS=%04X  GS=%04X"),
					dwEAX, dwEBX, dwECX, dwEDX,
					dwESI, dwEDI, dwEFLAGS,
					dwEBP, dwESP, dwEIP,
					dwCS, dwDS, dwSS, dwES, dwFS, dwGS);
			}
			break;
		case TPLAT_X64:
			{
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./rax"), strRegValue);
				DWORD dwRAX = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./rbx"), strRegValue);
				DWORD dwRBX = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./rcx"), strRegValue);
				DWORD dwRCX = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./rdx"), strRegValue);
				DWORD dwRDX = _tcstoul(strRegValue, NULL, 0);

				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./rsi"), strRegValue);
				DWORD dwRSI = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./rdi"), strRegValue);
				DWORD dwRDI = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./rsp"), strRegValue);
				DWORD dwRSP = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./rbp"), strRegValue);
				DWORD dwRBP = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./rip"), strRegValue);
				DWORD dwRIP = _tcstoul(strRegValue, NULL, 0);

				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./cs"), strRegValue);
				DWORD dwCS = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./ds"), strRegValue);
				DWORD dwDS = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./ss"), strRegValue);
				DWORD dwSS = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./es"), strRegValue);
				DWORD dwES = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./fs"), strRegValue);
				DWORD dwFS = _tcstoul(strRegValue, NULL, 0);
				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./gs"), strRegValue);
				DWORD dwGS = _tcstoul(strRegValue, NULL, 0);

				GetXMLNodeText(pXMLNodeRegisters, OLESTR("./eflags"), strRegValue);
				DWORD dwEFLAGS = _tcstoul(strRegValue, NULL, 0);

				strRegValue.Format(
					_T("RAX=%016X  RBX=%016X\r\n")
					_T("RCX=%016X  RDX=%016X\r\n")
					_T("RSI=%016X  RDI=%016X\r\n")
					_T("FLG=%08X          RBP=%016X\r\n")
					_T("RSP=%016X  RIP=%016X\r\n")
					_T("CS=%04X  DS=%04X  SS=%04X  ES=%04X  FS=%04X  GS=%04X"),
					dwRAX, dwRBX, dwRCX, dwRDX,
					dwRSI, dwRDI, dwEFLAGS,
					dwRBP, dwRSP, dwRIP,
					dwCS, dwDS, dwSS, dwES, dwFS, dwGS);
			}
			break;
		default:
			_ASSERT(FALSE);
		}

		strLogText += szRegistersMsg;
		strLogText += szDividerMsg;
		strLogText += strRegValue;
	}

	CComPtr<IXMLDOMNode> pXMLNodeCpus;
	if (SelectXMLNode(m_pXMLElementDocument, OLESTR("./cpus"), pXMLNodeCpus))
	{
		CString strNumCpus;
		GetXMLNodeText(pXMLNodeCpus, OLESTR("./number"), strNumCpus);
		if (! strNumCpus.IsEmpty() && _tcstoul(strNumCpus, NULL, 0) != 0)
		{
			static const TCHAR szCpuMsg[] = _T("\r\n\r\nCPU:\r\n");
			static const TCHAR szArchitectureMsg[] = _T("Architecture: ");
			static const TCHAR szNumCpusMsg[] = _T("Number of Processors:  ");
			static const TCHAR szCpuDescriptionsMsg[] = _T("\r\nProcessors Descriptions:");

			strLogText += szCpuMsg;
			strLogText += szDividerMsg;

			CString strArchitecture;
			GetXMLNodeText(pXMLNodeCpus, OLESTR("./architecture"), strArchitecture);
			if (! strArchitecture.IsEmpty())
			{
				strLogText += szArchitectureMsg;
				strLogText += strArchitecture;
				strLogText += szNewLine;
			}

			strLogText += szNumCpusMsg;
			strLogText += strNumCpus;
			strLogText += szCpuDescriptionsMsg;

			CString strCpuDescription;
			CComPtr<IXMLDOMNodeList> pXMLNodeListCpus;
			if (SelectXMLNodes(pXMLNodeCpus, OLESTR("./cpu"), pXMLNodeListCpus))
			{
				for (int iItemPos = 1; ; ++iItemPos)
				{
					CComPtr<IXMLDOMNode> pXMLNodeCpu;
					hRes = pXMLNodeListCpus->nextNode(&pXMLNodeCpu);
					CHECK_HRESULT(hRes);
					if (hRes != S_OK)
						break;

					GetXMLNodeText(pXMLNodeCpu, OLESTR("./description"), strCpuDescription);
					TCHAR szItemPos[16];
					_itot_s(iItemPos, szItemPos, countof(szItemPos), 10);

					strLogText += szNewLine;
					strLogText += szItemPos;
					strLogText += _T(". ");
					strLogText += strCpuDescription;
				}
			}
		}
	}

	CComPtr<IXMLDOMNode> pXMLNodeOs;
	if (SelectXMLNode(m_pXMLElementDocument, OLESTR("./os"), pXMLNodeOs))
	{
		static const TCHAR szOSMsg[] = _T("\r\n\r\nOperating System:\r\n");
		static const TCHAR szOsVersionMsg[] = _T("OS Version:    ");
		static const TCHAR szBuildNumberMsg[] = _T("Build Number:  ");

		CString strOsVersion;
		GetXMLNodeText(pXMLNodeOs, OLESTR("./version"), strOsVersion);
		CString strServicePack;
		GetXMLNodeText(pXMLNodeOs, OLESTR("./spack"), strServicePack);
		CString strBuildNumber;
		GetXMLNodeText(pXMLNodeOs, OLESTR("./build"), strBuildNumber);

		strLogText += szOSMsg;
		strLogText += szDividerMsg;
		strLogText += szOsVersionMsg;
		strLogText += strOsVersion;
		if (! strServicePack.IsEmpty())
		{
			strLogText += _T(' ');
			strLogText += strServicePack;
		}
		strLogText += szNewLine;
		strLogText += szBuildNumberMsg;
		strLogText += strBuildNumber;
	}

	CComPtr<IXMLDOMNode> pXMLNodeMemory;
	if (SelectXMLNode(m_pXMLElementDocument, OLESTR("./memory"), pXMLNodeMemory))
	{
		static const TCHAR szMemMsg[] = _T("\r\n\r\nMemory Usage:\r\n");

		CString strMemValue;

		GetXMLNodeText(pXMLNodeMemory, OLESTR("./load"), strMemValue);
		DWORD dwCurMemLoad = _tcstoul(strMemValue, NULL, 0);
		GetXMLNodeText(pXMLNodeMemory, OLESTR("./totalphys"), strMemValue);
		DWORD dwTotalPhysMem = _tcstoul(strMemValue, NULL, 0);
		GetXMLNodeText(pXMLNodeMemory, OLESTR("./availphys"), strMemValue);
		DWORD dwAvailPhysMem = _tcstoul(strMemValue, NULL, 0);
		GetXMLNodeText(pXMLNodeMemory, OLESTR("./totalpage"), strMemValue);
		DWORD dwTotalPageMem = _tcstoul(strMemValue, NULL, 0);
		GetXMLNodeText(pXMLNodeMemory, OLESTR("./availpage"), strMemValue);
		DWORD dwAvailPageMem = _tcstoul(strMemValue, NULL, 0);

		strMemValue.Format(
			_T("Current Memory Load:         %lu%%\r\n")
			_T("Total Physical Memory:       %lu MB\r\n")
			_T("Available Physical Memory:   %lu MB\r\n")
			_T("Total Page File Memory:      %lu MB\r\n")
			_T("Available Page File Memory:  %lu MB"),
			dwCurMemLoad,
			dwTotalPhysMem / (1024 * 1024),
			dwAvailPhysMem / (1024 * 1024),
			dwTotalPageMem / (1024 * 1024),
			dwAvailPageMem / (1024 * 1024));

		strLogText += szMemMsg;
		strLogText += szDividerMsg;
		strLogText += strMemValue;
	}

	CStackEntry StackEntry;
	CComPtr<IXMLDOMNodeList> pXMLNodeListThreads;
	if (SelectXMLNodes(m_pXMLElementDocument, OLESTR("./threads/thread"), pXMLNodeListThreads))
	{
		static const TCHAR szTraceMsg[] = _T("\r\nStack Trace: ");
		static const TCHAR szThreadIDMsg[] = _T(", TID: ");
		static const TCHAR szThreadMsg[] = _T(" Thread");

		strLogText += szNewLine;

		CString strThreadID, strThreadStatus;
		for (;;)
		{
			CComPtr<IXMLDOMNode> pXMLNodeThread;
			hRes = pXMLNodeListThreads->nextNode(&pXMLNodeThread);
			CHECK_HRESULT(hRes);
			if (hRes != S_OK)
				break;

			GetXMLNodeText(pXMLNodeThread, OLESTR("./id"), strThreadID);
			GetXMLNodeText(pXMLNodeThread, OLESTR("./status"), strThreadStatus);
			if (! strThreadStatus.IsEmpty())
			{
				strThreadStatus.MakeLower();
				strThreadStatus.SetAt(0, (TCHAR)_totupper(strThreadStatus.GetAt(0)));
				strThreadStatus += szThreadMsg;
			}

			strLogText += szTraceMsg;
			strLogText += strThreadStatus;
			strLogText += szThreadIDMsg;
			strLogText += strThreadID;
			strLogText += szNewLine;
			strLogText += szDividerMsg;

			CComPtr<IXMLDOMNodeList> pXMLNodeListStackFrames;
			if (SelectXMLNodes(pXMLNodeThread, OLESTR("./stack/frame"), pXMLNodeListStackFrames))
			{
				for (;;)
				{
					CComPtr<IXMLDOMNode> pXMLNodeStackFrame;
					hRes = pXMLNodeListStackFrames->nextNode(&pXMLNodeStackFrame);
					CHECK_HRESULT(hRes);
					if (hRes != S_OK)
						break;
					GetStackEntry(pXMLNodeStackFrame, StackEntry);

					if (! StackEntry.m_strModule.IsEmpty())
					{
						strLogText += _T('\"');
						strLogText += StackEntry.m_strModule;
						strLogText += _T('\"');
					}
					if (! StackEntry.m_strAddress.IsEmpty())
					{
						strLogText += _T(" at ");
						strLogText += StackEntry.m_strAddress;
					}
					if (! StackEntry.m_strFunctionInfo.IsEmpty())
					{
						strLogText += _T(", ");
						strLogText += StackEntry.m_strFunctionInfo;
					}

					if (! StackEntry.m_strSourceFile.IsEmpty())
					{
						strLogText += _T(" in ");
						strLogText += StackEntry.m_strSourceFile;
					}

					if (! StackEntry.m_strLineInfo.IsEmpty())
					{
						strLogText += _T(", ");
						strLogText += StackEntry.m_strLineInfo;
					}

					strLogText += szNewLine;
				}
			}
		}
	}

	CString strCommandLine;
	GetXMLNodeText(m_pXMLElementDocument, OLESTR("./cmdline"), strCommandLine);
	if (! strCommandLine.IsEmpty())
	{
		static const TCHAR szCommandLineMsg[] = _T("\r\nCommand Line:\r\n");

		strLogText += szCommandLineMsg;
		strLogText += szDividerMsg;
		strLogText += strCommandLine;
	}

	CString strCurrentDirectory;
	GetXMLNodeText(m_pXMLElementDocument, OLESTR("./curdir"), strCurrentDirectory);
	if (! strCurrentDirectory.IsEmpty())
	{
		static const TCHAR szCurrentDirMsg[] = _T("\r\n\r\nCurrent Directory:\r\n");

		strLogText += szCurrentDirMsg;
		strLogText += szDividerMsg;
		strLogText += strCurrentDirectory;
	}

	CComPtr<IXMLDOMNodeList> pXMLNodeListEnvVars;
	if (SelectXMLNodes(m_pXMLElementDocument, OLESTR("./environment/variable"), pXMLNodeListEnvVars))
	{
		static const TCHAR szEnvironmentMsg[] = _T("\r\n\r\nEnvironment Variables:\r\n");

		strLogText += szEnvironmentMsg;
		strLogText += szDividerMsg;

		CString strVarName, strVarValue;
		for (;;)
		{
			CComPtr<IXMLDOMNode> pXMLNodeEnvVar;
			hRes = pXMLNodeListEnvVars->nextNode(&pXMLNodeEnvVar);
			CHECK_HRESULT(hRes);
			if (hRes != S_OK)
				break;

			GetXMLNodeText(pXMLNodeEnvVar, OLESTR("./name"), strVarName);
			GetXMLNodeText(pXMLNodeEnvVar, OLESTR("./value"), strVarValue);

			strLogText += strVarName;
			strLogText += _T('=');
			strLogText += strVarValue;
			strLogText += szNewLine;
		}
	}

	CComPtr<IXMLDOMNodeList> pXMLNodeListProcesses;
	if (SelectXMLNodes(m_pXMLElementDocument, OLESTR("./processes/process"), pXMLNodeListProcesses))
	{
		static const TCHAR szProcessMsg[] = _T("\r\nProcess: ");
		static const TCHAR szModulesMsg[] = _T(", Modules:\r\n");
		static const TCHAR szProcessIDMsg[] = _T(", PID: ");
		static const TCHAR szBaseMsg[] = _T(", Base: ");

		CString strProcessID, strProcessName, strModuleName, strModuleVersion, strModuleBase;
		for (;;)
		{
			CComPtr<IXMLDOMNode> pXMLNodeProcess;
			hRes = pXMLNodeListProcesses->nextNode(&pXMLNodeProcess);
			CHECK_HRESULT(hRes);
			if (hRes != S_OK)
				break;

			GetXMLNodeText(pXMLNodeProcess, OLESTR("./name"), strProcessName);
			GetXMLNodeText(pXMLNodeProcess, OLESTR("./id"), strProcessID);

			strLogText += szProcessMsg;
			strLogText += strProcessName;
			strLogText += szProcessIDMsg;
			strLogText += strProcessID;
			strLogText += szModulesMsg;
			strLogText += szDividerMsg;

			CComPtr<IXMLDOMNodeList> pXMLNodeListModules;
			if (SelectXMLNodes(pXMLNodeProcess, OLESTR("./modules/module"), pXMLNodeListModules))
			{
				for (;;)
				{
					CComPtr<IXMLDOMNode> pXMLNodeModule;
					hRes = pXMLNodeListModules->nextNode(&pXMLNodeModule);
					CHECK_HRESULT(hRes);
					if (hRes != S_OK)
						break;

					GetXMLNodeText(pXMLNodeModule, OLESTR("./name"), strModuleName);
					GetXMLNodeText(pXMLNodeModule, OLESTR("./version"), strModuleVersion);
					GetXMLNodeText(pXMLNodeModule, OLESTR("./base"), strModuleBase);

					strLogText += strModuleName;

					if (! strModuleVersion.IsEmpty())
					{
						strLogText += _T(" (");
						strLogText += strModuleVersion;
						strLogText += _T(')');
					}

					if (! strModuleBase.IsEmpty())
					{
						PVOID ptrBaseAddress = (PVOID)_tcstoui64(strModuleBase, NULL, 0);
						if (ptrBaseAddress != NULL)
						{
							strModuleBase.Format(_T("%08lX"), ptrBaseAddress);
							strLogText += szBaseMsg;
							strLogText += strModuleBase;
						}
					}

					strLogText += szNewLine;
				}
			}
		}
	}
}

/**
 * @param uiTimeStamp - time-stamp value.
 * @param rDateTime - date-time value.
 */
void CExpressModeDlg::GetDateTime(ULONGLONG uiTimeStamp, SYSTEMTIME& rDateTime)
{
	ULARGE_INTEGER uint;
	uint.QuadPart = uiTimeStamp;
	FILETIME ftime;
	ftime.dwLowDateTime = uint.LowPart;
	ftime.dwHighDateTime = uint.HighPart;
	FileTimeToSystemTime(&ftime, &rDateTime);
}

/**
 * @param uiTimeStamp - time-stamp value.
 * @param strTimeStamp - date-time string.
 */
void CExpressModeDlg::GetDateTimeString(ULONGLONG uiTimeStamp, CString& strTimeStamp)
{
	SYSTEMTIME DateTime;
	GetDateTime(uiTimeStamp, DateTime);
	const DWORD dwDateTimeSize = 64;
	PTSTR pszDateTime = strTimeStamp.GetBuffer(dwDateTimeSize);
	// Computers use different locales, so it's desirable to use universal date and time format.
	const LCID lcidEnglishUS = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);
	DWORD dwOutputSize = GetDateFormat(lcidEnglishUS, LOCALE_USE_CP_ACP | DATE_LONGDATE | LOCALE_NOUSEROVERRIDE, &DateTime, NULL, pszDateTime, dwDateTimeSize);
	if (dwOutputSize < dwDateTimeSize)
		pszDateTime[dwOutputSize - 1] = _T(' ');
	GetTimeFormat(lcidEnglishUS, LOCALE_USE_CP_ACP | LOCALE_NOUSEROVERRIDE, &DateTime, NULL, pszDateTime + dwOutputSize, dwDateTimeSize - dwOutputSize);
	strTimeStamp.ReleaseBuffer();
}

/**
 * @return true if document has been properly loaded.
 */
bool CExpressModeDlg::ValidateXMLDocument()
{
	if (! m_pXMLDocument)
		OnCalculate(0, 0, NULL);
	return (m_pXMLDocument != NULL);
}

/**
 * @param error - exception information.
 */
void CExpressModeDlg::HandleException(const std::exception& error)
{
	CString strProjectTitle;
	strProjectTitle.LoadString(IDS_PROJECTTITLE);
	MessageBox(CA2CT(error.what()), strProjectTitle, MB_OK | MB_ICONERROR);
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
void CExpressModeDlg::OnCopy(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	try
	{
		if (! ValidateXMLDocument())
			return;
		CString strLogText;
		GetErrorLog(strLogText);
		m_CopyTextObject.SetObjectText(strLogText);
		HRESULT hRes = m_CopyTextObject.SetClipboard();
		CHECK_HRESULT(hRes);
	}
	catch (std::exception& error)
	{
		HandleException(error);
	}
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
void CExpressModeDlg::OnSave(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	try
	{
		if (! ValidateXMLDocument())
			return;
		CString strLogFile;
		GetReportFileName(strLogFile);
		CCustomFileDialog dlgSaveFile(FALSE,
			_T("xml"),
			strLogFile,
			OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST,
			_T("Log Files\0*.log\0All Files\0*.*\0"),
			m_hWnd);
		if (dlgSaveFile.DoModal(m_hWnd) == IDOK)
		{
			CAtlFile fileLog;
			if (fileLog.Create(dlgSaveFile.m_szFileName, GENERIC_WRITE, 0, CREATE_ALWAYS) == S_OK)
			{
				CString strLogText;
				GetErrorLog(strLogText);
				CT2CW pchUnicodeText(strLogText);
				int nDestSize = AtlUnicodeToUTF8(pchUnicodeText, strLogText.GetLength(), NULL, 0);
				if (nDestSize >= 0)
				{
					static const BYTE arrUTF8Preamble[] = { 0xEF, 0xBB, 0xBF };
					int nTotalDestSize = sizeof(arrUTF8Preamble) + nDestSize;
					boost::scoped_array<CHAR> pchDest(new CHAR[nTotalDestSize]);
					CopyMemory(pchDest.get(), arrUTF8Preamble, sizeof(arrUTF8Preamble));
					AtlUnicodeToUTF8(pchUnicodeText, strLogText.GetLength(), pchDest.get() + sizeof(arrUTF8Preamble), nDestSize);
					fileLog.Write(pchDest.get(), nTotalDestSize);
				}
			}
		}
	}
	catch (std::exception& error)
	{
		HandleException(error);
	}
}

/**
 * @param strReportName - report file name.
 */
void CExpressModeDlg::GetReportFileName(CString& strReportName)
{
	strReportName.Empty();
	if (! m_pXMLElementDocument)
		return;
	CString strTimeStamp;
	GetXMLNodeText(m_pXMLElementDocument, OLESTR("./timestamp"), strTimeStamp);
	if (! strTimeStamp.IsEmpty())
	{
		ULONGLONG uiTimeStamp = _tcstoui64(strTimeStamp, NULL, 0);
		if (uiTimeStamp != 0)
		{
			CString strApplication;
			GetXMLNodeText(m_pXMLElementDocument, OLESTR("./application"), strApplication);
			if (! strApplication.IsEmpty())
			{
				int nLength = strApplication.GetLength();
				for (int i = 0; i < nLength; ++i)
				{
					TCHAR chValue = strApplication.GetAt(i);
					if (_istalnum(chValue))
						strReportName += chValue;
				}
				strReportName += _T('_');
			}
			SYSTEMTIME DateTime;
			GetDateTime(uiTimeStamp, DateTime);
			CString strTimeStamp;
			strTimeStamp.Format(_T("error_report_%02d%02d%02d-%02d%02d%02d.log"),
				DateTime.wYear % 100, DateTime.wMonth, DateTime.wDay,
				DateTime.wHour, DateTime.wMinute, DateTime.wSecond);
			strReportName += strTimeStamp;
		}
	}
}

void CExpressModeDlg::SaveState(void)
{
	CRegKey reg;
	if (reg.Create(HKEY_CURRENT_USER, g_szExpressModeSettingsRegKey) == ERROR_SUCCESS)
	{
		CString strText;

		m_txtLogFile.GetWindowText(strText);
		reg.SetStringValue(g_szLogFile, strText);

		m_txtMapPdbFolder.GetWindowText(strText);
		reg.SetStringValue(g_szMapPdbFolder, strText);

		reg.Close();
	}
}

void CExpressModeDlg::LoadState(void)
{
	CRegKey reg;
	if (reg.Open(HKEY_CURRENT_USER, g_szExpressModeSettingsRegKey) == ERROR_SUCCESS)
	{
		TCHAR szText[MAX_PATH];

		ULONG ulNumChars = countof(szText);
		reg.QueryStringValue(g_szLogFile, szText, &ulNumChars);
		m_txtLogFile.SetWindowText(szText);

		ulNumChars = countof(szText);
		reg.QueryStringValue(g_szMapPdbFolder, szText, &ulNumChars);
		m_txtMapPdbFolder.SetWindowText(szText);

		reg.Close();
	}
}

/**
 * @return target machine's platform.
 */
CExpressModeDlg::TARGET_PLATFORM CExpressModeDlg::GetPlatform(void) const
{
	if (m_strPlatform.CompareNoCase(_T("Win32")) == 0 ||
		m_strPlatform.CompareNoCase(_T("Win32-x86")) == 0)
	{
		return TPLAT_X86;
	}
	else if (m_strPlatform.CompareNoCase(_T("Win64-x64")) == 0)
	{
		return TPLAT_X64;
	}
	else
	{
		CStringA strErrorMessage;
		strErrorMessage.LoadString(IDS_ERROR_UNSUPPORTEDPLATFORM);
		throw com_error(strErrorMessage);
	}
}