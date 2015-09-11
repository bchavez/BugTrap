/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Animated progress bar control.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

class CAnimProgressBar
{
public:
	/// Initialize the object.
	CAnimProgressBar(void);
	/// Attach progress bar object to window handle.
	void Attach(HWND hwnd);
	/// Detach progress bar object to window handle.
	void Detach(void);
	/// Start animation.
	void Play();
	/// Stop animation.
	void Stop();
	/// Return true if animation is active.
	BOOL IsActive() const;
	/// Get update timeout.
	DWORD GetTimeout() const;
	/// Set update timeout.
	void SetTimeout(DWORD dwTimeout);
	/// Get number of visible chunks.
	DWORD GetNumChunks() const;
	/// Set number of visible chunks.
	void SetNumChunks(DWORD dwNumChunks);

private:
	/// Protect the class from being accidentally copied.
	CAnimProgressBar(const CAnimProgressBar& rAnimProgressBar);
	/// Protect the class from being accidentally copied.
	CAnimProgressBar& operator=(const CAnimProgressBar& rAnimProgressBar);
	/// Window procedure of progress bar window.
	static LRESULT CALLBACK AnimProgressBarWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	/// Draw window client area.
	void DrawAnimProgressBar(HDC hdc, const RECT* prcPaint) const;

	/// Window handle.
	HWND m_hwnd;
	/// True if animation is active.
	BOOL m_bActive;
	/// Update timeout.
	DWORD m_dwTimeout;
	/// Number of visible chunks.
	DWORD m_dwNumChunks;
	/// Current position.
	DWORD m_dwPosition;
	/// Old static window procedure.
	WNDPROC m_pfnAnimProgressBarWndProc;
};

/**
 * @return true if animation is active.
 */
inline BOOL CAnimProgressBar::IsActive() const
{
	return m_bActive;
}

/**
 * @return current update timeout.
 */
inline DWORD CAnimProgressBar::GetTimeout() const
{
	return m_dwTimeout;
}

/**
 * @return number of visible chunks.
 */
inline DWORD CAnimProgressBar::GetNumChunks() const
{
	return m_dwNumChunks;
}

/**
 * @param dwNumChunks - number of visible chunks.
 */
inline void CAnimProgressBar::SetNumChunks(DWORD dwNumChunks)
{
	m_dwNumChunks = dwNumChunks;
}
