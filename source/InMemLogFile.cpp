/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Base class for in-memory log file.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "InMemLogFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @param dwInitialLogSizeInBytes - initial log file size.
 */
CInMemLogFile::CInMemLogFile(DWORD dwInitialLogSizeInBytes)
{
	m_dwLogSizeInEntries = MAXDWORD;
	m_dwLogSizeInBytes = MAXDWORD;
	m_dwNumEntries = 0;
	m_dwInitialLogSizeInBytes = dwInitialLogSizeInBytes;
	m_dwNumBytes = dwInitialLogSizeInBytes;
	m_pFirstEntry = NULL;
	m_pLastEntry = NULL;
}

/**
 * @param pLogEntry - new log entry.
 */
void CInMemLogFile::AddToHead(CLogEntry* pLogEntry)
{
	_ASSERTE(pLogEntry != NULL);
	pLogEntry->m_pPrevEntry = NULL;
	pLogEntry->m_pNextEntry = m_pFirstEntry;
	if (m_pFirstEntry)
		m_pFirstEntry->m_pPrevEntry = pLogEntry;
	else
		m_pLastEntry = pLogEntry;
	m_pFirstEntry = pLogEntry;
	m_dwNumBytes += pLogEntry->m_dwSize;
	++m_dwNumEntries;
	FreeTail();
}

/**
 * @param pLogEntry - new log entry.
 */
void CInMemLogFile::AddToTail(CLogEntry* pLogEntry)
{
	_ASSERTE(pLogEntry != NULL);
	pLogEntry->m_pPrevEntry = m_pLastEntry;
	pLogEntry->m_pNextEntry = NULL;
	if (m_pLastEntry)
		m_pLastEntry->m_pNextEntry = pLogEntry;
	else
		m_pFirstEntry = pLogEntry;
	m_pLastEntry = pLogEntry;
	m_dwNumBytes += pLogEntry->m_dwSize;
	++m_dwNumEntries;
	FreeHead();
}

void CInMemLogFile::DeleteHead(void)
{
	_ASSERTE(m_pFirstEntry != NULL);
	CLogEntry* pLogEntry = m_pFirstEntry;
	m_pFirstEntry = pLogEntry->m_pNextEntry;
	if (m_pFirstEntry)
		m_pFirstEntry->m_pPrevEntry = NULL;
	else
		m_pLastEntry = NULL;
	m_dwNumBytes -= pLogEntry->m_dwSize;
	--m_dwNumEntries;
	delete[] (PBYTE)pLogEntry;
}

void CInMemLogFile::DeleteTail(void)
{
	_ASSERTE(m_pLastEntry != NULL);
	CLogEntry* pLogEntry = m_pLastEntry;
	m_pLastEntry = pLogEntry->m_pPrevEntry;
	if (m_pLastEntry)
		m_pLastEntry->m_pNextEntry = NULL;
	else
		m_pFirstEntry = NULL;
	m_dwNumBytes -= pLogEntry->m_dwSize;
	--m_dwNumEntries;
	delete[] (PBYTE)pLogEntry;
}

void CInMemLogFile::FreeHead(void)
{
	if (m_dwLogSizeInEntries != MAXDWORD)
	{
		while (m_dwNumEntries > m_dwLogSizeInEntries && m_dwNumEntries > 0)
			DeleteHead();
	}
	if (m_dwLogSizeInBytes != MAXDWORD)
	{
		while (m_dwNumBytes > m_dwLogSizeInBytes && m_dwNumEntries > 0)
			DeleteHead();
	}
}

void CInMemLogFile::FreeTail(void)
{
	if (m_dwLogSizeInEntries != MAXDWORD)
	{
		while (m_dwNumEntries > m_dwLogSizeInEntries && m_dwNumEntries > 0)
			DeleteTail();
	}
	if (m_dwLogSizeInBytes != MAXDWORD)
	{
		while (m_dwNumBytes > m_dwLogSizeInBytes && m_dwNumEntries > 0)
			DeleteTail();
	}
}

void CInMemLogFile::FreeEntries(void)
{
	while (m_pFirstEntry)
	{
		CLogEntry* pNextEntry = m_pFirstEntry->m_pNextEntry;
		delete[] (PBYTE)m_pFirstEntry;
		m_pFirstEntry = pNextEntry;
	}
	m_pLastEntry = NULL;
	m_dwNumEntries = 0;
	m_dwNumBytes = m_dwInitialLogSizeInBytes;
}
