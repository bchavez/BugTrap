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

#include "StdAfx.h"
#include "MapFileInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
 * @param strLibraryFileName - library file name.
 * @param strSourceFileName - source file name.
 * @param strSegmentName - code segment name.
 */
CMapFileInfo::CMapFileInfo(const std::string& strLibraryFileName,
						  const std::string& strSourceFileName,
						  const std::string& strSegmentName) :
			m_strLibraryFileName(strLibraryFileName),
			m_strSourceFileName(strSourceFileName),
			m_strSegmentName(strSegmentName)
{
}
