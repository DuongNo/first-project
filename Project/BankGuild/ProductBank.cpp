#include "StdAfx.h"
#include "ProductBank.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"


CProductBank::CProductBank(void)
{
	InitData();
	InitMainCountValue();
	InitJigCountValue();
}


CProductBank::~CProductBank(void)
{

}

void CProductBank::InitData()
{
	int nTemp = 0;
	m_MapData.clear();

	ProductInfo tInfo;

	tElement tHour;
	tElement tCount;
	tElement tDefect;
	tElement tReject;
	tElement tDefMCR;
	tElement tDefAlign;
	tElement tDefCont;

	
	for(int i = 0; i < 3; i++ )
	{
		int iVal = 8 * i;

		tHour.iValue = 1 + iVal;
		tHour.uID = IDC_GXLBL_REPORT_TIME_3;
		tCount.iValue = 0;
		tCount.uID = IDC_GXLBL_REPORT_NUM_3;
		tDefect.iValue = 0;
		tDefect.uID = IDC_GXLBL_REPORT_GOOD_3;
		tReject.iValue = 0;
		tReject.uID = IDC_GXLBL_REPORT_REJECT_3;
		tDefMCR.iValue = 0;
		tDefMCR.uID = IDC_GXLBL_REPORT_MCR_3;
		tDefAlign.iValue = 0;
		tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_3;
		tDefCont.iValue = 0;
		tDefCont.uID = IDC_GXLBL_REPORT_CON_3;

		tInfo.tHour = tHour;
		tInfo.tCount = tCount;
		tInfo.tDefect = tDefect;
		tInfo.tReject = tReject;
		tInfo.tDefMCR = tDefMCR;
		tInfo.tDefAlign = tDefAlign;
		tInfo.tDefCont = tDefCont;

		m_MapData.insert(std::make_pair(1 + iVal ,tInfo));				//		세번째 칸 데이터 Initialize
		
		m_MapDataBuff.insert(std::make_pair(1 + iVal ,tInfo));		

		tHour.iValue = 2 + iVal;
		tHour.uID = IDC_GXLBL_REPORT_TIME_4;
		tCount.iValue = 0;
		tCount.uID = IDC_GXLBL_REPORT_NUM_4;
		tDefect.iValue = 0;
		tDefect.uID = IDC_GXLBL_REPORT_GOOD_4;
		tReject.iValue = 0;
		tReject.uID = IDC_GXLBL_REPORT_REJECT_4;
		tDefMCR.iValue = 0;
		tDefMCR.uID = IDC_GXLBL_REPORT_MCR_4;
		tDefAlign.iValue = 0;
		tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_4;
		tDefCont.iValue = 0;
		tDefCont.uID = IDC_GXLBL_REPORT_CON_4;

		tInfo.tHour = tHour;
		tInfo.tCount = tCount;
		tInfo.tDefect = tDefect;
		tInfo.tReject = tReject;
		tInfo.tDefMCR = tDefMCR;
		tInfo.tDefAlign = tDefAlign;
		tInfo.tDefCont = tDefCont;

		m_MapData.insert(std::make_pair(2 + iVal,tInfo));				//		네번째 칸 데이터 Initialize
		
		m_MapDataBuff.insert(std::make_pair(2 + iVal ,tInfo));	
		
		tHour.iValue = 3 + iVal;
		tHour.uID = IDC_GXLBL_REPORT_TIME_5;
		tCount.iValue = 0;
		tCount.uID = IDC_GXLBL_REPORT_NUM_5;
		tDefect.iValue = 0;
		tDefect.uID = IDC_GXLBL_REPORT_GOOD_5;
		tReject.iValue = 0;
		tReject.uID = IDC_GXLBL_REPORT_REJECT_5;
		tDefMCR.iValue = 0;
		tDefMCR.uID = IDC_GXLBL_REPORT_MCR_5;
		tDefAlign.iValue = 0;
		tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_5;
		tDefCont.iValue = 0;
		tDefCont.uID = IDC_GXLBL_REPORT_CON_5;

		tInfo.tHour = tHour;
		tInfo.tCount = tCount;
		tInfo.tDefect = tDefect;
		tInfo.tReject = tReject;
		tInfo.tDefMCR = tDefMCR;
		tInfo.tDefAlign = tDefAlign;
		tInfo.tDefCont = tDefCont;

		m_MapData.insert(std::make_pair(3 + iVal,tInfo));				//		다섯째 칸 데이터 Initialize
	
		m_MapDataBuff.insert(std::make_pair(3 + iVal ,tInfo));	

		tHour.iValue = 4 + iVal;
		tHour.uID = IDC_GXLBL_REPORT_TIME_6;
		tCount.iValue = 0;
		tCount.uID = IDC_GXLBL_REPORT_NUM_6;
		tDefect.iValue = 0;
		tDefect.uID = IDC_GXLBL_REPORT_GOOD_6;
		tReject.iValue = 0;
		tReject.uID = IDC_GXLBL_REPORT_REJECT_6;
		tDefMCR.iValue = 0;
		tDefMCR.uID = IDC_GXLBL_REPORT_MCR_6;
		tDefAlign.iValue = 0;
		tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_6;
		tDefCont.iValue = 0;
		tDefCont.uID = IDC_GXLBL_REPORT_CON_6;

		tInfo.tHour = tHour;
		tInfo.tCount = tCount;
		tInfo.tDefect = tDefect;
		tInfo.tReject = tReject;
		tInfo.tDefMCR = tDefMCR;
		tInfo.tDefAlign = tDefAlign;
		tInfo.tDefCont = tDefCont;

		m_MapData.insert(std::make_pair(4 + iVal,tInfo));				//		여섯째 칸 데이터 Initialize
		
		m_MapDataBuff.insert(std::make_pair(4 + iVal ,tInfo));	

		tHour.iValue = 5 + iVal;
		tHour.uID = IDC_GXLBL_REPORT_TIME_7;
		tCount.iValue = 0;
		tCount.uID = IDC_GXLBL_REPORT_NUM_7;
		tDefect.iValue = 0;
		tDefect.uID = IDC_GXLBL_REPORT_GOOD_7;
		tReject.iValue = 0;
		tReject.uID = IDC_GXLBL_REPORT_REJECT_7;
		tDefMCR.iValue = 0;
		tDefMCR.uID = IDC_GXLBL_REPORT_MCR_7;
		tDefAlign.iValue = 0;
		tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_7;
		tDefCont.iValue = 0;
		tDefCont.uID = IDC_GXLBL_REPORT_CON_7;

		tInfo.tHour = tHour;
		tInfo.tCount = tCount;
		tInfo.tDefect = tDefect;
		tInfo.tReject = tReject;
		tInfo.tDefMCR = tDefMCR;
		tInfo.tDefAlign = tDefAlign;
		tInfo.tDefCont = tDefCont;

		m_MapData.insert(std::make_pair(5 + iVal,tInfo));				//		일곱째 칸 데이터 Initialize
	
		m_MapDataBuff.insert(std::make_pair(5 + iVal ,tInfo));	

		tHour.iValue = 6 + iVal;
		tHour.uID = IDC_GXLBL_REPORT_TIME_8;
		tCount.iValue = 0;
		tCount.uID = IDC_GXLBL_REPORT_NUM_8;
		tDefect.iValue = 0;
		tDefect.uID = IDC_GXLBL_REPORT_GOOD_8;
		tReject.iValue = 0;
		tReject.uID = IDC_GXLBL_REPORT_REJECT_8;
		tDefMCR.iValue = 0;
		tDefMCR.uID = IDC_GXLBL_REPORT_MCR_8;
		tDefAlign.iValue = 0;
		tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_8;
		tDefCont.iValue = 0;
		tDefCont.uID = IDC_GXLBL_REPORT_CON_8;

		tInfo.tHour = tHour;
		tInfo.tCount = tCount;
		tInfo.tDefect = tDefect;
		tInfo.tReject = tReject;
		tInfo.tDefMCR = tDefMCR;
		tInfo.tDefAlign = tDefAlign;
		tInfo.tDefCont = tDefCont;

		m_MapData.insert(std::make_pair(6 + iVal,tInfo));				//		여덟째 칸 데이터 Initialize
	
		m_MapDataBuff.insert(std::make_pair(6 + iVal ,tInfo));	

		tHour.iValue = 7 + iVal;
		tHour.uID = IDC_GXLBL_REPORT_TIME_1;
		tCount.iValue = 0;
		tCount.uID = IDC_GXLBL_REPORT_NUM_1;
		tDefect.iValue = 0;
		tDefect.uID = IDC_GXLBL_REPORT_GOOD_1;
		tReject.iValue = 0;
		tReject.uID = IDC_GXLBL_REPORT_REJECT_1;
		tDefMCR.iValue = 0;
		tDefMCR.uID = IDC_GXLBL_REPORT_MCR_1;
		tDefAlign.iValue = 0;
		tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_1;
		tDefCont.iValue = 0;
		tDefCont.uID = IDC_GXLBL_REPORT_CON_1;

		tInfo.tHour = tHour;
		tInfo.tCount = tCount;
		tInfo.tDefect = tDefect;
		tInfo.tReject = tReject;
		tInfo.tDefMCR = tDefMCR;
		tInfo.tDefAlign = tDefAlign;
		tInfo.tDefCont = tDefCont;

		m_MapData.insert(std::make_pair(7 + iVal,tInfo));				//		첫번째 칸 데이터 Initialize
		
		m_MapDataBuff.insert(std::make_pair(7 + iVal ,tInfo));	
		//2017-05-26,SKCHO , 00시 데이터 표시안되는 것 수정
		nTemp =  8 + iVal;
		if(nTemp == 24) nTemp = 0;
		tHour.iValue = nTemp;
		//tHour.iValue = 8 + iVal;
		tHour.uID = IDC_GXLBL_REPORT_TIME_2;
		tCount.iValue = 0;
		tCount.uID = IDC_GXLBL_REPORT_NUM_2;
		tDefect.iValue = 0;
		tDefect.uID = IDC_GXLBL_REPORT_GOOD_2;
		tReject.iValue = 0;
		tReject.uID = IDC_GXLBL_REPORT_REJECT_2;
		tDefMCR.iValue = 0;
		tDefMCR.uID = IDC_GXLBL_REPORT_MCR_2;
		tDefAlign.iValue = 0;
		tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_2;
		tDefCont.iValue = 0;
		tDefCont.uID = IDC_GXLBL_REPORT_CON_2;

		tInfo.tHour = tHour;
		tInfo.tCount = tCount;
		tInfo.tDefect = tDefect;
		tInfo.tReject = tReject;
		tInfo.tDefMCR = tDefMCR;
		tInfo.tDefAlign = tDefAlign;
		tInfo.tDefCont = tDefCont;

		m_MapData.insert(std::make_pair(nTemp,tInfo));	
		//m_MapData.insert(std::make_pair(8 + iVal,tInfo));				//		두번째 칸 데이터 Initialize
	 	m_MapDataBuff.insert(std::make_pair(nTemp ,tInfo));			
}

//	m_strCurTime.Empty();
}

void CProductBank::LoadReportFile(CString szTime)
{
	CString szPath;
	szPath.Format(_T("%s\\%s\\Report.txt"), theConfigBank.m_System.m_strDebugLogPath, szTime) ;

	CStdioFile file;
	CFileException e;

	InitData();			//생성된 파일에 Init값으로 파일 쓰고 읽기

	//m_strCurTime = szTime;

	if ( !file.Open(szPath.GetBuffer(), CFile::modeRead, &e))			//	후에 파일이 없는경우 생성하도록 변경해야함
	{
		if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ) )
		{		
			m_MapDataBuff.clear();
			SaveReportFile(&file);
		}
	}
	else
	{
		CString szLine, szData;
		short	iTime = 0;
		CTokenizer t;

		while(file.ReadString(szLine))
		{
			t = CTokenizer(szLine);

			szData = t.GetNextToken(_T("\t"));
			iTime = _ttoi(szData);	

			m_MapData[iTime].tHour.iValue = iTime;

			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tCount.iValue = _ttoi(szData);

			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tDefect.iValue = _ttoi(szData);

			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tReject.iValue = _ttoi(szData);
			
			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tDefMCR.iValue = _ttoi(szData);
			
			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tDefAlign.iValue = _ttoi(szData);

			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tDefCont.iValue = _ttoi(szData);	
		}
		file.Close();
	}
}

//2018-04-16,NAM, Load Report for monitoring

void CProductBank::LoadReportFile1(CString szTime)
{
	CString szPath;
	szPath.Format(_T("%s\\%s\\Report.txt"), theConfigBank.m_System.m_strDebugLogPath, szTime) ;

	CStdioFile file;
	CFileException e;

	InitData();			//생성된 파일에 Init값으로 파일 쓰고 읽기



	if ( file.Open(szPath.GetBuffer(), CFile::modeRead, &e))		
	
	{
		CString szLine, szData;
		short	iTime = 0;
		CTokenizer t;

		while(file.ReadString(szLine))
		{
			t = CTokenizer(szLine);

			szData = t.GetNextToken(_T("\t"));
			iTime = _ttoi(szData);	

			m_MapData[iTime].tHour.iValue = iTime;

			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tCount.iValue = _ttoi(szData);

			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tDefect.iValue = _ttoi(szData);

			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tReject.iValue = _ttoi(szData);
			
			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tDefMCR.iValue = _ttoi(szData);
			
			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tDefAlign.iValue = _ttoi(szData);

			szData = t.GetNextToken(_T("\t"));
			m_MapData[iTime].tDefCont.iValue = _ttoi(szData);	
		}
		file.Close();
	}
}

////2017-05-25, SKCHO, 프로그램 시작시 생산 수량 읽는 함수
void CProductBank::LoadProductFile(CString szTime)
{
	CString szPath;
	szPath.Format(_T("%s\\%s\\Report.txt"), theConfigBank.m_System.m_strDebugLogPath, szTime) ;

	CStdioFile file;
	CFileException e;

	InitData();			//생성된 파일에 Init값으로 파일 쓰고 읽기

	m_strCurTime = szTime;

	if ( !file.Open(szPath.GetBuffer(), CFile::modeRead, &e))			//	후에 파일이 없는경우 생성하도록 변경해야함
	{

		if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ) )
		{		
			SaveReportFile(&file);
		}
	}
	else
	{
		CString szLine, szData;
		short	iTime = 0;
		CTokenizer t;

		while(file.ReadString(szLine))
		{
			t = CTokenizer(szLine);

			szData = t.GetNextToken(_T("\t"));
			iTime = _ttoi(szData);	

			m_MapDataBuff[iTime].tHour.iValue = iTime;

			szData = t.GetNextToken(_T("\t"));
			m_MapDataBuff[iTime].tCount.iValue = _ttoi(szData);

			szData = t.GetNextToken(_T("\t"));
			m_MapDataBuff[iTime].tDefect.iValue = _ttoi(szData);

			szData = t.GetNextToken(_T("\t"));
			m_MapDataBuff[iTime].tReject.iValue = _ttoi(szData);
			
			szData = t.GetNextToken(_T("\t"));
			m_MapDataBuff[iTime].tDefMCR.iValue = _ttoi(szData);
			
			szData = t.GetNextToken(_T("\t"));
			m_MapDataBuff[iTime].tDefAlign.iValue = _ttoi(szData);

			szData = t.GetNextToken(_T("\t"));
			m_MapDataBuff[iTime].tDefCont.iValue = _ttoi(szData);	
		}
		file.Close();
	}
}
//2017-05-25, SKCHO
void CProductBank::SaveReportFile(CStdioFile* cFile)
{
	map<short, ProductInfo>::iterator it;

	int iValue = 0;
	for(UINT i = 0; i < m_MapDataBuff.size(); i++ )
	{
		iValue = i+7;

		//2017-05-26,SKCHO, 00시 기록이 안되어서 수정
		if ( iValue >= 24 )		iValue = iValue - 24;
		//if ( iValue > 24 )		iValue = iValue - 24;

		CString szHour = CStringSupport::N2C(m_MapDataBuff[iValue].tHour.iValue);
		CString szCount = CStringSupport::N2C(m_MapDataBuff[iValue].tCount.iValue);
		CString szDefect = CStringSupport::N2C(m_MapDataBuff[iValue].tDefect.iValue);
		CString szReject = CStringSupport::N2C(m_MapDataBuff[iValue].tReject.iValue);
		CString szPerMCR = CStringSupport::N2C(m_MapDataBuff[iValue].tDefMCR.iValue);
		CString szPerAlign = CStringSupport::N2C(m_MapDataBuff[iValue].tDefAlign.iValue);
		CString szPerCont = CStringSupport::N2C(m_MapDataBuff[iValue].tDefCont.iValue);
	
		cFile->SeekToEnd();
		cFile->WriteString( szHour + "\t" );
		cFile->WriteString( szCount + "\t" );
		cFile->WriteString( szDefect + "\t" );
		cFile->WriteString( szReject + "\t" );
		cFile->WriteString( szPerMCR + "\t" );
		cFile->WriteString( szPerAlign + "\t" );
		cFile->WriteString( szPerCont + "\n" );
	
	}	
}
//2017-05-25, SKCHO, 삭제
/*
void CProductBank::SaveReportFile(CStdioFile* cFile)
{
	map<short, ProductInfo>::iterator it;

	int iValue = 0;
	for(UINT i = 0; i < m_MapData.size(); i++ )
	{
		iValue = i+7;

		if ( iValue > 24 )		iValue = iValue - 24;

		CString szHour = CStringSupport::N2C(m_MapData[iValue].tHour.iValue);
		CString szCount = CStringSupport::N2C(m_MapData[iValue].tCount.iValue);
		CString szDefect = CStringSupport::N2C(m_MapData[iValue].tDefect.iValue);
		CString szReject = CStringSupport::N2C(m_MapData[iValue].tReject.iValue);
		CString szPerMCR = CStringSupport::N2C(m_MapData[iValue].tDefMCR.iValue);
		CString szPerAlign = CStringSupport::N2C(m_MapData[iValue].tDefAlign.iValue);
		CString szPerCont = CStringSupport::N2C(m_MapData[iValue].tDefCont.iValue);
	
		cFile->SeekToEnd();
		cFile->WriteString( szHour + "\t" );
		cFile->WriteString( szCount + "\t" );
		cFile->WriteString( szDefect + "\t" );
		cFile->WriteString( szReject + "\t" );
		cFile->WriteString( szPerMCR + "\t" );
		cFile->WriteString( szPerAlign + "\t" );
		cFile->WriteString( szPerCont + "\n" );
	
	}	
}
*/
//2017-05-25, SKCHO, 삭제
/*
void CProductBank::SetProductData(BOOL bSt, BOOL bMCR, BOOL bAlign, BOOL bCont)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	CTime offsettime(time);
	CEtc::ApplyTimeOffset(offsettime, theConfigBank.m_Option.m_OffsetHour, 0);
	CString szCurTime, szPath;
	szCurTime.Format(_T("%04u-%02u-%02u"),offsettime.GetYear(), offsettime.GetMonth(), offsettime.GetDay());	
	// 날짜가 바뀌면 기존껄 초기화 하고 적는다
	if(m_strCurTime != szCurTime)
	{
		InitData();
		m_strCurTime = szCurTime;
	}

	int sHour = time.wHour;

	m_MapData[sHour].tHour.iValue = sHour;

	if ( bSt == TRUE )	m_MapData[sHour].tDefect.iValue += 1;
	else				m_MapData[sHour].tReject.iValue += 1;

	if ( bMCR == TRUE )	m_MapData[sHour].tDefMCR.iValue += 1;
	if ( bAlign == TRUE )	m_MapData[sHour].tDefAlign.iValue += 1;
	if ( bCont == TRUE )	m_MapData[sHour].tDefCont.iValue +=1 ;
	

	m_MapData[sHour].tCount.iValue += 1;

	//----------------------------------------------------------------------------------------

	szPath.Format(_T("%s\\%s\\Report.txt"), theConfigBank.m_System.m_strDebugLogPath, szCurTime) ;

	CStdioFile file;
	CFileException e;

	
	if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ) )
	{
		SaveReportFile(&file);
	}
}
*/
//2017-05-25, SKCHO, 실제 생산 수량을 관리하는 변수로 내용을 변경
void CProductBank::SetProductData(BOOL bSt, BOOL bMCR, BOOL bAlign, BOOL bCont)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	CTime offsettime(time);
	CEtc::ApplyTimeOffset(offsettime, theConfigBank.m_Option.m_OffsetHour, 0);
	CString szCurTime, szPath;
	szCurTime.Format(_T("%04u-%02u-%02u"),offsettime.GetYear(), offsettime.GetMonth(), offsettime.GetDay());	
	// 날짜가 바뀌면 기존껄 초기화 하고 적는다
	if(m_strCurTime != szCurTime)
	{
		InitData();
		m_MapDataBuff.clear();
		m_strCurTime = szCurTime;
	}

	int sHour = time.wHour;

	m_MapDataBuff[sHour].tHour.iValue = sHour;

	if ( bSt == TRUE )	m_MapDataBuff[sHour].tDefect.iValue += 1;
	else				m_MapDataBuff[sHour].tReject.iValue += 1;

	if ( bMCR == TRUE )	m_MapDataBuff[sHour].tDefMCR.iValue += 1;
	if ( bAlign == TRUE )	m_MapDataBuff[sHour].tDefAlign.iValue += 1;
	if ( bCont == TRUE )	m_MapDataBuff[sHour].tDefCont.iValue +=1 ;
	

	m_MapDataBuff[sHour].tCount.iValue += 1;

	//----------------------------------------------------------------------------------------

	szPath.Format(_T("%s\\%s\\Report.txt"), theConfigBank.m_System.m_strDebugLogPath, szCurTime) ;

	CStdioFile file;
	CFileException e;

	
	if ( file.Open(szPath.GetBuffer(), CFile::modeCreate|CFile::modeWrite, &e ) )
	{
		SaveReportFile(&file);
	}
}

CString CProductBank::CalcPercent(int iEntire, int iValue)
{
	CString szPer;
	if( (iEntire == 0) || (iValue == 0) )
		szPer = "0";
	else
	{
		double dPer = (double)iValue/ (double)iEntire;
		szPer.Format(_T("%.2f"), dPer*100.);
		szPer += _T("%");
	}
	return szPer;
}

// 2018-03-14, NAM - Calculate Percentage ( Return Double)
double CProductBank::CalcPercent1(int iEntire, int iValue)
{
	double dPer;
	if( (iEntire == 0) || (iValue == 0) )
		dPer = 0;
	else
	{
		 dPer = (double)iValue/ (double)iEntire;
	}
	return dPer;
}

void CProductBank::InfoClear()
{
	m_VecDataSum.clear();
	ProductInfo Day, Sw, Gy;

	Day.tCount.iValue = 0;
	Day.tCount.uID = IDC_GXLBL_REPORT_NUM_SUN1;
	Day.tDefect.iValue = 0;
	Day.tDefect.uID = IDC_GXLBL_REPORT_GOOD_SUN1;
	Day.tReject.iValue = 0;
	Day.tReject.uID = IDC_GXLBL_REPORT_REJECT_SUN1;
	Day.tDefMCR.iValue = 0;
	Day.tDefMCR.uID = IDC_GXLBL_REPORT_MCR_SUN1;
	Day.tDefAlign.iValue = 0;
	Day.tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_SUN1;
	Day.tDefCont.iValue = 0;
	Day.tDefCont.uID = IDC_GXLBL_REPORT_CON_SUN1;

	m_VecDataSum.push_back(Day);

	Sw.tCount.iValue = 0;
	Sw.tCount.uID = IDC_GXLBL_REPORT_NUM_SUN2;
	Sw.tDefect.iValue = 0;
	Sw.tDefect.uID = IDC_GXLBL_REPORT_GOOD_SUN2;
	Sw.tReject.iValue = 0;
	Sw.tReject.uID = IDC_GXLBL_REPORT_REJECT_SUN2;
	Sw.tDefMCR.iValue = 0;
	Sw.tDefMCR.uID = IDC_GXLBL_REPORT_MCR_SUN2;
	Sw.tDefAlign.iValue = 0;
	Sw.tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_SUN2;
	Sw.tDefCont.iValue = 0;
	Sw.tDefCont.uID = IDC_GXLBL_REPORT_CON_SUN2;

	m_VecDataSum.push_back(Sw);

	Gy.tCount.iValue = 0;
	Gy.tCount.uID = IDC_GXLBL_REPORT_NUM_SUN3;
	Gy.tDefect.iValue = 0;
	Gy.tDefect.uID = IDC_GXLBL_REPORT_GOOD_SUN3;
	Gy.tReject.iValue = 0;
	Gy.tReject.uID = IDC_GXLBL_REPORT_REJECT_SUN3;
	Gy.tDefMCR.iValue = 0;
	Gy.tDefMCR.uID = IDC_GXLBL_REPORT_MCR_SUN3;
	Gy.tDefAlign.iValue = 0;
	Gy.tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_SUN3;
	Gy.tDefCont.iValue = 0;
	Gy.tDefCont.uID = IDC_GXLBL_REPORT_CON_SUN3;

	m_VecDataSum.push_back(Gy);

	//---	WHOLE DAY INIT

	m_DataWholeDay.tCount.iValue = 0;
	m_DataWholeDay.tCount.uID = IDC_GXLBL_REPORT_NUM_SUN4;

	m_DataWholeDay.tDefect.iValue = 0;
	m_DataWholeDay.tDefect.uID = IDC_GXLBL_REPORT_GOOD_SUN4;

	m_DataWholeDay.tReject.iValue = 0;
	m_DataWholeDay.tReject.uID = IDC_GXLBL_REPORT_REJECT_SUN4;

	m_DataWholeDay.tDefMCR.iValue = 0;
	m_DataWholeDay.tDefMCR.uID = IDC_GXLBL_REPORT_MCR_SUN4;

	m_DataWholeDay.tDefAlign.iValue = 0;
	m_DataWholeDay.tDefAlign.uID = IDC_GXLBL_REPORT_ALIGN_SUN4;

	m_DataWholeDay.tDefCont.iValue = 0;
	m_DataWholeDay.tDefCont.uID = IDC_GXLBL_REPORT_CON_SUN4;

}

void CProductBank::InitMainCountValue()
{
	m_MainCountINPUT.iValue = 0;
	m_MainCountINPUT.uID = IDC_GXLBL_COUNT_INPUT;
	m_MainCountGOOD.iValue = 0;
	m_MainCountGOOD.uID = IDC_GXLBL_COUNT_GOOD;
	m_MainCountREJECT.iValue = 0;
	m_MainCountREJECT.uID = IDC_GXLBL_COUNT_REJECT;
	m_MainCountMCR.iValue = 0;
	m_MainCountMCR.uID = IDC_GXLBL_COUNT_MCR;
	m_MainCountCONTACTALIGN.iValue = 0;
	m_MainCountCONTACTALIGN.uID = IDC_GXLBL_COUNT_CONTACTALIGN;

	//20170206 byskcho
	//결과율 추가
	m_MainCountEDGEALIGN.iValue = 0;
	m_MainCountEDGEALIGN.uID = IDC_GXLBL_COUNT_EDGE;

	m_MainCountCONTACT.iValue = 0;
	m_MainCountCONTACT.uID = IDC_GXLBL_COUNT_CONTACT;

	m_MainCountVISIONALIGN.iValue = 0;
	m_MainCountVISIONALIGN.uID = IDC_GXLBL_COUNT_VISION_ALIGN;
	
}



void CProductBank::UpdateMainInputCount()
{
	m_MainCountINPUT.iValue++;
	SaveMainCount();
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_COUNT_UPDATE, NULL, NULL);
}

void CProductBank::UpdateMainInputCountMinus()
{
	m_MainCountINPUT.iValue--;
	SaveMainCount();
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_COUNT_UPDATE, NULL, NULL);
}

void CProductBank::UpdateMainOutputCount( CCellInfo *pCell )
{
	if(pCell->m_LastClass == GOOD_CELL)
	{
		m_MainCountGOOD.iValue++;
	}
	else
	{
		m_MainCountREJECT.iValue++;
	}

	if(pCell->m_strCellID != FAIL)
	//if(pCell->m_strCellID != UV_FAIL)//2017-04-05,skcho, 코그닉스에서 FAIL 로 데이터를 줌
	{
		m_MainCountMCR.iValue++;
	}

	if (pCell->Pre_AlignResultXY == ALIGN_RESULT_GOOD)
	{
		m_MainCountCONTACTALIGN.iValue++;
	}

	if (pCell->m_Zone_B.m_nVI_AlignResult == ALIGN_RESULT_GOOD)
	{
		m_MainCountVISIONALIGN.iValue++;
	}

	if(pCell->m_strEdgeResult == _T("GOOD"))
	{
		m_MainCountEDGEALIGN.iValue++;
	}

	if(pCell->m_strContactResult == _T("GOOD"))
	{
		m_MainCountCONTACT.iValue++;
	}

	SaveMainCount();
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eFORM_Main)->m_hWnd, UDMSG_COUNT_UPDATE, NULL, NULL);
}

void CProductBank::SaveMainCount()
{
	CString strPath;
	strPath.Format(_T("%s\\MainCount.ini"), theConfigBank.m_System.m_strConfigPath);

	CIni ini(strPath);
	ini.WriteLong(_T("COUNT"), _T("INPUT"), m_MainCountINPUT.iValue);
	ini.WriteLong(_T("COUNT"), _T("GOOD"), m_MainCountGOOD.iValue);
	ini.WriteLong(_T("COUNT"), _T("REJECT"), m_MainCountREJECT.iValue);
	ini.WriteLong(_T("COUNT"), _T("MCR"), m_MainCountMCR.iValue);
	ini.WriteLong(_T("COUNT"), _T("CONTACTALIGN"), m_MainCountCONTACTALIGN.iValue);
	ini.WriteLong(_T("COUNT"), _T("VISIONALIGN"), m_MainCountVISIONALIGN.iValue);

	//20170206 byskcho
	ini.WriteLong(_T("COUNT"), _T("EDGEALIGN"), m_MainCountEDGEALIGN.iValue);
	ini.WriteLong(_T("COUNT"), _T("CONTACT"), m_MainCountCONTACT.iValue);

	ini.WriteLong(_T("TIME"), _T("YEAR"), m_timeMainCount.GetYear());
	ini.WriteLong(_T("TIME"), _T("MONTH"), m_timeMainCount.GetMonth());
	ini.WriteLong(_T("TIME"), _T("DAY"), m_timeMainCount.GetDay());
	ini.WriteLong(_T("TIME"), _T("HOUR"), m_timeMainCount.GetHour());
	ini.WriteLong(_T("TIME"), _T("MIN"), m_timeMainCount.GetMinute());
	ini.WriteLong(_T("TIME"), _T("SEC"), m_timeMainCount.GetSecond());

	//2017-12-01,SKCHO
	ini.WriteLong(_T("RLDROBOT"), _T("TOTAL"),	m_LDROBOT_RetryInfo.nTotal);
	ini.WriteLong(_T("RLDROBOT"), _T("SUCESS"), m_LDROBOT_RetryInfo.nSucess);
	ini.WriteLong(_T("RPATR"), _T("TOTAL"),		m_PATR_RetryInfo.nTotal);
	ini.WriteLong(_T("RPATR"), _T("SUCESS"),	m_PATR_RetryInfo.nSucess);
	ini.WriteLong(_T("RPASTAGE"), _T("TOTAL"),	m_PASTAGE_RetryInfo.nTotal);
	ini.WriteLong(_T("RPASTAGE"), _T("SUCESS"), m_PASTAGE_RetryInfo.nSucess);
	ini.WriteLong(_T("RULDTR"), _T("TOTAL"),	m_ULDTR_RetryInfo.nTotal);
	ini.WriteLong(_T("RULDTR"), _T("SUCESS"),	m_ULDTR_RetryInfo.nSucess);
	ini.WriteLong(_T("ROKTR"), _T("TOTAL"),		m_OKTR_RetryInfo.nTotal);
	ini.WriteLong(_T("ROKTR"), _T("SUCESS"),	m_OKTR_RetryInfo.nSucess);
	ini.WriteLong(_T("RNGTR"), _T("TOTAL"),		m_NGTR_RetryInfo.nTotal);
	ini.WriteLong(_T("RNGTR"), _T("SUCESS"),	m_NGTR_RetryInfo.nSucess);

}

void CProductBank::LoadMainCount()
{
	CString strPath;
	strPath.Format(_T("%s\\MainCount.ini"), theConfigBank.m_System.m_strConfigPath);

	CIni ini(strPath);
	m_MainCountINPUT.iValue = ini.GetLong(_T("COUNT"), _T("INPUT"), 0);
	m_MainCountGOOD.iValue = ini.GetLong(_T("COUNT"), _T("GOOD"), 0);
	m_MainCountREJECT.iValue = ini.GetLong(_T("COUNT"), _T("REJECT"), 0);
	m_MainCountMCR.iValue = ini.GetLong(_T("COUNT"), _T("MCR"), 0);
	m_MainCountCONTACTALIGN.iValue = ini.GetLong(_T("COUNT"), _T("CONTACTALIGN"), 0);
	m_MainCountVISIONALIGN.iValue = ini.GetLong(_T("COUNT"), _T("VISIONALIGN"), 0);
	
	//20170206 byskcho
	m_MainCountEDGEALIGN.iValue = ini.GetLong(_T("COUNT"), _T("EDGEALIGN"), 0);
	m_MainCountCONTACT.iValue = ini.GetLong(_T("COUNT"), _T("CONTACT"), 0);

	int nYear = ini.GetLong(_T("TIME"), _T("YEAR"), 2000);
	int nMonth = ini.GetLong(_T("TIME"), _T("MONTH"), 1);
	int nDay = ini.GetLong(_T("TIME"), _T("DAY"), 1);
	int nHour = ini.GetLong(_T("TIME"), _T("HOUR"), 0);
	int nMin = ini.GetLong(_T("TIME"), _T("MIN"), 0);
	int nSec = ini.GetLong(_T("TIME"), _T("SEC"), 0);

	m_timeMainCount = CTime(nYear, nMonth, nDay, nHour, nMin, nSec);

	//2017-12-01,SKCHO
	m_LDROBOT_RetryInfo.nTotal	= ini.GetLong(_T("RLDROBOT"), _T("TOTAL"),	0);
	m_LDROBOT_RetryInfo.nSucess = ini.GetLong(_T("RLDROBOT"), _T("SUCESS"), 0);
	m_PATR_RetryInfo.nTotal		= ini.GetLong(_T("RPATR"), _T("TOTAL"),		0);
	m_PATR_RetryInfo.nSucess	= ini.GetLong(_T("RPATR"), _T("SUCESS"),	0);
	m_PASTAGE_RetryInfo.nTotal	= ini.GetLong(_T("RPASTAGE"), _T("TOTAL"),	0);
	m_PASTAGE_RetryInfo.nSucess = ini.GetLong(_T("RPASTAGE"), _T("SUCESS"), 0);
	m_ULDTR_RetryInfo.nTotal	= ini.GetLong(_T("RULDTR"), _T("TOTAL"),	0);
	m_ULDTR_RetryInfo.nSucess	= ini.GetLong(_T("RULDTR"), _T("SUCESS"),	0);
	m_OKTR_RetryInfo.nTotal		= ini.GetLong(_T("ROKTR"), _T("TOTAL"),		0);
	m_OKTR_RetryInfo.nSucess	= ini.GetLong(_T("ROKTR"), _T("SUCESS"),	0);
	m_NGTR_RetryInfo.nTotal		= ini.GetLong(_T("RNGTR"), _T("TOTAL"),		0);
	m_NGTR_RetryInfo.nSucess	= ini.GetLong(_T("RNGTR"), _T("SUCESS"),	0);


}

void CProductBank::CalcAllInfo()
{
	InfoClear();

	short iHour = 0, iSize = m_MapData.size();
	for(int i = 0; i < iSize+1; i++ )
	{
		iHour = m_MapData[i].tHour.iValue;
		//if ( iHour == 0 )	continue;
		if ( iHour > 6 && iHour < 15 )					// DAY
		{
			m_VecDataSum[0].tCount.iValue += m_MapData[i].tCount.iValue;
			m_VecDataSum[0].tDefect.iValue += m_MapData[i].tDefect.iValue;
			m_VecDataSum[0].tReject.iValue += m_MapData[i].tReject.iValue;
			m_VecDataSum[0].tDefMCR.iValue += m_MapData[i].tDefMCR.iValue;
			m_VecDataSum[0].tDefAlign.iValue += m_MapData[i].tDefAlign.iValue;
			m_VecDataSum[0].tDefCont.iValue += m_MapData[i].tDefCont.iValue;	

			//20170206 byskcho
			m_VecDataSum[0].tDefEdgeAlign.iValue += m_MapData[i].tDefEdgeAlign.iValue;
			m_VecDataSum[0].tDefContact.iValue += m_MapData[i].tDefContact.iValue;

		}
		else if ( iHour > 14 && iHour < 23)				//	SW
		{
			m_VecDataSum[1].tCount.iValue += m_MapData[i].tCount.iValue;
			m_VecDataSum[1].tDefect.iValue += m_MapData[i].tDefect.iValue;
			m_VecDataSum[1].tReject.iValue += m_MapData[i].tReject.iValue;
			m_VecDataSum[1].tDefMCR.iValue += m_MapData[i].tDefMCR.iValue;
			m_VecDataSum[1].tDefAlign.iValue += m_MapData[i].tDefAlign.iValue;
			m_VecDataSum[1].tDefCont.iValue += m_MapData[i].tDefCont.iValue;

			//20170206 byskcho
			m_VecDataSum[1].tDefEdgeAlign.iValue += m_MapData[i].tDefEdgeAlign.iValue;
			m_VecDataSum[1].tDefContact.iValue += m_MapData[i].tDefContact.iValue;
		}
		else											//	GY
		{
			m_VecDataSum[2].tCount.iValue += m_MapData[i].tCount.iValue;
			m_VecDataSum[2].tDefect.iValue += m_MapData[i].tDefect.iValue;
			m_VecDataSum[2].tReject.iValue += m_MapData[i].tReject.iValue;
			m_VecDataSum[2].tDefMCR.iValue += m_MapData[i].tDefMCR.iValue;
			m_VecDataSum[2].tDefAlign.iValue += m_MapData[i].tDefAlign.iValue;
			m_VecDataSum[2].tDefCont.iValue += m_MapData[i].tDefCont.iValue;

			//20170206 byskcho
			m_VecDataSum[2].tDefEdgeAlign.iValue += m_MapData[i].tDefEdgeAlign.iValue;
			m_VecDataSum[2].tDefContact.iValue += m_MapData[i].tDefContact.iValue;
		}
	}

	m_DataWholeDay.tCount.iValue = m_VecDataSum[0].tCount.iValue +			m_VecDataSum[1].tCount.iValue +		m_VecDataSum[2].tCount.iValue;
	m_DataWholeDay.tDefect.iValue = m_VecDataSum[0].tDefect.iValue +		m_VecDataSum[1].tDefect.iValue +	m_VecDataSum[2].tDefect.iValue;
	m_DataWholeDay.tReject.iValue = m_VecDataSum[0].tReject.iValue +		m_VecDataSum[1].tReject.iValue +	m_VecDataSum[2].tReject.iValue;
	m_DataWholeDay.tDefMCR.iValue = m_VecDataSum[0].tDefMCR.iValue +		m_VecDataSum[1].tDefMCR.iValue +	m_VecDataSum[2].tDefMCR.iValue;
	m_DataWholeDay.tDefAlign.iValue = m_VecDataSum[0].tDefAlign.iValue +	m_VecDataSum[1].tDefAlign.iValue +	m_VecDataSum[2].tDefAlign.iValue;
	m_DataWholeDay.tDefCont.iValue = m_VecDataSum[0].tDefCont.iValue +		m_VecDataSum[1].tDefCont.iValue +	m_VecDataSum[2].tDefCont.iValue;

	m_DataWholeDay.tDefEdgeAlign.iValue = m_VecDataSum[0].tDefEdgeAlign.iValue +	m_VecDataSum[1].tDefEdgeAlign.iValue +	m_VecDataSum[2].tDefEdgeAlign.iValue;
	m_DataWholeDay.tDefContact.iValue = m_VecDataSum[0].tDefContact.iValue +		m_VecDataSum[1].tDefContact.iValue +	m_VecDataSum[2].tDefContact.iValue;
}

// 2018-03-16, NAM, To get whole Day Data. 
void CProductBank::ProductDayCall()
{
	InfoClear();

	short iHour = 0, iSize = m_MapData.size();
	for(int i = 0; i < iSize+1; i++ )
	{
		iHour = m_MapData[i].tHour.iValue;
		
			m_VecDataSum[0].tCount.iValue += m_MapData[i].tCount.iValue;
			m_VecDataSum[0].tDefect.iValue += m_MapData[i].tDefect.iValue;
			m_VecDataSum[0].tReject.iValue += m_MapData[i].tReject.iValue;
			m_VecDataSum[0].tDefMCR.iValue += m_MapData[i].tDefMCR.iValue;
			m_VecDataSum[0].tDefAlign.iValue += m_MapData[i].tDefAlign.iValue;
			m_VecDataSum[0].tDefCont.iValue += m_MapData[i].tDefCont.iValue;	

			//20170206 byskcho
			m_VecDataSum[0].tDefEdgeAlign.iValue += m_MapData[i].tDefEdgeAlign.iValue;
			m_VecDataSum[0].tDefContact.iValue += m_MapData[i].tDefContact.iValue;
		
	}

	m_DataWholeDay.tCount.iValue = m_VecDataSum[0].tCount.iValue ;
	m_DataWholeDay.tDefect.iValue = m_VecDataSum[0].tDefect.iValue ;
	m_DataWholeDay.tReject.iValue = m_VecDataSum[0].tReject.iValue ;
	m_DataWholeDay.tDefMCR.iValue = m_VecDataSum[0].tDefMCR.iValue ;
	m_DataWholeDay.tDefAlign.iValue = m_VecDataSum[0].tDefAlign.iValue ;
	m_DataWholeDay.tDefCont.iValue = m_VecDataSum[0].tDefCont.iValue ;

	m_DataWholeDay.tDefEdgeAlign.iValue = m_VecDataSum[0].tDefEdgeAlign.iValue ;
	m_DataWholeDay.tDefContact.iValue = m_VecDataSum[0].tDefContact.iValue ;
}

// 2018-03-16, NAM, To save data for only from 7am to 1 hour ago.  
void CProductBank::ProductDayCall1()
{
	InfoClear();

	short iHour = 0, iSize = m_MapData.size();
	for(int i = 0; i < iSize+1; i++ )
	{
		iHour = m_MapData[i].tHour.iValue;
		if (iHour<=theProcBank.m_CurDate3.wHour)
		{
			m_VecDataSum[0].tCount.iValue += m_MapData[i].tCount.iValue;
			m_VecDataSum[0].tDefect.iValue += m_MapData[i].tDefect.iValue;
			m_VecDataSum[0].tReject.iValue += m_MapData[i].tReject.iValue;
			m_VecDataSum[0].tDefMCR.iValue += m_MapData[i].tDefMCR.iValue;
			m_VecDataSum[0].tDefAlign.iValue += m_MapData[i].tDefAlign.iValue;
			m_VecDataSum[0].tDefCont.iValue += m_MapData[i].tDefCont.iValue;	

			//20170206 byskcho
			m_VecDataSum[0].tDefEdgeAlign.iValue += m_MapData[i].tDefEdgeAlign.iValue;
			m_VecDataSum[0].tDefContact.iValue += m_MapData[i].tDefContact.iValue;
		}
	}

	m_DataWholeDay.tCount.iValue = m_VecDataSum[0].tCount.iValue ;
	m_DataWholeDay.tDefect.iValue = m_VecDataSum[0].tDefect.iValue ;
	m_DataWholeDay.tReject.iValue = m_VecDataSum[0].tReject.iValue ;
	m_DataWholeDay.tDefMCR.iValue = m_VecDataSum[0].tDefMCR.iValue ;
	m_DataWholeDay.tDefAlign.iValue = m_VecDataSum[0].tDefAlign.iValue ;
	m_DataWholeDay.tDefCont.iValue = m_VecDataSum[0].tDefCont.iValue ;

	m_DataWholeDay.tDefEdgeAlign.iValue = m_VecDataSum[0].tDefEdgeAlign.iValue ;
	m_DataWholeDay.tDefContact.iValue = m_VecDataSum[0].tDefContact.iValue ;
}


void CProductBank::SetCurrentTime(CString strTime)
{
	m_strCurTime = strTime;
}

void CProductBank::InitJigCountValue()
{
	//2017-06-18 JSJUNG, JIG CONTACT COUNT 추가
	m_JigTotalCountCONTACT[0][0].iValue = 0;
	m_JigTotalCountCONTACT[0][0].uID = IDC_GXLBL_STATEMENT_A1_TOTAL;

	m_JigTotalCountCONTACT[0][1].iValue = 0;
	m_JigTotalCountCONTACT[0][1].uID = IDC_GXLBL_STATEMENT_A2_TOTAL;

	m_JigTotalCountCONTACT[1][0].iValue = 0;
	m_JigTotalCountCONTACT[1][0].uID = IDC_GXLBL_STATEMENT_B1_TOTAL;

	m_JigTotalCountCONTACT[1][1].iValue = 0;
	m_JigTotalCountCONTACT[1][1].uID = IDC_GXLBL_STATEMENT_B2_TOTAL;

	m_JigTotalCountCONTACT[2][0].iValue = 0;
	m_JigTotalCountCONTACT[2][0].uID = IDC_GXLBL_STATEMENT_C1_TOTAL;

	m_JigTotalCountCONTACT[2][1].iValue = 0;
	m_JigTotalCountCONTACT[2][1].uID = IDC_GXLBL_STATEMENT_C2_TOTAL;

	m_JigTotalCountCONTACT[3][0].iValue = 0;
	m_JigTotalCountCONTACT[3][0].uID = IDC_GXLBL_STATEMENT_D1_TOTAL;

	m_JigTotalCountCONTACT[3][1].iValue = 0;
	m_JigTotalCountCONTACT[3][1].uID = IDC_GXLBL_STATEMENT_D2_TOTAL;

	m_JigGoodCountCONTACT[0][0].iValue = 0;
	m_JigGoodCountCONTACT[0][0].uID = IDC_GXLBL_STATEMENT_A1_AUTO;

	m_JigGoodCountCONTACT[0][1].iValue = 0;
	m_JigGoodCountCONTACT[0][1].uID = IDC_GXLBL_STATEMENT_A2_AUTO;

	m_JigGoodCountCONTACT[1][0].iValue = 0;
	m_JigGoodCountCONTACT[1][0].uID = IDC_GXLBL_STATEMENT_B1_AUTO;

	m_JigGoodCountCONTACT[1][1].iValue = 0;
	m_JigGoodCountCONTACT[1][1].uID = IDC_GXLBL_STATEMENT_B2_AUTO;

	m_JigGoodCountCONTACT[2][0].iValue = 0;
	m_JigGoodCountCONTACT[2][0].uID = IDC_GXLBL_STATEMENT_C1_AUTO;

	m_JigGoodCountCONTACT[2][1].iValue = 0;
	m_JigGoodCountCONTACT[2][1].uID = IDC_GXLBL_STATEMENT_C2_AUTO;

	m_JigGoodCountCONTACT[3][0].iValue = 0;
	m_JigGoodCountCONTACT[3][0].uID = IDC_GXLBL_STATEMENT_D1_AUTO;

	m_JigGoodCountCONTACT[3][1].iValue = 0;
	m_JigGoodCountCONTACT[3][1].uID = IDC_GXLBL_STATEMENT_D2_AUTO;

}

//2017-06-18 JSJUNG, JIG CONTACT COUNT 추가
void CProductBank::UpdateJigOutputCount( CCellInfo *pCell )
{
	m_JigTotalCountCONTACT[pCell->m_JigId][pCell->m_JigCh].iValue++;
	if(pCell->m_strContactResult == _T("GOOD"))
	{
		m_JigGoodCountCONTACT[pCell->m_JigId][pCell->m_JigCh].iValue++;
	}

	SaveJigCount();
	::PostMessage(GetMainHandler()->m_pGUIFrame->GetView(eDLG_Statement)->m_hWnd, UDMSG_CONTACT_JIGCNT, NULL, NULL);
}

//2017-06-18 JSJUNG, JIG CONTACT COUNT 추가
void CProductBank::SaveJigCount()
{
	CString strPath;
	strPath.Format(_T("%s\\JigCount.ini"), theConfigBank.m_System.m_strConfigPath);

	CIni ini(strPath);
	ini.WriteLong(_T("TOTAL"), _T("A1"), m_JigTotalCountCONTACT[0][0].iValue);
	ini.WriteLong(_T("TOTAL"), _T("A2"), m_JigTotalCountCONTACT[0][1].iValue);
	ini.WriteLong(_T("TOTAL"), _T("B1"), m_JigTotalCountCONTACT[1][0].iValue);
	ini.WriteLong(_T("TOTAL"), _T("B2"), m_JigTotalCountCONTACT[1][1].iValue);
	ini.WriteLong(_T("TOTAL"), _T("C1"), m_JigTotalCountCONTACT[2][0].iValue);
	ini.WriteLong(_T("TOTAL"), _T("C2"), m_JigTotalCountCONTACT[2][1].iValue);
	ini.WriteLong(_T("TOTAL"), _T("D1"), m_JigTotalCountCONTACT[3][0].iValue);
	ini.WriteLong(_T("TOTAL"), _T("D2"), m_JigTotalCountCONTACT[3][1].iValue);

	ini.WriteLong(_T("GOODCNT"), _T("A1"), m_JigGoodCountCONTACT[0][0].iValue);
	ini.WriteLong(_T("GOODCNT"), _T("A2"), m_JigGoodCountCONTACT[0][1].iValue);
	ini.WriteLong(_T("GOODCNT"), _T("B1"), m_JigGoodCountCONTACT[1][0].iValue);
	ini.WriteLong(_T("GOODCNT"), _T("B2"), m_JigGoodCountCONTACT[1][1].iValue);
	ini.WriteLong(_T("GOODCNT"), _T("C1"), m_JigGoodCountCONTACT[2][0].iValue);
	ini.WriteLong(_T("GOODCNT"), _T("C2"), m_JigGoodCountCONTACT[2][1].iValue);
	ini.WriteLong(_T("GOODCNT"), _T("D1"), m_JigGoodCountCONTACT[3][0].iValue);
	ini.WriteLong(_T("GOODCNT"), _T("D2"), m_JigGoodCountCONTACT[3][1].iValue);

}

//2017-06-18 JSJUNG, JIG CONTACT COUNT 추가
void CProductBank::LoadJigCount()
{
	CString strPath;
	strPath.Format(_T("%s\\JigCount.ini"), theConfigBank.m_System.m_strConfigPath);

	CIni ini(strPath);

	m_JigTotalCountCONTACT[0][0].iValue = ini.GetLong(_T("TOTAL"), _T("A1"), 0);
	m_JigTotalCountCONTACT[0][1].iValue = ini.GetLong(_T("TOTAL"), _T("A2"), 0);
	m_JigTotalCountCONTACT[1][0].iValue	= ini.GetLong(_T("TOTAL"), _T("B1"), 0);
	m_JigTotalCountCONTACT[1][1].iValue	= ini.GetLong(_T("TOTAL"), _T("B2"), 0);
	m_JigTotalCountCONTACT[2][0].iValue	= ini.GetLong(_T("TOTAL"), _T("C1"), 0);
	m_JigTotalCountCONTACT[2][1].iValue	= ini.GetLong(_T("TOTAL"), _T("C2"), 0);
	m_JigTotalCountCONTACT[3][0].iValue	= ini.GetLong(_T("TOTAL"), _T("D1"), 0);
	m_JigTotalCountCONTACT[3][1].iValue	= ini.GetLong(_T("TOTAL"), _T("D2"), 0);

	m_JigGoodCountCONTACT[0][0].iValue = ini.GetLong(_T("GOODCNT"), _T("A1"), 0);
	m_JigGoodCountCONTACT[0][1].iValue = ini.GetLong(_T("GOODCNT"), _T("A2"), 0);
	m_JigGoodCountCONTACT[1][0].iValue = ini.GetLong(_T("GOODCNT"), _T("B1"), 0);
	m_JigGoodCountCONTACT[1][1].iValue = ini.GetLong(_T("GOODCNT"), _T("B2"), 0);
	m_JigGoodCountCONTACT[2][0].iValue = ini.GetLong(_T("GOODCNT"), _T("C1"), 0);
	m_JigGoodCountCONTACT[2][1].iValue = ini.GetLong(_T("GOODCNT"), _T("C2"), 0);
	m_JigGoodCountCONTACT[3][0].iValue = ini.GetLong(_T("GOODCNT"), _T("D1"), 0);
	m_JigGoodCountCONTACT[3][1].iValue = ini.GetLong(_T("GOODCNT"), _T("D2"), 0);


}

void CProductBank::InitRetryCount()
{
	memset(&m_LDROBOT_RetryInfo, 0,sizeof(tRetryInfo));
	memset(&m_PATR_RetryInfo, 0,sizeof(tRetryInfo));
	memset(&m_PASTAGE_RetryInfo, 0,sizeof(tRetryInfo));
	memset(&m_ULDTR_RetryInfo, 0,sizeof(tRetryInfo));
	memset(&m_OKTR_RetryInfo, 0,sizeof(tRetryInfo));
	memset(&m_NGTR_RetryInfo, 0,sizeof(tRetryInfo));
}