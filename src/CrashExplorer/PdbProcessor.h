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

#pragma once

#include "BaseProcessor.h"
#include "PdbFnInfo.h"
#include "PdbFileInfo.h"
#include "PdbLineInfo.h"

class CPdbProcessor : public CBaseProcessor
{
	DECLARE_RUNTIME_CLASS(CPdbProcessor)
public:
	/// Initialize PDB processor.
	CPdbProcessor(PCTSTR pszSearchPath = NULL);
	/// De-initialize PDB processor.
	virtual ~CPdbProcessor();
	/// Load new module to the process address space.
	void LoadModule(PCTSTR pszModuleName, PVOID pBaseAddr, DWORD dwModuleSize);
	/// Find public function info by function address.
	bool FindFunctionInfo(PVOID ptrAddress, CPdbFnInfo& rFnInfo, DWORD64& dwDisplacement64) const;
	/// Find public function info by function address.
	virtual bool FindFunctionInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFnInfo>& pFnInfo, DWORD64& dwDisplacement64) const;
	/// Find line info by line address.
	bool FindLineInfo(PVOID ptrAddress, CPdbFileInfo& rFileInfo, CPdbLineInfo& rLineInfo, DWORD& dwDisplacement32) const;
	/// Find line info by line address.
	virtual bool FindLineInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFileInfo>& pFileInfo, boost::shared_ptr<CBaseLineInfo>& pLineInfo, DWORD& dwDisplacement32) const;

private:
	/// Handle that identifies the caller.
	HANDLE m_hSymProcess;
};
