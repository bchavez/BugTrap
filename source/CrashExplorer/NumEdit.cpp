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

#include "stdafx.h"
#include "resource.h"
#include "NumEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @param eNumFormat - numeric format.
 */
void CNumEdit::SetNumFormat(NUM_FORMAT eNumFormat)
{
	if (m_eNumFormat == eNumFormat)
		return;
	if (m_hWnd == NULL)
	{
		m_eNumFormat = eNumFormat;
		return;
	}
	TCHAR szValue[64];
	GetWindowText(szValue, countof(szValue));
	if (*szValue)
	{
		switch (m_eNumFormat)
		{
		case NF_HEX:
			{
				UINT64 uValue = _tcstoui64(szValue, NULL, 16);
				_ui64tot_s(uValue, szValue, countof(szValue), 10);
			}
			break;
		case NF_DEC:
			{
				bool bNegative = false;
				for (int i = 0; ; ++i)
				{
					if (szValue[i] != _T(' '))
					{
						if (szValue[i] == _T('-'))
							bNegative = true;
						break;
					}
				}
				if (bNegative)
				{
					INT64 iValue = _tcstoi64(szValue, NULL, 10);
					_i64tot_s(iValue, szValue, countof(szValue), 16);
				}
				else
				{
					UINT64 uValue = _tcstoui64(szValue, NULL, 10);
					_ui64tot_s(uValue, szValue, countof(szValue), 16);
				}
			}
			break;
		default:
			*szValue = _T('\0');
		}
		SetValue(szValue, countof(szValue));
	}
	m_eNumFormat = eNumFormat;
	UpdateRadixTitle();
}

void CNumEdit::UpdateRadixTitle()
{
	ATLASSERT(m_hwndRadixMenu != NULL);
	switch (m_eNumFormat)
	{
	case NF_HEX:
		::SetWindowText(m_hwndRadixMenu, _T("h"));
		break;
	case NF_DEC:
		::SetWindowText(m_hwndRadixMenu, _T("d"));
		break;
	}
}

/**
 * @param iValue - numeric value.
 */
void CNumEdit::GetValue(INT32& iValue)
{
	TCHAR szValue[64];
	GetWindowText(szValue, countof(szValue));
	switch (m_eNumFormat)
	{
	case NF_HEX:
		iValue = _tcstol(szValue, NULL, 16);
		break;
	case NF_DEC:
		iValue = _tcstol(szValue, NULL, 10);
		break;
	default:
		iValue = 0;
	}
}

/**
 * @param uValue - numeric value.
 */
void CNumEdit::GetValue(UINT32& uValue)
{
	TCHAR szValue[64];
	GetWindowText(szValue, countof(szValue));
	switch (m_eNumFormat)
	{
	case NF_HEX:
		uValue = _tcstoul(szValue, NULL, 16);
		break;
	case NF_DEC:
		uValue = _tcstoul(szValue, NULL, 10);
		break;
	default:
		uValue = 0;
	}
}

/**
 * @param iValue - numeric value.
 */
void CNumEdit::GetValue(INT64& iValue)
{
	TCHAR szValue[64];
	GetWindowText(szValue, countof(szValue));
	switch (m_eNumFormat)
	{
	case NF_HEX:
		iValue = _tcstoi64(szValue, NULL, 16);
		break;
	case NF_DEC:
		iValue = _tcstoi64(szValue, NULL, 10);
		break;
	default:
		iValue = 0;
	}
}

/**
 * @param uValue - numeric value.
 */
void CNumEdit::GetValue(UINT64& uValue)
{
	TCHAR szValue[64];
	GetWindowText(szValue, countof(szValue));
	switch (m_eNumFormat)
	{
	case NF_HEX:
		uValue = _tcstoui64(szValue, NULL, 16);
		break;
	case NF_DEC:
		uValue = _tcstoui64(szValue, NULL, 10);
		break;
	default:
		uValue = 0;
	}
}

/**
 * @param iValue - new field value.
 */
void CNumEdit::SetValue(INT32 iValue)
{
	TCHAR szValue[64];
	_ltot_s(iValue, szValue, countof(szValue), m_eNumFormat);
	SetValue(szValue, countof(szValue));
}

/**
 * @param uValue - new field value.
 */
void CNumEdit::SetValue(UINT32 uValue)
{
	TCHAR szValue[64];
	_ultot_s(uValue, szValue, countof(szValue), m_eNumFormat);
	SetValue(szValue, countof(szValue));
}

/**
 * @param iValue - new field value.
 */
void CNumEdit::SetValue(INT64 iValue)
{
	TCHAR szValue[64];
	_i64tot_s(iValue, szValue, countof(szValue), m_eNumFormat);
	SetValue(szValue, countof(szValue));
}

/**
 * @param uValue - new field value.
 */
void CNumEdit::SetValue(UINT64 uValue)
{
	TCHAR szValue[64];
	_ui64tot_s(uValue, szValue, countof(szValue), m_eNumFormat);
	SetValue(szValue, countof(szValue));
}

/**
 * @param pszValue - new field value.
 * @param nValueSize - buffer size.
 */
void CNumEdit::SetValue(PTSTR pszValue, int nValueSize)
{
	_tcsupr_s(pszValue, nValueSize);
	SetWindowText(pszValue);
}

/**
 * @param pCreateStruct - points to a @a CREATESTRUCT structure that contains information about the window being created.
 * @return 0 to continue the creation of the window. If the application returns –1, the window will be destroyed.
 */
LRESULT CNumEdit::OnCreate(LPCREATESTRUCT pCreateStruct)
{
	SetMsgHandled(FALSE);
	UpdateRadixTitle();
	pCreateStruct->style |= ES_UPPERCASE;
	return 0;
}

/**
 * @param hwndEdit - window handle.
 */
void CNumEdit::Attach(HWND hwndEdit)
{
	CWindowImpl::Attach(hwndEdit);
	ModifyStyle(0, ES_UPPERCASE);
	UpdateRadixTitle();
}

/**
 * @param hwndEdit - window handle.
 */
void CNumEdit::SubclassWindow(HWND hwndEdit)
{
	CWindowImpl::SubclassWindow(hwndEdit);
	ModifyStyle(0, ES_UPPERCASE);
	UpdateRadixTitle();
}

/**
 * @param chValue - contains the character code value of the key.
 * @param uRepCnt - contains the repeat count, the number of times the keystroke is repeated when user holds down the key.
 * @param uFlags - contains the scan code, key-transition code, previous key state, and context code.
 */
void CNumEdit::OnChar(TCHAR chValue, UINT /*uRepCnt*/, UINT /*uFlags*/)
{
	if ((m_eNumFormat == NF_HEX && _istxdigit(chValue)) ||
		(m_eNumFormat == NF_DEC && _istdigit(chValue)) ||
		chValue == VK_BACK)
	{
		const _ATL_MSG* pMsg = GetCurrentMessage();
		DefWindowProc(WM_CHAR, toupper(chValue), pMsg->lParam);
	}
}

void CNumEdit::OnPaste()
{
	LockWindowUpdate(TRUE);
	DefWindowProc();
	int nStart, nEnd;
	GetSel(nStart, nEnd);
	TCHAR szValue[64];
	GetWindowText(szValue, countof(szValue));
	int i = 0, j = 0;
	for (;;)
	{
		TCHAR chValue = szValue[i++];
		if (chValue == _T('\0'))
		{
			szValue[j] = _T('\0');
			break;
		}
		switch (m_eNumFormat)
		{
		case NF_HEX:
			if (_istxdigit(chValue))
				szValue[j++] = (TCHAR)_totupper(chValue);
			break;
		case NF_DEC:
			if (_istdigit(chValue))
				szValue[j++] = chValue;
			break;
		}
	}
	if (nStart > j)
		nStart = j;
	if (nEnd > j)
		nEnd = j;
	SetWindowText(szValue);
	SetSel(nStart, nEnd);
	LockWindowUpdate(FALSE);
}
