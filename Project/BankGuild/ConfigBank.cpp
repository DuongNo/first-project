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

	// 파일이 존재하는지 점검
	if(CFileSupport::IsFile(strFileName) == FALSE) 	
	{	
		return FALSE;
	}

	// .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. .. ..

	CString sLine;						// 파일에서 읽어온 데이터	

	CString		sKey;					// 검색할 때 사용되는 Key값, 공백제거, 대문자 변환
	CString		sName1;					// 파일에서 읽어들인 Defect 글자
	CString		sName2;					// 영문 및 한글 defect
	CString		sName3;					
	CString		sCode;					// MES 보고용 Defect Code

	//파일을 한줄씩 읽어가면서 넣어준다.;
	// Text 파일 UniCode 방식으로 설정 변경 ( 베트남어 읽고 쓰기 가능)
	FILE *fStream;
	errno_t e = _tfopen_s(&fStream, strFileName.GetString(), _T("r,ccs=UTF-8"));	// UNICODE"));
	if (e != 0) return FALSE;

	CStdioFile		file(fStream);
	//map<CString, DEFECT_MES_CODE> iMapDefectList;
	// 파일에서 1줄씩 읽어와서 처리한다.
	int nIndex = 0;
	while ( file.ReadString(sLine) )
	{
		if (sLine.IsEmpty() )	continue;			// 공백일 경우 무시
		if (sLine.Left(1) == _T("?"))	continue;	// 처음 글자가 " ? " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T("/"))	continue;	// 처음 글자가 " / " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T("!"))	continue;	// 처음 글자가 " ! " 일경우 다음 문장으로 넘어간다.;
		if (sLine.Left(1) == _T(";"))	continue;	// 처음 글자가 " ; " 일경우 다음 문장으로 넘어간다.;

		::AfxExtractSubString( sName1,	sLine	, 0, ',');			
		::AfxExtractSubString( sName2,	sLine	, 1, ',');			
		::AfxExtractSubString( sName3,	sLine	, 2, ',');			
		::AfxExtractSubString( sCode,	sLine	, 3, ',');			

		if ( sName1.IsEmpty() || sCode.IsEmpty() ) continue;			// 공백이라면 무시

		// 검색할 때 사용 할 키를 만든다.
		// 		sKey = sName1;
		// 		sKey.Trim();							// 좌우 공백제거
		// 		sKey.Replace(_T(" "), _T(""));			// 글자내 공백제거
		// 		sKey.MakeUpper();						// 모두 대문자로 바꾼다.


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