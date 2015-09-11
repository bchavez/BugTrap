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

#pragma once

class CTextObject;

class CTextProvider
{
public:
	CTextProvider();
	virtual ~CTextProvider();

	void SetTextObject(CTextObject* pTextObject);
	CTextObject* GetTextObject() const;

	virtual HRESULT SetObjectText(PCTSTR pszText);
	virtual HRESULT SetWideObjectText(PCWSTR pszWideText);
	virtual HRESULT SetAnsiObjectText(PCSTR pszAnsiText);

	virtual HRESULT GetObjectText(PCTSTR& pszText);
	virtual HRESULT GetWideObjectText(PCWSTR& pszWideText);
	virtual HRESULT GetAnsiObjectText(PCSTR& pszAnsiText);

private:
	CTextObject* m_pTextObject;
};

inline CTextProvider::CTextProvider()
{
	m_pTextObject = NULL;
}

inline CTextProvider::~CTextProvider()
{
	SetTextObject(NULL);
}

inline CTextObject* CTextProvider::GetTextObject() const
{
	return m_pTextObject;
}

inline HRESULT CTextProvider::SetObjectText(PCTSTR /*pszText*/)
{
	return E_NOTIMPL;
}

inline HRESULT CTextProvider::SetWideObjectText(PCWSTR /*pszWideText*/)
{
	return E_NOTIMPL;
}

inline HRESULT CTextProvider::SetAnsiObjectText(PCSTR /*pszAnsiText*/)
{
	return E_NOTIMPL;
}

inline HRESULT CTextProvider::GetObjectText(PCTSTR& pszText)
{
	pszText = NULL;
	return E_NOTIMPL;
}

inline HRESULT CTextProvider::GetWideObjectText(PCWSTR& pszWideText)
{
	pszWideText = NULL;
	return E_NOTIMPL;
}

inline HRESULT CTextProvider::GetAnsiObjectText(PCSTR& pszAnsiText)
{
	pszAnsiText = NULL;
	return E_NOTIMPL;
}

class CTextObject : public IDataObject
{
public:
	CTextObject(CTextProvider* pTextProvider = NULL);
	~CTextObject();

	// IUnknown
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

	// IDataObject
	STDMETHOD(GetData)(FORMATETC* pFormatEtc, STGMEDIUM* pMedium);
	STDMETHOD(GetDataHere)(FORMATETC* pFormatEtc, STGMEDIUM* pMedium);
	STDMETHOD(QueryGetData)(FORMATETC* pFormatEtc);
	STDMETHOD(GetCanonicalFormatEtc)(FORMATETC* pFormatEtcIn, FORMATETC* pFormatEtcOut);
	STDMETHOD(SetData)(FORMATETC* pFormatEtc, STGMEDIUM* pMedium, BOOL fRelease);
	STDMETHOD(EnumFormatEtc)(DWORD dwDirection, IEnumFORMATETC** ppEnumFormatEtc);
	STDMETHOD(DAdvise)(FORMATETC* pFormatEtc, DWORD advf, IAdviseSink* pAdvSink, DWORD* pdwConnection);
	STDMETHOD(DUnadvise)(DWORD dwConnection);
	STDMETHOD(EnumDAdvise)(IEnumSTATDATA** ppEnumAdvise);

	void SetTextProvider(CTextProvider* pTextProvider);
	CTextProvider* GetTextProvider() const;

	bool AcceptDataFormat(IDataObject* pDataObject);
	bool SetAnsiObjectText(PCSTR pszAnsiText);
	bool SetWideObjectText(PCWSTR pszWideText);
	bool SetObjectText(PCTSTR pszText);
	bool SetObjectText(IDataObject* pDataObject);
	void ClearObjectText();
	void UpdateObjectText();

	PCTSTR GetObjectText();
	PCWSTR GetWideObjectText();
	PCSTR GetAnsiObjectText();

	HRESULT DoDragDrop(IDropSource* pDropSource, DWORD dwOKEffect, DWORD* pdwEffect = NULL);
	HRESULT SetClipboard();
	HRESULT GetClipboard();
	static HRESULT EmptyClipboard();
	void ReleaseObjectText();

private:
	CTextObject(const CTextObject& rTextObject);
	CTextObject& operator=(const CTextObject& rTextObject);

	static PWSTR AllocWideText(PCSTR pszAnsiText);
	static PSTR AllocAnsiText(PCWSTR pszWideText);

	void AttachAnsiObjectText(PSTR pszText);
	void AttachWideObjectText(PWSTR pszText);

	HRESULT InitializeDataHere(STGMEDIUM* pMedium, const void* pData, int nDataSize);
	HRESULT InitializeData(DWORD tymed, STGMEDIUM* pMedium, const void* pData, int nDataSize);
	HRESULT InitializeData(const FORMATETC* pFormatEtc, STGMEDIUM* pMedium, bool bGetDataHere);

	void FireTextChange();
	void FireDataStop();
	bool CheckDataFormat(IDataObject* pDataObject, CLIPFORMAT cfFormat);
	CLIPFORMAT GetDataFormat(IDataObject* pDataObject);
	bool ReadDataFromObject(IDataObject* pDataObject, CLIPFORMAT cfFormat);
	HGLOBAL StoreDataToHGlobal(const void* pData, int nDataSize);
	bool ReadDataFromHGlobal(HGLOBAL hGlobal, CLIPFORMAT cfFormat);
	PWSTR CreateDataFile(const void* pData, int nDataSize);
	bool StoreDataToFile(PCTSTR pszFileName, const void* pData, int nDataSize);
	bool ReadDataFromFile(PCTSTR pszFileName, CLIPFORMAT cfFormat);
	bool ReadDataFromStream(IStream* pStream, CLIPFORMAT cfFormat);

	PSTR m_pszAnsiText;
	PWSTR m_pszWideText;
	CTextProvider* m_pTextProvider;
	CComPtr<IDataAdviseHolder> m_spDataAdviseHolder;
};

inline CTextObject::~CTextObject()
{
	ReleaseObjectText();
	SetTextProvider(NULL); // also destroy the text
}

inline STDMETHODIMP_(ULONG) CTextObject::AddRef()
{
	return 1;
}

inline STDMETHODIMP_(ULONG) CTextObject::Release()
{
	return 1;
}

inline CTextProvider* CTextObject::GetTextProvider() const
{
	return m_pTextProvider;
}

inline PCTSTR CTextObject::GetObjectText()
{
#ifdef _UNICODE
	return GetWideObjectText();
#else
	return GetAnsiObjectText();
#endif
}

inline bool CTextObject::SetObjectText(PCTSTR pszText)
{
#ifdef _UNICODE
	return SetWideObjectText(pszText);
#else
	return SetAnsiObjectText(pszText);
#endif
}

inline HRESULT CTextObject::SetClipboard()
{
	return OleSetClipboard(this);
}

inline HRESULT CTextObject::EmptyClipboard()
{
	return OleSetClipboard(NULL);
}

class ATL_NO_VTABLE CDragDropHandler :
	public IDropSource,
	public IDropTarget
{
public:
	// IUnknown
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject);
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
};

inline STDMETHODIMP_(ULONG) CDragDropHandler::AddRef()
{
	return 1;
}

inline STDMETHODIMP_(ULONG) CDragDropHandler::Release()
{
	return 1;
}
