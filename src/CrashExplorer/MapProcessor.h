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

#pragma once

#include "BaseProcessor.h"
#include "MapFnInfo.h"
#include "MapLineInfo.h"
#include "MapFileData.h"

/// Map file processor.
class CMapProcessor : public CBaseProcessor
{
	DECLARE_RUNTIME_CLASS(CMapProcessor)
public:
	/// Initialize the object.
	CMapProcessor();
	/// Get base address.
	PVOID GetBaseAddress() const;
	/// Set base address.
	void SetBaseAddress(PVOID ptrBaseAddress);
	/// Set map text.
	void SetMapText(const std::string& strMapText);
	/// Set map text.
	void SetMapText(PCSTR pszMapText);
	/// Set map text.
	void SetMapText(const CHAR* pchMapText, UINT uMapTextLength);
	/// Load map text from the file.
	void LoadMapText(PCTSTR pszFileName);
	/// Get map text.
	const std::string& GetMapText() const;
	/// Get module preferred base address.
	PVOID GetPreferredBaseAddress() const;
	/// Retrieve the list of public functions.
	boost::shared_ptr< std::list<CMapFnInfo> > GetPublicFunctions() const;
	/// Find public function info by function address.
	bool FindFunctionInfo(PVOID ptrAddress, CMapFnInfo& rFnInfo, DWORD64& dwDisplacement64) const;
	/// Find public function info by function address.
	virtual bool FindFunctionInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFnInfo>& pFnInfo, DWORD64& dwDisplacement64) const;
	/// Retrieve the list of source files.
	boost::shared_ptr< std::list<CMapFileData> > GetFilesList() const;
	/// Find line info by line address.
	bool FindLineInfo(PVOID ptrAddress, CMapFileInfo& rFileInfo, CMapLineInfo& rLineInfo, DWORD& dwDisplacement32) const;
	/// Find line info by line address.
	virtual bool FindLineInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFileInfo>& pFileInfo, boost::shared_ptr<CBaseLineInfo>& pLineInfo, DWORD& dwDisplacement32) const;
	/// Get image offset.
	PVOID GetImageOffset(PVOID ptrAddress) const;
	/// Get image address.
	PVOID GetImageAddress(PVOID ptrOffset) const;

private:
	/// Clear data.
	void Clear();
	/// Compare function info blocks.
	static bool CompareFns(const CMapFnInfo& fn1, const CMapFnInfo& fn2);
	/// Compare line info blocks.
	static bool CompareLns(const CMapLineInfo& ln1, const CMapLineInfo& ln2);
	/// Base address.
	PVOID m_ptrBaseAddress;
	/// Map file text.
	std::string m_strMapText;
	/// Preferred base address.
	mutable PVOID m_ptrPreferredBaseAddress;
	/// List of public functions.
	mutable boost::shared_ptr<std::list<CMapFnInfo> > m_plstPubFns;
	/// List of source files.
	mutable boost::shared_ptr<std::list<CMapFileData> > m_plstFilesList;
};

inline CMapProcessor::CMapProcessor()
{
	Clear();
}

/**
 * @return map file text.
 */
inline const std::string& CMapProcessor::GetMapText() const
{
	return m_strMapText;
}

/**
 * @param fn1 - 1st function info block.
 * @param fn2 - 2nd function info block.
 * @return true if 1st function is logically less than 2nd function.
 */
inline bool CMapProcessor::CompareFns(const CMapFnInfo& fn1, const CMapFnInfo& fn2)
{
	return (fn1.GetAddress() < fn2.GetAddress());
}

/**
 * @param ln1 - 1st line info block.
 * @param ln2 - 2nd line info block.
 * @return true if 1st line is logically less than 2nd line.
 */
inline bool CMapProcessor::CompareLns(const CMapLineInfo& ln1, const CMapLineInfo& ln2)
{
	return (ln1.GetAddress() < ln2.GetAddress());
}

/**
 * @param ptrAddress - physical address.
 * @return image offset.
 */
inline PVOID CMapProcessor::GetImageOffset(PVOID ptrAddress) const
{
	return (PVOID)((INT_PTR)ptrAddress - (INT_PTR)m_ptrBaseAddress - (INT_PTR)0x1000);
}

/**
 * @param ptrOffset - relative offset.
 * @return image address.
 */
inline PVOID CMapProcessor::GetImageAddress(PVOID ptrOffset) const
{
	return (PVOID)((INT_PTR)ptrOffset + (INT_PTR)m_ptrBaseAddress + (INT_PTR)0x1000);
}

/**
 * @return module base address.
 */
inline PVOID CMapProcessor::GetBaseAddress() const
{
	return m_ptrBaseAddress;
}

/**
 * @param ptrBaseAddress - module base address.
 */
inline void CMapProcessor::SetBaseAddress(PVOID ptrBaseAddress)
{
	m_ptrBaseAddress = ptrBaseAddress;
}
