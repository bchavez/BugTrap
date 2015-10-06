/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: MAP function information.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "MapFnInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMapFnInfo::CMapFnInfo() :
			m_ptrAddress(NULL),
			m_uSection(0),
			m_ptrOffset(NULL),
			m_ptrRvaBase(NULL)
{
}

/**
 * @param ptrAddress - function address.
 * @param uSection - section data.
 * @param ptrOffest - function offset.
 * @param strFunction - decorated function name.
 * @param ptrRvaBase - RVA base address.
 * @param strLibrary - library name.
 * @param strObject - object file name.
 */
CMapFnInfo::CMapFnInfo(PVOID ptrAddress,
					   UINT uSection,
					   PVOID ptrOffest,
					   const std::string& strFunction,
					   PVOID ptrRvaBase,
					   const std::string& strLibrary,
					   const std::string& strObject) :
			m_ptrAddress(ptrAddress),
			m_uSection(uSection),
			m_ptrOffset(ptrOffest),
			m_strFunction(strFunction),
			m_ptrRvaBase(ptrRvaBase),
			m_strLibrary(strLibrary),
			m_strObject(strObject)
{
}

/**
 * @return undecorated function name.
 */
std::string CMapFnInfo::GetUndecoratedFunctionName() const
{
/*
 * Name decoration usually refers to C++ naming conventions, but can apply
 * to a number of C cases as well. By default, C++ uses the function name,
 * parameters, and return type to create a linker name for the function.
 * Consider the following function:
 *
 * void CALLTYPE test(void)
 *
 * The following table shows the linker name for various calling conventions.
 *
 *  Calling convention                          | extern "C" or .c file | .cpp, .cxx or /TP
 * ---------------------------------------------+-----------------------+-------------------
 *  C naming convention (__cdecl)               | _test                 | ?test@@ZAXXZ
 *  Fast-call naming convention (__fastcall)    | @test@0               | ?test@@YIXXZ
 *  Standard Call naming convention (__stdcall) | _test@0               | ?test@@YGXXZ
 */
	char chFirstChar = m_strFunction[0];
	if (chFirstChar == '_' || chFirstChar == '@')
	{
		std::string strUndecoratedName, strDecoratedName = m_strFunction.substr(1);
		size_t nAtPos = strDecoratedName.find('@');
		if (nAtPos != std::string::npos)
		{
			strDecoratedName.resize(nAtPos);
			if (chFirstChar == '_')
				strUndecoratedName = "__stdcall ";
			else /* if (chFirstChar == '@') */
				strUndecoratedName = "__fastcall ";
		}
		else
			strUndecoratedName = "__cdecl ";
		strUndecoratedName += strDecoratedName;
		strUndecoratedName += "()";
		return strUndecoratedName;
	}
	else
	{
		CHAR szNameBuffer[1024];
		if (UnDecorateSymbolName(m_strFunction.c_str(), szNameBuffer, countof(szNameBuffer), UNDNAME_COMPLETE))
			return std::string(szNameBuffer);
		else
			return m_strFunction;
	}
}
