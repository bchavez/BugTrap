/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Helper object for clipboard and drag-and-drop operations.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "stdafx.h"
#include "TextObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SUPPORTED_MEDIUM_TYPES      (TYMED_HGLOBAL | TYMED_FILE | TYMED_ISTREAM)

namespace ATL
{

template <>
class _Copy<FORMATETC>
{
public:
	static HRESULT copy(FORMATETC* p1, FORMATETC* p2);
	static void init(FORMATETC* p);
	static void destroy(FORMATETC* p);
};

HRESULT _Copy<FORMATETC>::copy(FORMATETC* p1, FORMATETC* p2)
{
	*p1 = *p2;
	if (p2->ptd != NULL)
	{
		p1->ptd = (DVTARGETDEVICE*)CoTaskMemAlloc(sizeof(*p1->ptd));
		if (p1->ptd == NULL)
			return E_OUTOFMEMORY;
		*p1->ptd = *p2->ptd;
	}
	return S_OK;
}

void _Copy<FORMATETC>::init(FORMATETC* p)
{
	p->cfFormat = CLIPFORMAT(0);
	p->dwAspect = DVASPECT_CONTENT;
	p->lindex = -1;
	p->ptd = NULL;
	p->tymed = TYMED_NULL;
}

void _Copy<FORMATETC>::destroy(FORMATETC* p)
{
	if (p->ptd != NULL)
	{
		CoTaskMemFree(p->ptd);
		p->ptd = NULL;
	}
}

}

CTextObject::CTextObject(CTextProvider* pTextProvider)
{
	m_pTextProvider = pTextProvider;
	m_pszAnsiText = NULL;
	m_pszWideText = NULL;
}

void CTextObject::ReleaseObjectText()
{
	FireDataStop();
	if (OleIsCurrentClipboard(this) == S_OK)
		OleFlushClipboard();
}

HRESULT CTextObject::DoDragDrop(IDropSource* pDropSource, DWORD dwOKEffect, DWORD* pdwEffect)
{
	DWORD dwEffect;
	if (pdwEffect == NULL)
	{
		dwEffect = DROPEFFECT_NONE;
		pdwEffect = &dwEffect;
	}
	return ::DoDragDrop(this, pDropSource, dwOKEffect, pdwEffect);
}

HRESULT CTextObject::GetClipboard()
{
	IDataObject* pDataObject;
	HRESULT hRes = OleGetClipboard(&pDataObject);
	if (SUCCEEDED(hRes))
	{
		if (! SetObjectText(pDataObject))
			hRes = E_FAIL;
		pDataObject->Release();
	}
	return hRes;
}

STDMETHODIMP CTextObject::QueryInterface(REFIID riid, void** ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown) ||
		IsEqualIID(riid, IID_IDataObject))
	{
		*ppvObject = this;
		return S_OK;
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
}

void CTextProvider::SetTextObject(CTextObject* pTextObject)
{
	if (m_pTextObject != pTextObject)
	{
		CTextObject* pOldTextObject = m_pTextObject;
		m_pTextObject = pTextObject;
		if (pOldTextObject != NULL)
			pOldTextObject->SetTextProvider(NULL);
		if (pTextObject != NULL)
			pTextObject->SetTextProvider(this);
	}
}

void CTextObject::SetTextProvider(CTextProvider* pTextProvider)
{
	ClearObjectText();
	if (m_pTextProvider != pTextProvider)
	{
		CTextProvider* pOldTextProvider = m_pTextProvider;
		m_pTextProvider = pTextProvider;
		if (pOldTextProvider != NULL)
			pOldTextProvider->SetTextObject(NULL);
		if (pTextProvider != NULL)
			pTextProvider->SetTextObject(this);
	}
}

void CTextObject::ClearObjectText()
{
	if (m_pszAnsiText != NULL)
	{
		free(m_pszAnsiText);
		m_pszAnsiText = NULL;
	}
	if (m_pszWideText != NULL)
	{
		free(m_pszWideText);
		m_pszWideText = NULL;
	}
}

void CTextObject::FireTextChange()
{
	if (m_spDataAdviseHolder != NULL)
		m_spDataAdviseHolder->SendOnDataChange(this, 0, 0);
}

void CTextObject::FireDataStop()
{
	if (m_spDataAdviseHolder != NULL)
		m_spDataAdviseHolder->SendOnDataChange(this, 0, ADVF_DATAONSTOP);
}

void CTextObject::AttachAnsiObjectText(PSTR pszText)
{
	ClearObjectText();
	m_pszAnsiText = pszText;
	FireTextChange();
}

void CTextObject::AttachWideObjectText(PWSTR pszText)
{
	ClearObjectText();
	m_pszWideText = pszText;
	FireTextChange();
}

void CTextObject::UpdateObjectText()
{
	if (m_pTextProvider != NULL)
	{
		ClearObjectText();
		FireTextChange();
	}
}

bool CTextObject::SetAnsiObjectText(PCSTR pszAnsiText)
{
	if (m_pTextProvider == NULL)
	{
		PSTR pszNewText;
		if (pszAnsiText != NULL)
		{
			int nTextSize = (int)strlen(pszAnsiText) + 1;
			pszNewText = (PCHAR)malloc(nTextSize * sizeof(CHAR));
			if (pszNewText == NULL)
				return false;
			strcpy_s(pszNewText, nTextSize, pszAnsiText);
		}
		else
			pszNewText = NULL;
		AttachAnsiObjectText(pszNewText);
		return true;
	}
	else
	{
		HRESULT hRes = m_pTextProvider->SetAnsiObjectText(pszAnsiText);
		if (hRes == E_NOTIMPL)
		{
#ifdef _UNICODE
			CA2W pszWideText(pszAnsiText);
			hRes = m_pTextProvider->SetObjectText(pszWideText);
			if (hRes == E_NOTIMPL)
				hRes = m_pTextProvider->SetWideObjectText(pszWideText);
#else
			hRes = m_pTextProvider->SetObjectText(pszAnsiText);
			if (hRes == E_NOTIMPL)
				hRes = m_pTextProvider->SetWideObjectText(CA2W(pszAnsiText));
#endif
		}
		return SUCCEEDED(hRes);
	}
}

bool CTextObject::SetWideObjectText(PCWSTR pszWideText)
{
	if (m_pTextProvider == NULL)
	{
		PWSTR pszNewText;
		if (pszWideText != NULL)
		{
			int nTextSize = (int)wcslen(pszWideText) + 1;
			pszNewText = (PWCHAR)malloc(nTextSize * sizeof(WCHAR));
			if (pszNewText == NULL)
				return false;
			wcscpy_s(pszNewText, nTextSize, pszWideText);
		}
		else
			pszNewText = NULL;
		AttachWideObjectText(pszNewText);
		return true;
	}
	else
	{
		HRESULT hRes = m_pTextProvider->SetWideObjectText(pszWideText);
		if (hRes == E_NOTIMPL)
		{
#ifdef _UNICODE
			hRes = m_pTextProvider->SetObjectText(pszWideText);
			if (hRes == E_NOTIMPL)
				hRes = m_pTextProvider->SetAnsiObjectText(CW2A(pszWideText));
#else
			CW2A pszAnsiText(pszWideText);
			hRes = m_pTextProvider->SetObjectText(pszAnsiText);
			if (hRes == E_NOTIMPL)
				hRes = m_pTextProvider->SetAnsiObjectText(pszAnsiText);
#endif
		}
		return SUCCEEDED(hRes);
	}
}

PWSTR CTextObject::AllocWideText(PCSTR pszAnsiText)
{
	if (pszAnsiText == NULL)
		return NULL;
	int nTextLength = (int)strlen(pszAnsiText);
	PWSTR pszWideText = (PWCHAR)malloc((nTextLength + 1) * sizeof(WCHAR));
	if (pszWideText != NULL)
	{
		if (! MultiByteToWideChar(CP_ACP, 0, pszAnsiText, nTextLength, pszWideText, nTextLength))
		{
			free(pszWideText);
			pszWideText = NULL;
		}
		else
			pszWideText[nTextLength] = L'\0';
	}
	return pszWideText;
}

PSTR CTextObject::AllocAnsiText(PCWSTR pszWideText)
{
	if (pszWideText == NULL)
		return NULL;
	int nTextLength = (int)wcslen(pszWideText);
	PSTR pszAnsiText = (PCHAR)malloc((nTextLength + 1) * sizeof(CHAR));
	if (pszAnsiText != NULL)
	{
		if (! WideCharToMultiByte(CP_ACP, 0, pszWideText, nTextLength, pszAnsiText, nTextLength, NULL, NULL))
		{
			free(pszAnsiText);
			pszAnsiText = NULL;
		}
		else
			pszAnsiText[nTextLength] = '\0';
	}
	return pszAnsiText;
}

PCWSTR CTextObject::GetWideObjectText()
{
	if (m_pszWideText != NULL)
		return m_pszWideText;
	if (m_pTextProvider != NULL)
	{
		PCWSTR pszWideText;
		HRESULT hRes = m_pTextProvider->GetWideObjectText(pszWideText);
		if (hRes == E_NOTIMPL)
		{
			PCTSTR pszText;
			hRes = m_pTextProvider->GetObjectText(pszText);
			if (SUCCEEDED(hRes))
			{
#ifdef _UNICODE
				pszWideText = pszText;
#else
				pszWideText = m_pszWideText = AllocWideText(pszText);
				if (pszWideText == NULL)
					hRes = E_OUTOFMEMORY;
#endif
			}
			else if (hRes == E_NOTIMPL)
			{
				if (m_pszAnsiText != NULL)
				{
					pszWideText = m_pszWideText = AllocWideText(m_pszAnsiText);
					hRes = pszWideText != NULL ? S_OK : E_OUTOFMEMORY;
				}
				else
				{
					PCSTR pszAnsiText;
					hRes = m_pTextProvider->GetAnsiObjectText(pszAnsiText);
					if (SUCCEEDED(hRes))
					{
						pszWideText = m_pszWideText = AllocWideText(pszAnsiText);
						if (pszWideText == NULL)
							hRes = E_OUTOFMEMORY;
					}
				}
			}
		}
		return FAILED(hRes) ? NULL : pszWideText;
	}
	else
	{
		if (m_pszAnsiText != NULL)
		{
			m_pszWideText = AllocWideText(m_pszAnsiText);
			return m_pszWideText;
		}
		return NULL;
	}
}

PCSTR CTextObject::GetAnsiObjectText()
{
	if (m_pszAnsiText != NULL)
		return m_pszAnsiText;
	if (m_pTextProvider != NULL)
	{
		PCSTR pszAnsiText;
		HRESULT hRes = m_pTextProvider->GetAnsiObjectText(pszAnsiText);
		if (hRes == E_NOTIMPL)
		{
			PCTSTR pszText;
			hRes = m_pTextProvider->GetObjectText(pszText);
			if (SUCCEEDED(hRes))
			{
#ifdef _UNICODE
				pszAnsiText = m_pszAnsiText = AllocAnsiText(pszText);
				if (pszAnsiText == NULL)
					hRes = E_OUTOFMEMORY;
#else
				pszAnsiText = pszText;
#endif
			}
			else if (hRes == E_NOTIMPL)
			{
				if (m_pszWideText != NULL)
				{
					pszAnsiText = m_pszAnsiText = AllocAnsiText(m_pszWideText);
					hRes = pszAnsiText != NULL ? S_OK : E_OUTOFMEMORY;
				}
				else
				{
					PCWSTR pszWideText;
					hRes = m_pTextProvider->GetWideObjectText(pszWideText);
					if (SUCCEEDED(hRes))
					{
						pszAnsiText = m_pszAnsiText = AllocAnsiText(pszWideText);
						if (pszAnsiText == NULL)
							hRes = E_OUTOFMEMORY;
					}
				}
			}
		}
		return FAILED(hRes) ? NULL : pszAnsiText;
	}
	else
	{
		if (m_pszWideText != NULL)
		{
			m_pszAnsiText = AllocAnsiText(m_pszWideText);
			return m_pszAnsiText;
		}
		return NULL;
	}
}

HGLOBAL CTextObject::StoreDataToHGlobal(const void* pData, int nDataSize)
{
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nDataSize);
	if (hGlobal != NULL)
	{
		CopyMemory(GlobalLock(hGlobal), pData, nDataSize);
		GlobalUnlock(hGlobal);
	}
	return hGlobal;
}

bool CTextObject::StoreDataToFile(PCTSTR pszFileName, const void* pData, int nDataSize)
{
	HANDLE hTempFile = CreateFile(pszFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hTempFile == INVALID_HANDLE_VALUE)
		return false;
	DWORD dwNumWritten;
	WriteFile(hTempFile, pData, nDataSize, &dwNumWritten, NULL);
	CloseHandle(hTempFile);
	return true;
}

PWSTR CTextObject::CreateDataFile(const void* pData, int nDataSize)
{
	TCHAR szTempPath[MAX_PATH];
	GetTempPath(countof(szTempPath), szTempPath);
	TCHAR szTempFileName[MAX_PATH];
	GetTempFileName(szTempPath, _T("OBJ"), 0, szTempFileName);
	if (! StoreDataToFile(szTempFileName, pData, nDataSize))
		return NULL;
	PWSTR pszFileName = (PWSTR)CoTaskMemAlloc(MAX_PATH * sizeof(WCHAR));
	if (pszFileName == NULL)
	{
		DeleteFile(szTempFileName);
		return NULL;
	}
#ifdef _UNICODE
	wcscpy_s(pszFileName, MAX_PATH, szTempFileName);
#else
	if (! MultiByteToWideChar(CP_ACP, 0, szTempFileName, -1, pszFileName, MAX_PATH))
	{
		DeleteFile(szTempFileName);
		CoTaskMemFree(pszFileName);
		return NULL;
	}
#endif
	return pszFileName;
}

HRESULT CTextObject::InitializeDataHere(STGMEDIUM* pMedium, const void* pData, int nDataSize)
{
	switch (pMedium->tymed)
	{
	case TYMED_HGLOBAL:
		{
			HGLOBAL hGlobal = pMedium->hGlobal;
			if (hGlobal == NULL)
				return DV_E_FORMATETC;
			hGlobal = GlobalReAlloc(hGlobal, nDataSize, 0);
			if (hGlobal == NULL)
				return E_OUTOFMEMORY;
			ATLASSERT(pMedium->hGlobal == hGlobal);
			CopyMemory(GlobalLock(hGlobal), pData, nDataSize);
			GlobalUnlock(hGlobal);
		}
		break;
	case TYMED_FILE:
		{
			PWSTR pszFileName = pMedium->lpszFileName;
			if (pszFileName == NULL)
				return DV_E_FORMATETC;
			if (! StoreDataToFile(CW2TEX<MAX_PATH>(pszFileName), pData, nDataSize))
				return STG_E_MEDIUMFULL;
		}
		break;
	case TYMED_ISTREAM:
		{
			IStream* pStream = pMedium->pstm;
			if (pStream == NULL)
				return DV_E_FORMATETC;
			HRESULT hRes = pStream->Write(pData, nDataSize, NULL);
			if (FAILED(hRes))
				return hRes;
		}
		break;
	}
	pMedium->pUnkForRelease = NULL;
	return S_OK;
}

HRESULT CTextObject::InitializeData(DWORD tymed, STGMEDIUM* pMedium, const void* pData, int nDataSize)
{
	if (tymed & TYMED_HGLOBAL)
	{
		HGLOBAL hGlobal = StoreDataToHGlobal(pData, nDataSize);
		if (hGlobal == NULL)
			return STG_E_MEDIUMFULL;
		pMedium->hGlobal = hGlobal;
		pMedium->tymed = TYMED_HGLOBAL;
	}
	else if (tymed & TYMED_FILE)
	{
		PWSTR pszFileName = CreateDataFile(pData, nDataSize);
		if (pszFileName == NULL)
			return STG_E_MEDIUMFULL;
		pMedium->lpszFileName = pszFileName;
		pMedium->tymed = TYMED_FILE;
	}
	else if (tymed & TYMED_ISTREAM)
	{
		IStream* pStream;
		HRESULT hRes = CreateStreamOnHGlobal(NULL, TRUE, &pStream);
		if (FAILED(hRes))
			return hRes;
		hRes = pStream->Write(pData, nDataSize, NULL);
		if (FAILED(hRes))
		{
			pStream->Release();
			return hRes;
		}
		pMedium->pstm = pStream;
		pMedium->tymed = TYMED_ISTREAM;
	}
	else
		return DV_E_TYMED;
	pMedium->pUnkForRelease = NULL;
	return S_OK;
}

HRESULT CTextObject::InitializeData(const FORMATETC* pFormatEtc, STGMEDIUM* pMedium, bool bGetDataHere)
{
	if (pFormatEtc->lindex != -1)
		return DV_E_LINDEX;
	if (pFormatEtc->dwAspect != DVASPECT_CONTENT)
		return DV_E_DVASPECT;
	const void* pData;
	int nDataSize;
	switch (pFormatEtc->cfFormat)
	{
	case CF_TEXT:
		{
			PCSTR pszText = GetAnsiObjectText();
			if (pszText == NULL)
				return E_OUTOFMEMORY;
			pData = pszText;
			nDataSize = (int)(strlen(pszText) + 1) * sizeof(CHAR);
		}
		break;
	case CF_UNICODETEXT:
		{
			PCWSTR pszText = GetWideObjectText();
			if (pszText == NULL)
				return E_OUTOFMEMORY;
			pData = pszText;
			nDataSize = (int)(wcslen(pszText) + 1) * sizeof(WCHAR);
		}
		break;
	default:
		return DV_E_FORMATETC;
	}
	return (bGetDataHere ?
		InitializeDataHere(pMedium, pData, nDataSize) :
		InitializeData(pFormatEtc->tymed, pMedium, pData, nDataSize));
}

STDMETHODIMP CTextObject::GetData(FORMATETC* pFormatEtc, STGMEDIUM* pMedium)
{
	if (pFormatEtc == NULL || pMedium == NULL)
		return E_INVALIDARG;
	return InitializeData(pFormatEtc, pMedium, false);
}

STDMETHODIMP CTextObject::GetDataHere(FORMATETC* pFormatEtc, STGMEDIUM* pMedium)
{
	if (pFormatEtc == NULL || pMedium == NULL)
		return E_INVALIDARG;
	return InitializeData(pFormatEtc, pMedium, true);
}

STDMETHODIMP CTextObject::QueryGetData(FORMATETC* pFormatEtc)
{
	if (pFormatEtc == NULL)
		return E_INVALIDARG;
	if (pFormatEtc->lindex != -1)
		return DV_E_LINDEX;
	if (pFormatEtc->dwAspect != DVASPECT_CONTENT)
		return DV_E_DVASPECT;
	if (pFormatEtc->cfFormat != CF_TEXT && pFormatEtc->cfFormat != CF_UNICODETEXT)
		return DV_E_FORMATETC;
	if ((pFormatEtc->tymed & SUPPORTED_MEDIUM_TYPES) == 0)
		return DV_E_TYMED;
	return S_OK;
}

STDMETHODIMP CTextObject::GetCanonicalFormatEtc(FORMATETC* pFormatEtcIn, FORMATETC* pFormatEtcOut)
{
	if (pFormatEtcIn == NULL || pFormatEtcOut == NULL)
		return E_INVALIDARG;
	HRESULT hRes = _Copy<FORMATETC>::copy(pFormatEtcOut, pFormatEtcIn);
	if (FAILED(hRes))
		return hRes;
	return DATA_S_SAMEFORMATETC;
}

bool CTextObject::ReadDataFromHGlobal(HGLOBAL hGlobal, CLIPFORMAT cfFormat)
{
	PVOID pData = GlobalLock(hGlobal);
	bool bResult = false;
	switch (cfFormat)
	{
	case CF_TEXT:
		bResult = SetAnsiObjectText((PCSTR)pData);
		break;
	case CF_UNICODETEXT:
		bResult = SetWideObjectText((PCWSTR)pData);
		break;
	}
	GlobalUnlock(hGlobal);
	return bResult;
}

bool CTextObject::ReadDataFromFile(PCTSTR pszFileName, CLIPFORMAT cfFormat)
{
	HANDLE hDataFile = CreateFile(pszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hDataFile == INVALID_HANDLE_VALUE)
		return false;
	DWORD nNumRead, nDataSize = GetFileSize(hDataFile, NULL);
	bool bResult = false;
	switch (cfFormat)
	{
	case CF_TEXT:
		{
			PSTR pszText = (PCHAR)malloc((nDataSize + 1) * sizeof(CHAR));
			if (pszText != NULL)
			{
				if (ReadFile(hDataFile, pszText, nDataSize, &nNumRead, NULL))
				{
					pszText[nNumRead] = '\0';
					AttachAnsiObjectText(pszText);
					bResult = true;
					break;
				}
				free(pszText);
			}
		}
		break;
	case CF_UNICODETEXT:
		{
			PWSTR pszText = (PWCHAR)malloc((nDataSize + 1) * sizeof(WCHAR));
			if (pszText != NULL)
			{
				if (ReadFile(hDataFile, pszText, nDataSize, &nNumRead, NULL))
				{
					pszText[nNumRead] = '\0';
					AttachWideObjectText(pszText);
					bResult = true;
					break;
				}
				free(pszText);
			}
		}
		break;
	}
	CloseHandle(hDataFile);
	return bResult;
}

bool CTextObject::ReadDataFromStream(IStream* pStream, CLIPFORMAT cfFormat)
{
	LARGE_INTEGER nSetSeekPos;
	nSetSeekPos.QuadPart = 0;
	ULARGE_INTEGER nGetSeekPos;
	HRESULT hRes = pStream->Seek(nSetSeekPos, STREAM_SEEK_SET, &nGetSeekPos);
	if (SUCCEEDED(hRes))
		return false;
	ULONG nNumRead, nDataSize = (ULONG)nGetSeekPos.QuadPart;
	bool bResult = false;
	switch (cfFormat)
	{
	case CF_TEXT:
		{
			PSTR pszText = (PCHAR)malloc((nDataSize + 1) * sizeof(CHAR));
			if (pszText != NULL)
			{
				if (SUCCEEDED(pStream->Read(pszText, nDataSize, &nNumRead)))
				{
					pszText[nNumRead] = '\0';
					AttachAnsiObjectText(pszText);
					bResult = true;
					break;
				}
				free(pszText);
			}
		}
		break;
	case CF_UNICODETEXT:
		{
			PWSTR pszText = (PWCHAR)malloc((nDataSize + 1) * sizeof(WCHAR));
			if (pszText != NULL)
			{
				if (SUCCEEDED(pStream->Read(pszText, nDataSize, &nNumRead)))
				{
					pszText[nNumRead] = '\0';
					AttachWideObjectText(pszText);
					bResult = true;
					break;
				}
				free(pszText);
			}
		}
		break;
	}
	return bResult;
}

STDMETHODIMP CTextObject::SetData(FORMATETC* pFormatEtc, STGMEDIUM* pMedium, BOOL fRelease)
{
	if (pMedium == NULL)
		return E_INVALIDARG;
	HRESULT hRes;
	if (pFormatEtc != NULL)
	{
		if (pFormatEtc->cfFormat == CF_TEXT || pFormatEtc->cfFormat == CF_UNICODETEXT)
		{
			switch (pMedium->tymed)
			{
			case TYMED_HGLOBAL:
				{
					HGLOBAL hGlobal = pMedium->hGlobal;
					if (hGlobal != NULL)
						hRes = ReadDataFromHGlobal(hGlobal, pFormatEtc->cfFormat) ? S_OK : E_OUTOFMEMORY;
					else
						hRes = DV_E_FORMATETC;
				}
				break;
			case TYMED_FILE:
				{
					PWSTR pszFileName = pMedium->lpszFileName;
					if (pszFileName != NULL)
						hRes = ReadDataFromFile(CW2TEX<MAX_PATH>(pszFileName), pFormatEtc->cfFormat) ? S_OK : E_OUTOFMEMORY;
					else
						hRes = DV_E_FORMATETC;
				}
				break;
			case TYMED_ISTREAM:
				{
					IStream* pStream = pMedium->pstm;
					if (pStream != NULL)
						hRes = ReadDataFromStream(pStream, pFormatEtc->cfFormat) ? S_OK : E_OUTOFMEMORY;
					else
						hRes = DV_E_FORMATETC;
				}
				break;
			default:
				hRes = DV_E_TYMED;
			}
		}
		else
			hRes = DV_E_FORMATETC;
	}
	else
		hRes = E_INVALIDARG;
	if (fRelease)
		ReleaseStgMedium(pMedium);
	return hRes;
}

STDMETHODIMP CTextObject::EnumFormatEtc(DWORD dwDirection, IEnumFORMATETC** ppEnumFormatEtc)
{
	if (dwDirection != DATADIR_GET && dwDirection != DATADIR_SET)
		return E_INVALIDARG;
	typedef CComEnum<IEnumFORMATETC, &IID_IEnumFORMATETC, FORMATETC, _Copy<FORMATETC>, CComSingleThreadModel> CEnumFORMATETC;
	CComObject<CEnumFORMATETC>* pEnumerator;
	HRESULT hRes = CComObject<CEnumFORMATETC>::CreateInstance(&pEnumerator);
	if (FAILED(hRes))
		return hRes;
	static FORMATETC formats[] =
	{
		{ CF_TEXT, NULL, DVASPECT_CONTENT, -1, SUPPORTED_MEDIUM_TYPES },
		{ CF_UNICODETEXT, NULL, DVASPECT_CONTENT, -1, SUPPORTED_MEDIUM_TYPES }
	};
	hRes = pEnumerator->Init(formats, formats + countof(formats), this, ATL::AtlFlagNoCopy);
	if (SUCCEEDED(hRes))
		hRes = pEnumerator->QueryInterface(ppEnumFormatEtc);
	if (FAILED(hRes))
		delete pEnumerator;
	return hRes;
}

STDMETHODIMP CTextObject::DAdvise(FORMATETC* pFormatEtc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection)
{
	HRESULT hRes = m_spDataAdviseHolder == NULL ? CreateDataAdviseHolder(&m_spDataAdviseHolder) : S_OK;
	if (SUCCEEDED(hRes))
		hRes = m_spDataAdviseHolder->Advise(this, pFormatEtc, advf, pAdvSink, pdwConnection);
	return hRes;
}

STDMETHODIMP CTextObject::DUnadvise(DWORD dwConnection)
{
	if (m_spDataAdviseHolder == NULL)
		return OLE_E_NOCONNECTION;
	return m_spDataAdviseHolder->Unadvise(dwConnection);
}

STDMETHODIMP CTextObject::EnumDAdvise(IEnumSTATDATA** ppEnumAdvise)
{
	if (m_spDataAdviseHolder == NULL)
		return OLE_E_NOCONNECTION;
	return m_spDataAdviseHolder->EnumAdvise(ppEnumAdvise);
}

bool CTextObject::CheckDataFormat(IDataObject* pDataObject, CLIPFORMAT cfFormat)
{
	FORMATETC format;
	format.cfFormat = cfFormat;
	format.dwAspect = DVASPECT_CONTENT;
	format.lindex = -1;
	format.ptd = NULL;
	format.tymed = SUPPORTED_MEDIUM_TYPES;
	return SUCCEEDED(pDataObject->QueryGetData(&format));
}

CLIPFORMAT CTextObject::GetDataFormat(IDataObject* pDataObject)
{
	CLIPFORMAT cfFormat;
#ifdef _UNICODE
	if (CheckDataFormat(pDataObject, CF_UNICODETEXT))
		cfFormat = CF_UNICODETEXT;
	else if (CheckDataFormat(pDataObject, CF_TEXT))
		cfFormat = CF_TEXT;
	else
		cfFormat = CLIPFORMAT(0);
#else
	if (CheckDataFormat(pDataObject, CF_TEXT))
		cfFormat = CF_TEXT;
	else if (CheckDataFormat(pDataObject, CF_UNICODETEXT))
		cfFormat = CF_UNICODETEXT;
	else
		cfFormat = CLIPFORMAT(0);
#endif
	return cfFormat;
}

bool CTextObject::AcceptDataFormat(IDataObject* pDataObject)
{
	if (pDataObject == NULL)
		return false;
	CLIPFORMAT cfFormat = GetDataFormat(pDataObject);
	return (cfFormat != CLIPFORMAT(0));
}

bool CTextObject::SetObjectText(IDataObject* pDataObject)
{
	if (pDataObject == NULL)
		return false;
	CLIPFORMAT cfFormat = GetDataFormat(pDataObject);
	if (cfFormat == CLIPFORMAT(0))
		return false;
	FORMATETC format;
	format.cfFormat = cfFormat;
	format.dwAspect = DVASPECT_CONTENT;
	format.lindex = -1;
	format.ptd = NULL;
	format.tymed = SUPPORTED_MEDIUM_TYPES;
	STGMEDIUM stgm;
	stgm.tymed = TYMED_NULL;
	stgm.pUnkForRelease = NULL;
	HRESULT hRes = pDataObject->GetData(&format, &stgm);
	if (FAILED(hRes))
		return false;
	hRes = SetData(&format, &stgm, TRUE);
	return SUCCEEDED(hRes);
}

STDMETHODIMP CDragDropHandler::QueryInterface(REFIID riid, void** ppvObject)
{
	if (IsEqualIID(riid, IID_IUnknown) ||
		IsEqualIID(riid, IID_IDropTarget) ||
		IsEqualIID(riid, IID_IDropSource))
	{
		*ppvObject = this;
		return S_OK;
	}
	else
	{
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
}
