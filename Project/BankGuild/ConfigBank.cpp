#include "StdAfx.h"
#include "ConfigBank.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CConfigBank::CConfigBank(void)
{
}


CConfigBank::~CConfigBank(void)
{
}

void CConfigBank::ReadSystemFile()
{
	CString strPath;
	//strPath.Format(_T("%s\\System.ini"), PATH_SYSTEM);
	//20170118 byskcho
	strPath.Format(_T("%s\\%s"), PATH_SYSTEM,NAEM_FILE_SYSTEM);
	m_System.ReadSystemFile(strPath);
}

void CConfigBank::ReadOptionFile()
{
	CString strPath;
	//20170118 bkskcho
	//strPath.Format(_T("%s\\Option.ini"), m_System.m_strConfigPath);	
	strPath.Format(_T("%s\\%s"), m_System.m_strConfigPath,NAME_FILE_OPTION);
	m_Option.ReadOptionFile(strPath);
}

void CConfigBank::SaveOptionFile()
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;
	strPath.Format(_T("%s\\Option.ini"), m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\%d_%d\\Option%s.ini"), m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.Format(_T("%H%M%S")));
	m_Option.SaveOptionFile(strPath, strBackupPath);
}

BOOL CConfigBank::LoadDefectNameTable()
{
	CString strFileName;

	//20170118 byskcho
	//strFileName.Format(_T("%s\\NgCodeList.csv"),PATH_SYSTEM);
	strFileName.Format(_T("%s\\%s"),PATH_SYSTEM,NMAE_FILE_DEFECT_TABLE);

	// ������ �����ϴ��� ����
	if(CFileSupport::IsFile(strFileName) == FALSE) 	
	{	
		return FALSE;
	}

	// .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..

	CString sLine;						// ���Ͽ��� �о�� ������	

	CString		sKey;					// �˻��� �� ���Ǵ� Key��, ��������, �빮�� ��ȯ
	CString		sName1;					// ���Ͽ��� �о���� Defect ����
	CString		sName2;					// ���� �� �ѱ� defect
	CString		sName3;					
	CString		sCode;					// MES ����� Defect Code

	//������ ���پ� �о�鼭 �־��ش�.;
	// Text ���� UniCode ������� ���� ���� ( ��Ʈ���� �а� ���� ����)
	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strFileName.GetString(), _T("r,ccs=UTF-8"));	// UNICODE"));
	if (e != 0) return FALSE;

	CStdioFile		file(fStream);
	//map<CString, DEFECT_MES_CODE> iMapDefectList;
	// ���Ͽ��� 1�پ� �о�ͼ� ó���Ѵ�.
	int nIndex = 0;
	while ( file.ReadString(sLine) )
	{
		if (sLine.IsEmpty() )	continue;			// ������ ��� ����
		if (sLine.Left(1) == _T("?"))	continue;	// ó�� ���ڰ� " ? " �ϰ�� ���� �������� �Ѿ��.;
		if (sLine.Left(1) == _T("/"))	continue;	// ó�� ���ڰ� " / " �ϰ�� ���� �������� �Ѿ��.;
		if (sLine.Left(1) == _T("!"))	continue;	// ó�� ���ڰ� " ! " �ϰ�� ���� �������� �Ѿ��.;
		if (sLine.Left(1) == _T(";"))	continue;	// ó�� ���ڰ� " ; " �ϰ�� ���� �������� �Ѿ��.;

		::AfxExtractSubString( sName1,	sLine	, 0, ',');			
		::AfxExtractSubString( sName2,	sLine	, 1, ',');			
		::AfxExtractSubString( sName3,	sLine	, 2, ',');			
		::AfxExtractSubString( sCode,	sLine	, 3, ',');			

		if ( sName1.IsEmpty() || sCode.IsEmpty() ) continue;			// �����̶�� ����

		// �˻��� �� ��� �� Ű�� �����.
		// 		sKey = sName1;
		// 		sKey.Trim();							// �¿� ��������
		// 		sKey.Replace(_T(" "), _T(""));			// ���ڳ� ��������
		// 		sKey.MakeUpper();						// ��� �빮�ڷ� �ٲ۴�.


		DEFECT_MES_CODE defectMesCode;
		defectMesCode.strPopup = sName1;
		defectMesCode.strPopup_KOR = sName2;
		defectMesCode.strPopup_ENG = sName3;
		defectMesCode.strMES_CODE = sCode;
		//theProcBank.iMapBZoneSkipDefect.insert(pair<JIG_CH, B_ZONE_DEFECT>(JIG_CH_1, BZoneDefect));
		theProcBank.iMapDefectList.insert(pair<int, DEFECT_MES_CODE>(nIndex, defectMesCode));
		nIndex++;
	}//of while

	file.Close();
	fclose(fStream);

	return TRUE;
}

void CConfigBank::ReadNGCodeFile()
{
	LoadDefectNameTable();	
}
void CConfigBank::ReadUnLoadRunInfo()
{ 
	m_UnloadRunInfo.ReadUnloadRuleFileInfo();
}
void CConfigBank::SaveUnLoadRunInfo()
{
	m_UnloadRunInfo.SaveUnloadRuleFileInfo();
}