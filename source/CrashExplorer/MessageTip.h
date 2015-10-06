/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Floating information message.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

/// Floating information message.
class CMessageTip : public CWindowImpl<CMessageTip, CToolTipCtrl>
{
public:
	/// Destroy the object.
	~CMessageTip();
	/// Create tooltip object.
	bool Create(HWND hwndParent);
	/// Show information message on the screen.
	void ShowMessage(HWND hwndCtrl, UINT uMessageID);
	/// Show information message on the screen.
	void ShowMessage(HWND hwndCtrl, PCTSTR pszMessage);
	/// Show information message on the screen.
	void ShowMessage(UINT uMessageID, const POINT& ptStem);
	/// Show information message on the screen.
	void ShowMessage(PCTSTR pszMessage, const POINT& ptStem);
	/// Hide message.
	void HideMessage();

	DECLARE_WND_SUPERCLASS(NULL, TOOLTIPS_CLASS)

	BEGIN_MSG_MAP_EX(CMessageTip)
		MSG_WM_TIMER(OnTimer)
	END_MSG_MAP()

protected:
	/// WM_TIMER event handler.
	void OnTimer(UINT uTimerID);

	/// Timer identifier.
	enum { HIDE_TIP_TIMER_ID = 0x12345, HIDE_TIP_TIMEOUT = 5000 };
};

namespace MsgTip
{
	/// Show information message on the screen.
	void ShowMessage(HWND hwndCtrl, UINT uMessageID);
	/// Show information message on the screen.
	void ShowMessage(HWND hwndCtrl, PCTSTR pszMessage);
	/// Show information message on the screen.
	void ShowMessage(UINT uMessageID, const POINT& ptStem);
	/// Show information message on the screen.
	void ShowMessage(PCTSTR pszMessage, const POINT& ptStem);
	/// Hide message.
	void HideMessage();
}
