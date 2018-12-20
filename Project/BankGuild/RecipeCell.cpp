#include "StdAfx.h"
#include "RecipeCell.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRecipeCell::CRecipeCell(void)
{
	m_dWidth = 0.;
	m_dHeight = 0.;
	m_dMarkDistance = 0.;
	m_bFPCBuse= FALSE;
}


CRecipeCell::~CRecipeCell(void)
{
}

void CRecipeCell::ReadFile( CString strPath )
{
	CIni ini(strPath);
	m_dWidth					= ini.GetDouble(_T("CELL")	, _T("m_dWidth")				, 0.);
	m_dHeight					= ini.GetDouble(_T("CELL")	, _T("m_dHeight")			, 0.);
	m_dMarkDistance				= ini.GetDouble(_T("CELL")	, _T("m_dMarkDistance")			, 0.);
	m_bFPCBuse					= ini.GetBoolean(_T("CELL")	, _T("m_bFPCBuse")			, FALSE);
}

void CRecipeCell::SaveFile( CString strPath, CString strBackupPath )
{
	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}
	strFolderPath = CFileSupport::GetPathName(strBackupPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	CIni ini(strPath);

	ini.WriteDouble(_T("CELL")	, _T("m_dWidth")				, m_dWidth				);
	ini.WriteDouble(_T("CELL")	, _T("m_dHeight")			, m_dHeight					);
	ini.WriteDouble(_T("CELL")	, _T("m_dMarkDistance")			, m_dMarkDistance					);
	ini.WriteBoolean(_T("CELL")	, _T("m_bFPCBuse")			, m_bFPCBuse					);
}
