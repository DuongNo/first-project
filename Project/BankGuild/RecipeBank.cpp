#include "StdAfx.h"
#include "RecipeBank.h"
#include "CheetahApp.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CRecipeBank::CRecipeBank(void)
{
	//kjaprk 20161014 FPBC ��� ���� Ȯ��, Vacuum ���� ����ϱ� ����
	m_Cell.m_bFPCBuse = FALSE;
}


CRecipeBank::~CRecipeBank(void)
{
}

void CRecipeBank::ReadLastRecipeFile()
{
	CString strPath;
	strPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigPath);

	CIni ini(strPath);
	CString strName = ini.GetString(_T("LAST_DATA"), _T("RECIPE_NAME"));
	ReadRecipeFile(strName);
}

void CRecipeBank::ReadRecipeFile( CString strRecipeName )
{
	// ���� �ҷ����°� ���� Recipe�� �����Ѵ�
	m_strRecipeName = strRecipeName;

	CString strPath;

	strPath.Format(_T("%s\\%s\\Cell.ini"), theConfigBank.m_System.m_strRecipePath, m_strRecipeName);
	m_Cell.ReadFile(strPath);
	strPath.Format(_T("%s\\%s\\Tray.ini"), theConfigBank.m_System.m_strRecipePath, m_strRecipeName);
	m_Tray.ReadFile(strPath);
}

void CRecipeBank::SaveRecipeFile(CString strRecipe)
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;

	strPath.Format(_T("%s\\%s\\Cell.ini"), theConfigBank.m_System.m_strRecipePath, strRecipe);
	strBackupPath.Format(_T("%s\\%d_%d\\%s\\Cell_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), m_strRecipeName, time.Format(_T("%H%M%S")));
	m_Cell.SaveFile(strPath, strBackupPath);

	strPath.Format(_T("%s\\%s\\Tray.ini"), theConfigBank.m_System.m_strRecipePath, strRecipe);
	strBackupPath.Format(_T("%s\\%d_%d\\%s\\Tray_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), m_strRecipeName, time.Format(_T("%H%M%S")));
	m_Tray.SaveFile(strPath, strBackupPath);
}

void CRecipeBank::SaveLastRecipeFile()
{
	CString strPath,strBackupPath;;
	strPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\LastData.ini"), theConfigBank.m_System.m_strConfigBackupPath);
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
		// ���� ���� ���
		::CopyFile(strPath, strBackupPath, FALSE);
		// File ������ ����ϰ� �ϱ� ���� ������ ����� ���� ���´�
		::DeleteFile(strPath);
	}

	CIni ini(strPath);

	ini.WriteString(_T("LAST_DATA")		, _T("RECIPE_NAME")			, theRecipeBank.m_strRecipeName				);
	
}
