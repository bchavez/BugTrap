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

#include "StdAfx.h"
#include "MessageTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMessageTip::~CMessageTip()
{
	if (m_hWnd)
		DestroyWindow();
}

/**
 * @param hwndParent - parent window handle.
 * @return true if tool-tip window was normally created.
 */
bool CMessageTip::Create(HWND hwndParent)
{
	HWND hwndToolTip = CWindowImpl<CMessageTip, CToolTipCtrl>::Create(hwndParent, NULL, NULL,
																	  WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON,
																	  WS_EX_TOOLWINDOW | WS_EX_TOPMOST);
	if (hwndToolTip != NULL)
	{
		int nHorDlgUnit = LOWORD(GetDialogBaseUnits());
		SetMaxTipWidth(nHorDlgUnit * 50);
		TOOLINFO tinfo;
		ZeroMemory(&tinfo, sizeof(tinfo));
		tinfo.cbSize = sizeof(tinfo);
		if (AddTool(&tinfo) == FALSE)
			DestroyWindow();
		else
			return true;
	}
	return false;
}

/**
 * @param hwndCtrl - input control handle.
 * @param uMessageID - message resource identifier.
 */
void CMessageTip::ShowMessage(HWND hwndCtrl, UINT uMessageID)
{
	CWindow ctlWnd(hwndCtrl);
	ctlWnd.SetFocus();
	CRect rcWnd;
	ctlWnd.GetWindowRect(&rcWnd);
	ShowMessage(uMessageID, rcWnd.CenterPoint());
}

/**
 * @param hwndCtrl - input control handle.
 * @param pszMessage - message text.
 */
void CMessageTip::ShowMessage(HWND hwndCtrl, PCTSTR pszMessage)
{
	CWindow ctlWnd(hwndCtrl);
	ctlWnd.SetFocus();
	CRect rcWnd;
	ctlWnd.GetWindowRect(&rcWnd);
	ShowMessage(pszMessage, rcWnd.CenterPoint());
}

/**
 * @param uMessageID - message resource identifier.
 * @param ptStem - stem point.
 */
void CMessageTip::ShowMessage(UINT uMessageID, const POINT& ptStem)
{
	TOOLINFO tinfo;
	ZeroMemory(&tinfo, sizeof(tinfo));
	tinfo.cbSize = sizeof(tinfo);
	tinfo.hinst = _Module.GetResourceInstance();
	tinfo.uFlags = TTF_TRACK;
	tinfo.lpszText = (PTSTR)UIntToPtr(uMessageID);
	SetToolInfo(&tinfo);
	TrackPosition(ptStem.x, ptStem.y);
	TrackActivate(&tinfo, TRUE);
	SetTimer(HIDE_TIP_TIMER_ID, HIDE_TIP_TIMEOUT);
}

/**
 * @param pszMessage - message text.
 * @param ptStem - stem point.
 */
void CMessageTip::ShowMessage(PCTSTR pszMessage, const POINT& ptStem)
{
	TOOLINFO tinfo;
	ZeroMemory(&tinfo, sizeof(tinfo));
	tinfo.cbSize = sizeof(tinfo);
	tinfo.uFlags = TTF_TRACK;
	tinfo.lpszText = (PTSTR)pszMessage;
	SetToolInfo(&tinfo);
	TrackPosition(ptStem.x, ptStem.y);
	TrackActivate(&tinfo, TRUE);
	SetTimer(HIDE_TIP_TIMER_ID, HIDE_TIP_TIMEOUT);
}

/**
 * @param uTimerID - timer identifier.
 */
void CMessageTip::OnTimer(UINT uTimerID)
{
	if (uTimerID == HIDE_TIP_TIMER_ID)
	{
		KillTimer(HIDE_TIP_TIMER_ID);
		HideMessage();
	}
	else
		SetMsgHandled(FALSE);
}

void CMessageTip::HideMessage()
{
	TOOLINFO tinfo;
	ZeroMemory(&tinfo, sizeof(tinfo));
	tinfo.cbSize = sizeof(tinfo);
	TrackActivate(&tinfo, FALSE);
}

namespace MsgTip
{
	/// Global information message.
	static CMessageTip g_MessageTip;

	/**
	 * Initialize global message tip object.
	 * @return true if message tip has been initialized.
	 */
	inline static BOOL CreateMessage()
	{
		return (g_MessageTip.m_hWnd == NULL ? g_MessageTip.Create(NULL) : TRUE);
	}

	/**
	 * @param hwndCtrl - input control handle.
	 * @param uMessageID - message resource identifier.
	 */
	void ShowMessage(HWND hwndCtrl, UINT uMessageID)
	{
		if (CreateMessage())
			g_MessageTip.ShowMessage(hwndCtrl, uMessageID);
	}

	/**
	 * @param hwndCtrl - input control handle.
	 * @param pszMessage - message text.
	 */
	void ShowMessage(HWND hwndCtrl, PCTSTR pszMessage)
	{
		if (CreateMessage())
			g_MessageTip.ShowMessage(hwndCtrl, pszMessage);
	}

	/**
	 * @param uMessageID - message resource identifier.
	 * @param ptStem - stem point.
	 */
	void ShowMessage(UINT uMessageID, const POINT& ptStem)
	{
		if (CreateMessage())
			g_MessageTip.ShowMessage(uMessageID, ptStem);
	}

	/**
	 * @param pszMessage - message text.
	 * @param ptStem - stem point.
	 */
	void ShowMessage(PCTSTR pszMessage, const POINT& ptStem)
	{
		if (CreateMessage())
			g_MessageTip.ShowMessage(pszMessage, ptStem);
	}

	void HideMessage()
	{
		if (g_MessageTip.m_hWnd != NULL)
			g_MessageTip.HideMessage();
	}
}
