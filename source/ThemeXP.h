/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2001-2002 David Yuheng Zhao.
 *
 * Distribute and change freely, except: don't remove my name from the source
 *
 * No warrantee of any kind, express or implied, is included with this
 * software; use at your own risk, responsibility for damages (if any) to
 * anyone resulting from the use of this software rests entirely with the
 * user.
 *
 * Based on the _ThemeHelper struct in MFC7.0 source code (winctrl3.cpp),
 * and the difference is that this implementation wraps the full set of
 * visual style APIs from the platform SDK August 2001
 *
 * If you have any questions, I can be reached as follows:
 *  yuheng_zhao@yahoo.com
 *
 * Description: XP themes support.
 * Updated by: Maksim Pyatkovskiy.
 * Note: Based on code developed by David Y. Zhao.
 * Downloaded from: http://www.codeproject.com/w2k/xpvisualstyle.asp
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include <uxtheme.h>
#if ((NTDDI_VERSION >= NTDDI_LONGHORN || defined(__VSSYM32_H__)) && !defined(SCHEMA_VERIFY_VSSYM32))
#include <vssym32.h>
#else
#include <tmschema.h>
#endif

#pragma warning(push)
#pragma warning(disable:4100) // unreferenced formal parameter

class CThemeXP
{
public:
	CThemeXP(HTHEME hTheme = NULL);
	CThemeXP(HWND hwnd, LPCWSTR pszClassList);
	~CThemeXP();

	BOOL Attach(HTHEME hTheme);
	HTHEME Detach();
	HTHEME GetHandle() const;
	operator HTHEME() const;

	HTHEME OpenTheme(HWND hwnd, LPCWSTR pszClassList);
	HRESULT CloseTheme();
	HRESULT DrawThemeBackground(HDC hdc, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect);
	HRESULT DrawThemeText(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT* pRect);
	HRESULT GetThemeBackgroundContentRect(HDC hdc, int iPartId, int iStateId, const RECT* pBoundingRect, RECT* pContentRect);
	HRESULT GetThemeBackgroundExtent(HDC hdc, int iPartId, int iStateId, const RECT* pContentRect, RECT* pExtentRect);
	HRESULT GetThemePartSize(HDC hdc, int iPartId, int iStateId, RECT* pRect, enum THEMESIZE eSize, SIZE* psz);
	HRESULT GetThemeTextExtent(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, const RECT* pBoundingRect, RECT* pExtentRect);
	HRESULT GetThemeTextMetrics(HDC hdc, int iPartId, int iStateId, TEXTMETRIC* ptm);
	HRESULT GetThemeBackgroundRegion(HDC hdc, int iPartId, int iStateId, const RECT* pRect, HRGN* pRegion);
	HRESULT HitTestThemeBackground(HDC hdc, int iPartId, int iStateId, DWORD dwOptions, const RECT* pRect, HRGN hrgn, POINT ptTest, WORD* pwHitTestCode);
	HRESULT DrawThemeEdge(HDC hdc, int iPartId, int iStateId, const RECT* pDestRect, UINT uEdge, UINT uFlags, RECT* pContentRect);
	HRESULT DrawThemeIcon(HDC hdc, int iPartId, int iStateId, const RECT* pRect, HIMAGELIST himl, int iImageIndex);
	BOOL IsThemePartDefined(int iPartId, int iStateId);
	BOOL IsThemeBackgroundPartiallyTransparent(int iPartId, int iStateId);
	HRESULT GetThemeColor(int iPartId, int iStateId, int iPropId, COLORREF* pColor);
	HRESULT GetThemeMetric(HDC hdc, int iPartId, int iStateId, int iPropId, int* piVal);
	HRESULT GetThemeString(int iPartId, int iStateId, int iPropId, LPWSTR pszBuff, int cchMaxBuffChars);
	HRESULT GetThemeBool(int iPartId, int iStateId, int iPropId, BOOL* pfVal);
	HRESULT GetThemeInt(int iPartId, int iStateId, int iPropId, int* piVal);
	HRESULT GetThemeEnumValue(int iPartId, int iStateId, int iPropId, int* piVal);
	HRESULT GetThemePosition(int iPartId, int iStateId, int iPropId, POINT* pPoint);
	HRESULT GetThemeFont(HDC hdc, int iPartId, int iStateId, int iPropId, LOGFONT* pFont);
	HRESULT GetThemeRect(int iPartId, int iStateId, int iPropId, RECT* pRect);
	HRESULT GetThemeMargins(HDC hdc, int iPartId, int iStateId, int iPropId, RECT* prc, MARGINS* pMargins);
	HRESULT GetThemeIntList(int iPartId, int iStateId, int iPropId, INTLIST* pIntList);
	HRESULT GetThemePropertyOrigin(int iPartId, int iStateId, int iPropId, enum PROPERTYORIGIN* pOrigin);
	HRESULT GetThemeFilename(int iPartId, int iStateId, int iPropId, LPWSTR pszThemeFileName, int cchMaxBuffChars);
	COLORREF GetThemeSysColor(int iColorId);
	HBRUSH GetThemeSysColorBrush(int iColorId);
	BOOL GetThemeSysBool(int iBoolId);
	int GetThemeSysSize(int iSizeId);
	HRESULT GetThemeSysFont(int iFontId, LOGFONT* plf);
	HRESULT GetThemeSysString(int iStringId, LPWSTR pszStringBuff, int cchMaxStringChars);
	HRESULT GetThemeSysInt(int iIntId, int* piValue);

	static BOOL LoadThemeDll();
	static void UnloadThemeDll();

	static HTHEME OpenThemeData(HWND hwnd, LPCWSTR pszClassList);
	static HRESULT CloseThemeData(HTHEME hTheme);
	static HRESULT DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect);
	static HRESULT DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT* pRect);
	static HRESULT GetThemeBackgroundContentRect(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pBoundingRect, RECT* pContentRect);
	static HRESULT GetThemeBackgroundExtent(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pContentRect, RECT* pExtentRect);
	static HRESULT GetThemePartSize(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, RECT* pRect, enum THEMESIZE eSize, SIZE* psz);
	static HRESULT GetThemeTextExtent(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, const RECT* pBoundingRect, RECT* pExtentRect);
	static HRESULT GetThemeTextMetrics(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, TEXTMETRIC* ptm);
	static HRESULT GetThemeBackgroundRegion(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, HRGN* pRegion);
	static HRESULT HitTestThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, DWORD dwOptions, const RECT* pRect, HRGN hrgn, POINT ptTest, WORD* pwHitTestCode);
	static HRESULT DrawThemeEdge(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pDestRect, UINT uEdge, UINT uFlags, RECT* pContentRect);
	static HRESULT DrawThemeIcon(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, HIMAGELIST himl, int iImageIndex);
	static BOOL IsThemePartDefined(HTHEME hTheme, int iPartId, int iStateId);
	static BOOL IsThemeBackgroundPartiallyTransparent(HTHEME hTheme, int iPartId, int iStateId);
	static HRESULT GetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF* pColor);
	static HRESULT GetThemeMetric(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, int* piVal);
	static HRESULT GetThemeString(HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPWSTR pszBuff, int cchMaxBuffChars);
	static HRESULT GetThemeBool(HTHEME hTheme, int iPartId, int iStateId, int iPropId, BOOL* pfVal);
	static HRESULT GetThemeInt(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int* piVal);
	static HRESULT GetThemeEnumValue(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int* piVal);
	static HRESULT GetThemePosition(HTHEME hTheme, int iPartId, int iStateId, int iPropId, POINT* pPoint);
	static HRESULT GetThemeFont(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, LOGFONT* pFont);
	static HRESULT GetThemeRect(HTHEME hTheme, int iPartId, int iStateId, int iPropId, RECT* pRect);
	static HRESULT GetThemeMargins(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, RECT* prc, MARGINS* pMargins);
	static HRESULT GetThemeIntList(HTHEME hTheme, int iPartId, int iStateId, int iPropId, INTLIST* pIntList);
	static HRESULT GetThemePropertyOrigin(HTHEME hTheme, int iPartId, int iStateId, int iPropId, enum PROPERTYORIGIN* pOrigin);
	static HRESULT SetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
	static HRESULT GetThemeFilename(HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPWSTR pszThemeFileName, int cchMaxBuffChars);
	static COLORREF GetThemeSysColor(HTHEME hTheme, int iColorId);
	static HBRUSH GetThemeSysColorBrush(HTHEME hTheme, int iColorId);
	static BOOL GetThemeSysBool(HTHEME hTheme, int iBoolId);
	static int GetThemeSysSize(HTHEME hTheme, int iSizeId);
	static HRESULT GetThemeSysFont(HTHEME hTheme, int iFontId, LOGFONT* plf);
	static HRESULT GetThemeSysString(HTHEME hTheme, int iStringId, LPWSTR pszStringBuff, int cchMaxStringChars);
	static HRESULT GetThemeSysInt(HTHEME hTheme, int iIntId, int* piValue);
	static BOOL IsThemeActive();
	static BOOL IsAppThemed();
	static HTHEME GetWindowTheme(HWND hwnd);
	static HRESULT EnableThemeDialogTexture(HWND hwnd, DWORD dwFlags);
	static BOOL IsThemeDialogTextureEnabled(HWND hwnd);
	static DWORD GetThemeAppProperties();
	static void SetThemeAppProperties(DWORD dwFlags);
	static HRESULT GetCurrentThemeName(LPWSTR pszThemeFileName, int cchMaxNameChars, LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars);
	static HRESULT GetThemeDocumentationProperty(LPCWSTR pszThemeName, LPCWSTR pszPropertyName, LPWSTR pszValueBuff, int cchMaxValChars);
	static HRESULT DrawThemeParentBackground(HWND hwnd, HDC hdc, RECT* prc);
	static HRESULT EnableTheming(BOOL fEnable);

private:
	CThemeXP(const CThemeXP& rTheme);
	CThemeXP& operator=(const CThemeXP& rTheme);

	static void GetThemeProcAddress(LPCSTR szProc, FARPROC& pfnProc, FARPROC pfnFailProc);

	typedef HTHEME (WINAPI * PFNOPENTHEMEDATA)(HWND hwnd, LPCWSTR pszClassList);
	static HTHEME WINAPI OpenThemeDataFail(HWND hwnd, LPCWSTR pszClassList) { return NULL; }

	typedef HRESULT (WINAPI * PFNCLOSETHEMEDATA)(HTHEME hTheme);
	static HRESULT WINAPI CloseThemeDataFail(HTHEME hTheme) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNDRAWTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect);
	static HRESULT WINAPI DrawThemeBackgroundFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNDRAWTHEMETEXT)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT* pRect);
	static HRESULT WINAPI DrawThemeTextFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT* pRect) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEBACKGROUNDCONTENTRECT)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pBoundingRect, RECT* pContentRect);
	static HRESULT WINAPI GetThemeBackgroundContentRectFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pBoundingRect, RECT* pContentRect) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEBACKGROUNDEXTENT)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pContentRect, RECT* pExtentRect);
	static HRESULT WINAPI GetThemeBackgroundExtentFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pContentRect, RECT* pExtentRect) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEPARTSIZE)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, RECT* pRect, enum THEMESIZE eSize, SIZE* psz);
	static HRESULT WINAPI GetThemePartSizeFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, RECT* pRect, enum THEMESIZE eSize, SIZE* psz) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMETEXTEXTENT)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, const RECT* pBoundingRect, RECT* pExtentRect);
	static HRESULT WINAPI GetThemeTextExtentFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, const RECT* pBoundingRect, RECT* pExtentRect) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMETEXTMETRICS)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, TEXTMETRIC* ptm);
	static HRESULT WINAPI GetThemeTextMetricsFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, TEXTMETRIC* ptm) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEBACKGROUNDREGION)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, HRGN* pRegion);
	static HRESULT WINAPI GetThemeBackgroundRegionFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, HRGN* pRegion) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNHITTESTTHEMEBACKGROUND)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, DWORD dwOptions, const RECT* pRect, HRGN hrgn, POINT ptTest, WORD* pwHitTestCode);
	static HRESULT WINAPI HitTestThemeBackgroundFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, DWORD dwOptions, const RECT* pRect, HRGN hrgn, POINT ptTest, WORD* pwHitTestCode) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNDRAWTHEMEEDGE)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pDestRect, UINT uEdge, UINT uFlags, RECT* pContentRect);
	static HRESULT WINAPI DrawThemeEdgeFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pDestRect, UINT uEdge, UINT uFlags, RECT* pContentRect) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNDRAWTHEMEICON)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, HIMAGELIST himl, int iImageIndex);
	static HRESULT WINAPI DrawThemeIconFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, HIMAGELIST himl, int iImageIndex) { return E_FAIL; }

	typedef BOOL (WINAPI * PFNISTHEMEPARTDEFINED)(HTHEME hTheme, int iPartId, int iStateId);
	static BOOL WINAPI IsThemePartDefinedFail(HTHEME hTheme, int iPartId, int iStateId) { return FALSE; }

	typedef BOOL (WINAPI * PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT)(HTHEME hTheme, int iPartId, int iStateId);
	static BOOL WINAPI IsThemeBackgroundPartiallyTransparentFail(HTHEME hTheme, int iPartId, int iStateId) { return FALSE; }

	typedef HRESULT (WINAPI * PFNGETTHEMECOLOR)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF* pColor);
	static HRESULT WINAPI GetThemeColorFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF* pColor) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEMETRIC)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, int* piVal);
	static HRESULT WINAPI GetThemeMetricFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, int* piVal) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMESTRING)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPWSTR pszBuff, int cchMaxBuffChars);
	static HRESULT WINAPI GetThemeStringFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPWSTR pszBuff, int cchMaxBuffChars) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEBOOL)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, BOOL* pfVal);
	static HRESULT WINAPI GetThemeBoolFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, BOOL* pfVal) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEINT)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int* piVal);
	static HRESULT WINAPI GetThemeIntFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int* piVal) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEENUMVALUE)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int* piVal);
	static HRESULT WINAPI GetThemeEnumValueFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int* piVal) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEPOSITION)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, POINT* pPoint);
	static HRESULT WINAPI GetThemePositionFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, POINT* pPoint) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEFONT)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, LOGFONT* pFont);
	static HRESULT WINAPI GetThemeFontFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, LOGFONT* pFont) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMERECT)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, RECT* pRect);
	static HRESULT WINAPI GetThemeRectFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, RECT* pRect) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEMARGINS)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, RECT* prc, MARGINS* pMargins);
	static HRESULT WINAPI GetThemeMarginsFail(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, RECT* prc, MARGINS* pMargins) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEINTLIST)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, INTLIST* pIntList);
	static HRESULT WINAPI GetThemeIntListFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, INTLIST* pIntList) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEPROPERTYORIGIN)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, enum PROPERTYORIGIN* pOrigin);
	static HRESULT WINAPI GetThemePropertyOriginFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, enum PROPERTYORIGIN* pOrigin) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNSETWINDOWTHEME)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
	static HRESULT WINAPI SetWindowThemeFail(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEFILENAME)(HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPWSTR pszThemeFileName, int cchMaxBuffChars);
	static HRESULT WINAPI GetThemeFilenameFail(HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPWSTR pszThemeFileName, int cchMaxBuffChars) { return E_FAIL; }

	typedef COLORREF (WINAPI * PFNGETTHEMESYSCOLOR)(HTHEME hTheme, int iColorId);
	static COLORREF WINAPI GetThemeSysColorFail(HTHEME hTheme, int iColorId) { return RGB(255, 255, 255); }

	typedef HBRUSH (WINAPI * PFNGETTHEMESYSCOLORBRUSH)(HTHEME hTheme, int iColorId);
	static HBRUSH WINAPI GetThemeSysColorBrushFail(HTHEME hTheme, int iColorId) { return NULL; }

	typedef BOOL (WINAPI * PFNGETTHEMESYSBOOL)(HTHEME hTheme, int iBoolId);
	static BOOL WINAPI GetThemeSysBoolFail(HTHEME hTheme, int iBoolId) { return FALSE; }

	typedef int (WINAPI * PFNGETTHEMESYSSIZE)(HTHEME hTheme, int iSizeId);
	static int WINAPI GetThemeSysSizeFail(HTHEME hTheme, int iSizeId) { return 0; }

	typedef HRESULT (WINAPI * PFNGETTHEMESYSFONT)(HTHEME hTheme, int iFontId, LOGFONT* plf);
	static HRESULT WINAPI GetThemeSysFontFail(HTHEME hTheme, int iFontId, LOGFONT* plf) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMESYSSTRING)(HTHEME hTheme, int iStringId, LPWSTR pszStringBuff, int cchMaxStringChars);
	static HRESULT WINAPI GetThemeSysStringFail(HTHEME hTheme, int iStringId, LPWSTR pszStringBuff, int cchMaxStringChars) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMESYSINT)(HTHEME hTheme, int iIntId, int* piValue);
	static HRESULT WINAPI GetThemeSysIntFail(HTHEME hTheme, int iIntId, int* piValue) { return E_FAIL; }

	typedef BOOL (WINAPI * PFNISTHEMEACTIVE)();
	static BOOL WINAPI IsThemeActiveFail() { return FALSE; }

	typedef BOOL(WINAPI * PFNISAPPTHEMED)();
	static BOOL WINAPI IsAppThemedFail() { return FALSE; }

	typedef HTHEME (WINAPI * PFNGETWINDOWTHEME)(HWND hwnd);
	static HTHEME WINAPI GetWindowThemeFail(HWND hwnd) { return NULL; }

	typedef HRESULT (WINAPI * PFNENABLETHEMEDIALOGTEXTURE)(HWND hwnd, DWORD dwFlags);
	static HRESULT WINAPI EnableThemeDialogTextureFail(HWND hwnd, DWORD dwFlags) { return E_FAIL; }

	typedef BOOL (WINAPI * PFNISTHEMEDIALOGTEXTUREENABLED)(HWND hwnd);
	static BOOL WINAPI IsThemeDialogTextureEnabledFail(HWND hwnd) { return FALSE; }

	typedef DWORD (WINAPI * PFNGETTHEMEAPPPROPERTIES)();
	static DWORD WINAPI GetThemeAppPropertiesFail() { return 0; }

	typedef void (WINAPI * PFNSETTHEMEAPPPROPERTIES)(DWORD dwFlags);
	static void WINAPI SetThemeAppPropertiesFail(DWORD dwFlags) { }

	typedef HRESULT (WINAPI * PFNGETCURRENTTHEMENAME)(LPWSTR pszThemeFileName, int cchMaxNameChars, LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars);
	static HRESULT WINAPI GetCurrentThemeNameFail(LPWSTR pszThemeFileName, int cchMaxNameChars, LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNGETTHEMEDOCUMENTATIONPROPERTY)(LPCWSTR pszThemeName, LPCWSTR pszPropertyName, LPWSTR pszValueBuff, int cchMaxValChars);
	static HRESULT WINAPI GetThemeDocumentationPropertyFail(LPCWSTR pszThemeName, LPCWSTR pszPropertyName, LPWSTR pszValueBuff, int cchMaxValChars) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNDRAWTHEMEPARENTBACKGROUND)(HWND hwnd, HDC hdc, RECT* prc);
	static HRESULT WINAPI DrawThemeParentBackgroundFail(HWND hwnd, HDC hdc, RECT* prc) { return E_FAIL; }

	typedef HRESULT (WINAPI * PFNENABLETHEMING)(BOOL fEnable);
	static HRESULT WINAPI EnableThemingFail(BOOL fEnable) { return E_FAIL; }

	struct FUNCTIONPOINTERS
	{
		PFNOPENTHEMEDATA m_pfnOpenThemeData;
		PFNCLOSETHEMEDATA m_pfnCloseThemeData;
		PFNDRAWTHEMEBACKGROUND m_pfnDrawThemeBackground;
		PFNDRAWTHEMETEXT m_pfnDrawThemeText;
		PFNGETTHEMEBACKGROUNDCONTENTRECT m_pfnGetThemeBackgroundContentRect;
		PFNGETTHEMEBACKGROUNDEXTENT m_pfnGetThemeBackgroundExtent;
		PFNGETTHEMEPARTSIZE m_pfnGetThemePartSize;
		PFNGETTHEMETEXTEXTENT m_pfnGetThemeTextExtent;
		PFNGETTHEMETEXTMETRICS m_pfnGetThemeTextMetrics;
		PFNGETTHEMEBACKGROUNDREGION m_pfnGetThemeBackgroundRegion;
		PFNHITTESTTHEMEBACKGROUND m_pfnHitTestThemeBackground;
		PFNDRAWTHEMEEDGE m_pfnDrawThemeEdge;
		PFNDRAWTHEMEICON m_pfnDrawThemeIcon;
		PFNISTHEMEPARTDEFINED m_pfnIsThemePartDefined;
		PFNISTHEMEBACKGROUNDPARTIALLYTRANSPARENT m_pfnIsThemeBackgroundPartiallyTransparent;
		PFNGETTHEMECOLOR m_pfnGetThemeColor;
		PFNGETTHEMEMETRIC m_pfnGetThemeMetric;
		PFNGETTHEMESTRING m_pfnGetThemeString;
		PFNGETTHEMEBOOL m_pfnGetThemeBool;
		PFNGETTHEMEINT m_pfnGetThemeInt;
		PFNGETTHEMEENUMVALUE m_pfnGetThemeEnumValue;
		PFNGETTHEMEPOSITION m_pfnGetThemePosition;
		PFNGETTHEMEFONT m_pfnGetThemeFont;
		PFNGETTHEMERECT m_pfnGetThemeRect;
		PFNGETTHEMEMARGINS m_pfnGetThemeMargins;
		PFNGETTHEMEINTLIST m_pfnGetThemeIntList;
		PFNGETTHEMEPROPERTYORIGIN m_pfnGetThemePropertyOrigin;
		PFNSETWINDOWTHEME m_pfnSetWindowTheme;
		PFNGETTHEMEFILENAME m_pfnGetThemeFilename;
		PFNGETTHEMESYSCOLOR m_pfnGetThemeSysColor;
		PFNGETTHEMESYSCOLORBRUSH m_pfnGetThemeSysColorBrush;
		PFNGETTHEMESYSBOOL m_pfnGetThemeSysBool;
		PFNGETTHEMESYSSIZE m_pfnGetThemeSysSize;
		PFNGETTHEMESYSFONT m_pfnGetThemeSysFont;
		PFNGETTHEMESYSSTRING m_pfnGetThemeSysString;
		PFNGETTHEMESYSINT m_pfnGetThemeSysInt;
		PFNISTHEMEACTIVE m_pfnIsThemeActive;
		PFNISAPPTHEMED m_pfnIsAppThemed;
		PFNGETWINDOWTHEME m_pfnGetWindowTheme;
		PFNENABLETHEMEDIALOGTEXTURE m_pfnEnableThemeDialogTexture;
		PFNISTHEMEDIALOGTEXTUREENABLED m_pfnIsThemeDialogTextureEnabled;
		PFNGETTHEMEAPPPROPERTIES m_pfnGetThemeAppProperties;
		PFNSETTHEMEAPPPROPERTIES m_pfnSetThemeAppProperties;
		PFNGETCURRENTTHEMENAME m_pfnGetCurrentThemeName;
		PFNGETTHEMEDOCUMENTATIONPROPERTY m_pfnGetThemeDocumentationProperty;
		PFNDRAWTHEMEPARENTBACKGROUND m_pfnDrawThemeParentBackground;
		PFNENABLETHEMING m_pfnEnableTheming;
	};

	HTHEME m_hTheme;
	static UINT m_uCount;
	static HMODULE m_hThemeDll;
	static FUNCTIONPOINTERS m_fnPtrs;
};

#pragma warning(pop)

inline HTHEME CThemeXP::OpenThemeData(HWND hwnd, LPCWSTR pszClassList)
{
	GetThemeProcAddress("OpenThemeData", (FARPROC&)m_fnPtrs.m_pfnOpenThemeData, (FARPROC)OpenThemeDataFail);
	return (*m_fnPtrs.m_pfnOpenThemeData)(hwnd, pszClassList);
}

inline HRESULT CThemeXP::CloseThemeData(HTHEME hTheme)
{
	GetThemeProcAddress("CloseThemeData", (FARPROC&)m_fnPtrs.m_pfnCloseThemeData, (FARPROC)CloseThemeDataFail);
	return (*m_fnPtrs.m_pfnCloseThemeData)(hTheme);
}

inline HRESULT CThemeXP::DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect)
{
	GetThemeProcAddress("DrawThemeBackground", (FARPROC&)m_fnPtrs.m_pfnDrawThemeBackground, (FARPROC)DrawThemeBackgroundFail);
	return (*m_fnPtrs.m_pfnDrawThemeBackground)(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

inline HRESULT CThemeXP::DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT* pRect)
{
	GetThemeProcAddress("DrawThemeText", (FARPROC&)m_fnPtrs.m_pfnDrawThemeText, (FARPROC)DrawThemeTextFail);
	return (*m_fnPtrs.m_pfnDrawThemeText)(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect);
}

inline HRESULT CThemeXP::GetThemeBackgroundContentRect(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pBoundingRect, RECT* pContentRect)
{
	GetThemeProcAddress("GetThemeBackgroundContentRect", (FARPROC&)m_fnPtrs.m_pfnGetThemeBackgroundContentRect, (FARPROC)GetThemeBackgroundContentRectFail);
	return (*m_fnPtrs.m_pfnGetThemeBackgroundContentRect)(hTheme, hdc, iPartId, iStateId, pBoundingRect, pContentRect);
}

inline HRESULT CThemeXP::GetThemeBackgroundExtent(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pContentRect, RECT* pExtentRect)
{
	GetThemeProcAddress("GetThemeBackgroundExtent", (FARPROC&)m_fnPtrs.m_pfnGetThemeBackgroundExtent, (FARPROC)GetThemeBackgroundExtentFail);
	return (*m_fnPtrs.m_pfnGetThemeBackgroundExtent)(hTheme, hdc, iPartId, iStateId, pContentRect, pExtentRect);
}

inline HRESULT CThemeXP::GetThemePartSize(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, RECT* pRect, enum THEMESIZE eSize, SIZE* psz)
{
	GetThemeProcAddress("GetThemePartSize", (FARPROC&)m_fnPtrs.m_pfnGetThemePartSize, (FARPROC)GetThemePartSizeFail);
	return (*m_fnPtrs.m_pfnGetThemePartSize)(hTheme, hdc, iPartId, iStateId, pRect, eSize, psz);
}

inline HRESULT CThemeXP::GetThemeTextExtent(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, const RECT* pBoundingRect, RECT* pExtentRect)
{
	GetThemeProcAddress("GetThemeTextExtent", (FARPROC&)m_fnPtrs.m_pfnGetThemeTextExtent, (FARPROC)GetThemeTextExtentFail);
	return (*m_fnPtrs.m_pfnGetThemeTextExtent)(hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, pBoundingRect, pExtentRect);
}

inline HRESULT CThemeXP::GetThemeTextMetrics(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, TEXTMETRIC* ptm)
{
	GetThemeProcAddress("GetThemeTextMetrics", (FARPROC&)m_fnPtrs.m_pfnGetThemeTextMetrics, (FARPROC)GetThemeTextMetricsFail);
	return (*m_fnPtrs.m_pfnGetThemeTextMetrics)(hTheme, hdc, iPartId, iStateId, ptm);
}

inline HRESULT CThemeXP::GetThemeBackgroundRegion(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, HRGN* pRegion)
{
	GetThemeProcAddress("GetThemeBackgroundRegion", (FARPROC&)m_fnPtrs.m_pfnGetThemeBackgroundRegion, (FARPROC)GetThemeBackgroundRegionFail);
	return (*m_fnPtrs.m_pfnGetThemeBackgroundRegion)(hTheme, hdc, iPartId, iStateId, pRect, pRegion);
}

inline HRESULT CThemeXP::HitTestThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, DWORD dwOptions, const RECT* pRect, HRGN hrgn, POINT ptTest, WORD* pwHitTestCode)
{
	GetThemeProcAddress("HitTestThemeBackground", (FARPROC&)m_fnPtrs.m_pfnHitTestThemeBackground, (FARPROC)HitTestThemeBackgroundFail);
	return (*m_fnPtrs.m_pfnHitTestThemeBackground)(hTheme, hdc, iPartId, iStateId, dwOptions, pRect, hrgn, ptTest, pwHitTestCode);
}

inline HRESULT CThemeXP::DrawThemeEdge(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pDestRect, UINT uEdge, UINT uFlags, RECT* pContentRect)
{
	GetThemeProcAddress("DrawThemeEdge", (FARPROC&)m_fnPtrs.m_pfnDrawThemeEdge, (FARPROC)DrawThemeEdgeFail);
	return (*m_fnPtrs.m_pfnDrawThemeEdge)(hTheme, hdc, iPartId, iStateId, pDestRect, uEdge, uFlags, pContentRect);
}

inline HRESULT CThemeXP::DrawThemeIcon(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT* pRect, HIMAGELIST himl, int iImageIndex)
{
	GetThemeProcAddress("DrawThemeIcon", (FARPROC&)m_fnPtrs.m_pfnDrawThemeIcon, (FARPROC)DrawThemeIconFail);
	return (*m_fnPtrs.m_pfnDrawThemeIcon)(hTheme, hdc, iPartId, iStateId, pRect, himl, iImageIndex);
}

inline BOOL CThemeXP::IsThemePartDefined(HTHEME hTheme, int iPartId, int iStateId)
{
	GetThemeProcAddress("IsThemePartDefined", (FARPROC&)m_fnPtrs.m_pfnIsThemePartDefined, (FARPROC)IsThemePartDefinedFail);
	return (*m_fnPtrs.m_pfnIsThemePartDefined)(hTheme, iPartId, iStateId);
}

inline BOOL CThemeXP::IsThemeBackgroundPartiallyTransparent(HTHEME hTheme, int iPartId, int iStateId)
{
	GetThemeProcAddress("IsThemeBackgroundPartiallyTransparent", (FARPROC&)m_fnPtrs.m_pfnIsThemeBackgroundPartiallyTransparent, (FARPROC)IsThemeBackgroundPartiallyTransparentFail);
	return (*m_fnPtrs.m_pfnIsThemeBackgroundPartiallyTransparent)(hTheme, iPartId, iStateId);
}

inline HRESULT CThemeXP::GetThemeColor(HTHEME hTheme, int iPartId, int iStateId, int iPropId, COLORREF* pColor)
{
	GetThemeProcAddress("GetThemeColor", (FARPROC&)m_fnPtrs.m_pfnGetThemeColor, (FARPROC)GetThemeColorFail);
	return (*m_fnPtrs.m_pfnGetThemeColor)(hTheme, iPartId, iStateId, iPropId, pColor);
}

inline HRESULT CThemeXP::GetThemeMetric(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, int* piVal)
{
	GetThemeProcAddress("GetThemeMetric", (FARPROC&)m_fnPtrs.m_pfnGetThemeMetric, (FARPROC)GetThemeMetricFail);
	return (*m_fnPtrs.m_pfnGetThemeMetric)(hTheme, hdc, iPartId, iStateId, iPropId, piVal);
}

inline HRESULT CThemeXP::GetThemeString(HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPWSTR pszBuff, int cchMaxBuffChars)
{
	GetThemeProcAddress("GetThemeString", (FARPROC&)m_fnPtrs.m_pfnGetThemeString, (FARPROC)GetThemeStringFail);
	return (*m_fnPtrs.m_pfnGetThemeString)(hTheme, iPartId, iStateId, iPropId, pszBuff, cchMaxBuffChars);
}

inline HRESULT CThemeXP::GetThemeBool(HTHEME hTheme, int iPartId, int iStateId, int iPropId, BOOL* pfVal)
{
	GetThemeProcAddress("GetThemeBool", (FARPROC&)m_fnPtrs.m_pfnGetThemeBool, (FARPROC)GetThemeBoolFail);
	return (*m_fnPtrs.m_pfnGetThemeBool)(hTheme, iPartId, iStateId, iPropId, pfVal);
}

inline HRESULT CThemeXP::GetThemeInt(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int* piVal)
{
	GetThemeProcAddress("GetThemeInt", (FARPROC&)m_fnPtrs.m_pfnGetThemeInt, (FARPROC)GetThemeIntFail);
	return (*m_fnPtrs.m_pfnGetThemeInt)(hTheme, iPartId, iStateId, iPropId, piVal);
}

inline HRESULT CThemeXP::GetThemeEnumValue(HTHEME hTheme, int iPartId, int iStateId, int iPropId, int* piVal)
{
	GetThemeProcAddress("GetThemeEnumValue", (FARPROC&)m_fnPtrs.m_pfnGetThemeEnumValue, (FARPROC)GetThemeEnumValueFail);
	return (*m_fnPtrs.m_pfnGetThemeEnumValue)(hTheme, iPartId, iStateId, iPropId, piVal);
}

inline HRESULT CThemeXP::GetThemePosition(HTHEME hTheme, int iPartId, int iStateId, int iPropId, POINT* pPoint)
{
	GetThemeProcAddress("GetThemePosition", (FARPROC&)m_fnPtrs.m_pfnGetThemePosition, (FARPROC)GetThemePositionFail);
	return (*m_fnPtrs.m_pfnGetThemePosition)(hTheme, iPartId, iStateId, iPropId, pPoint);
}

inline HRESULT CThemeXP::GetThemeFont(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, LOGFONT* pFont)
{
	GetThemeProcAddress("GetThemeFont", (FARPROC&)m_fnPtrs.m_pfnGetThemeFont, (FARPROC)GetThemeFontFail);
	return (*m_fnPtrs.m_pfnGetThemeFont)(hTheme, hdc, iPartId, iStateId, iPropId, pFont);
}

inline HRESULT CThemeXP::GetThemeRect(HTHEME hTheme, int iPartId, int iStateId, int iPropId, RECT* pRect)
{
	GetThemeProcAddress("GetThemeRect", (FARPROC&)m_fnPtrs.m_pfnGetThemeRect, (FARPROC)GetThemeRectFail);
	return (*m_fnPtrs.m_pfnGetThemeRect)(hTheme, iPartId, iStateId, iPropId, pRect);
}

inline HRESULT CThemeXP::GetThemeMargins(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, int iPropId, RECT* prc, MARGINS* pMargins)
{
	GetThemeProcAddress("GetThemeMargins", (FARPROC&)m_fnPtrs.m_pfnGetThemeMargins, (FARPROC)GetThemeMarginsFail);
	return (*m_fnPtrs.m_pfnGetThemeMargins)(hTheme, hdc, iPartId, iStateId, iPropId, prc, pMargins);
}

inline HRESULT CThemeXP::GetThemeIntList(HTHEME hTheme, int iPartId, int iStateId, int iPropId, INTLIST* pIntList)
{
	GetThemeProcAddress("GetThemeIntList", (FARPROC&)m_fnPtrs.m_pfnGetThemeIntList, (FARPROC)GetThemeIntListFail);
	return (*m_fnPtrs.m_pfnGetThemeIntList)(hTheme, iPartId, iStateId, iPropId, pIntList);
}

inline HRESULT CThemeXP::GetThemePropertyOrigin(HTHEME hTheme, int iPartId, int iStateId, int iPropId, enum PROPERTYORIGIN* pOrigin)
{
	GetThemeProcAddress("GetThemePropertyOrigin", (FARPROC&)m_fnPtrs.m_pfnGetThemePropertyOrigin, (FARPROC)GetThemePropertyOriginFail);
	return (*m_fnPtrs.m_pfnGetThemePropertyOrigin)(hTheme, iPartId, iStateId, iPropId, pOrigin);
}

inline HRESULT CThemeXP::SetWindowTheme(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList)
{
	GetThemeProcAddress("SetWindowTheme", (FARPROC&)m_fnPtrs.m_pfnSetWindowTheme, (FARPROC)SetWindowThemeFail);
	return (*m_fnPtrs.m_pfnSetWindowTheme)(hwnd, pszSubAppName, pszSubIdList);
}

inline HRESULT CThemeXP::GetThemeFilename(HTHEME hTheme, int iPartId, int iStateId, int iPropId, LPWSTR pszThemeFileName, int cchMaxBuffChars)
{
	GetThemeProcAddress("GetThemeFilename", (FARPROC&)m_fnPtrs.m_pfnGetThemeFilename, (FARPROC)GetThemeFilenameFail);
	return (*m_fnPtrs.m_pfnGetThemeFilename)(hTheme, iPartId, iStateId, iPropId, pszThemeFileName, cchMaxBuffChars);
}

inline COLORREF CThemeXP::GetThemeSysColor(HTHEME hTheme, int iColorId)
{
	GetThemeProcAddress("GetThemeSysColor", (FARPROC&)m_fnPtrs.m_pfnGetThemeSysColor, (FARPROC)GetThemeSysColorFail);
	return (*m_fnPtrs.m_pfnGetThemeSysColor)(hTheme, iColorId);
}

inline HBRUSH CThemeXP::GetThemeSysColorBrush(HTHEME hTheme, int iColorId)
{
	GetThemeProcAddress("GetThemeSysColorBrush", (FARPROC&)m_fnPtrs.m_pfnGetThemeSysColorBrush, (FARPROC)GetThemeSysColorBrushFail);
	return (*m_fnPtrs.m_pfnGetThemeSysColorBrush)(hTheme, iColorId);
}

inline BOOL CThemeXP::GetThemeSysBool(HTHEME hTheme, int iBoolId)
{
	GetThemeProcAddress("GetThemeSysBool", (FARPROC&)m_fnPtrs.m_pfnGetThemeSysBool, (FARPROC)GetThemeSysBoolFail);
	return (*m_fnPtrs.m_pfnGetThemeSysBool)(hTheme, iBoolId);
}

inline int CThemeXP::GetThemeSysSize(HTHEME hTheme, int iSizeId)
{
	GetThemeProcAddress("GetThemeSysSize", (FARPROC&)m_fnPtrs.m_pfnGetThemeSysSize, (FARPROC)GetThemeSysSizeFail);
	return (*m_fnPtrs.m_pfnGetThemeSysSize)(hTheme, iSizeId);
}

inline HRESULT CThemeXP::GetThemeSysFont(HTHEME hTheme, int iFontId, LOGFONT* plf)
{
	GetThemeProcAddress("GetThemeSysFont", (FARPROC&)m_fnPtrs.m_pfnGetThemeSysFont, (FARPROC)GetThemeSysFontFail);
	return (*m_fnPtrs.m_pfnGetThemeSysFont)(hTheme, iFontId, plf);
}

inline HRESULT CThemeXP::GetThemeSysString(HTHEME hTheme, int iStringId, LPWSTR pszStringBuff, int cchMaxStringChars)
{
	GetThemeProcAddress("GetThemeSysString", (FARPROC&)m_fnPtrs.m_pfnGetThemeSysString, (FARPROC)GetThemeSysStringFail);
	return (*m_fnPtrs.m_pfnGetThemeSysString)(hTheme, iStringId, pszStringBuff, cchMaxStringChars);
}

inline HRESULT CThemeXP::GetThemeSysInt(HTHEME hTheme, int iIntId, int* piValue)
{
	GetThemeProcAddress("GetThemeSysInt", (FARPROC&)m_fnPtrs.m_pfnGetThemeSysInt, (FARPROC)GetThemeSysIntFail);
	return (*m_fnPtrs.m_pfnGetThemeSysInt)(hTheme, iIntId, piValue);
}

inline BOOL CThemeXP::IsThemeActive()
{
	GetThemeProcAddress("IsThemeActive", (FARPROC&)m_fnPtrs.m_pfnIsThemeActive, (FARPROC)IsThemeActiveFail);
	return (*m_fnPtrs.m_pfnIsThemeActive)();
}

inline BOOL CThemeXP::IsAppThemed()
{
	GetThemeProcAddress("IsAppThemed", (FARPROC&)m_fnPtrs.m_pfnIsAppThemed, (FARPROC)IsAppThemedFail);
	return (*m_fnPtrs.m_pfnIsAppThemed)();
}

inline HTHEME CThemeXP::GetWindowTheme(HWND hwnd)
{
	GetThemeProcAddress("GetWindowTheme", (FARPROC&)m_fnPtrs.m_pfnGetWindowTheme, (FARPROC)GetWindowThemeFail);
	return (*m_fnPtrs.m_pfnGetWindowTheme)(hwnd);
}

inline HRESULT CThemeXP::EnableThemeDialogTexture(HWND hwnd, DWORD dwFlags)
{
	GetThemeProcAddress("EnableThemeDialogTexture", (FARPROC&)m_fnPtrs.m_pfnEnableThemeDialogTexture, (FARPROC)EnableThemeDialogTextureFail);
	return (*m_fnPtrs.m_pfnEnableThemeDialogTexture)(hwnd, dwFlags);
}

inline BOOL CThemeXP::IsThemeDialogTextureEnabled(HWND hwnd)
{
	GetThemeProcAddress("IsThemeDialogTextureEnabled", (FARPROC&)m_fnPtrs.m_pfnIsThemeDialogTextureEnabled, (FARPROC)IsThemeDialogTextureEnabledFail);
	return (*m_fnPtrs.m_pfnIsThemeDialogTextureEnabled)(hwnd);
}

inline DWORD CThemeXP::GetThemeAppProperties()
{
	GetThemeProcAddress("GetThemeAppProperties", (FARPROC&)m_fnPtrs.m_pfnGetThemeAppProperties, (FARPROC)GetThemeAppPropertiesFail);
	return (*m_fnPtrs.m_pfnGetThemeAppProperties)();
}

inline void CThemeXP::SetThemeAppProperties(DWORD dwFlags)
{
	GetThemeProcAddress("SetThemeAppProperties", (FARPROC&)m_fnPtrs.m_pfnSetThemeAppProperties, (FARPROC)SetThemeAppPropertiesFail);
	(*m_fnPtrs.m_pfnSetThemeAppProperties)(dwFlags);
}

inline HRESULT CThemeXP::GetCurrentThemeName(LPWSTR pszThemeFileName, int cchMaxNameChars, LPWSTR pszColorBuff, int cchMaxColorChars, LPWSTR pszSizeBuff, int cchMaxSizeChars)
{
	GetThemeProcAddress("GetCurrentThemeName", (FARPROC&)m_fnPtrs.m_pfnGetCurrentThemeName, (FARPROC)GetCurrentThemeNameFail);
	return (*m_fnPtrs.m_pfnGetCurrentThemeName)(pszThemeFileName, cchMaxNameChars, pszColorBuff, cchMaxColorChars, pszSizeBuff, cchMaxSizeChars);
}

inline HRESULT CThemeXP::GetThemeDocumentationProperty(LPCWSTR pszThemeName, LPCWSTR pszPropertyName, LPWSTR pszValueBuff, int cchMaxValChars)
{
	GetThemeProcAddress("GetThemeDocumentationProperty", (FARPROC&)m_fnPtrs.m_pfnGetThemeDocumentationProperty, (FARPROC)GetThemeDocumentationPropertyFail);
	return (*m_fnPtrs.m_pfnGetThemeDocumentationProperty)(pszThemeName, pszPropertyName, pszValueBuff, cchMaxValChars);
}

inline HRESULT CThemeXP::DrawThemeParentBackground(HWND hwnd, HDC hdc, RECT* prc)
{
	GetThemeProcAddress("DrawThemeParentBackground", (FARPROC&)m_fnPtrs.m_pfnDrawThemeParentBackground, (FARPROC)DrawThemeParentBackgroundFail);
	return (*m_fnPtrs.m_pfnDrawThemeParentBackground)(hwnd, hdc, prc);
}

inline HRESULT CThemeXP::EnableTheming(BOOL fEnable)
{
	GetThemeProcAddress("EnableTheming", (FARPROC&)m_fnPtrs.m_pfnEnableTheming, (FARPROC)EnableThemingFail);
	return (*m_fnPtrs.m_pfnEnableTheming)(fEnable);
}

inline HRESULT CThemeXP::CloseTheme()
{
	_ASSERTE(m_hTheme != NULL);
	return CloseThemeData(m_hTheme);
}

inline HRESULT CThemeXP::DrawThemeBackground(HDC hdc, int iPartId, int iStateId, const RECT* pRect, const RECT* pClipRect)
{
	_ASSERTE(m_hTheme != NULL);
	return DrawThemeBackground(m_hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

inline HRESULT CThemeXP::DrawThemeText(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, DWORD dwTextFlags2, const RECT* pRect)
{
	_ASSERTE(m_hTheme != NULL);
	return DrawThemeText(m_hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, dwTextFlags2, pRect);
}

inline HRESULT CThemeXP::GetThemeBackgroundContentRect(HDC hdc, int iPartId, int iStateId, const RECT* pBoundingRect, RECT* pContentRect)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeBackgroundContentRect(m_hTheme, hdc, iPartId, iStateId, pBoundingRect, pContentRect);
}

inline HRESULT CThemeXP::GetThemeBackgroundExtent(HDC hdc, int iPartId, int iStateId, const RECT* pContentRect, RECT* pExtentRect)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeBackgroundExtent(m_hTheme, hdc, iPartId, iStateId, pContentRect, pExtentRect);
}

inline HRESULT CThemeXP::GetThemePartSize(HDC hdc, int iPartId, int iStateId, RECT* pRect, enum THEMESIZE eSize, SIZE* psz)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemePartSize(m_hTheme, hdc, iPartId, iStateId, pRect, eSize, psz);
}

inline HRESULT CThemeXP::GetThemeTextExtent(HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int iCharCount, DWORD dwTextFlags, const RECT* pBoundingRect, RECT* pExtentRect)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeTextExtent(m_hTheme, hdc, iPartId, iStateId, pszText, iCharCount, dwTextFlags, pBoundingRect, pExtentRect);
}

inline HRESULT CThemeXP::GetThemeTextMetrics(HDC hdc, int iPartId, int iStateId, TEXTMETRIC* ptm)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeTextMetrics(m_hTheme, hdc, iPartId, iStateId, ptm);
}

inline HRESULT CThemeXP::GetThemeBackgroundRegion(HDC hdc, int iPartId, int iStateId, const RECT* pRect, HRGN* pRegion)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeBackgroundRegion(m_hTheme, hdc, iPartId, iStateId, pRect, pRegion);
}

inline HRESULT CThemeXP::HitTestThemeBackground(HDC hdc, int iPartId, int iStateId, DWORD dwOptions, const RECT* pRect, HRGN hrgn, POINT ptTest, WORD* pwHitTestCode)
{
	_ASSERTE(m_hTheme != NULL);
	return HitTestThemeBackground(m_hTheme, hdc, iPartId, iStateId, dwOptions, pRect, hrgn, ptTest, pwHitTestCode);
}

inline HRESULT CThemeXP::DrawThemeEdge(HDC hdc, int iPartId, int iStateId, const RECT* pDestRect, UINT uEdge, UINT uFlags, RECT* pContentRect)
{
	_ASSERTE(m_hTheme != NULL);
	return DrawThemeEdge(m_hTheme, hdc, iPartId, iStateId, pDestRect, uEdge, uFlags, pContentRect);
}

inline HRESULT CThemeXP::DrawThemeIcon(HDC hdc, int iPartId, int iStateId, const RECT* pRect, HIMAGELIST himl, int iImageIndex)
{
	_ASSERTE(m_hTheme != NULL);
	return DrawThemeIcon(m_hTheme, hdc, iPartId, iStateId, pRect, himl, iImageIndex);
}

inline BOOL CThemeXP::IsThemePartDefined(int iPartId, int iStateId)
{
	_ASSERTE(m_hTheme != NULL);
	return IsThemePartDefined(m_hTheme, iPartId, iStateId);
}

inline BOOL CThemeXP::IsThemeBackgroundPartiallyTransparent(int iPartId, int iStateId)
{
	_ASSERTE(m_hTheme != NULL);
	return IsThemeBackgroundPartiallyTransparent(m_hTheme, iPartId, iStateId);
}

inline HRESULT CThemeXP::GetThemeColor(int iPartId, int iStateId, int iPropId, COLORREF* pColor)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeColor(m_hTheme, iPartId, iStateId, iPropId, pColor);
}

inline HRESULT CThemeXP::GetThemeMetric(HDC hdc, int iPartId, int iStateId, int iPropId, int* piVal)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeMetric(m_hTheme, hdc, iPartId, iStateId, iPropId, piVal);
}

inline HRESULT CThemeXP::GetThemeString(int iPartId, int iStateId, int iPropId, LPWSTR pszBuff, int cchMaxBuffChars)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeString(m_hTheme, iPartId, iStateId, iPropId, pszBuff, cchMaxBuffChars);
}

inline HRESULT CThemeXP::GetThemeBool(int iPartId, int iStateId, int iPropId, BOOL* pfVal)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeBool(m_hTheme, iPartId, iStateId, iPropId, pfVal);
}

inline HRESULT CThemeXP::GetThemeInt(int iPartId, int iStateId, int iPropId, int* piVal)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeInt(m_hTheme, iPartId, iStateId, iPropId, piVal);
}

inline HRESULT CThemeXP::GetThemeEnumValue(int iPartId, int iStateId, int iPropId, int* piVal)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeEnumValue(m_hTheme, iPartId, iStateId, iPropId, piVal);
}

inline HRESULT CThemeXP::GetThemePosition(int iPartId, int iStateId, int iPropId, POINT* pPoint)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemePosition(m_hTheme, iPartId, iStateId, iPropId, pPoint);
}

inline HRESULT CThemeXP::GetThemeFont(HDC hdc, int iPartId, int iStateId, int iPropId, LOGFONT* pFont)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeFont(m_hTheme, hdc, iPartId, iStateId, iPropId, pFont);
}

inline HRESULT CThemeXP::GetThemeRect(int iPartId, int iStateId, int iPropId, RECT* pRect)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeRect(m_hTheme, iPartId, iStateId, iPropId, pRect);
}

inline HRESULT CThemeXP::GetThemeMargins(HDC hdc, int iPartId, int iStateId, int iPropId, RECT* prc, MARGINS* pMargins)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeMargins(m_hTheme, hdc, iPartId, iStateId, iPropId, prc, pMargins);
}

inline HRESULT CThemeXP::GetThemeIntList(int iPartId, int iStateId, int iPropId, INTLIST* pIntList)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeIntList(m_hTheme, iPartId, iStateId, iPropId, pIntList);
}

inline HRESULT CThemeXP::GetThemePropertyOrigin(int iPartId, int iStateId, int iPropId, enum PROPERTYORIGIN* pOrigin)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemePropertyOrigin(m_hTheme, iPartId, iStateId, iPropId, pOrigin);
}

inline HRESULT CThemeXP::GetThemeFilename(int iPartId, int iStateId, int iPropId, LPWSTR pszThemeFileName, int cchMaxBuffChars)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeFilename(m_hTheme, iPartId, iStateId, iPropId, pszThemeFileName, cchMaxBuffChars);
}

inline COLORREF CThemeXP::GetThemeSysColor(int iColorId)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeSysColor(m_hTheme, iColorId);
}

inline HBRUSH CThemeXP::GetThemeSysColorBrush(int iColorId)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeSysColorBrush(m_hTheme, iColorId);
}

inline BOOL CThemeXP::GetThemeSysBool(int iBoolId)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeSysBool(m_hTheme, iBoolId);
}

inline int CThemeXP::GetThemeSysSize(int iSizeId)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeSysSize(m_hTheme, iSizeId);
}

inline HRESULT CThemeXP::GetThemeSysFont(int iFontId, LOGFONT* plf)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeSysFont(m_hTheme, iFontId, plf);
}

inline HRESULT CThemeXP::GetThemeSysString(int iStringId, LPWSTR pszStringBuff, int cchMaxStringChars)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeSysString(m_hTheme, iStringId, pszStringBuff, cchMaxStringChars);
}

inline HRESULT CThemeXP::GetThemeSysInt(int iIntId, int* piValue)
{
	_ASSERTE(m_hTheme != NULL);
	return GetThemeSysInt(m_hTheme, iIntId, piValue);
}

inline HTHEME CThemeXP::OpenTheme(HWND hwnd, LPCWSTR pszClassList)
{
	m_hTheme = OpenThemeData(hwnd, pszClassList);
	return m_hTheme;
}

inline HTHEME CThemeXP::GetHandle() const
{
	return m_hTheme;
}

inline CThemeXP::operator HTHEME() const
{
	return m_hTheme;
}
