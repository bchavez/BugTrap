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

#pragma once

class CWindowSettings
{
private:
	WINDOWPLACEMENT m_WindowPlacement;

public:
	CWindowSettings(void);
	void GetFrom(CWindow& wnd);
	void ApplyTo(CWindow& wnd) const;

	bool Load(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hkRootKey = HKEY_CURRENT_USER);
	bool Save(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hkRootKey = HKEY_CURRENT_USER) const;
};

inline void CWindowSettings::GetFrom(CWindow& wnd)
{
	ATLASSERT(wnd.IsWindow());
	wnd.GetWindowPlacement(&m_WindowPlacement);
}

inline void CWindowSettings::ApplyTo(CWindow& wnd) const
{
	ATLASSERT(wnd.IsWindow());
	wnd.SetWindowPlacement(&m_WindowPlacement);
}

class CReBarSettings
{
private:
	struct CBandInfo
	{
		DWORD m_ID;
		DWORD m_cx;
		bool m_bBreakLine;
	}
	*m_pBands;

	DWORD m_cbBandCount;

	CReBarSettings(const CReBarSettings& rReBarSettings);
	CReBarSettings& operator=(const CReBarSettings& rReBarSettings);

public:
	CReBarSettings(void);
	~CReBarSettings(void);

	void GetFrom(CReBarCtrl& ReBar);
	void ApplyTo(CReBarCtrl& ReBar) const;

	bool Load(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hkRootKey = HKEY_CURRENT_USER);
	bool Save(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hkRootKey = HKEY_CURRENT_USER) const;
};

inline CReBarSettings::~CReBarSettings(void)
{
	delete[] m_pBands;
}

inline CReBarSettings::CReBarSettings(void)
{
	m_pBands = NULL;
	m_cbBandCount = 0;
}

class CSplitterSettings
{
private:
	DWORD m_dwPos;

public:
	template <class T> void GetFrom(const T& splt);
	template <class T> void ApplyTo(T& splt) const;

	bool Load(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hRootKey = HKEY_CURRENT_USER);
	bool Save(LPCTSTR pszRegKey, LPCTSTR pszPrefix, HKEY hRootKey = HKEY_CURRENT_USER) const;
};

template <class T> inline void CSplitterSettings::GetFrom(const T& splt)
{
	m_dwPos = splt.GetSplitterPos();
}

template <class T> inline void CSplitterSettings::ApplyTo(T& splt) const
{
	splt.SetSplitterPos(m_dwPos);
}
