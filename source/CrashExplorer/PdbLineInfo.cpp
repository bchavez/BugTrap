/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: PDB line information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "PdbLineInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPdbLineInfo::CPdbLineInfo() :
			m_ptrAddress(NULL),
			m_uNumber(0)
{
}

/**
 * @param ptrAddress - line address.
 * @param uNumber - line number.
 */
CPdbLineInfo::CPdbLineInfo(PVOID ptrAddress, UINT uNumber) :
			m_ptrAddress(ptrAddress),
			m_uNumber(uNumber)
{
}
