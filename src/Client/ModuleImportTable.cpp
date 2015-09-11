/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Table of modules that import SetUnhandledExceptionFilter() function.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#include "StdAfx.h"
#include "ModuleImportTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// Macro for adding pointers/DWORDs together without C arithmetic interfering. Taken from Matt Pietrek's book.
#define MakePtr(cast, ptr, offset) ((cast)((DWORD_PTR)(ptr) + (DWORD_PTR)(offset)))

/**
 * @param hModule - module handle.
 * @param bOverride - true if SetUnhandledExceptionFilter() must be overridden.
 * @param nNestedLevel - nested level  (useful for debugging).
 */
void CModuleImportTable::TraverseImportTables(HMODULE hModule, BOOL bOverride, size_t nNestedLevel)
{
	BOOL bCleanup = FALSE;
	if (nNestedLevel == 0)
	{
		if (! g_bWinNT)
			return;
		HMODULE hMainModule = GetModuleHandle(NULL);
		if (hModule == NULL)
			hModule = hMainModule;
		if (hModule == hMainModule && ! bOverride)
			bCleanup = TRUE;
#ifdef _DEBUG
		if (bOverride)
			OutputDebugString(_T("Injecting fake SetUnhandledExceptionFilter() handler\n"));
		else
			OutputDebugString(_T("Restoring SetUnhandledExceptionFilter() handler\n"));
#endif
	}
	PFSetUnhandledExceptionFilter pfnSavedSetUnhandledExceptionFilter;
	BOOL bFound = m_mapModuleEntries.Lookup(hModule, pfnSavedSetUnhandledExceptionFilter);
	if (bOverride ? bFound : ! bFound)
		return;
	PIMAGE_DOS_HEADER pDOSHeader = (PIMAGE_DOS_HEADER)hModule;
	if (pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return;
	PIMAGE_NT_HEADERS pNTHeaders = MakePtr(PIMAGE_NT_HEADERS, pDOSHeader, pDOSHeader->e_lfanew);
	if (pNTHeaders->Signature != IMAGE_NT_SIGNATURE)
		return;
	DWORD dwImportRva = pNTHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	if (dwImportRva == 0)
		return;
#ifdef _DEBUG
	TCHAR szModuleFileName[MAX_PATH];
	if (GetModuleFileName(hModule, szModuleFileName, countof(szModuleFileName)))
	{
		for (size_t i = 0; i < nNestedLevel; ++i)
			OutputDebugString(_T(" "));
		OutputDebugString(szModuleFileName);
		OutputDebugString(_T("\n"));
	}
#endif
	if (bOverride)
		m_mapModuleEntries.SetAt(hModule, NULL);
	else
		m_mapModuleEntries.Delete(hModule);
	PIMAGE_IMPORT_DESCRIPTOR pImageImportDesc = MakePtr(PIMAGE_IMPORT_DESCRIPTOR, pDOSHeader, dwImportRva);
	while (pImageImportDesc->Name)
	{
		PCSTR pszModuleName = MakePtr(PCSTR, pDOSHeader, pImageImportDesc->Name);
		if (_strcmpi(pszModuleName, "KERNEL32.DLL") == 0)
		{
			PIMAGE_THUNK_DATA pOriginalThunk = MakePtr(PIMAGE_THUNK_DATA, pDOSHeader, pImageImportDesc->OriginalFirstThunk);
			PIMAGE_THUNK_DATA pActualThink = MakePtr(PIMAGE_THUNK_DATA, pDOSHeader, pImageImportDesc->FirstThunk);
			while (pOriginalThunk->u1.Function)
			{
				if ((pOriginalThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) == 0)
				{
					PIMAGE_IMPORT_BY_NAME pImageImportByName = MakePtr(PIMAGE_IMPORT_BY_NAME, pDOSHeader, pOriginalThunk->u1.AddressOfData);
					PCSTR pszFunctionName = (PCSTR)pImageImportByName->Name;
					if (strcmp(pszFunctionName, "SetUnhandledExceptionFilter") == 0)
					{
						PFSetUnhandledExceptionFilter pfnCurrentSetUnhandledExceptionFilter = (PFSetUnhandledExceptionFilter)pActualThink->u1.Function;
						if (bOverride ? pfnCurrentSetUnhandledExceptionFilter != &DummySetUnhandledExceptionFilter : TRUE)
						{
							MEMORY_BASIC_INFORMATION MemBasicInfo;
							VirtualQuery(pActualThink, &MemBasicInfo, sizeof(MemBasicInfo));
							DWORD dwOldProtect;
							if (VirtualProtect(MemBasicInfo.BaseAddress, MemBasicInfo.RegionSize, PAGE_READWRITE, &dwOldProtect))
							{
								pActualThink->u1.Function = (DWORD_PTR)(bOverride ? &DummySetUnhandledExceptionFilter : pfnSavedSetUnhandledExceptionFilter);
								VirtualProtect(MemBasicInfo.BaseAddress, MemBasicInfo.RegionSize, MemBasicInfo.Protect, &dwOldProtect);
								if (bOverride)
									m_mapModuleEntries.SetAt(hModule, pfnCurrentSetUnhandledExceptionFilter);
								break;
							}
						}
					}
				}
				++pOriginalThunk;
				++pActualThink;
			}
		}
		else
		{
			HMODULE hNestedModule = GetModuleHandleA(pszModuleName);
			if (hNestedModule != NULL)
				TraverseImportTables(hNestedModule, bOverride, nNestedLevel + 1);
		}
		++pImageImportDesc;
	}
	if (bCleanup)
		Clear();
}
