#include "stdafx.h"
#include "DirHandler.h"

#include <windows.h>
#include <tchar.h>
#include <shellapi.h>


void DirHandler::CreateDirectories()
{
	LPCTSTR dir = TEXT(rootDir);
	DeleteDirectory(dir, false);
	_mkdir(rootDir);
	_mkdir(shadeDir);
}

bool DirHandler::DeleteDirectory(LPCTSTR lpszDir, bool noRecycleBin)
{
	int len = _tcslen(lpszDir);
	TCHAR *pszFrom = new TCHAR[len + 2];
	_tcscpy(pszFrom, lpszDir);
	pszFrom[len] = 0;
	pszFrom[len + 1] = 0;

	SHFILEOPSTRUCT fileop;
	fileop.hwnd = NULL;    // no status display
	fileop.wFunc = FO_DELETE;  // delete operation
	fileop.pFrom = pszFrom;  // source file name as double null terminated string
	fileop.pTo = NULL;    // no destination needed
	fileop.fFlags = FOF_NOCONFIRMATION | FOF_SILENT;  // do not prompt the user

	if (!noRecycleBin)
		fileop.fFlags |= FOF_ALLOWUNDO;

	fileop.fAnyOperationsAborted = FALSE;
	fileop.lpszProgressTitle = NULL;
	fileop.hNameMappings = NULL;

	int ret = SHFileOperation(&fileop);
	delete[] pszFrom;
	return (ret == 0);
}
