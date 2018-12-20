#include "StdAfx.h"
#include "RecipeTray.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRecipeTray::CRecipeTray(void)
{
	m_nCountX = 1;
	m_nCountY = 1;
	m_dPitchX = 0.;
	m_dPitchY = 0.;
	m_dPitchOffsetX = 0.;
	m_dPitchOffsetY = 0.;
	m_dTrayThickness = 10.;
}


CRecipeTray::~CRecipeTray(void)
{
}

void CRecipeTray::ReadFile( CString strPath )
{
	CIni ini(strPath);
	m_nCountX			= ini.GetLong(_T("TRAY")		, _T("m_nCountX")			, 1);
	m_nCountY			= ini.GetLong(_T("TRAY")		, _T("m_nCountY")			, 1);
	m_dPitchX			= ini.GetDouble(_T("TRAY")		, _T("m_dPitchX")			, 0.);
	m_dPitchY			= ini.GetDouble(_T("TRAY")		, _T("m_dPitchY")				, 0.);
	m_dPitchOffsetX		= ini.GetDouble(_T("TRAY")		, _T("m_dPitchOffsetX")		, 0.);
	m_dPitchOffsetY		= ini.GetDouble(_T("TRAY")		, _T("m_dPitchOffsetY")		, 0.);
	m_dTrayThickness	= ini.GetDouble(_T("TRAY")		, _T("m_dTrayThickness")	, 0.);
}

void CRecipeTray::SaveFile( CString strPath, CString strBackupPath )
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

	ini.WriteLong(_T("TRAY")		, _T("m_nCountX")			, m_nCountX				);
	ini.WriteLong(_T("TRAY")		, _T("m_nCountY")			, m_nCountY				);
	ini.WriteDouble(_T("TRAY")		, _T("m_dPitchX")			, m_dPitchX				);
	ini.WriteDouble(_T("TRAY")		, _T("m_dPitchY")				, m_dPitchY				);
	ini.WriteDouble(_T("TRAY")		, _T("m_dPitchOffsetX")		, m_dPitchOffsetX		);
	ini.WriteDouble(_T("TRAY")		, _T("m_dPitchOffsetY")		, m_dPitchOffsetY		);
	ini.WriteDouble(_T("TRAY")		, _T("m_dTrayThickness")	, m_dTrayThickness		);
}
