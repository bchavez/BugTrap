/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Radix menu.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

/// Radix menu.
class CRadixMenu : public CWindowImpl<CRadixMenu, CButton>
{
public:
	/// Object constructor.
	CRadixMenu();
	/// Get attached edit window.
	HWND GetEditWindow() const;
	/// Attach edit window.
	void SetEditWindow(HWND hwndEdit);

	DECLARE_WND_SUPERCLASS(NULL, _T("BUTTON"))

	BEGIN_MSG_MAP_EX(CRadixMenu)
		REFLECTED_COMMAND_CODE_HANDLER_EX(BN_CLICKED, OnClick)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

protected:
	/// BN_CLICKED event handled.
	void OnClick(UINT uNotifyCode, int nID, HWND hWndCtl);

	/// Attached window handle.
	HWND m_hwndEdit;
};

inline CRadixMenu::CRadixMenu()
{
	m_hwndEdit = NULL;
}

/**
 * @return edit window handle.
 */
inline HWND CRadixMenu::GetEditWindow() const
{
	return m_hwndEdit;
}

/**
 * @param hwndEdit - edit window handle.
 */
inline void CRadixMenu::SetEditWindow(HWND hwndEdit)
{
	m_hwndEdit = hwndEdit;
}
