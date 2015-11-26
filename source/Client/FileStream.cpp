/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: File stream class.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "FileStream.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CHECK_FILE_HANDLE(eResult) \
	_ASSERTE(m_hFile != INVALID_HANDLE_VALUE); \
	if (m_hFile == INVALID_HANDLE_VALUE) \
	{ \
		m_lLastError = ERROR_INVALID_HANDLE_STATE; \
		return eResult; \
	}

#define CHECK_LAST_ERROR(eResult) \
	m_lLastError = GetLastError(); \
	if (m_lLastError != NOERROR) \
		return eResult;

/**
 * @param lLastError - last error code.
 */
void CFileStream::ResetFile(LONG lLastError)
{
	m_hFile = INVALID_HANDLE_VALUE;
	*m_szFileName = _T('\0');
	m_bEndOfFile = false;
	m_lLastError = lLastError;
}

/**
 * @param nBufferSize - buffer size.
 */
void CFileStream::InitBuffer(size_t nBufferSize)
{
	ResetBuffer();
	m_nBufferSize = nBufferSize;
	if (nBufferSize > 0)
	{
		m_pBuffer = new BYTE[nBufferSize];
		if (m_pBuffer == NULL)
		{
			m_nBufferSize = 0;
			RaiseException(STATUS_NO_MEMORY, 0, 0, NULL);
		}
	}
	else
		m_pBuffer = NULL;
}

/**
 * @param pszFileName - file name.
 * @param dwCreationDisposition - an action to take on files that exist and do not exist.
 * @param dwDesiredAccess - the access to the object, which can be read, write, or both.
 * @param dwShareMode - the sharing mode of an object, which can be read, write, both, or none.
 * @param dwFlagsAndAttributes - the file attributes and flags.
 */
bool CFileStream::Open(PCTSTR pszFileName, DWORD dwCreationDisposition, DWORD dwDesiredAccess, DWORD dwShareMode, DWORD dwFlagsAndAttributes)
{
	_ASSERTE(m_hFile == INVALID_HANDLE_VALUE);
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		m_lLastError = ERROR_INVALID_HANDLE_STATE;
		return false;
	}
	m_hFile = CreateFile(pszFileName, dwDesiredAccess, dwShareMode, NULL, dwCreationDisposition, dwFlagsAndAttributes, NULL);
	m_lLastError = GetLastError();
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		m_bEndOfFile = false;
		_tcscpy_s(m_szFileName, countof(m_szFileName), pszFileName);
		return true;
	}
	return false;
}

void CFileStream::Close(void)
{
	if (m_hFile != INVALID_HANDLE_VALUE)
	{
		FlushBuffer();
		CloseHandle(m_hFile);
		ResetFile(GetLastError());
	}
}

/**
 * @param pszName - stream name buffer.
 * @param nNameSize - size of stream name buffer.
 * @return true if name was retrieved.
 */
bool CFileStream::GetName(PTSTR pszName, size_t nNameSize) const
{
	_tcscpy_s(pszName, nNameSize, m_szFileName);
	m_lLastError = NOERROR;
	return true;
}

/**
 * @return number of bytes in the stream.
 */
size_t CFileStream::GetLength(void) const
{
	CHECK_FILE_HANDLE(MAXSIZE_T);
	DWORD dwFileSize = GetFileSize(m_hFile, NULL);
	CHECK_LAST_ERROR(MAXSIZE_T);
	_ASSERTE((INT)dwFileSize >= 0); // check 2GB limit
	return dwFileSize;
}

/**
 * @param nOffset - offset from start point.
 * @param nMoveMethod - start point.
 * @return new position value.
 */
size_t CFileStream::SetPosition(ptrdiff_t nOffset, int nMoveMethod)
{
	CHECK_FILE_HANDLE(MAXSIZE_T);
	m_bEndOfFile = false;
	size_t nDeltaPos = 0;
	if (m_eBufferType == BT_READ)
	{
		if (nMoveMethod == FILE_CURRENT)
			nDeltaPos = m_nBufferLength - m_nBufferPos;
		ResetBuffer();
	}
	else
	{
		if (! FlushBuffer())
			return MAXSIZE_T;
	}
	DWORD dwFilePos = SetFilePointer(m_hFile, (LONG)(nOffset - nDeltaPos), NULL, nMoveMethod);
	CHECK_LAST_ERROR(MAXSIZE_T);
	_ASSERTE((INT)dwFilePos >= 0); // check 2GB limit
	return dwFilePos;
}

/**
 * @return current stream position.
 */
size_t CFileStream::GetPosition(void) const
{
	CHECK_FILE_HANDLE(MAXSIZE_T);
	DWORD dwFilePos = SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
	CHECK_LAST_ERROR(MAXSIZE_T);
	_ASSERTE((INT)dwFilePos >= 0); // check 2GB limit
	size_t nDeltaPos = m_nBufferLength - m_nBufferPos;
	return (dwFilePos - nDeltaPos);
}

/**
 * @param nLength - new stream length.
 * @return new file length.
 */
size_t CFileStream::SetLength(size_t nLength)
{
	CHECK_FILE_HANDLE(MAXSIZE_T);
	size_t nDeltaPos;
	if (m_eBufferType == BT_READ)
	{
		nDeltaPos = m_nBufferLength - m_nBufferPos;
		ResetBuffer();
	}
	else
	{
		nDeltaPos = 0;
		if (! FlushBuffer())
			return MAXSIZE_T;
	}
	m_bEndOfFile = false;
	DWORD dwFilePos = SetFilePointer(m_hFile, 0, NULL, FILE_CURRENT);
	CHECK_LAST_ERROR(MAXSIZE_T);
	dwFilePos -= (DWORD)nDeltaPos;
	_ASSERTE((INT)dwFilePos >= 0); // check 2GB limit
	DWORD dwLength = (LONG)nLength;
	_ASSERTE(dwLength == nLength);
	SetFilePointer(m_hFile, dwLength, NULL, FILE_BEGIN);
	CHECK_LAST_ERROR(MAXSIZE_T);
	SetEndOfFile(m_hFile);
	CHECK_LAST_ERROR(MAXSIZE_T);
	if (dwFilePos > dwLength)
		dwFilePos = dwLength;
	SetFilePointer(m_hFile, dwFilePos, NULL, FILE_BEGIN);
	CHECK_LAST_ERROR(MAXSIZE_T);
	return nLength;
}

/**
 * @param arrBytes - array of bytes.
 * @param nCount - size of the array.
 * @return number of retrieved bytes.
 */
size_t CFileStream::ReadBytes(unsigned char* arrBytes, size_t nCount)
{
	CHECK_FILE_HANDLE(MAXSIZE_T);
	if (m_pBuffer != NULL)
	{
		if (! FlushBuffer())
			return MAXSIZE_T;
		size_t nTotalLength = 0;
		for (;;)
		{
			size_t nNumBytes = m_nBufferLength - m_nBufferPos;
			if (nNumBytes > nCount)
				nNumBytes = nCount;
			if (nNumBytes > 0)
			{
				CopyMemory(arrBytes, m_pBuffer + m_nBufferPos, nNumBytes);
				arrBytes += nNumBytes;
				nTotalLength += nNumBytes;
				nCount -= nNumBytes;
				m_nBufferPos += nNumBytes;
			}
			if (nCount == 0 || m_bEndOfFile)
				break;
			_ASSERTE(m_nBufferPos == m_nBufferLength);
			DWORD dwNumRead = 0;
			ReadFile(m_hFile, m_pBuffer, (DWORD)m_nBufferSize, &dwNumRead, NULL);
			m_lLastError = GetLastError();
			m_nBufferPos = 0;
			m_nBufferLength = dwNumRead;
			m_eBufferType = m_nBufferLength > 0 ? BT_READ : BT_EMPTY;
			m_bEndOfFile = m_nBufferLength < m_nBufferSize;
			if (m_lLastError != NOERROR)
				break;
		}
		return nTotalLength;
	}
	else
	{
		DWORD dwNumRead = 0;
		ReadFile(m_hFile, arrBytes, (DWORD)nCount, &dwNumRead, NULL);
		m_lLastError = GetLastError();
		return dwNumRead;
	}
}

/**
 * @param arrBytes - array of bytes.
 * @param nCount - size of the array.
 * @return number of written bytes.
 */
size_t CFileStream::WriteBytes(const unsigned char* arrBytes, size_t nCount)
{
	CHECK_FILE_HANDLE(MAXSIZE_T);
	if (m_pBuffer != NULL)
	{
		if (! SynchronizeBuffer())
			return MAXSIZE_T;
		size_t nTotalLength = 0;
		for (;;)
		{
			size_t nNumBytes = m_nBufferSize - m_nBufferLength;
			if (nNumBytes > nCount)
				nNumBytes = nCount;
			if (nNumBytes > 0)
			{
				m_eBufferType = BT_WRITE;
				CopyMemory(m_pBuffer + m_nBufferLength, arrBytes, nNumBytes);
				arrBytes += nNumBytes;
				nTotalLength += nNumBytes;
				nCount -= nNumBytes;
				m_nBufferLength += nNumBytes;
			}
			if (nCount == 0)
				break;
			_ASSERTE(m_nBufferLength == m_nBufferSize);
			DWORD dwNumWritten = 0;
			WriteFile(m_hFile, m_pBuffer, (DWORD)m_nBufferSize, &dwNumWritten, NULL);
			m_lLastError = GetLastError();
			ResetBuffer();
			if (m_lLastError != NOERROR)
				break;
		}
		return nTotalLength;
	}
	else
	{
		DWORD dwNumWritten = 0;
		WriteFile(m_hFile, arrBytes, (DWORD)nCount, &dwNumWritten, NULL);
		m_lLastError = GetLastError();
		return dwNumWritten;
	}
}

/**
 * @param nBufferSize - buffer size.
 */
void CFileStream::SetBufferSize(size_t nBufferSize)
{
	if (m_nBufferSize != nBufferSize)
	{
		if (! FlushBuffer())
			return;
		delete[] m_pBuffer;
		InitBuffer(nBufferSize);
	}
}

void CFileStream::ResetBuffer(void)
{
	m_eBufferType = BT_EMPTY;
	m_nBufferPos = 0;
	m_nBufferLength = 0;
}

/**
 * @return true if file pointer has been synchronized.
 */
bool CFileStream::SynchronizeBuffer(void)
{
	if (m_eBufferType == BT_READ)
	{
		_ASSERTE(m_hFile != INVALID_HANDLE_VALUE);
		if (m_nBufferPos < m_nBufferLength)
		{
			SetFilePointer(m_hFile, (LONG)(m_nBufferPos - m_nBufferLength), NULL, FILE_CURRENT);
			CHECK_LAST_ERROR(false);
		}
		ResetBuffer();
	}
	return true;
}

/**
 * @return true if buffer has been be flushed.
 */
bool CFileStream::FlushBuffer(void)
{
	if (m_eBufferType == BT_WRITE)
	{
		_ASSERTE(m_hFile != INVALID_HANDLE_VALUE);
		if (m_nBufferLength > 0)
		{
			DWORD dwNumWritten = 0;
			WriteFile(m_hFile, m_pBuffer, (DWORD)m_nBufferLength, &dwNumWritten, NULL);
			CHECK_LAST_ERROR(false);
		}
		ResetBuffer();
	}
	return true;
}
