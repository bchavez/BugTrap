/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Layout manager class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#define UWM_INIT_SHEET (WM_APP + 1)
#define UWM_INIT_PAGE  (WM_APP + 2)

/// Left-side alignment.
const int ALIGN_LEFT    = 0;
/// Top-side alignment.
const int ALIGN_TOP     = 0;
/// Center-side alignment.
const int ALIGN_CENTER  = 1;
/// Right-side alignment.
const int ALIGN_RIGHT   = 2;
/// Bottom-side alignment.
const int ALIGN_BOTTOM  = 2;
/// Alignment range.
const int ALIGN_RANGE   = 2;

/// Private layout info block.
struct CONTROL_LAYOUT_INFO
{
	/// Initialize the object.
	CONTROL_LAYOUT_INFO();
	/// Initialize the object.
	CONTROL_LAYOUT_INFO(int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2);

	/// Left coordinate ratio.
	int m_nRatioX1;
	/// Top coordinate ratio.
	int m_nRatioY1;
	/// Right coordinate ratio.
	int m_nRatioX2;
	/// Bottom coordinate ratio.
	int m_nRatioY2;
	/// Original control coordinates.
	mutable RECT m_rcOriginal;
};

inline CONTROL_LAYOUT_INFO::CONTROL_LAYOUT_INFO()
{
}

/**
 * @param nRatioX1 - left coordinate ratio.
 * @param nRatioY1 - top coordinate ratio.
 * @param nRatioX2 - right coordinate ratio.
 * @param nRatioY2 - bottom coordinate ratio.
 */
inline CONTROL_LAYOUT_INFO::CONTROL_LAYOUT_INFO(int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2)
{
	m_nRatioX1 = nRatioX1;
	m_nRatioY1 = nRatioY1;
	m_nRatioX2 = nRatioX2;
	m_nRatioY2 = nRatioY2;
}

/// Layout info block used for the initialization.
struct LAYOUT_INFO : public CONTROL_LAYOUT_INFO
{
	/// Initialize the object.
	LAYOUT_INFO();
	/// Initialize the object.
	LAYOUT_INFO(HWND hwndCtl, int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2);
	/// Initialize the object.
	LAYOUT_INFO(int nCtlID, int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2);

	/// True if control handle is used instead of control ID.
	bool m_bCtlIDValid;
	/// Keeps either control ID or control handle.
	union
	{
		/// Control window handle.
		HWND m_hwndCtl;
		/// Control ID.
		int m_nCtlID;
	};
};

/// Initialize the object.
inline LAYOUT_INFO::LAYOUT_INFO() : CONTROL_LAYOUT_INFO()
{
}

/**
 * @param hwndCtl - control handle.
 * @param nRatioX1 - left coordinate ratio.
 * @param nRatioY1 - top coordinate ratio.
 * @param nRatioX2 - right coordinate ratio.
 * @param nRatioY2 - bottom coordinate ratio.
 */
inline LAYOUT_INFO::LAYOUT_INFO(HWND hwndCtl, int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2) : CONTROL_LAYOUT_INFO(nRatioX1, nRatioY1, nRatioX2, nRatioY2)
{
	m_bCtlIDValid = false;
	m_hwndCtl = hwndCtl;
}

/**
 * @param nCtlID - control ID.
 * @param nRatioX1 - left coordinate ratio.
 * @param nRatioY1 - top coordinate ratio.
 * @param nRatioX2 - right coordinate ratio.
 * @param nRatioY2 - bottom coordinate ratio.
 */
inline LAYOUT_INFO::LAYOUT_INFO(int nCtlID, int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2) : CONTROL_LAYOUT_INFO(nRatioX1, nRatioY1, nRatioX2, nRatioY2)
{
	m_bCtlIDValid = true;
	m_nCtlID = nCtlID;
}

/// Window layout manager class.
template <class T>
class CLayoutManager
{
protected:
	/// Minimal acceptable window size.
	POINT m_ptMinWindowSize;
	/// Minimal window client size.
	POINT m_ptMinClientSize;
	/// Size-box window handle.
	CWindow m_wndSizeBox;
	/// Array of control layout information.
	CAtlMap<HWND, CONTROL_LAYOUT_INFO> m_mapLayout;
	/// True if layout manager should not resize controls under the limit.
	bool m_bTrackMinSize;

	/// Protect the class from being accidentally copied.
	CLayoutManager(const CLayoutManager& rLayoutManager);
	/// Protect the class from being accidentally copied.
	const CLayoutManager& operator=(const CLayoutManager& rLayoutManager);

	/// WM_SIZE event handler.
	void OnSize(UINT uType, CSize size);
	/// WM_GETMINMAXINFO event handler.
	void OnGetMinMaxInfo(PMINMAXINFO pMinMaxInfo);
	/// Dynamically add control to the parent window layout.
	void AddControlToLayout(HWND hwndCtl, const CONTROL_LAYOUT_INFO& rLayoutInfo);
	/// Dynamically add control to the parent window layout.
	void AddControlToLayout(int nCtlID, const CONTROL_LAYOUT_INFO& rLayoutInfo);

public:
	/// Initialize the object.
	CLayoutManager();
	/// Initialize layout manager.
	void InitLayout(bool bAddSizeBox = true, bool bTrackMinSize = true);
	/// Dynamically add control to the parent window layout.
	void AddControlToLayout(HWND hwndCtl, int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2);
	/// Dynamically add control to the parent window layout.
	void AddControlToLayout(int nCtlID, int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2);
	/// Dynamically add control to the parent window layout.
	void AddControlToLayout(const LAYOUT_INFO& rLayoutInfo);
	/// Dynamically add controls group to the parent window layout.
	void AddControlsToLayout(const LAYOUT_INFO arrLayoutInfo[], int nNumLayouts);
	/// Dynamically delete control from the parent window layout.
	void DeleteControlFromLayout(HWND hwndCtl);
	/// Dynamically delete control from the parent window layout.
	void DeleteControlFromLayout(int nCtlID);
	/// Reset window layout by deleting all controls.
	void ResetLayout();
	/// Force layout update.
	void UpdateLayout();

	BEGIN_MSG_MAP_EX(CLayoutManager<T>)
		MSG_WM_SIZE(OnSize)
		MSG_WM_GETMINMAXINFO(OnGetMinMaxInfo)
	END_MSG_MAP()
};

template <class T>
inline CLayoutManager<T>::CLayoutManager()
{
	m_bTrackMinSize = false;
}

/**
 * @param pMinMaxInfo - pointer to structure that contains information about a window's maximized size and position and its minimum and maximum tracking size.
 */
template <class T>
void CLayoutManager<T>::OnGetMinMaxInfo(PMINMAXINFO pMinMaxInfo)
{
	if (m_bTrackMinSize)
		pMinMaxInfo->ptMinTrackSize = m_ptMinWindowSize;
	else
		SetMsgHandled(FALSE);
}

/**
 * @param bAddSizeBox - pass true if you want to show a gripper.
 * @param bTrackMinSize - pass true if you want to prevent layout manager from resizing controls under the limit.
 */
template <class T>
void CLayoutManager<T>::InitLayout(bool bAddSizeBox, bool bTrackMinSize)
{
	m_bTrackMinSize = bTrackMinSize;

	((T*)this)->ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	RECT rcWindow;
	((T*)this)->GetWindowRect(&rcWindow);
	m_ptMinWindowSize.x = rcWindow.right - rcWindow.left;
	m_ptMinWindowSize.y = rcWindow.bottom - rcWindow.top;

	RECT rcClient;
	((T*)this)->GetClientRect(&rcClient);
	m_ptMinClientSize.x = rcClient.right;
	m_ptMinClientSize.y = rcClient.bottom;

	if (bAddSizeBox)
		m_wndSizeBox.Create(WC_SCROLLBAR, ((T*)this)->m_hWnd, &rcClient, NULL, WS_CHILD | WS_VISIBLE | WS_GROUP | SBS_SIZEGRIP | SBS_SIZEBOXBOTTOMRIGHTALIGN | WS_CLIPSIBLINGS);
}

/**
 * @param hwndCtl - control handle.
 * @param nRatioX1 - left coordinate ratio.
 * @param nRatioY1 - top coordinate ratio.
 * @param nRatioX2 - right coordinate ratio.
 * @param nRatioY2 - bottom coordinate ratio.
 */
template <class T>
void CLayoutManager<T>::AddControlToLayout(HWND hwndCtl, int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2)
{
	CONTROL_LAYOUT_INFO linfo(nRatioX1, nRatioY1, nRatioX2, nRatioY2);
	CLayoutManager<T>::AddControlToLayout(hwndCtl, linfo);
}

/**
 * @param nCtlID - control ID.
 * @param nRatioX1 - left coordinate ratio.
 * @param nRatioY1 - top coordinate ratio.
 * @param nRatioX2 - right coordinate ratio.
 * @param nRatioY2 - bottom coordinate ratio.
 */
template <class T>
inline void CLayoutManager<T>::AddControlToLayout(int nCtlID, int nRatioX1, int nRatioY1, int nRatioX2, int nRatioY2)
{
	HWND hwndCtl = ((T*)this)->GetDlgItem(nCtlID);
	CLayoutManager<T>::AddControlToLayout(hwndCtl, nRatioX1, nRatioY1, nRatioX2, nRatioY2);
}

/**
 * @param rLayoutInfo - control layout info.
 */
template <class T>
void CLayoutManager<T>::AddControlToLayout(const LAYOUT_INFO& rLayoutInfo)
{
	if (rLayoutInfo.m_bCtlIDValid)
		CLayoutManager<T>::AddControlToLayout(rLayoutInfo.m_nCtlID, rLayoutInfo);
	else
		CLayoutManager<T>::AddControlToLayout(rLayoutInfo.m_hwndCtl, rLayoutInfo);
}

/**
 * @param arrLayoutInfo - array of control layouts.
 * @param nNumLayouts - number of control layouts in the array.
 */
template <class T>
void CLayoutManager<T>::AddControlsToLayout(const LAYOUT_INFO arrLayoutInfo[], int nNumLayouts)
{
	for (int nLayoutNum = 0; nLayoutNum < nNumLayouts; ++nLayoutNum)
		CLayoutManager<T>::AddControlToLayout(arrLayoutInfo[nLayoutNum]);
}

/**
 * @param hwndCtl - control handle.
 * @param rLayoutInfo - control layout info.
 */
template <class T>
void CLayoutManager<T>::AddControlToLayout(HWND hwndCtl, const CONTROL_LAYOUT_INFO& rLayoutInfo)
{
	ATLASSERT(hwndCtl != NULL);
	ATLASSERT(m_mapLayout.Lookup(hwndCtl) == NULL);
	::GetWindowRect(hwndCtl, &rLayoutInfo.m_rcOriginal);
	((T*)this)->ScreenToClient(&rLayoutInfo.m_rcOriginal);
	m_mapLayout.SetAt(hwndCtl, rLayoutInfo);
}

/**
 * @param nCtlID - control ID.
 * @param rLayoutInfo - control layout info.
 */
template <class T>
void CLayoutManager<T>::AddControlToLayout(int nCtlID, const CONTROL_LAYOUT_INFO& rLayoutInfo)
{
	HWND hwndCtl = ((T*)this)->GetDlgItem(nCtlID);
	CLayoutManager<T>::AddControlToLayout(hwndCtl, rLayoutInfo);
}

/**
 * @param hwndCtl - control handle.
 */
template <class T>
inline void CLayoutManager<T>::DeleteControlFromLayout(HWND hwndCtl)
{
	m_mapLayout.RemoveKey(hwndCtl);
}

/**
 * @param nCtlID - control ID.
 */
template <class T>
inline void CLayoutManager<T>::DeleteControlFromLayout(int nCtlID)
{
	HWND hwndCtl = ((T*)this)->GetDlgItem(nCtlID);
	CLayoutManager<T>::DeleteControlFromLayout(hwndCtl);
}

template <class T>
inline void CLayoutManager<T>::ResetLayout()
{
	m_mapLayout.RemoveAll();
}

template <class T>
void CLayoutManager<T>::UpdateLayout()
{
	RECT rcClient;
	((T*)this)->GetClientRect(&rcClient);
	const int nWindowDeltaX = rcClient.right - m_ptMinClientSize.x;
	const int nWindowDeltaY = rcClient.bottom - m_ptMinClientSize.y;

	BOOL bVisible = ((T*)this)->IsWindowVisible();
	if (bVisible)
		((T*)this)->SetRedraw(FALSE);

	POSITION posLayout = m_mapLayout.GetStartPosition();
	while (posLayout != NULL)
	{
		const CAtlMap<HWND, CONTROL_LAYOUT_INFO>::CPair* pLayoutInfoPair = m_mapLayout.GetNext(posLayout);
		ATLASSERT(pLayoutInfoPair != NULL);
		const CONTROL_LAYOUT_INFO& rLayoutInfo = pLayoutInfoPair->m_value;
		const RECT& rcOriginal = rLayoutInfo.m_rcOriginal;
		RECT rcCtl;

		if (nWindowDeltaX > 0)
		{
			rcCtl.left   = rcOriginal.left   + nWindowDeltaX * rLayoutInfo.m_nRatioX1 / ALIGN_RANGE;
			rcCtl.right  = rcOriginal.right  + nWindowDeltaX * rLayoutInfo.m_nRatioX2 / ALIGN_RANGE;
		}
		else
		{
			rcCtl.left   = rcOriginal.left;
			rcCtl.right  = rcOriginal.right;
		}

		if (nWindowDeltaY > 0)
		{
			rcCtl.top    = rcOriginal.top    + nWindowDeltaY * rLayoutInfo.m_nRatioY1 / ALIGN_RANGE;
			rcCtl.bottom = rcOriginal.bottom + nWindowDeltaY * rLayoutInfo.m_nRatioY2 / ALIGN_RANGE;
		}
		else
		{
			rcCtl.top    = rcOriginal.top;
			rcCtl.bottom = rcOriginal.bottom;
		}

		int nHorScrollVal = ((T*)this)->GetScrollPos(SB_HORZ);
		rcCtl.left   -= nHorScrollVal;
		rcCtl.right  -= nHorScrollVal;
		int nVertScrollVal = ((T*)this)->GetScrollPos(SB_VERT);
		rcCtl.top    -= nVertScrollVal;
		rcCtl.bottom -= nVertScrollVal;

		::SetWindowPos(pLayoutInfoPair->m_key,
						NULL,
						rcCtl.left,
						rcCtl.top,
						rcCtl.right - rcCtl.left,
						rcCtl.bottom - rcCtl.top,
						SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOREDRAW);
	}

	if (m_wndSizeBox.m_hWnd != NULL)
	{
		RECT rcCtl;
		m_wndSizeBox.GetWindowRect(&rcCtl);
		m_wndSizeBox.SetWindowPos(NULL,
						rcClient.right - (rcCtl.right - rcCtl.left),
						rcClient.bottom - (rcCtl.bottom - rcCtl.top),
						0, 0,
						SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOSIZE);
		m_wndSizeBox.ShowWindow(((T*)this)->IsZoomed() ? SW_HIDE : SW_SHOW);
	}

	if (bVisible)
	{
		((T*)this)->SetRedraw(TRUE);
		((T*)this)->RedrawWindow(NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}
}

/**
 * @param uType - specifies the type of resizing requested.
 * @param size - new window size.
 */
template <class T>
inline void CLayoutManager<T>::OnSize(UINT /*uType*/, CSize /*size*/)
{
	SetMsgHandled(FALSE);
	CLayoutManager<T>::UpdateLayout();
}

/// Resizeable property page layout manager.
template <class T>
class CResizeablePropertyPageImpl :
	public CPropertyPageImpl<T>,
	public CLayoutManager<T>
{
public:
	enum { IDD = IDD_PROJECTPLUGINS };

	BEGIN_MSG_MAP_EX(CResizeablePropertyPageImpl<T>)
		NOTIFY_CODE_HANDLER_EX(PSN_SETACTIVE, OnNotify)
		CHAIN_MSG_MAP(CPropertyPageImpl<T>)
		CHAIN_MSG_MAP(CLayoutManager<T>)
	END_MSG_MAP()

	/// Initialize layout manager.
	void InitLayout(bool bAddSizeBox = true, bool bTrackMinSize = true);

private:
	/// WM_NOTIFY event handler.
	LRESULT OnNotify(LPNMHDR pnmh);
};

/**
 * @param pnmh - notification info block.
 * @return notification result.
 */
template <class T>
LRESULT CResizeablePropertyPageImpl<T>::OnNotify(LPNMHDR /*pnmh*/)
{
	SetMsgHandled(FALSE);
	CPropertyPageImpl<T>::GetPropertySheet().SendMessage(UWM_INIT_PAGE, 0, (LPARAM)m_hWnd);
	return 0;
}

/**
 * @param bAddSizeBox - pass true if you want to show a gripper.
 * @param bTrackMinSize - pass true if you want to prevent layout manager from resizing controls under the limit.
 */
template <class T>
void CResizeablePropertyPageImpl<T>::InitLayout(bool bAddSizeBox, bool bTrackMinSize)
{
	CPropertyPageImpl<T>::GetPropertySheet().SendMessage(UWM_INIT_SHEET);
	CLayoutManager<T>::InitLayout(bAddSizeBox, bTrackMinSize);
}

/// Resizeable property sheet layout manager.
template <class T>
class CResizeablePropertySheetImpl :
	public CPropertySheetImpl<T>,
	public CLayoutManager<T>
{
public:
	BEGIN_MSG_MAP_EX(CResizeablePropertySheetImpl<T>)
		MESSAGE_HANDLER_EX(UWM_INIT_SHEET, OnInitSheet)
		MESSAGE_HANDLER_EX(UWM_INIT_PAGE, OnInitPage)
		CHAIN_MSG_MAP(CPropertySheetImpl<T>)
		CHAIN_MSG_MAP(CLayoutManager<T>)
	END_MSG_MAP()

	/// Initialize layout manager.
	void InitLayout(bool bAutoCenter = true, bool bAddSizebox = true);
	/// Property sheet callback.
	static int CALLBACK PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam);
	/// Add property page to window layout.
	void AddPageToLayout(HWND hwndCtl);

private:
	/// Called upon property sheet initialization.
	LRESULT OnInitSheet(UINT uMsg, WPARAM wParam, LPARAM lParam);
	/// Called upon property page initialization.
	LRESULT OnInitPage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	/// Passed initialization flag.
	bool m_bInitialized  : 1;
	/// Auto-center flag.
	bool m_bAutoCenter   : 1;
	/// True if size-boz should be added to property sheet.
	bool m_bAddSizeBox   : 1;
	/// True if layout manager should not resize controls under the limit.
	bool m_bTrackMinSize : 1;
	/// Original property sheet size and location.
	RECT m_rcOriginal;
};

/**
 * @param hwndCtl - page window handle.
 */
template <class T>
void CResizeablePropertySheetImpl<T>::AddPageToLayout(HWND hwndCtl)
{
	ATLASSERT(hwndCtl != NULL);
	if (m_mapLayout.Lookup(hwndCtl) == NULL)
	{
		CONTROL_LAYOUT_INFO linfo(ALIGN_LEFT, ALIGN_TOP, ALIGN_RIGHT, ALIGN_BOTTOM);
		linfo.m_rcOriginal = m_rcOriginal;
		m_mapLayout.SetAt(hwndCtl, linfo);
	}
}

/**
 * @param bAutoCenter - pass true if you want to center property sheet against parent window.
 * @param bAddSizeBox - pass true if you want to show a gripper.
 */
template <class T>
inline void CResizeablePropertySheetImpl<T>::InitLayout(bool bAutoCenter, bool bAddSizeBox)
{
	m_bInitialized  = false;
	m_bAutoCenter = bAutoCenter;
	m_bAddSizeBox = bAddSizeBox;
	m_bTrackMinSize = true;
}

/**
 * @param hWnd - property sheet window handle.
 * @param uMsg - message identifier.
 * @param lParam - extra message parameter.
 * @return message result code.
 */
template <class T>
int CALLBACK CResizeablePropertySheetImpl<T>::PropSheetCallback(HWND hWnd, UINT uMsg, LPARAM lParam)
{
	int nResult = CPropertySheetImpl<T>::PropSheetCallback(hWnd, uMsg, lParam);
	if (uMsg == PSCB_PRECREATE)
		((LPDLGTEMPLATE)lParam)->style |= WS_THICKFRAME;
	return nResult;
}

/**
 * @param uMsg - message identifier.
 * @param wParam - 1st message parameter.
 * @param lParam - 2nd message parameter.
 * @return message result code.
 */
template <class T>
LRESULT CResizeablePropertySheetImpl<T>::OnInitSheet(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	if (! m_bInitialized)
	{
		m_bInitialized = true;
		if (m_bAutoCenter)
			CPropertySheetImpl<T>::CenterWindow(CPropertySheetImpl<T>::GetParent());

		CTabCtrl wndTabCtrl(CPropertySheetImpl<T>::GetTabControl());
		wndTabCtrl.GetWindowRect(&m_rcOriginal);
		wndTabCtrl.AdjustRect(FALSE, &m_rcOriginal);
		CPropertySheetImpl<T>::ScreenToClient(&m_rcOriginal);

		CLayoutManager<T>::InitLayout(m_bAddSizeBox, m_bTrackMinSize);
		CLayoutManager<T>::AddControlToLayout(IDOK, ALIGN_RIGHT, ALIGN_BOTTOM, ALIGN_RIGHT, ALIGN_BOTTOM);
		CLayoutManager<T>::AddControlToLayout(IDCANCEL, ALIGN_RIGHT, ALIGN_BOTTOM, ALIGN_RIGHT, ALIGN_BOTTOM);
		CLayoutManager<T>::AddControlToLayout(ID_APPLY_NOW, ALIGN_RIGHT, ALIGN_BOTTOM, ALIGN_RIGHT, ALIGN_BOTTOM);
		CLayoutManager<T>::AddControlToLayout(IDHELP, ALIGN_RIGHT, ALIGN_BOTTOM, ALIGN_RIGHT, ALIGN_BOTTOM);
		CLayoutManager<T>::AddControlToLayout(wndTabCtrl.m_hWnd, ALIGN_LEFT, ALIGN_TOP, ALIGN_RIGHT, ALIGN_BOTTOM);
	}
	return 0;
}

/**
 * @param uMsg - message identifier.
 * @param wParam - 1st message parameter.
 * @param lParam - 2nd message parameter.
 * @return message result code.
 */
template <class T>
LRESULT CResizeablePropertySheetImpl<T>::OnInitPage(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam)
{
	CResizeablePropertySheetImpl<T>::AddPageToLayout((HWND)lParam);
	return 0;
}
