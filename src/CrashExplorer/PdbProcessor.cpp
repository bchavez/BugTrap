/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Pdb file processor.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "PdbProcessor.h"
#include "ComError.h"
#include "WinVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_RUNTIME_CLASS(CPdbProcessor, CBaseProcessor)

/**
 * @param pszSearchPath - search path for loaded modules.
 */
CPdbProcessor::CPdbProcessor(PCTSTR pszSearchPath)
{
	DWORD dwOptions = SymGetOptions();
	SymSetOptions(dwOptions | SYMOPT_DEFERRED_LOADS | SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
	m_hSymProcess = (HANDLE)1;
	if (! SymInitialize(m_hSymProcess, CT2A(pszSearchPath), FALSE))
	{
		m_hSymProcess = NULL;
		CHECK_WIN32RESULT(GetLastError());
	}
}

CPdbProcessor::~CPdbProcessor()
{
	if (m_hSymProcess != NULL)
		SymCleanup(m_hSymProcess);
}

/**
 * @param pszModuleName - module file name.
 * @param pBaseAddr - module base address.
 * @param dwModuleSize - module size.
 */
void CPdbProcessor::LoadModule(PCTSTR pszModuleName, PVOID pBaseAddr, DWORD dwModuleSize)
{
	_ASSERTE(m_hSymProcess != NULL);
	if (! SymLoadModule64(m_hSymProcess, NULL, CT2A(pszModuleName), NULL, (DWORD64)pBaseAddr, dwModuleSize))
		CHECK_WIN32RESULT(GetLastError());
}

/**
 * @param ptrAddress - function address.
 * @param rFnInfo - reference to function info block.
 * @param dwDisplacement64 - symbol displacement.
 * @return true if function info has been found.
 */
bool CPdbProcessor::FindFunctionInfo(PVOID ptrAddress, CPdbFnInfo& rFnInfo, DWORD64& dwDisplacement64) const
{
	_ASSERTE(m_hSymProcess != NULL);
	BYTE arrSymBuffer[512];
	ZeroMemory(arrSymBuffer, sizeof(arrSymBuffer));
	PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)arrSymBuffer;
	pSymbol->SizeOfStruct = sizeof(*pSymbol);
	pSymbol->MaxNameLen = sizeof(arrSymBuffer) - sizeof(*pSymbol) + 1;
	if (SymFromAddr(m_hSymProcess, (DWORD64)ptrAddress, &dwDisplacement64, pSymbol))
	{
		rFnInfo = CPdbFnInfo((PVOID)pSymbol->Address, std::string(pSymbol->Name));
		return true;
	}
	else
	{
		rFnInfo = CPdbFnInfo();
		dwDisplacement64 = 0;
		return false;
	}
}

/**
 * @param ptrAddress - function address.
 * @param dwDisplacement64 - symbol displacement.
 * @param pFnInfo - pointer to function info block.
 * @return true if function info has been found.
 */
bool CPdbProcessor::FindFunctionInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFnInfo>& pFnInfo, DWORD64& dwDisplacement64) const
{
	_ASSERTE(m_hSymProcess != NULL);
	CPdbFnInfo FnInfo;
	if (FindFunctionInfo(ptrAddress, FnInfo, dwDisplacement64))
	{
		pFnInfo.reset(new CPdbFnInfo(FnInfo));
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
 * @param rFileInfo - pointer to file information.
 * @param rLineInfo - pointer to line number information.
 * @param dwDisplacement32 - symbol displacement.
 * @return true if line was found.
 */
bool CPdbProcessor::FindLineInfo(PVOID ptrAddress, CPdbFileInfo& rFileInfo, CPdbLineInfo& rLineInfo, DWORD& dwDisplacement32) const
{
	_ASSERTE(m_hSymProcess != NULL);
	IMAGEHLP_LINE64 il;
	ZeroMemory(&il, sizeof(il));
	il.SizeOfStruct = sizeof(il);
	if (SymGetLineFromAddr64(m_hSymProcess, (DWORD64)ptrAddress, &dwDisplacement32, &il))
	{
		rFileInfo = CPdbFileInfo(il.FileName);
		rLineInfo = CPdbLineInfo((PVOID)il.Address, il.LineNumber);
		return true;
	}
	else
	{
		rFileInfo = CPdbFileInfo();
		rLineInfo = CPdbLineInfo();
		dwDisplacement32 = 0;
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
bool CPdbProcessor::FindLineInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFileInfo>& pFileInfo, boost::shared_ptr<CBaseLineInfo>& pLineInfo, DWORD& dwDisplacement32) const
{
	_ASSERTE(m_hSymProcess != NULL);
	CPdbFileInfo FileInfo;
	CPdbLineInfo LineInfo;
	if (FindLineInfo(ptrAddress, FileInfo, LineInfo, dwDisplacement32))
	{
		pFileInfo.reset(new CPdbFileInfo(FileInfo));
		pLineInfo.reset(new CPdbLineInfo(LineInfo));
		return true;
	}
	else
	{
		pFileInfo.reset();
		pLineInfo.reset();
		return false;
	}
}
