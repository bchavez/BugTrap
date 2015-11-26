/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: MAP line information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "MapLineInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMapLineInfo::CMapLineInfo() :
			m_ptrAddress(NULL),
			m_uSection(0),
			m_ptrOffset(NULL),
			m_uLineNumber(0)
{
}

/**
 * @param ptrAddress - line address.
 * @param uSection - code section.
 * @param ptrOffset - line offset.
 * @param uLineNumber - line number.
 */
CMapLineInfo::CMapLineInfo(PVOID ptrAddress, UINT uSection, PVOID ptrOffset, UINT uLineNumber) :
			m_ptrAddress(ptrAddress),
			m_uSection(uSection),
			m_ptrOffset(ptrOffset),
			m_uLineNumber(uLineNumber)
{
}
