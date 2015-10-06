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

#include "BaseFileInfo.h"

/// Source file information.
class CMapFileInfo : public CBaseFileInfo
{
public:
	/// Initialize empty object.
	CMapFileInfo();
	/// Initialize the object.
	CMapFileInfo(const std::string& strLibraryFileName,
				 const std::string& strSourceFileName,
				 const std::string& strSegmentName);
	/**
	 * @brief Get library file name.
	 * @return library file name.
	 */
	const std::string& GetLibraryFileName() const { return m_strLibraryFileName; }
	/**
	 * @brief Get source file name.
	 * @return source file name.
	 */
	const std::string& GetSourceFileName() const { return m_strSourceFileName; }
	/**
	 * @brief Get code segment name.
	 * @return code segment name.
	 */
	const std::string& GetSegmentName() const { return m_strSegmentName; }
	/**
	 * @brief Get source file name.
	 * @return source file name.
	 */
	virtual const std::string& GetFileName() const { return m_strSourceFileName; }

private:
	/// Library file name.
	std::string m_strLibraryFileName;
	/// Source file name.
	std::string m_strSourceFileName;
	/// Code segment name.
	std::string m_strSegmentName;
};

inline CMapFileInfo::CMapFileInfo()
{
}
