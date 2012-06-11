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

#include "StdAfx.h"
#include "AnimProgressBar.h"
#include "ThemeXP.h"

#define USE_MEM_DC

#define TIMER_ID             101
#define DEFAULT_TIMEOUT        5
#define DEFAULT_NUM_CHUNKS     5

CAnimProgressBar::CAnimProgressBar(void)
{
	m_bActive = FALSE;
	m_dwTimeout = DEFAULT_TIMEOUT;
	m_dwNumChunks = DEFAULT_NUM_CHUNKS;
	m_dwPosition = 0;
	m_hwnd = NULL;
	m_pfnAnimProgressBarWndProc = NULL;
}

/**
 * @param hwnd - progress bar window handle.
 */
void CAnimProgressBar::Attach(HWND hwnd)
{
	_ASSERTE(hwnd != NULL);
	_ASSERTE(m_hwnd == NULL);
	m_hwnd = hwnd;
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
	m_pfnAnimProgressBarWndProc = SubclassWindow(hwnd, AnimProgressBarWndProc);
	InvalidateRect(hwnd, NULL, TRUE);
}

void CAnimProgressBar::Detach(void)
{
	if (m_pfnAnimProgressBarWndProc)
	{
		Stop();
		SubclassWindow(m_hwnd, m_pfnAnimProgressBarWndProc);
		SetWindowLongPtr(m_hwnd, GWLP_USERDATA, NULL);
		InvalidateRect(m_hwnd, NULL, TRUE);
		m_pfnAnimProgressBarWndProc = NULL;
		m_hwnd = NULL;
	}
}

void CAnimProgressBar::Play()
{
	if (m_bActive)
		return;
	m_bActive = TRUE;
	SetTimer(m_hwnd, TIMER_ID, m_dwTimeout, NULL);
}

void CAnimProgressBar::Stop()
{
	if (! m_bActive)
		return;
	m_bActive = FALSE;
	KillTimer(m_hwnd, TIMER_ID);
}

/**
 * @param dwTimeout - new update timeout.
 */
void CAnimProgressBar::SetTimeout(DWORD dwTimeout)
{
	m_dwTimeout = dwTimeout;
	if (m_bActive)
		SetTimer(m_hwnd, TIMER_ID, dwTimeout, NULL);
}

/**
 * @param hdc - drawing context.
 * @param prcPaint - the rectangle where the painting is requested.
 */
void CAnimProgressBar::DrawAnimProgressBar(HDC hdc, const RECT* prcPaint) const
{
	if (prcPaint == NULL)
	{
		RECT rcClient;
		GetClientRect(m_hwnd, &rcClient);
		prcPaint = &rcClient;
	}
	if (IsRectEmpty(prcPaint))
		return;

#ifdef USE_MEM_DC
	int nClientWidth = prcPaint->right - prcPaint->left;
	int nClientHeight = prcPaint->bottom - prcPaint->top;
	HBITMAP hbmpMem;
	hbmpMem = CreateCompatibleBitmap(hdc, nClientWidth, nClientHeight);
	if (hbmpMem == NULL)
		return;
	HDC hdcMem;
	hdcMem = CreateCompatibleDC(hdc);
	if (hdcMem == NULL)
	{
		DeleteBitmap(hbmpMem);
		return;
	}
	SetViewportOrgEx(hdcMem, -prcPaint->left, -prcPaint->top, NULL);
	HBITMAP hbmpSafe = SelectBitmap(hdcMem, hbmpMem);
#else
	// CS_PARENTDC sets the clipping rectangle of the child window to that of the parent window
	// so that the child can draw on the parent. Progress bar inherits this style from sub-classed
	// static control. This causes problems with unclipped output.
	HRGN hrgn = CreateRectRgnIndirect(prcPaint);
	SelectClipRgn(hdc, hrgn);
	DeleteRgn(hrgn);
	HDC hdcMem = hdc;
#endif

	CThemeXP themeXP;
	BOOL bAppThemed = themeXP.IsAppThemed();
	if (bAppThemed)
	{
		if (! themeXP.OpenTheme(m_hwnd, L"PROGRESS"))
			bAppThemed = FALSE;
	}

	RECT rcClient;
	GetClientRect(m_hwnd, &rcClient);
	RECT rcClip;
	GetClipBox(hdc, &rcClip);

	if (bAppThemed)
	{
		themeXP.DrawThemeBackground(hdcMem, PP_BAR, 0, &rcClient, &rcClip);
		RECT rcContent;
		themeXP.GetThemeBackgroundContentRect(hdcMem, PP_BAR, 0, &rcClient, &rcContent);
		SIZE szChunk;
		themeXP.GetThemePartSize(hdcMem, PP_CHUNK, 0, &rcContent, TS_TRUE, &szChunk);
		if (szChunk.cx <= 1)
			szChunk.cx = szChunk.cy * 3 / 4;
		DWORD dwChunksWidth = m_dwNumChunks * szChunk.cx;
		DWORD dwPosition = m_dwPosition % (rcContent.right - rcContent.left + dwChunksWidth);
		int nPosition = (int)dwPosition - (int)dwChunksWidth;
		RECT rcChunk;
		rcChunk.left = rcContent.left + nPosition;
		rcChunk.top = rcContent.top;
		rcChunk.bottom = rcContent.bottom;
		for (DWORD dwChunkNum = 0; dwChunkNum < m_dwNumChunks; ++dwChunkNum)
		{
			rcChunk.right = rcChunk.left + szChunk.cx;
			if (rcClip.left <= rcChunk.right && rcChunk.left < rcClip.right)
				themeXP.DrawThemeBackground(hdcMem, PP_CHUNK, 0, &rcChunk, &rcClip);
			rcChunk.left = rcChunk.right;
		}
	}
	else
	{
		HBRUSH brushBtnFace = GetSysColorBrush(COLOR_BTNFACE);
		FillRect(hdcMem, &rcClip, brushBtnFace);
		HBRUSH brushHighlight = GetSysColorBrush(COLOR_HIGHLIGHT);
		int nCXEdge = GetSystemMetrics(SM_CXEDGE);
		int nCYEdge = GetSystemMetrics(SM_CYEDGE);
		SIZE szChunk;
		szChunk.cy = rcClient.bottom - rcClient.top;
		szChunk.cx = szChunk.cy * 3 / 4;
		DWORD dwChunksWidth = m_dwNumChunks * szChunk.cx;
		DWORD dwPosition = m_dwPosition % (rcClient.right - rcClient.left + dwChunksWidth);
		int nPosition = (int)dwPosition - (int)dwChunksWidth;
		RECT rcChunk;
		rcChunk.left = rcClient.left + nPosition + nCXEdge;
		rcChunk.top = rcClient.top + nCYEdge;
		rcChunk.bottom = rcClient.bottom - nCYEdge;
		szChunk.cx -= 2 * nCXEdge;
		for (DWORD dwChunkNum = 0; dwChunkNum < m_dwNumChunks; ++dwChunkNum)
		{
			rcChunk.right = rcChunk.left + szChunk.cx;
			if (rcClip.left <= rcChunk.right && rcChunk.left < rcClip.right)
				FillRect(hdcMem, &rcChunk, brushHighlight);
			rcChunk.left = rcChunk.right + nCXEdge;
		}
	}

#ifdef USE_MEM_DC
	BitBlt(hdc, prcPaint->left, prcPaint->top, nClientWidth, nClientHeight, hdcMem, prcPaint->left, prcPaint->top, SRCCOPY);
	SelectBitmap(hdcMem, hbmpSafe);
	DeleteDC(hdcMem);
	DeleteBitmap(hbmpMem);
#endif
}

/**
 * @param hwnd - window handle.
 * @param uMsg - message identifier.
 * @param wParam - first message parameter.
 * @param lParam - second message parameter.
 */
LRESULT CALLBACK CAnimProgressBar::AnimProgressBarWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	CAnimProgressBar* _this  = (CAnimProgressBar*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	_ASSERTE(_this != NULL);
	switch(uMsg)
	{
	case WM_ERASEBKGND:
		return TRUE;
	case WM_PAINT:
		hdc = (HDC)wParam;
		if (! hdc)
		{
			hdc = BeginPaint(hwnd, &ps);
			if (hdc)
			{
				_this->DrawAnimProgressBar(hdc, &ps.rcPaint);
				EndPaint(hwnd, &ps);
			}
		}
		else
			_this->DrawAnimProgressBar(hdc, NULL);
		return 0;
	case WM_PRINTCLIENT:
		hdc = (HDC)wParam;
		_this->DrawAnimProgressBar(hdc, NULL);
		return 0;
	case WM_TIMER:
		if (! _this->m_bActive)
			return 0;
		++_this->m_dwPosition;
		RedrawWindow(_this->m_hwnd, NULL, NULL, RDW_INVALIDATE);
		return 0;
	case WM_DESTROY:
		_this->Stop();
		return 0;
	case WM_NCHITTEST:
		return HTTRANSPARENT;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
