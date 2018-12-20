// CreateDirectory()호출시 마지막에 DirectoryCheck()함수 자동 호출하도록 수정 [3/24/2010 OSC]
// static BOOL GetCurFileVersion(WORD &n1st, WORD &n2nd, WORD &n3rd, WORD &n4th) 함수 구현 [8/18/2015 OSC]
// WriteSystemLog 함수 제거 [8/18/2015 OSC]
// GetCurFileVersion(), GetCurProductVersion() 메모리누수 제거 [9/1/2015 OSC]
// T매크로 추가 안된 무자열 수정 [3/8/2016 OSC]
#pragma once

class CFileSupport
{
public:
	static void DeleteOldFiles(LPCTSTR lpPathName, CTimeSpan ts = CTimeSpan(60, 0, 0, 0));
	static void DeleteOldDirectorys(LPCTSTR lpPathName, CTimeSpan ts = CTimeSpan(60, 0, 0, 0));
	static BOOL FindPath( HWND hOwner , CString& path,  LPCTSTR szStartDir = NULL, LPCTSTR caption = _T("Select Folder")) ;
	static CString FindFilePath ( BOOL isOpen , LPCTSTR defaultFilePath , LPCTSTR filter = _T("All Files(*.*)|*.*||"));
	static CString FindFilePath ( BOOL isOpen , LPCTSTR defaultFilePath , UINT filterID );

	static BOOL FileCheck(LPCTSTR path, BOOL fOkAtBlank = FALSE );
	static BOOL DirectoryCheck(LPCTSTR path, BOOL fOkAtBlank = FALSE );
	static BOOL FileDate(LPCTSTR path, COleDateTime& fileDate);
	static BOOL RemoveReadonly(LPCTSTR path);

	// new interface
	static BOOL IsReadonly(LPCTSTR path);
	static BOOL IsFile(LPCTSTR path, BOOL fOkAtBlank = FALSE );
	static BOOL IsDirectory(LPCTSTR path, BOOL fOkAtBlank = FALSE );

	static CString GetFileName(CString fullPath);
	static CString GetPathName(CString fullPath);
	static void GetPathAndFileName ( LPCTSTR fullPath , CString& path , CString& fileName );

	static BOOL CreateDirectory( LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes = NULL);
	static BOOL CopyDirectory(LPCTSTR sourcePath, LPCTSTR targetPath, BOOL bSubDirCopy = FALSE );
	static BOOL MoveDirectory(LPCTSTR sourcePath, LPCTSTR targetPath, BOOL bSubDirCopy = FALSE );
	static void DeleteDirectory(LPCTSTR lpPathName);
	static BOOL ValidateName(CString name);

	static BOOL SafeSave(LPCTSTR tempFileName, LPCTSTR bakFileName, LPCTSTR fileName);
	static BOOL MoveFile(LPCTSTR lpExistingFileName, LPCTSTR lpNewFileName);

	static BOOL Copy(LPCTSTR srcName, LPCTSTR destName);

	static CString GetCurExePath();
	// 숫자 4개로 이루어진 자기 자신의 실행파일 버전 알아오는 함수 [8/18/2015 OSC]
	static BOOL GetCurFileVersion(WORD &n1st, WORD &n2nd, WORD &n3rd, WORD &n4th);
	static BOOL GetCurProductVersion(WORD &n1st, WORD &n2nd, WORD &n3rd, WORD &n4th);
};
