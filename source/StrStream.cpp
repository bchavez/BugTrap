/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: String (memory) stream class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "StrStream.h"
#include "StrHolder.h"
#include "BugTrapUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CStrStream::InitBuffer(void)
{
	m_pszData = NULL;
	m_nSize = m_nLength = 0;
}

/**
 * @param nSize - initial buffer size.
 */
void CStrStream::InitBuffer(size_t nSize)
{
	if (nSize)
	{
		m_nLength = 0;
		m_pszData = new TCHAR[nSize];
		if (m_pszData)
		{
			*m_pszData = _T('\0');
			m_nSize = nSize;
		}
		else
		{
			m_nSize = 0;
			RaiseException(STATUS_NO_MEMORY, 0, 0, NULL);
		}
	}
	else
		InitBuffer();
}

/**
 * @param nSize - requested buffer size.
 * @param bAdaptiveGrowth - true for adaptive growth.
 */
void CStrStream::EnsureSize(size_t nSize, bool bAdaptiveGrowth)
{
	if (m_nSize < nSize)
	{
		if (bAdaptiveGrowth)
			nSize *= 2;
		PTSTR pszData = new TCHAR[nSize];
		if (! pszData)
			RaiseException(STATUS_NO_MEMORY, 0, 0, NULL);
		if (m_pszData)
		{
			_tcscpy_s(pszData, nSize, m_pszData);
			delete[] m_pszData;
		}
		m_pszData = pszData;
		m_nSize = nSize;
	}
}


/**
 * @param pszStrData - data source.
 * @param nLength - string length.
 */
void CStrStream::InitData(PCTSTR pszStrData, size_t nLength)
{
	if (pszStrData && *pszStrData)
	{
		size_t nSize = nLength + 1;
		InitBuffer(nSize);
		_tcsncpy_s(m_pszData, nSize, pszStrData, nLength);
		m_nLength = nLength;
	}
	else
		InitBuffer();
}

/**
 * @param pszStrData - data source.
 * @param nLength - string length.
 */
void CStrStream::CopyData(PCTSTR pszStrData, size_t nLength)
{
	if (m_pszData != pszStrData)
	{
		if (pszStrData && *pszStrData)
		{
			size_t nSize = nLength + 1;
			EnsureSize(nSize, false);
			_tcsncpy_s(m_pszData, nSize, pszStrData, nLength);
			m_nLength = nLength;
		}
		else
			Reset();
	}
}

/**
 * @param pszStrData - data source.
 * @param nLength - string length.
 */
void CStrStream::AppendData(PCTSTR pszStrData, size_t nLength)
{
	if (pszStrData && *pszStrData)
	{
		size_t nNewLength = m_nLength + nLength;
		EnsureSize(nNewLength + 1, true);
		_tcsncpy_s(m_pszData + m_nLength, nLength + 1, pszStrData, nLength);
		m_nLength = nNewLength;
	}
}

#ifdef _UNICODE

/**
 * @param pszStrData - data source.
 */
void CStrStream::InitData(PCSTR pszStrData)
{
	if (pszStrData && *pszStrData)
	{
		int nSize = MultiByteToWideChar(CP_ACP, 0, pszStrData, -1, NULL, 0);
		_ASSERTE(nSize > 0);
		InitBuffer(nSize);
		MultiByteToWideChar(CP_ACP, 0, pszStrData, -1, m_pszData, nSize);
		m_nLength = nSize - 1;
	}
	else
		InitBuffer();
}

/**
 * @param pszStrData - data source.
 */
void CStrStream::CopyData(PCSTR pszStrData)
{
	if (pszStrData && *pszStrData)
	{
		int nSize = MultiByteToWideChar(CP_ACP, 0, pszStrData, -1, NULL, 0);
		_ASSERTE(nSize > 0);
		EnsureSize(nSize, false);
		MultiByteToWideChar(CP_ACP, 0, pszStrData, -1, m_pszData, nSize);
		m_nLength = nSize - 1;
	}
	else
		Reset();
}

/**
 * @param pszStrData - data source.
 */
void CStrStream::AppendData(PCSTR pszStrData)
{
	if (pszStrData && *pszStrData)
	{
		int nSize = MultiByteToWideChar(CP_ACP, 0, pszStrData, -1, NULL, 0);
		_ASSERTE(nSize > 0);
		size_t nNewSize = m_nLength + nSize;
		EnsureSize(nNewSize, true);
		MultiByteToWideChar(CP_ACP, 0, pszStrData, -1, m_pszData + m_nLength, nSize);
		m_nLength = nNewSize - 1;
	}
}

#else

/**
 * @param pszStrData - data source.
 */
void CStrStream::InitData(PCWSTR pszStrData)
{
	if (pszStrData && *pszStrData)
	{
		size_t nSize = WideCharToMultiByte(CP_ACP, 0, pszStrData, -1, NULL, 0, NULL, NULL);
		_ASSERTE(nSize > 0);
		InitBuffer(nSize);
		WideCharToMultiByte(CP_ACP, 0, pszStrData, -1, m_pszData, (int)nSize, NULL, NULL);
		m_nLength = nSize - 1;
	}
	else
		InitBuffer();
}

/**
 * @param pszStrData - data source.
 */
void CStrStream::CopyData(PCWSTR pszStrData)
{
	if (pszStrData && *pszStrData)
	{
		size_t nSize = WideCharToMultiByte(CP_ACP, 0, pszStrData, -1, NULL, 0, NULL, NULL);
		_ASSERTE(nSize > 0);
		EnsureSize(nSize, false);
		WideCharToMultiByte(CP_ACP, 0, pszStrData, -1, m_pszData, (int)nSize, NULL, NULL);
		m_nLength = nSize - 1;
	}
	else
		Reset();
}

/**
 * @param pszStrData - data source.
 */
void CStrStream::AppendData(PCWSTR pszStrData)
{
	if (pszStrData && *pszStrData)
	{
		size_t nSize = WideCharToMultiByte(CP_ACP, 0, pszStrData, -1, NULL, 0, NULL, NULL);
		_ASSERTE(nSize > 0);
		size_t nNewSize = m_nLength + nSize;
		EnsureSize(nNewSize, true);
		WideCharToMultiByte(CP_ACP, 0, pszStrData, -1, m_pszData + m_nLength, (int)nSize, NULL, NULL);
		m_nLength = nNewSize - 1;
	}
}

#endif

/**
 * @return object string buffer.
 * @note The caller is responsible to free returned string buffer.
 */
PTSTR CStrStream::Detach(void)
{
	PTSTR pszData = m_pszData;
	InitBuffer();
	return pszData;
}

void CStrStream::Reset(void)
{
	if (m_pszData)
	{
		*m_pszData = _T('\0');
		m_nLength = 0;
	}
}

void CStrStream::Free(void)
{
	if (m_pszData)
	{
		delete[] m_pszData;
		InitBuffer();
	}
}

/**
 * @param nPosition - start position.
 * @param nLength - length of the extracted string.
 */
void CStrStream::ValidateIndex(size_t nPosition, size_t& nLength) const
{
	_ASSERTE(nPosition <= m_nLength);
	if (nPosition > m_nLength)
		RaiseException(STATUS_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
	size_t nMaxLength = m_nLength - nPosition;
	if (nLength == MAXSIZE_T)
		nLength = nMaxLength;
	else
	{
		_ASSERTE(nLength <= nMaxLength);
		if (nLength > nMaxLength)
			RaiseException(STATUS_ARRAY_BOUNDS_EXCEEDED, 0, 0, NULL);
	}
}

/**
 * @param rStrHolder - string buffer that receives the data.
 * @param nPosition - start position.
 * @param nLength - length of the extracted string.
 */
void CStrStream::Substring(CStrHolder& rStrHolder, size_t nPosition, size_t nLength) const
{
	ValidateIndex(nPosition, nLength);
	size_t nEndPosition = nPosition + nLength;
	TCHAR chData = m_pszData[nEndPosition];
	m_pszData[nEndPosition] = _T('\0');
	rStrHolder = m_pszData + nPosition;
	m_pszData[nEndPosition] = chData;
}

/**
 * @param pszSubstring - inserted string buffer.
 * @param nPosition - start position.
 * @param nLength - length of the inserted string.
 */
void CStrStream::Insert(PCTSTR pszSubstring, size_t nPosition, size_t nLength)
{
	ValidateIndex(nPosition);
	size_t nNewLength = m_nLength + nLength;
	EnsureSize(nNewLength + 1, true);
	size_t nNextPosition = nPosition + nLength;
	size_t nNextLength = m_nLength - nPosition;
	MoveMemory(m_pszData + nNextPosition, m_pszData + nPosition, (nNextLength + 1) * sizeof(TCHAR));
	MoveMemory(m_pszData + nPosition, pszSubstring, nLength * sizeof(TCHAR));
	m_nLength = nNewLength;
}

/**
 * @param nPosition - start position.
 * @param nLength - length of the extracted string.
 */
void CStrStream::Delete(size_t nPosition, size_t nLength)
{
	ValidateIndex(nPosition, nLength);
	size_t nNewLength = m_nLength - nLength;
	MoveMemory(m_pszData + nPosition, m_pszData + nLength, (nNewLength + 1) * sizeof(TCHAR));
	m_nLength = nNewLength;
}

void CStrStream::Trim(void)
{
	TrimSpaces(m_pszData);
	m_nLength = _tcslen(m_pszData);
}
