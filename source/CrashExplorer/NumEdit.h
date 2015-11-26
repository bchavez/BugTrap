/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Numeric editor.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#define UM_GETNUMFORMAT (WM_APP + 1)
#define UM_SETNUMFORMAT (WM_APP + 2)

/// Numeric editor.
class CNumEdit : public CWindowImpl<CNumEdit, CEdit>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, _T("EDIT"))

	BEGIN_MSG_MAP_EX(CNumEdit)
		MSG_WM_CREATE(OnCreate)
		MESSAGE_HANDLER_EX(UM_GETNUMFORMAT, OnGetNumFormat)
		MESSAGE_HANDLER_EX(UM_SETNUMFORMAT, OnSetNumFormat)
		MSG_WM_CHAR(OnChar)
		MSG_WM_PASTE(OnPaste)
		COMMAND_ID_HANDLER_EX(ID_RADIX_HEX, OnRadixHex)
		COMMAND_ID_HANDLER_EX(ID_RADIX_DEC, OnRadixDec)
	END_MSG_MAP()

	/// Numeric format.
	enum NUM_FORMAT
	{
		NF_DEC = 10,
		NF_HEX = 16
	};

	/// Object constructor.
	CNumEdit();
	/// Attach window handle.
	void Attach(HWND hwndEdit);
	/// Subclass existing window.
	void SubclassWindow(HWND hwndEdit);
	/// Get attached radix menu.
	HWND GetRadixMenu() const;
	/// Attach radix menu.
	void SetRadixMenu(HWND hwndRadixMenu);
	/// Get numeric format.
	NUM_FORMAT GetNumFormat() const;
	/// Set numeric format.
	void SetNumFormat(NUM_FORMAT eNumFormat);
	/// Get numeric value.
	void GetValue(INT32& iValue);
	/// Get numeric value.
	void GetValue(UINT32& uValue);
	/// Get numeric value.
	void GetValue(INT64& iValue);
	/// Get numeric value.
	void GetValue(UINT64& uValue);
	/// Set numeric value.
	void SetValue(INT32 iValue);
	/// Set numeric value.
	void SetValue(UINT32 uValue);
	/// Set numeric value.
	void SetValue(INT64 iValue);
	/// Set numeric value.
	void SetValue(UINT64 uValue);

protected:
	/// WM_CREATE event handler.
	LRESULT OnCreate(LPCREATESTRUCT pCreateStruct);
	/// ID_RADIX_HEX command handler.
	void OnRadixHex(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// ID_RADIX_DEC command handler.
	void OnRadixDec(UINT uNotifyCode, int nID, HWND hWndCtl);
	/// WM_CHAR event handler.
	void OnChar(TCHAR chValue, UINT uRepCnt, UINT uFlags);
	/// WM_PASTE event handler.
	void OnPaste();
	/// Update radix title.
	void UpdateRadixTitle();
	/// Get numeric format.
	LRESULT OnGetNumFormat(UINT uMsg, WPARAM wParam, LPARAM lParam);
	/// Get numeric format.
	LRESULT OnSetNumFormat(UINT uMsg, WPARAM wParam, LPARAM lParam);
	/// Set value.
	void SetValue(PTSTR pszValue, int nValueSize);

	/// Numeric format.
	NUM_FORMAT m_eNumFormat;
	/// Attached window handle.
	HWND m_hwndRadixMenu;
};

inline CNumEdit::CNumEdit()
{
	m_hwndRadixMenu = NULL;
	m_eNumFormat = NF_DEC;
}

/**
 * @return edit window handle.
 */
inline HWND CNumEdit::GetRadixMenu() const
{
	return m_hwndRadixMenu;
}

/**
 * @param hwndRadixMenu - radix menu.
 */
inline void CNumEdit::SetRadixMenu(HWND hwndRadixMenu)
{
	m_hwndRadixMenu = hwndRadixMenu;
}

/**
 * @return numeric format.
 */
inline CNumEdit::NUM_FORMAT CNumEdit::GetNumFormat() const
{
	return m_eNumFormat;
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
inline void CNumEdit::OnRadixHex(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	SetNumFormat(NF_HEX);
}

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
inline void CNumEdit::OnRadixDec(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	SetNumFormat(NF_DEC);
}

/**
 * @param uMsg - message identifier.
 * @param wParam - 1st message parameter.
 * @param lParam - 2nd message parameter.
 * @return message result.
 */
inline LRESULT CNumEdit::OnGetNumFormat(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return GetNumFormat();
}

/**
 * @param uMsg - message identifier.
 * @param wParam - 1st message parameter.
 * @param lParam - 2nd message parameter.
 * @return message result.
 */
inline LRESULT CNumEdit::OnSetNumFormat(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/)
{
	SetNumFormat((NUM_FORMAT)wParam);
	return 0;
}
