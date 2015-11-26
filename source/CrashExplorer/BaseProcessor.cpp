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

#include "stdafx.h"
#include "BaseProcessor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_ABSTRACT_RUNTIME_CLASS(CBaseProcessor, CObject)

/**
 * @param pszFileName - source file name.
 * @return file type.
 */
CBaseProcessor::PROCESSED_FILE_TYPE CBaseProcessor::GetDataType(PCTSTR pszFileName)
{
	static const DWORD dwMaxSignatureSize = 64;
	static const struct
	{
		const CHAR* m_pchFileSig;
		PROCESSED_FILE_TYPE m_eProcessedFileType;
	}
	arrFileSignatures[] =
	{
		{ "Microsoft C/C++ program database 2.00\r\n\032JG", PFT_PDB },
		{ "Microsoft C/C++ MSF 7.00\r\n\032DS", PFT_PDB },
		{ "MZ", PFT_PDB }
	};

	PROCESSED_FILE_TYPE eProcessedFileType = PFT_UNDEFINED;
	CAtlFile fileMapPdb;
	if (SUCCEEDED(fileMapPdb.Create(pszFileName, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING)))
	{
		CHAR arrBuffer[dwMaxSignatureSize];
		DWORD dwBytesRead;
		if (SUCCEEDED(fileMapPdb.Read(arrBuffer, sizeof(arrBuffer), dwBytesRead)))
		{
			for (DWORD dwSigPos = 0; dwSigPos < countof(arrFileSignatures); ++dwSigPos)
			{
				DWORD dwSignatureSize = strlen(arrFileSignatures[dwSigPos].m_pchFileSig) + 1;
				ATLASSERT(dwSignatureSize <= dwMaxSignatureSize);
				if (dwBytesRead < dwSignatureSize)
					continue;
				if (memcmp(arrBuffer, arrFileSignatures[dwSigPos].m_pchFileSig, dwSignatureSize) == 0)
				{
					eProcessedFileType = arrFileSignatures[dwSigPos].m_eProcessedFileType;
					break;
				}
			}
			if (eProcessedFileType == PFT_UNDEFINED)
			{
				eProcessedFileType = PFT_MAP;
				for (DWORD dwCharPos = 0; dwCharPos < dwBytesRead; ++dwCharPos)
				{
					if (arrBuffer[dwCharPos] == '\0')
					{
						eProcessedFileType = PFT_UNDEFINED;
						break;
					}
				}
			}
		}
	}
	return eProcessedFileType;
}

/**
 * @param pszFileName - source file name.
 * @return file type.
 */
CBaseProcessor::PROCESSED_FILE_TYPE CBaseProcessor::GetFileExtType(PCTSTR pszFileName)
{
	static const struct
	{
		PCTSTR m_pszFileExt;
		PROCESSED_FILE_TYPE m_eProcessedFileType;
	}
	arrFileTypes[] =
	{
		{ _T(".map"), PFT_MAP },
		{ _T(".pdb"), PFT_PDB },
		{ _T(".exe"), PFT_PDB },
		{ _T(".dll"), PFT_PDB }
	};
	PCTSTR pszFileExt = PathFindExtension(pszFileName);
	ATLASSERT(pszFileExt != NULL);
	for (DWORD dwFileExtPos = 0; dwFileExtPos < countof(arrFileTypes); ++dwFileExtPos)
	{
		if (_tcsicmp(arrFileTypes[dwFileExtPos].m_pszFileExt, pszFileExt) == 0)
			return arrFileTypes[dwFileExtPos].m_eProcessedFileType;
	}
	return PFT_UNDEFINED;
}

/**
 * @param pszFileName - source file name.
 * @return file type.
 */
CBaseProcessor::PROCESSED_FILE_TYPE CBaseProcessor::GetFileType(PCTSTR pszFileName)
{
	PROCESSED_FILE_TYPE eProcessedFileType = GetFileExtType(pszFileName);
	if (eProcessedFileType == PFT_UNDEFINED)
		eProcessedFileType = GetDataType(pszFileName);
	return eProcessedFileType;
}
