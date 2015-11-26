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

#pragma once

#include "BaseTabItem.h"
#include "BaseProcessor.h"
#include "LayoutManager.h"
#include "TextObject.h"

/// Express map mode dialog.
class CExpressModeDlg :
	public CDialogImpl<CExpressModeDlg, CBaseTabItem>,
	public CScrollImpl<CExpressModeDlg>,
	public CLayoutManager<CExpressModeDlg>
{
	DECLARE_RUNTIME_CLASS(CExpressModeDlg)
public:
	/// Dialog resource identifier.
	enum { IDD = IDD_EXPRESS_MODE };

	BEGIN_MSG_MAP_EX(CExpressModeDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_DESTROY(OnDestroy)
		COMMAND_ID_HANDLER_EX(IDOK, OnCalculate)
		COMMAND_ID_HANDLER_EX(IDCANCEL, OnCloseCmd)
		COMMAND_ID_HANDLER_EX(IDC_COPY, OnCopy)
		COMMAND_ID_HANDLER_EX(IDC_SAVE, OnSave)
		COMMAND_HANDLER_EX(IDC_LOGFILE_BROWSE, BN_CLICKED, OnLogFileBrowse)
		COMMAND_HANDLER_EX(IDC_MAPPDBFOLDER_BROWSE, BN_CLICKED, OnMapPdbFolderBrowse)
		CHAIN_MSG_MAP(CScrollImpl<CExpressModeDlg>)
		CHAIN_MSG_MAP(CLayoutManager<CExpressModeDlg>)
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
	/// Stack entry information.
	struct CStackEntry
	{
		/// Module name.
		CString m_strModule;
		/// Address.
		CString m_strAddress;
		/// Function name.
		CString m_strFunctionName;
		/// Function offset.
		CString m_strFunctionOffset;
		/// Textual function description.
		CString m_strFunctionInfo;
		/// Source file name.
		CString m_strSourceFile;
		/// Line number.
		CString m_strLineNumber;
		/// Line offset.
		CString m_strLineOffset;
		/// Textual line information.
		CString m_strLineInfo;
	};

	/// Map of module related information.
	typedef std::map<CString, boost::shared_ptr<CBaseProcessor> > CModuleMap;

	/// WM_INITDIALOG event handler.
	LRESULT OnInitDialog(HWND hWnd, LPARAM lParam);
	/// WM_DESTROY event handler.
	void OnDestroy();
	/// Browse button command handler.
	void OnLogFileBrowse(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// Browse button command handler.
	void OnMapPdbFolderBrowse(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// Calculate button command handler.
	void OnCalculate(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// Close active dialog.
	void OnCloseCmd(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// Copy error log to the clipboard.
	void OnCopy(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// Save error log to a file
	void OnSave(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// Load XML document to memory.
	void LoadXMLDocument(const CString& strFilePath);
	/// Set error reason text.
	void SetErrorReasonText();
	/// Set stack trace.
	void FillStackTraceList();
	/// Select XML node using XPath expression.
	bool SelectXMLNode(const CComPtr<IXMLDOMNode>& pXMLNodeParent, LPCOLESTR pszExpression, CComPtr<IXMLDOMNode>& pXMLNodeSelection);
	/// Select XML node using XPath expression.
	bool SelectXMLNode(const CComPtr<IXMLDOMElement>& pXMLElementParent, LPCOLESTR pszExpression, CComPtr<IXMLDOMNode>& pXMLNodeSelection);
	/// Select XML node using XPath expression.
	bool SelectXMLNodes(const CComPtr<IXMLDOMNode>& pXMLNodeParent, LPCOLESTR pszExpression, CComPtr<IXMLDOMNodeList>& pXMLNodeListSelection);
	/// Select XML node using XPath expression.
	bool SelectXMLNodes(const CComPtr<IXMLDOMElement>& pXMLElementParent, LPCOLESTR pszExpression, CComPtr<IXMLDOMNodeList>& pXMLNodeListSelection);
	/// Get node text.
	bool GetXMLNodeText(const CComPtr<IXMLDOMNode>& pXMLNodeParent, LPCOLESTR pszExpression, CString& strNodeText, PCTSTR pszDefaultText = NULL);
	/// Get node text.
	bool GetXMLNodeText(const CComPtr<IXMLDOMElement>& pXMLElementParent, LPCOLESTR pszExpression, CString& strNodeText, PCTSTR pszDefaultText = NULL);
	/// Get stack entry information.
	void GetStackEntry(const CComPtr<IXMLDOMNode>& pXMLNodeParent, CStackEntry& rStackEntry);
	/// Updates stack entry from map file.
	void UpdateStackEntryFromMap(CStackEntry& rStackEntry);
	/// Clear cached information.
	void ClearData();
	/// Find or, if necessary, create new module info.
	boost::shared_ptr<CBaseProcessor> GetModuleInfo(const CString& strModule);
	/// Get complete error log text.
	void GetErrorLog(CString& strLogText);
	/// Convert time-stamp value to date-time.
	void GetDateTime(ULONGLONG uiTimeStamp, SYSTEMTIME& rDateTime);
	/// Get string representation of time-stamp.
	void GetDateTimeString(ULONGLONG uiTimeStamp, CString& strTimeStamp);
	/// Get default report file name.
	void GetReportFileName(CString& strReportName);
	/// Get error reason text.
	void GetErrorReason(CString& strErrorReason);
	/// Check if XML document has been properly loaded.
	bool ValidateXMLDocument();
	/// Handle generic exception.
	void HandleException(const std::exception& error);
	/// Find file with requested extension.
	static bool FindFileByPattern(PCTSTR pszMapPdbFolder, PCTSTR pszModuleName, PCTSTR pszFileExt, PTSTR pszFileName);

	/// Describes target machine's HW platform.
	enum TARGET_PLATFORM
	{
		/// Intel-x86.
		TPLAT_X86,
		/// AMD-x64.
		TPLAT_X64
	};

	/// Determine target machine's platform.
	TARGET_PLATFORM GetPlatform(void) const;

	/// Points to XML document.
	CComPtr<IXMLDOMDocument2> m_pXMLDocument;
	/// Points to document root.
	CComPtr<IXMLDOMElement> m_pXMLElementDocument;
	/// Cached pointer to error information.
	CComPtr<IXMLDOMNode> m_pXMLNodeError;
	/// Cached pointer to broken process.
	CComPtr<IXMLDOMNode> m_pXMLNodeProcess;
	/// Target platform.
	CString m_strPlatform;
	/// Map of module related information.
	CModuleMap m_mapModules;
	/// Shared PDB processor.
	boost::shared_ptr<CBaseProcessor> m_pPdbProcessor;
	/// Shared MAP processor.
	boost::shared_ptr<CBaseProcessor> m_pMapProcessor;
	/// Log file edit box.
	CEdit m_txtLogFile;
	/// Map/pdb folder edit box.
	CEdit m_txtMapPdbFolder;
	/// Error reason edit box.
	CEdit m_txtErrorReason;
	/// Stack trace list control.
	CListViewCtrl m_lstStackTrace;
	/// Regular expression used to extract function name.
	boost::regex m_rxFunctionName;
	/// Helper object for clipboard operations.
	CTextObject m_CopyTextObject;
};

/**
 * @param hwndParent - parent window handle.
 */
inline BOOL CExpressModeDlg::CreateItem(HWND hwndParent)
{
	return (Create(hwndParent) != NULL);
}

/**
 * @param strTitle - page title buffer.
 */
inline void CExpressModeDlg::GetItemTitle(CString& strTitle)
{
	strTitle.LoadString(IDS_EXPRESSMODE);
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
inline void CExpressModeDlg::OnCloseCmd(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	GetParent().PostMessage(WM_CLOSE);
}

inline void CExpressModeDlg::DoPaint(CDCHandle /*dc*/)
{
}

/**
 * @param pXMLElementParent - parent XML node.
 * @param pszExpression - XPath expression.
 * @param pXMLNodeSelection - selected XML node.
 * @return true if selection is not empty.
 */
inline bool CExpressModeDlg::SelectXMLNode(const CComPtr<IXMLDOMElement>& pXMLElementParent, LPCOLESTR pszExpression, CComPtr<IXMLDOMNode>& pXMLNodeSelection)
{
	return SelectXMLNode(CComQIPtr<IXMLDOMNode>(pXMLElementParent), pszExpression, pXMLNodeSelection);
}

/**
 * @param pXMLElementParent - parent XML node.
 * @param pszExpression - XPath expression.
 * @param pXMLNodeListSelection - selected XML nodes.
 * @return true if selection is not empty.
 */
inline bool CExpressModeDlg::SelectXMLNodes(const CComPtr<IXMLDOMElement>& pXMLElementParent, LPCOLESTR pszExpression, CComPtr<IXMLDOMNodeList>& pXMLNodeListSelection)
{
	return SelectXMLNodes(CComQIPtr<IXMLDOMNode>(pXMLElementParent), pszExpression, pXMLNodeListSelection);
}

/**
 * @param pXMLElementParent - parent XML node.
 * @param pszExpression - XPath expression.
 * @param strNodeText - buffer that receives node text.
 * @param pszDefaultText - default node text used if node is unavailable.
 * @return true if selected node is present.
 */
inline bool CExpressModeDlg::GetXMLNodeText(const CComPtr<IXMLDOMElement>& pXMLElementParent, LPCOLESTR pszExpression, CString& strNodeText, PCTSTR pszDefaultText)
{
	return GetXMLNodeText(CComQIPtr<IXMLDOMNode>(pXMLElementParent), pszExpression, strNodeText, pszDefaultText);
}
