 #include "stdafx.h"
#include "FileSupport.h"

#include <windows.h>

#include <Winver.h>
#pragma comment(lib, "Version.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
int CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData) 
{
	TCHAR szDir[MAX_PATH];

	switch(uMsg) {
    case BFFM_INITIALIZED: 
		_tcscpy_s ( szDir , (LPTSTR)pData );
		if ( _tcscmp ( szDir , _T("") ) == 0 ) {
			GetCurrentDirectory(sizeof(szDir)/sizeof(TCHAR), szDir);
		}
		// WParam is TRUE since you are passing a path.
		// It would be FALSE if you were passing a pidl.
		SendMessage(hwnd,BFFM_SETSELECTION,TRUE,(LPARAM)szDir);

		break;
	case BFFM_SELCHANGED: 
		// Set the status window to the currently selected path.
		if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir)) 
			SendMessage(hwnd,BFFM_SETSTATUSTEXT,0,(LPARAM)szDir);
		break;
	default:
		break;
	}
	return 0;
}

BOOL CFileSupport::FindPath( HWND hOwner , CString& path,  LPCTSTR szStartDir /*= NULL*/, LPCTSTR caption /*= "Select Folder"*/ )
{
	BOOL fResult = FALSE;
    BROWSEINFO bi;
    TCHAR szDir[MAX_PATH];
    LPITEMIDLIST pidl;
    LPMALLOC pMalloc;

    if (SUCCEEDED(SHGetMalloc(&pMalloc)))
	{
		ZeroMemory(&bi,sizeof(bi));
        bi.hwndOwner = hOwner;
        bi.pszDisplayName = 0;
		bi.lpszTitle = caption;
        bi.pidlRoot = 0;
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
        bi.lpfn = BrowseCallbackProc;
		if(szStartDir != NULL)
			bi.lParam = (LPARAM)(LPCTSTR)szStartDir;

        pidl = SHBrowseForFolder(&bi);
        if (pidl) {
			if (SHGetPathFromIDList(pidl, szDir)) {
				if ( _tcscmp ( path , szDir ) != 0 ) {
					path.Format(_T("%s"), szDir);
					fResult = TRUE;
				} 
			}
	        pMalloc->Free(pidl);
        }
		pMalloc->Release();
    }
	return fResult;
}

CString CFileSupport::FindFilePath ( BOOL isOpen , LPCTSTR defaultFilePath , UINT filterID )
{
	CString filter;
	filter.LoadString ( filterID );
	return FindFilePath(isOpen,defaultFilePath,filter);
}

CString CFileSupport::FindFilePath ( BOOL isOpen , LPCTSTR defaultFilePath , LPCTSTR filter /*= *.**/)
{
//	char szCurDir[MAX_PATH];
//	GetCurrentDirectory(MAX_PATH,szCurDir);
	CFileDialog dlg(isOpen, NULL, defaultFilePath, OFN_HIDEREADONLY , filter );
//	dlg.m_ofn.lpstrInitialDir = szCurDir;
	if ( dlg.DoModal() == IDOK ) {
		return dlg.GetPathName();
	}
	return _T("");
}

BOOL CFileSupport::CreateDirectory( LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes /*=NULL*/)
{
	CString temp = lpPathName;
	temp.Replace(_T("\\"), _T("/"));
	CString parentFolder = temp.Left(temp.ReverseFind(_T('/')));
	if (parentFolder == _T(""))
		return FALSE;

	DWORD dwResult;

	BOOL fResult = TRUE;
	if ( (dwResult = GetFileAttributes(parentFolder)) == -1 ) {
		fResult = CFileSupport::CreateDirectory(parentFolder , lpSecurityAttributes);
	}

	if ( fResult && ( dwResult & FILE_ATTRIBUTE_DIRECTORY ) ) {
		if ( (dwResult = GetFileAttributes(lpPathName)) == -1 ) {
			::CreateDirectory ( lpPathName , lpSecurityAttributes ); 
		} 
	} 

	return DirectoryCheck(lpPathName);
}

BOOL CFileSupport::CopyDirectory( LPCTSTR sourcePath, LPCTSTR targetPath, BOOL bSubDirCopy /*= FALSE */ )
{
	SHFILEOPSTRUCT	FileOp;

	ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT)); 
	FileOp.hwnd = NULL;
	FileOp.wFunc = FO_COPY;
	if(bSubDirCopy)
		FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	else
		FileOp.fFlags = FOF_FILESONLY | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;

	//char szPath[_MAX_PATH];
	TCHAR szPath[_MAX_PATH];
	ZeroMemory(szPath, _MAX_PATH);


	//int nLength = sprintf_s(szPath, "%s%s", sourcePath, "\\*.*");
	int nLength = _stprintf_s(szPath, _countof(szPath), _T("%s%s"), sourcePath, _T("\\*.*"));

	//int nLength = swprintf_s(szPath, _countof(szPath), _T("%s%s"), sourcePath, _T("\\*.*"));
	szPath[nLength+1] = '\0';
	FileOp.pFrom	= szPath;
	FileOp.pTo		= targetPath;

	int nResult = SHFileOperation(&FileOp);
	if (CFileSupport::DirectoryCheck(targetPath, FALSE) == 0)
	{
		AfxMessageBox(_T("디렉토리 오류, 재설정..."), MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
		return FALSE;
	}
	if (nResult == 0 && FileOp.fAnyOperationsAborted == TRUE) {
		AfxMessageBox(_T("사용자에 의해 동작이 중단되었습니다!"));

		return FALSE;
	}

	return TRUE;
}

BOOL CFileSupport::MoveDirectory( LPCTSTR sourcePath, LPCTSTR targetPath, BOOL bSubDirCopy /*= FALSE */ )
{
	SHFILEOPSTRUCT	FileOp;

	ZeroMemory(&FileOp, sizeof(SHFILEOPSTRUCT)); 
	FileOp.hwnd = NULL;
	FileOp.wFunc = FO_MOVE;
	if(bSubDirCopy)
		FileOp.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
	else
		FileOp.fFlags = FOF_FILESONLY | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;

	TCHAR szPath[_MAX_PATH];
	ZeroMemory(szPath, _MAX_PATH);


	int nLength = _stprintf_s(szPath, _countof(szPath), _T("%s%s"), sourcePath, _T("\\*.*"));
	
	szPath[nLength+1] = _T('\0');
	FileOp.pFrom	= szPath;
	FileOp.pTo		= targetPath;

	int nResult = SHFileOperation(&FileOp);
	if (CFileSupport::DirectoryCheck(targetPath, FALSE) == 0)
	{
		AfxMessageBox(_T("디렉토리 오류, 재설정..."), MB_OK|MB_ICONERROR|MB_SYSTEMMODAL);
		return FALSE;
	}
	if (nResult == 0 && FileOp.fAnyOperationsAborted == TRUE) {
		AfxMessageBox(_T("사용자에 의해 동작이 중단되었습니다!"));

		return FALSE;
	}
	::RemoveDirectory(sourcePath);

	return TRUE;
}

BOOL CFileSupport::IsFile(LPCTSTR path, BOOL fOkAtBlank )
{
	return FileCheck(path, fOkAtBlank);
}

BOOL CFileSupport::FileCheck(LPCTSTR path, BOOL fOkAtBlank )
{
	if ( fOkAtBlank && _tcscmp(path,_T("")) == 0 ) {
		return TRUE;
	}

	DWORD result;
	if (((result=GetFileAttributes(path)) == -1 ) || (result&FILE_ATTRIBUTE_DIRECTORY) ) {
		return FALSE;
	}

	return TRUE;
}

BOOL CFileSupport::IsReadonly(LPCTSTR path)
{
	ASSERT(_tcscmp(path,_T("")) != 0);

	DWORD result;
	if (((result=GetFileAttributes(path)) != -1 ) && (result&FILE_ATTRIBUTE_READONLY) ) {
		return TRUE;
	}

	return FALSE;
}

BOOL CFileSupport::RemoveReadonly(LPCTSTR path)
{
	DWORD result;
	if (((result=GetFileAttributes(path)) != -1 ) && (result&FILE_ATTRIBUTE_READONLY) ) {
		result &= ~FILE_ATTRIBUTE_READONLY;
		return SetFileAttributes(path, result);
	}
	return TRUE;
}

BOOL CFileSupport::FileDate(LPCTSTR path, COleDateTime& fileDate)
{
	if (CFileSupport::FileCheck(path)) {
		HANDLE hFile = CreateFile(path, GENERIC_WRITE|GENERIC_READ , 
			FILE_SHARE_READ , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, NULL );

		FILETIME lastWrite;
		GetFileTime ( hFile , NULL, NULL, &lastWrite);

		SYSTEMTIME stLastWrite;
		FileTimeToSystemTime(&lastWrite, &stLastWrite);

		fileDate.SetDateTime(stLastWrite.wYear, stLastWrite.wMonth, stLastWrite.wDay, 
			stLastWrite.wHour, stLastWrite.wMinute, stLastWrite.wSecond);
	}
	return FALSE;
}


BOOL CFileSupport::IsDirectory(LPCTSTR path, BOOL fOkAtBlank )
{
	return DirectoryCheck(path, fOkAtBlank);
}

BOOL CFileSupport::DirectoryCheck(LPCTSTR path, BOOL fOkAtBlank )
{
	if ( fOkAtBlank && _tcscmp(path,_T("")) == 0 ) {
		return TRUE;
	}

	DWORD result;
	if (((result=GetFileAttributes(path)) == -1 ) || !(result&FILE_ATTRIBUTE_DIRECTORY) ) {
		return FALSE;
	}

	return TRUE;
}

CString CFileSupport::GetFileName(CString fullPath)
{
	fullPath.Replace(_T("\\"), _T("/"));
	return fullPath.Mid(fullPath.ReverseFind('/')+1);
}

CString CFileSupport::GetPathName(CString fullPath)
{
	return fullPath.Left(fullPath.ReverseFind('\\'));
}

// srcName과 destName 뒤에 \0 이 두개 있어야 한다.
BOOL CFileSupport::Copy(LPCTSTR srcName, LPCTSTR destName)
{
	SHFILEOPSTRUCT fileop;
	fileop.hwnd = NULL;
	fileop.wFunc = FO_COPY;
	fileop.fFlags = FOF_NOCONFIRMMKDIR|FOF_NOCONFIRMATION/*|FOF_SILENT*/;
	fileop.pFrom = srcName;
	fileop.pTo = destName;

	long result = SHFileOperation(&fileop);
	return result == 0;
}

void CFileSupport::DeleteDirectory( LPCTSTR lpPathName )
{
	CFileFind finder;
	CString strModelname;
//	int nDelCnt = 0;
//	int nMaxDelCnt = 5000;

	CString strFullPath = lpPathName;
	strFullPath.TrimRight(_T("\\/ "));

	try {
		CFileSupport::DirectoryCheck(strFullPath, 0);
	} catch ( UINT ) {
		TRACE(_T("The path is not directory or does not exist."));
		return;
	}

// 	CString msg;
// 	msg.Format("Do you want to delete the directory [ %s ]", lpPathName);
// 	if (fConfirm && AfxMessageBox(msg, MB_YESNO) == IDCANCEL)
// 		return;

	CString searchPath;
	searchPath.Format(_T("%s/*.*"), lpPathName);
	BOOL bWorking = finder.FindFile(searchPath);
	while (bWorking){
		bWorking = finder.FindNextFile();
		strModelname = finder.GetFileName();
		strFullPath.Format(_T("%s/%s"), lpPathName, strModelname);
		
		if(strModelname.Right(1) != ".") {
			if (finder.IsDirectory()) {
				DeleteDirectory(strFullPath);
			} else {
				SetFileAttributes( strFullPath, FILE_ATTRIBUTE_NORMAL );
				DeleteFile( strFullPath );
//				nDelCnt ++;
//				if (nDelCnt > nMaxDelCnt) {
//					CString strMSG;
//					strMSG.Format (" More Than Max_delete_count %d So Exit!!",nMaxDelCnt);
//					AfxMessageBox ( strMSG );
//					return;
//				}
			}
		}
	}
	finder.Close();
	SetFileAttributes( lpPathName, FILE_ATTRIBUTE_NORMAL );
	if (!::RemoveDirectory(lpPathName)) {
		DWORD result = GetLastError();

		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			result,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		// Process any inserts in lpMsgBuf.
		// ...
		// Display the string.
		CString errMsg;
		errMsg.Format(_T("%s\n\n%s를 삭제할 수 없습니다"), lpMsgBuf, lpPathName);
		MessageBox( NULL, (LPCTSTR)errMsg, _T("Error"), MB_OK | MB_ICONINFORMATION );
		// Free the buffer.
		LocalFree( lpMsgBuf );
	}
}

void CFileSupport::GetPathAndFileName ( LPCTSTR fullPath , CString& path , CString& fileName )
{ 
	CString fullPathStr = fullPath;
	fullPathStr.Replace(_T("\\"), _T("/"));
	int pos = fullPathStr.ReverseFind('/');
	path = fullPathStr.Left ( pos );
	fileName = fullPathStr.Mid ( pos + 1 );
}

BOOL CFileSupport::ValidateName(CString name)
{
	if (name.FindOneOf(_T("\\/:*?\"<>|")) > -1)
		return FALSE;
	else
		return TRUE;
}

BOOL CFileSupport::SafeSave(LPCTSTR tempFileName, LPCTSTR bakFileName, LPCTSTR fileName)
{
	try {
		if (!IsFile(tempFileName))
			throw 1;

		if (IsFile(bakFileName) && !::DeleteFile(bakFileName))
			throw 1;

		if (IsFile(fileName) && !::MoveFile(fileName, bakFileName))
			throw 1;

		if (!::MoveFile(tempFileName, fileName)) {
			MoveFile(bakFileName, fileName);
			throw 1;
		}
	}
	catch (int ) {
		return FALSE;
	}

	return TRUE;
}

BOOL CFileSupport::MoveFile( LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName )
{
	if (IsFile(lpExistingFileName)) {
		return ::MoveFile(lpExistingFileName, lpNewFileName);
	}
	else
		return FALSE;
}


void CFileSupport::DeleteOldFiles(LPCTSTR lpPathName, CTimeSpan ts)
{
	CTime dateFile;
	CTime dateCutoff = CTime::GetCurrentTime() - ts;

	CFileFind finder;
	CString strFullPath = lpPathName;
	strFullPath.TrimRight(_T("\\/ "));

	try {
		CFileSupport::DirectoryCheck(strFullPath, 0);
	} catch ( UINT ) {
// 		AfxMessageBox("The path is not directory or does not exist.");
		return;
	}

	CString searchPath;
	searchPath.Format(_T("%s/*.*"), lpPathName);
	BOOL bWorking = finder.FindFile(searchPath);
	while (bWorking){
		bWorking = finder.FindNextFile();

		finder.GetLastWriteTime(dateFile);		
		if (CTime(dateFile.GetTime()) < dateCutoff) {
			SetFileAttributes( finder.GetFilePath(), FILE_ATTRIBUTE_NORMAL );
			DeleteFile( finder.GetFilePath() );
		}
	}
	finder.Close();
}

void CFileSupport::DeleteOldDirectorys( LPCTSTR lpPathName, CTimeSpan ts /*= CTimeSpan(60, 0, 0, 0)*/ )
{
	CTime dateFile;
	CTime dateCutoff = CTime::GetCurrentTime() - ts;

	CFileFind finder;
	CString strFullPath = lpPathName;
	CString strModelname;
	strFullPath.TrimRight(_T("\\/ ")); 

	try {
		CFileSupport::DirectoryCheck(strFullPath, 0);
	} catch ( UINT ) {
		AfxMessageBox(_T("The path is not directory or does not exist."));
		return;
	}

	CString searchPath;
	searchPath.Format(_T("%s/*.*"), lpPathName);
	BOOL bWorking = finder.FindFile(searchPath);
	while (bWorking){
		bWorking = finder.FindNextFile();
		finder.GetLastWriteTime(dateFile);		
		strModelname = finder.GetFileName();

		if (CTime(dateFile.GetTime()) < dateCutoff) 
		{
			if (finder.IsDirectory()) {
				// \., \.. 폴더 제외
				if(strModelname.Right(1) != ".") 
					DeleteDirectory(finder.GetFilePath());
			}
			else
			{
				SetFileAttributes( finder.GetFilePath(), FILE_ATTRIBUTE_NORMAL );
				DeleteFile( finder.GetFilePath() );
			}
		}
	}
	finder.Close();
}

CString CFileSupport::GetCurExePath()
{
	// 현재실행파일 위치 알아내기 [8/13/2015 OSC]
	TCHAR szMainPath[_MAX_PATH];
	GetModuleFileName( ::AfxGetInstanceHandle(), szMainPath, _MAX_PATH);

	CString strPath;
	strPath.Format(_T("%s"), szMainPath);

	return strPath;
}

BOOL CFileSupport::GetCurFileVersion( WORD &n1st, WORD &n2nd, WORD &n3rd, WORD &n4th )
{
	//////////////////////////////////////////////////////////////////////////
	// 현재실행파일 위치 알아내기 [8/13/2015 OSC]
	CString strPath = GetCurExePath();
	//////////////////////////////////////////////////////////////////////////
	DWORD infoSize = 0;

	// 파일로부터 버전정보데이터의 크기가 얼마인지를 구합니다.
	infoSize = GetFileVersionInfoSize(strPath.GetString(), 0);
	if(infoSize==0)
	{
		AfxMessageBox(_T("Version info Read Fail"));
	}
	else
	{
		TCHAR *buffer = new TCHAR[infoSize+1];
		memset(buffer, NULL, sizeof(TCHAR)*(infoSize+1));

		// 버전정보데이터를 가져옵니다.
		if(GetFileVersionInfo(strPath.GetString(),0,infoSize, buffer)!=0)
		{
			VS_FIXEDFILEINFO* pFineInfo = NULL;
			UINT bufLen = 0;
			// buffer로 부터 VS_FIXEDFILEINFO 정보를 가져옵니다.
			if(VerQueryValue(buffer,_T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
			{    
				n1st = HIWORD(pFineInfo->dwFileVersionMS);
				n2nd = LOWORD(pFineInfo->dwFileVersionMS);
				n3rd = HIWORD(pFineInfo->dwFileVersionLS);
				n4th = LOWORD(pFineInfo->dwFileVersionLS);

				delete buffer;
				return TRUE;

			}
		}
		delete buffer;
	}
	//////////////////////////////////////////////////////////////////////////
	return FALSE;
}

BOOL CFileSupport::GetCurProductVersion( WORD &n1st, WORD &n2nd, WORD &n3rd, WORD &n4th )
{
	//////////////////////////////////////////////////////////////////////////
	// 현재실행파일 위치 알아내기 [8/13/2015 OSC]
	CString strPath = GetCurExePath();
	//////////////////////////////////////////////////////////////////////////
	DWORD infoSize = 0;

	// 파일로부터 버전정보데이터의 크기가 얼마인지를 구합니다.
	infoSize = GetFileVersionInfoSize(strPath.GetString(), 0);
	if(infoSize==0)
	{
		AfxMessageBox(_T("Version info Read Fail"));
	}
	else
	{
		TCHAR *buffer = new TCHAR[infoSize+1];
		memset(buffer, NULL, sizeof(TCHAR)*(infoSize+1));

		// 버전정보데이터를 가져옵니다.
		if(GetFileVersionInfo(strPath.GetString(),0,infoSize, buffer)!=0)
		{
			VS_FIXEDFILEINFO* pFineInfo = NULL;
			UINT bufLen = 0;
			// buffer로 부터 VS_FIXEDFILEINFO 정보를 가져옵니다.
			if(VerQueryValue(buffer,_T("\\"),(LPVOID*)&pFineInfo, &bufLen) !=0)
			{    
				n1st = HIWORD(pFineInfo->dwProductVersionMS);
				n2nd = LOWORD(pFineInfo->dwProductVersionMS);
				n3rd = HIWORD(pFineInfo->dwProductVersionLS);
				n4th = LOWORD(pFineInfo->dwProductVersionLS);

				delete buffer;
				return TRUE;

			}
		}
		delete buffer;
	}
	//////////////////////////////////////////////////////////////////////////
	return FALSE;
}
