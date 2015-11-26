/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Map file processor.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "MapProcessor.h"
#include "ComError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_RUNTIME_CLASS(CMapProcessor, CBaseProcessor)

void CMapProcessor::Clear()
{
	m_ptrPreferredBaseAddress = NULL;
	m_ptrBaseAddress = NULL;
	m_plstPubFns.reset();
	m_plstFilesList.reset();
	m_strMapText.clear();
}

/**
 * @param strMapText - map file text.
 */
void CMapProcessor::SetMapText(const std::string& strMapText)
{
	Clear();
	m_strMapText.assign(strMapText);
	m_ptrBaseAddress = GetPreferredBaseAddress();
}

/**
 * @param pszMapText - map file text.
 */
void CMapProcessor::SetMapText(PCSTR pszMapText)
{
	Clear();
	m_strMapText.assign(pszMapText);
	m_ptrBaseAddress = GetPreferredBaseAddress();
}

/**
 * @param pchMapText - map file text.
 * @param uMapTextLength - length of map file text.
 */
void CMapProcessor::SetMapText(const CHAR* pchMapText, UINT uMapTextLength)
{
	Clear();
	m_strMapText.assign(pchMapText, uMapTextLength);
	m_ptrBaseAddress = GetPreferredBaseAddress();
}

/**
 * @return module preferred base address.
 */
PVOID CMapProcessor::GetPreferredBaseAddress() const
{
	if (m_ptrPreferredBaseAddress == NULL && ! m_strMapText.empty())
	{
		boost::regex expression("Preferred load address is ([[:xdigit:]]+)");
		boost::match_results<std::string::const_iterator> what;
		if (boost::regex_search(m_strMapText, what, expression))
		{
			std::string strPreferredBaseAddress(what[1]);
			m_ptrPreferredBaseAddress = (PVOID)_strtoui64(strPreferredBaseAddress.c_str(), NULL, 16);
		}
	}
	return m_ptrPreferredBaseAddress;
}

/**
 * @return list of public functions.
 */
boost::shared_ptr< std::list<CMapFnInfo> > CMapProcessor::GetPublicFunctions() const
{
	if (! m_plstPubFns && ! m_strMapText.empty())
	{
		m_plstPubFns.reset(new std::list<CMapFnInfo>);
		boost::match_results<std::string::const_iterator> what;
		std::string::const_iterator end = m_strMapText.end();
		/* 0001:00019df3 Function 1001adf3 f Library:File.obj */
		boost::regex expressionEntry("\\A *([[:xdigit:]]+):([[:xdigit:]]+) +(\\S+) +([[:xdigit:]]+)(?: f)?(?: i)? +(?:(\\S+):)?(\\S+)\r\n");

		boost::regex expressionGlobals("Address +Publics by Value +Rva\\+Base +Lib:Object\r\n\r\n");
		if (boost::regex_search(m_strMapText, what, expressionGlobals))
		{
			std::string::const_iterator start = what[0].second;
			while (boost::regex_search(start, end, what, expressionEntry))
			{
				UINT uSection = strtoul(what[1].str().c_str(), NULL, 16);
				PVOID ptrOffset = (PVOID)_strtoui64(what[2].str().c_str(), NULL, 16);
				const std::string& strFunction = what[3].str();
				PVOID ptrRvaBase = (PVOID)_strtoui64(what[4].str().c_str(), NULL, 16);
				const std::string& strLibrary = what[5].str();
				const std::string& strObject = what[6].str();
				PVOID ptrAddress = (PVOID)((INT_PTR)ptrRvaBase - (INT_PTR)m_ptrPreferredBaseAddress + (INT_PTR)m_ptrBaseAddress);
				CMapFnInfo fnInfo(ptrAddress, uSection, ptrOffset, strFunction, ptrRvaBase, strLibrary, strObject);
				m_plstPubFns->push_back(fnInfo);
				start = what[0].second;
			}
		}

		boost::regex expressionStatic("Static symbols\r\n\r\n");
		if (boost::regex_search(m_strMapText, what, expressionStatic))
		{
			std::string::const_iterator start = what[0].second;
			while (boost::regex_search(start, end, what, expressionEntry))
			{
				UINT uSection = strtoul(what[1].str().c_str(), NULL, 16);
				PVOID ptrOffset = (PVOID)_strtoui64(what[2].str().c_str(), NULL, 16);
				const std::string& strFunction = what[3].str();
				PVOID ptrRvaBase = (PVOID)_strtoui64(what[4].str().c_str(), NULL, 16);
				const std::string& strLibrary = what[5].str();
				const std::string& strObject = what[6].str();
				PVOID ptrAddress = (PVOID)((INT_PTR)ptrRvaBase - (INT_PTR)m_ptrPreferredBaseAddress + (INT_PTR)m_ptrBaseAddress);
				CMapFnInfo fnInfo(ptrAddress, uSection, ptrOffset, strFunction, ptrRvaBase, strLibrary, strObject);
				m_plstPubFns->push_back(fnInfo);
				start = what[0].second;
			}
		}

		m_plstPubFns->sort(&CompareFns);
	}
	return m_plstPubFns;
}

/**
 * @param ptrAddress - function address.
 * @param rFnInfo - pointer to function info block.
 * @param dwDisplacement64 - symbol displacement.
 * @return true if function info has been found.
 */
bool CMapProcessor::FindFunctionInfo(PVOID ptrAddress, CMapFnInfo& rFnInfo, DWORD64& dwDisplacement64) const
{
	boost::shared_ptr< std::list<CMapFnInfo> > plstPubFns(GetPublicFunctions());
	if (plstPubFns.get() && ! plstPubFns->empty())
	{
		std::string strEmpty;
		CMapFnInfo fnKey(ptrAddress, 0u, NULL, strEmpty, NULL, strEmpty, strEmpty);
		std::list<CMapFnInfo>::iterator itFirstFn = plstPubFns->begin();
		std::list<CMapFnInfo>::iterator itLastFn = plstPubFns->end();
		std::list<CMapFnInfo>::iterator itNextFn = std::upper_bound(itFirstFn, itLastFn, fnKey, &CompareFns);
		if (itNextFn != itFirstFn)
		{
			--itNextFn;
			rFnInfo = *itNextFn;
			dwDisplacement64 = (DWORD64)((INT_PTR)ptrAddress - (INT_PTR)rFnInfo.GetAddress());
			return true;
		}
	}
	rFnInfo = CMapFnInfo();
	dwDisplacement64 = 0;
	return false;
}

/**
 * @return list of source files.
 */
boost::shared_ptr< std::list<CMapFileData> > CMapProcessor::GetFilesList() const
{
	if (! m_plstFilesList && ! m_strMapText.empty())
	{
		m_plstFilesList.reset(new std::list<CMapFileData>);
		boost::match_results<std::string::const_iterator> what;
		std::string::const_iterator start = m_strMapText.begin();
		std::string::const_iterator end = m_strMapText.end();
		/* 140 0001:00010788   141 0001:0001078b   142 0001:00010798   143 0001:0001079b */
		boost::regex expressionLine("\\A *(\\d+) ([[:xdigit:]]+):([[:xdigit:]]+)\\s+");
		/* Line numbers for C:\Path1\Library.lib(D:\Path2\File.c) segment .text */
		boost::regex expressionFile("Line numbers for ([^\\(]+)\\(([^\\)]+)\\) segment (\\S+)\r\n\r\n");
		while (boost::regex_search(start, end, what, expressionFile))
		{
			CMapFileInfo fileInfo(what[1].str(), what[2].str(), what[3].str());
			m_plstFilesList->push_back(fileInfo);
			std::list<CMapLineInfo>& rLinesInfo = m_plstFilesList->back().GetLines();
			start = what[0].second;
			while (boost::regex_search(start, end, what, expressionLine))
			{
				UINT uLineNumber = atol(what[1].str().c_str());
				UINT uSection = strtoul(what[2].str().c_str(), NULL, 16);
				PVOID ptrOffset = (PVOID)_strtoui64(what[3].str().c_str(), NULL, 16);
				PVOID ptrAddress = GetImageAddress(ptrOffset);
				CMapLineInfo lineInfo(ptrAddress, uSection, ptrOffset, uLineNumber);
				rLinesInfo.push_back(lineInfo);
				start = what[0].second;
			}
			rLinesInfo.sort(CompareLns);
		}
	}
	return m_plstFilesList;
}

/**
 * @param ptrAddress - line address.
 * @param rFileInfo - pointer to file information.
 * @param rLineInfo - pointer to line number information.
 * @param dwDisplacement32 - symbol displacement.
 * @return true if line was found.
 */
bool CMapProcessor::FindLineInfo(PVOID ptrAddress, CMapFileInfo& rFileInfo, CMapLineInfo& rLineInfo, DWORD& dwDisplacement32) const
{
	const CMapFileInfo* pFileInfo = NULL;
	const CMapLineInfo* pLineInfo = NULL;
	dwDisplacement32 = 0;
	boost::shared_ptr< std::list<CMapFileData> > plstFilesList(GetFilesList());
	if (plstFilesList.get() && ! plstFilesList->empty())
	{
		CMapLineInfo lnKey(ptrAddress, 0u, NULL, 0u);
		std::list<CMapFileData>::iterator itFirstFile = plstFilesList->begin();
		std::list<CMapFileData>::iterator itLastFile = plstFilesList->end();
		for (std::list<CMapFileData>::iterator itFile = itFirstFile; itFile != itLastFile; ++itFile)
		{
			std::list<CMapLineInfo>& rLines = itFile->GetLines();
			if (! rLines.empty() && rLines.front().GetAddress() <= ptrAddress && ptrAddress <= rLines.back().GetAddress())
			{
				std::list<CMapLineInfo>::iterator itFirstLn = rLines.begin();
				std::list<CMapLineInfo>::iterator itLastLn = rLines.end();
				std::list<CMapLineInfo>::iterator itNextLn = std::upper_bound(itFirstLn, itLastLn, lnKey, CompareLns);
				if (itNextLn != itFirstLn)
				{
					--itNextLn;
					if (pLineInfo == NULL || itNextLn->GetAddress() > pLineInfo->GetAddress())
					{
						pFileInfo = &*itFile;
						pLineInfo = &*itNextLn;
						dwDisplacement32 = (DWORD)((INT_PTR)ptrAddress - (INT_PTR)pLineInfo->GetAddress());
					}
				}
			}
		}
	}
	if (pFileInfo != NULL && pLineInfo != NULL)
	{
		rFileInfo = *pFileInfo;
		rLineInfo = *pLineInfo;
		return true;
	}
	else
	{
		rFileInfo = CMapFileInfo();
		rLineInfo = CMapLineInfo();
		return false;
	}
}

/**
 * @param pszFileName - MAP file name.
 */
void CMapProcessor::LoadMapText(PCTSTR pszFileName)
{
	Clear();
	CAtlFile fileMap;
	HRESULT hRes = fileMap.Create(pszFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
	CHECK_HRESULT(hRes);
	ULONGLONG ullFileSize;
	fileMap.GetSize(ullFileSize);
	UINT uFileLength = (UINT)ullFileSize;
	boost::scoped_array<CHAR> pchMapBuffer(new CHAR[uFileLength]);
	fileMap.Read(pchMapBuffer.get(), uFileLength);
	SetMapText(pchMapBuffer.get(), uFileLength);
}

/**
 * @param ptrAddress - function address.
 * @param dwDisplacement64 - symbol displacement.
 * @param pFnInfo - pointer to function info block.
 * @return true if function info has been found.
 */
bool CMapProcessor::FindFunctionInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFnInfo>& pFnInfo, DWORD64& dwDisplacement64) const
{
	CMapFnInfo FnInfo;
	if (FindFunctionInfo(ptrAddress, FnInfo, dwDisplacement64))
	{
		pFnInfo.reset(new CMapFnInfo(FnInfo));
		return true;
	}
	else
	{
		pFnInfo.reset();
		return false;
	}
}

/**
 * @param ptrAddress - line address.
 * @param pFileInfo - pointer to file information.
 * @param pLineInfo - pointer to line number information.
 * @param dwDisplacement32 - symbol displacement.
 * @return true if line was found.
 */
bool CMapProcessor::FindLineInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFileInfo>& pFileInfo, boost::shared_ptr<CBaseLineInfo>& pLineInfo, DWORD& dwDisplacement32) const
{
	CMapFileInfo FileInfo;
	CMapLineInfo LineInfo;
	if (FindLineInfo(ptrAddress, FileInfo, LineInfo, dwDisplacement32))
	{
		pFileInfo.reset(new CMapFileInfo(FileInfo));
		pLineInfo.reset(new CMapLineInfo(LineInfo));
		return true;
	}
	else
	{
		pFileInfo.reset();
		pLineInfo.reset();
		return false;
	}
}
