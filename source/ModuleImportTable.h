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

#pragma once

#include "Hash.h"
#include "Globals.h"

/// Type definition of pointer to SetUnhandledExceptionFilter() function.
typedef PTOP_LEVEL_EXCEPTION_FILTER (WINAPI *PFSetUnhandledExceptionFilter)(PTOP_LEVEL_EXCEPTION_FILTER pTopLevelExceptionFilter);

/// Table of modules that import SetUnhandledExceptionFilter() function.
class CModuleImportTable
{
public:
	/// Object constructor.
	CModuleImportTable(void);
	/// Object destructor.
	~CModuleImportTable(void);
	/// Override SetUnhandledExceptionFilter().
	void Override(HMODULE hModule);
	/// Restore original function pointers.
	void Restore(HMODULE hModule);
	/// Override/restore original function pointers.
	void Intercept(HMODULE hModule, BOOL bOverride);
	/// Free module entries.
	void Clear(void);
	/// Return original address of SetUnhandledExceptionFilter().
	PFSetUnhandledExceptionFilter GetOriginalProcAddress(void) const;

private:
	/// Protects object from being accidentally copied.
	CModuleImportTable(const CModuleImportTable& rModuleImportTable);
	/// Protects object from being accidentally copied.
	CModuleImportTable& operator=(const CModuleImportTable& rModuleImportTable);

	/// Dummy version of SetUnhandledExceptionFilter().
	static PTOP_LEVEL_EXCEPTION_FILTER WINAPI DummySetUnhandledExceptionFilter(PTOP_LEVEL_EXCEPTION_FILTER pTopLevelExceptionFilter);

	/// Traverse import tables recurrently and replace SetUnhandledExceptionFilter().
	void TraverseImportTables(HMODULE hModule, BOOL bOverride, size_t nNestedLevel);

	/// Map of module entries.
	CHash<HMODULE, PFSetUnhandledExceptionFilter> m_mapModuleEntries;
};

inline CModuleImportTable::CModuleImportTable(void)
{
}

inline CModuleImportTable::~CModuleImportTable(void)
{
	Restore(NULL);
}

inline void CModuleImportTable::Clear(void)
{
	m_mapModuleEntries.DeleteAll(true);
}

/**
 * @param hModule - module handle.
 */
inline void CModuleImportTable::Override(HMODULE hModule)
{
	TraverseImportTables(hModule, TRUE, 0);
}

/**
 * @param hModule - module handle.
 */
inline void CModuleImportTable::Restore(HMODULE hModule)
{
	TraverseImportTables(hModule, FALSE, 0);
}

/**
 * @param hModule - module handle.
 * @param bOverride - true if SetUnhandledExceptionFilter() must be overridden.
 */
inline void CModuleImportTable::Intercept(HMODULE hModule, BOOL bOverride)
{
	TraverseImportTables(hModule, bOverride, 0);
}

/**
 * @param pTopLevelExceptionFilter - not used.
 * @return NULL pointer.
 */
inline PTOP_LEVEL_EXCEPTION_FILTER WINAPI CModuleImportTable::DummySetUnhandledExceptionFilter(PTOP_LEVEL_EXCEPTION_FILTER /*pTopLevelExceptionFilter*/)
{
	return NULL;
}

/**
 * @return original address of SetUnhandledExceptionFilter().
 */
inline PFSetUnhandledExceptionFilter CModuleImportTable::GetOriginalProcAddress(void) const
{
	const PFSetUnhandledExceptionFilter* ppfnSetUnhandledExceptionFilter = m_mapModuleEntries.Lookup(g_hInstance);
	return (ppfnSetUnhandledExceptionFilter != NULL ? *ppfnSetUnhandledExceptionFilter : &SetUnhandledExceptionFilter);
}
