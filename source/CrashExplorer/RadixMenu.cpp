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

#include "stdafx.h"
#include "resource.h"
#include "RadixMenu.h"
#include "NumEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @param uNotifyCode - notification code if the message is from a control. If the message is from an accelerator, this value is 1. If the message is from a menu, this value is zero.
 * @param nID - specifies the identifier of the menu item, control, or accelerator.
 * @param hWndCtl - handle to the control sending the message if the message is from a control. Otherwise, this parameter is NULL.
 */
void CRadixMenu::OnClick(UINT /*uNotifyCode*/, int /*nID*/, HWND /*hWndCtl*/)
{
	SetState(TRUE);
	CRect rect;
	GetWindowRect(&rect);
	CMenu menuRadixContext;
	if (! menuRadixContext.LoadMenu(IDR_RADIX_CONTEXT))
		return;
	CMenuHandle menuPopup = menuRadixContext.GetSubMenu(0);
	ATLASSERT(menuPopup != NULL);
	if (menuPopup == NULL)
		return;
	switch (::SendMessage(m_hwndEdit, UM_GETNUMFORMAT, 0, 0))
	{
	case CNumEdit::NF_HEX:
		menuPopup.CheckMenuItem(ID_RADIX_HEX, MF_BYCOMMAND | MF_CHECKED);
		break;
	case CNumEdit::NF_DEC:
		menuPopup.CheckMenuItem(ID_RADIX_DEC, MF_BYCOMMAND | MF_CHECKED);
		break;
	}
	menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, rect.left, rect.bottom, m_hwndEdit);
	SetState(FALSE);
}
