/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2001 Magomed Abdurakhmanov
 * maq@hotbox.ru, http://mickels.iwt.ru/
 *
 * No warranties are given. Use at your own risk.
 *
 * Description: Saving and restoring window appearance.
 * Updated by: Maksim Pyatkovskiy.
 * Note: Based on code developed by Magomed Abdurakhmanov.
 * Downloaded from: http://www.codeproject.com/wtl/regst.asp
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "stdafx.h"
#include "RegSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// CWindowSettings

static const TCHAR g_szWindowPlacementSuffix[] = _T("WindowPlacement");

CWindowSettings::CWindowSettings(void)
{
	ZeroMemory(&m_WindowPlacement, sizeof(m_WindowPlacement));
	m_WindowPlacement.length = sizeof(m_WindowPlacement);

	CRect rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
	rc.DeflateRect(100, 100);
	m_WindowPlacement.rcNormalPosition = rc;
	m_WindowPlacement.showCmd = SW_SHOWNORMAL;
}

bool CWindowSettings::Load(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hRootKey)
{
	CRegKey reg;
	DWORD dwError = reg.Open(hRootKey, pszRegKey, KEY_READ);

	if (dwError == ERROR_SUCCESS)
	{
		DWORD dwSize = sizeof(m_WindowPlacement);
		dwError = reg.QueryBinaryValue(CString(pszPrefix) + g_szWindowPlacementSuffix, &m_WindowPlacement, &dwSize);
		if (dwError == ERROR_SUCCESS && m_WindowPlacement.showCmd == SW_SHOWMINIMIZED)
			m_WindowPlacement.showCmd = SW_SHOWNORMAL;
	}

	return (dwError == ERROR_SUCCESS);
}

bool CWindowSettings::Save(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hRootKey) const
{
	CRegKey reg;
	DWORD dwError = reg.Create(hRootKey, pszRegKey);

	if (dwError == ERROR_SUCCESS)
	{
		dwError = reg.SetBinaryValue(CString(pszPrefix) + g_szWindowPlacementSuffix, &m_WindowPlacement, sizeof(m_WindowPlacement));
	}

	return (dwError == ERROR_SUCCESS);
}

//////////////////////////////////////////////////////////////////////
// CReBarSettings

static const TCHAR g_szBarBandCountSuffix[] = _T("BandCount");
static const TCHAR g_szIDSuffix[]           = _T("ID");
static const TCHAR g_szCXSuffix[]           = _T("CX");
static const TCHAR g_szBreakLineSuffix[]    = _T("BreakLine");

bool CReBarSettings::Load(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hRootKey)
{
	delete[] m_pBands;
	m_pBands = NULL;
	m_cbBandCount = 0;

	CRegKey reg;
	DWORD dwError = reg.Open(hRootKey, pszRegKey, KEY_READ);
	if (dwError == ERROR_SUCCESS)
	{
		dwError = reg.QueryDWORDValue(CString(pszPrefix) + g_szBarBandCountSuffix, m_cbBandCount);
		if (dwError == ERROR_SUCCESS && m_cbBandCount > 0)
		{
			m_pBands = new CBandInfo[m_cbBandCount];
			for (DWORD i = 0; i < m_cbBandCount; ++i)
			{
				CString str;
				str.Format(_T("%s%i_"), pszPrefix, i);
				reg.QueryDWORDValue(str + g_szIDSuffix, m_pBands[i].m_ID);
				reg.QueryDWORDValue(str + g_szCXSuffix, m_pBands[i].m_cx);

				DWORD dw;
				reg.QueryDWORDValue(str + g_szBreakLineSuffix, dw);
				m_pBands[i].m_bBreakLine = dw != 0;
			}
		}
	}

	return (dwError == ERROR_SUCCESS);
}

bool CReBarSettings::Save(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hRootKey) const
{
	CRegKey reg;
	DWORD dwError = reg.Create(hRootKey, pszRegKey);
	if (dwError == ERROR_SUCCESS)
	{
		reg.SetDWORDValue(CString(pszPrefix) + g_szBarBandCountSuffix, m_cbBandCount);
		for (DWORD i = 0; i < m_cbBandCount; ++i)
		{
			CString str;
			str.Format(_T("%s%i_"),pszPrefix,i);
			reg.SetDWORDValue(str + g_szIDSuffix, m_pBands[i].m_ID);
			reg.SetDWORDValue(str + g_szCXSuffix, m_pBands[i].m_cx);

			DWORD dw = m_pBands[i].m_bBreakLine;
			reg.SetDWORDValue(str + g_szBreakLineSuffix, dw);
		}
	}

	return (dwError == ERROR_SUCCESS);
}

void CReBarSettings::GetFrom(CReBarCtrl& ReBar)
{
	ATLASSERT(ReBar.IsWindow());

	delete[] m_pBands;
	m_pBands = NULL;
	m_cbBandCount = ReBar.GetBandCount();
	if (m_cbBandCount > 0)
	{
		m_pBands = new CBandInfo[m_cbBandCount];
		for (DWORD i = 0; i < m_cbBandCount; ++i)
		{
			REBARBANDINFO rbi;
			rbi.cbSize = sizeof(rbi);
			rbi.fMask = RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
			ReBar.GetBandInfo(i, &rbi);
			m_pBands[i].m_ID = rbi.wID;
			m_pBands[i].m_cx = rbi.cx;
			m_pBands[i].m_bBreakLine = (rbi.fStyle & RBBS_BREAK) != 0;
		}
	}
}

void CReBarSettings::ApplyTo(CReBarCtrl& ReBar) const
{
	ATLASSERT(ReBar.IsWindow());
	for(DWORD i = 0; i < m_cbBandCount; ++i)
	{
		ReBar.MoveBand(ReBar.IdToIndex(m_pBands[i].m_ID), i);

		REBARBANDINFO rbi;
		rbi.cbSize = sizeof(rbi);
		rbi.fMask = RBBIM_ID | RBBIM_SIZE | RBBIM_STYLE;
		ReBar.GetBandInfo(i, &rbi);

		rbi.cx = m_pBands[i].m_cx;
		if (m_pBands[i].m_bBreakLine)
			rbi.fStyle |= RBBS_BREAK;
		else
			rbi.fStyle &= ~RBBS_BREAK;

		ReBar.SetBandInfo(i, &rbi);
	}
}

//////////////////////////////////////////////////////////////////////
// CSplitterSettings

static const TCHAR g_szSplitterPosSuffix[] = _T("SplitterPos");

bool CSplitterSettings::Load(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hRootKey)
{
	CRegKey reg;
	DWORD dwError = reg.Open(hRootKey, pszRegKey, KEY_READ);
	if (dwError == ERROR_SUCCESS)
	{
		reg.QueryDWORDValue(CString(pszPrefix) + g_szSplitterPosSuffix, m_dwPos);
	}
	return (dwError == ERROR_SUCCESS);
}

bool CSplitterSettings::Save(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hRootKey) const
{
	CRegKey reg;
	DWORD dwError = reg.Create(hRootKey, pszRegKey);
	if (dwError == ERROR_SUCCESS)
	{
		reg.SetDWORDValue(CString(pszPrefix) + g_szSplitterPosSuffix, m_dwPos);
	}
	return (dwError == ERROR_SUCCESS);
}
