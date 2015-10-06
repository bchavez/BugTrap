/*
 * This is a part of the BugTrap package.
 * Copyright (c) 2005-2009 IntelleSoft.
 * All rights reserved.
 *
 * Description: Base file processor.
 * Author: Maksim Pyatkovskiy.
 *
 * This source code is only intended as a supplement to the
 * BugTrap package reference and related electronic documentation
 * provided with the product. See these sources for detailed
 * information regarding the BugTrap package.
 */

#pragma once

#include "BaseFnInfo.h"
#include "BaseFileInfo.h"
#include "BaseLineInfo.h"
#include "RuntimeClass.h"

/// Base file processor.
class CBaseProcessor : public CObject
{
	DECLARE_ABSTRACT_RUNTIME_CLASS(CBaseProcessor)
public:
	/// Supported file types.
	enum PROCESSED_FILE_TYPE
	{
		/// Undefined type.
		PFT_UNDEFINED,
		/// MAP file.
		PFT_MAP,
		/// PDB file.
		PFT_PDB
	};

	/// Object destructor.
	virtual ~CBaseProcessor() { }
	/// Determine file type.
	static PROCESSED_FILE_TYPE GetFileType(PCTSTR pszFileName);
	/// Determine file type.
	static PROCESSED_FILE_TYPE GetDataType(PCTSTR pszFileName);
	/// Determine file type.
	static PROCESSED_FILE_TYPE GetFileExtType(PCTSTR pszFileName);
	/// Find public function info by function address.
	virtual bool FindFunctionInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFnInfo>& pFnInfo, DWORD64& dwDisplacement64) const = 0;
	/// Find line info by line address.
	virtual bool FindLineInfo(PVOID ptrAddress, boost::shared_ptr<CBaseFileInfo>& pFileInfo, boost::shared_ptr<CBaseLineInfo>& pLineInfo, DWORD& dwDisplacement32) const = 0;
};
