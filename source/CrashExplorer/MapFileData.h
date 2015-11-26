/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Source file information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "MapFileInfo.h"
#include "MapLineInfo.h"

/// Source file information.
class CMapFileData : public CMapFileInfo
{
public:
	/// Initialize empty object.
	CMapFileData();
	/// Initialize the object.
	CMapFileData(const CMapFileInfo& rMapFileInfo);
	/// Initialize the object.
	CMapFileData(const std::string& strLibraryFileName,
				 const std::string& strSourceFileName,
				 const std::string& strSegmentName);
	/**
	 * @brief Get list of line info blocks.
	 * @return list of line info blocks.
	 */
	std::list<CMapLineInfo>& GetLines() { return m_lstLines; }
	/**
	 * @brief Get list of line info blocks.
	 * @return list of line info blocks.
	 */
	const std::list<CMapLineInfo>& GetLines() const { return m_lstLines; }

private:
	/// List of line info blocks.
	std::list<CMapLineInfo> m_lstLines;
};

inline CMapFileData::CMapFileData()
{
}
