#include "StdAfx.h"
#include "LossTimeBank.h"

#include "GxUtility.h"
#include "File\GxIniFile.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxMsgBox.h"


CLossTimeBank::CLossTimeBank(void)
{

}


CLossTimeBank::~CLossTimeBank(void)
{
}
// 비가동 정보를 파일에서 읽어온다.
BOOL CLossTimeBank::LoadLosstimeItemList()
{
	//20170118  byskcho 삭제
	//CGxIniFile iniFile;
	CIni iniFile;
	CString sFilename;
	CString sGroup, sKey, sData, sLine, sTok;
	int i, j, k, n, n1stCount, n2ndCount, n3rdCount;
	int nLangIdx = 1;//CGxGUIFrame::GetInstance()->GetDispLanguage();			// 현재 표시되고 있는 언어 index

	sFilename.Format(_T("%s\\%s"), PATH_SYSTEM, NAME_FILE_LOSSTIME_LIST);

	// 파일이 존재하는지 점검
	if (CFileSupport::IsFile( sFilename ) == FALSE ) 
	{
		CString sMsg;
		sMsg.Format(_T("* File not found : %s"), sFilename);
		
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
		dlgMsgBox.SetMessage(FALSE, sMsg, sMsg, sMsg, nLangIdx);
		dlgMsgBox.DoModal();

		//::AfxMessageBox( sMsg );																// 오류 내용을 보여준다.
		return FALSE;
	}

	// 파일이 존재하면 파일에서 데이터를 읽어온다.
	iniFile.SetIniFilename(sFilename);

	// AUto Run-Down 항목을 읽어온다.
	m_rLosstimeList.sAutoRunDownCode	= iniFile.GetString(_T("Setting"), _T("AutoRunDown"),		_T("RD>15190>AUTO RUN DOWN>AUTO RUN DOWN>AUTO RUN DOWN"));		// AUTO RUN DOWN
	m_rLosstimeList.sOPLossCode		= iniFile.GetString(_T("Setting"), _T("OPLoss"),			_T("RD>0110>OP LOSS>OP LOSS>OP LOSS"));								// OP LOSS 선택 항목
	m_rLosstimeList.sTactTimeOver		= iniFile.GetString(_T("Setting"), _T("TactTimeOver"),	_T("RD>01200>TACT TIME OVER>TACT TIME OVER>TACT TIME OVER"));	// TATC TIME OVER 선택 항목


	// 전체 First Class 그룹 수량
	n1stCount = iniFile.GetLong( _T("GROUP"), _T("COUNT"),	0);
	
	// 1st class 최대 수량을 제한한다.
	if ( n1stCount > TLosstimeList::eMAX_1stCount ) n1stCount = TLosstimeList::eMAX_1stCount;

	m_rLosstimeList.nCount_1st	= n1stCount;				// 1st class 수

	for (i = 0; i < n1stCount; i++)
	{
		sGroup.Format( _T("GROUP_%02d"), i );			// 그룹이름, 00 ~ 07

		m_rLosstimeList.rLosstime[i].sName = iniFile.GetString( sGroup, _T("NAME"), _T("") );		// 1st class 이름
		n2ndCount = iniFile.GetLong( sGroup, _T("COUNT"),	0);												// 해당 1st Class의 2nd class 수

		if ( n2ndCount > TLoss1stClass::eMAX_2ndCount ) n2ndCount = TLoss1stClass::eMAX_2ndCount;	// 2nd class수 제한

		m_rLosstimeList.rLosstime[i].n2ndCount = n2ndCount;			// 해당 1stClass내 존재하는 2nd class 갯수

		// 2nd class 해당되는  항목을 읽어온다.
		for (j = 0; j < n2ndCount; j++ )
		{
			sGroup.Format( _T("GROUP_%02d"), i );			// 그룹이름, 00 ~ 07
			sKey.Format(_T("SUB_%02d"), j);				// Key
			m_rLosstimeList.rLosstime[i].r2ndClass[j].sLine	= iniFile.GetString( sGroup, sKey, _T("") );		// 2st class 이름
			
			// 각 언어별로 2개의 2nd class Name이 존재한다.
			sLine = m_rLosstimeList.rLosstime[i].r2ndClass[j].sLine;
			for (n = 0; n < eMAX_Language; n++)
			{
				// 각 언어별 2nd Class 이름을 '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
				if ( AfxExtractSubString( m_rLosstimeList.rLosstime[i].r2ndClass[j].sName[n], sLine, n, DELIMITER_DEFECT) == FALSE )	break;
			}//of for n				

			// 3rd Class data 읽기
			sGroup.Format(_T("GROUP_%02d_SUB_%02d"), i, j);				// 3rd Class를 읽기위한 Group 이름 지정
			n3rdCount = iniFile.GetLong( sGroup, _T("COUNT"), 0 );		// 3rd class 항목 수

			if ( n3rdCount > TLoss2ndClass::eMAX_3rdCount ) n3rdCount = TLoss2ndClass::eMAX_3rdCount;	// 3rd class수 제한

			m_rLosstimeList.rLosstime[i].r2ndClass[j].n3rdCount = n3rdCount;			// 해당 2ndClass내 존재하는 3rd class 갯수
			
			// 최종적인 3rd Class 항목을 읽어온다.
			for (k = 0; k < n3rdCount; k++)
			{
				sKey.Format(_T("ITEM_%02d"), k);		// Key
				m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sLine	= iniFile.GetString( sGroup, sKey, _T("") );		// 3rd class Data

				sLine = m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sLine;

				//예: ITEM_00   = PM>000>Shift점검>프로그램 점검>Check Program

				// Code 발췌 (현재는 미사용)
				AfxExtractSubString( m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sCode, sLine, 1, DELIMITER_DEFECT);	// 첫번째 항목인 Defect Code를 읽어낸다.

				for (n = 0; n < eMAX_Language; n++)
				{
					// 각 언어별 3rd Class 이름을 '>' 기호로 분리하지 못한 경우에는 바로 끝낸다.
					if ( AfxExtractSubString(m_rLosstimeList.rLosstime[i].r2ndClass[j].r3rdClass[k].sReason[n], sLine, (3 + n), DELIMITER_DEFECT) == FALSE )	break;
				}//of for n				
			}//of for k
		}//of for j
	}//of for i
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 비가동 로그 관련
//
// 비가동 정보를 임시 기억하기위한 초기화
void CLossTimeBank::InitLosstimeInfo()
{
	// ini file 설정 
	m_iniLosstime.m_sIniFile.Format(_T("%s\\%s"), PATH_SYSTEM, NAME_FILE_LOSSTIME_DATA);	// data 파일 경로 지정

	m_iniLosstime.AddStr(_T("LastInfo:sReasonLine"),	m_rLosstimeRecord.sReasonLine);				// 선택된 전체 Code 내용
	m_iniLosstime.AddStr(_T("LastInfo:sUserID"),		m_rLosstimeRecord.sID);							// Stop 시킨 작업자 ID
	m_iniLosstime.AddStr(_T("LastInfo:sUserIDNo"),		m_rLosstimeRecord.sIDNo);						// 작업자 사원번호;
	m_iniLosstime.AddStr(_T("LastInfo:sAlarmType"),		m_rLosstimeRecord.sAlarmType);				// Alarm 여부, 중/경 알람여부
	m_iniLosstime.AddStr(_T("LastInfo:sAlarmCode"),		m_rLosstimeRecord.sAlarmCode);				// Alarm Code
	m_iniLosstime.AddStr(_T("LastInfo:sUserMsg"),		m_rLosstimeRecord.sUserMsg);					// 사용자 입력 내용

	m_iniLosstime.AddStr(_T("LastInfo:sStartTime"),		m_rLosstimeRecord.sStartTime);		// 정지 시작 시각
	m_iniLosstime.AddStr(_T("LastInfo:sResetTime"),		m_rLosstimeRecord.sResetTime);		// 인지 시각

	m_iniLosstime.AddStr(_T("LastInfo:sTPSystemName"),	m_rLosstimeRecord.sTPSystemName);		// TP System loss name
	m_iniLosstime.AddStr(_T("LastInfo:sTPSystemCode"),	m_rLosstimeRecord.sTPSystemCode);		// TP System loss code

}

// 파일에 저장된 비가동 정보를 읽어온다. (Program 실행시)
void CLossTimeBank::LoadLosstimeInfo()
{
	m_iniLosstime.Load();

	// 문자열 -> 날짜형으로 변형시켜준다.
	CGxUtility::StringToSystemTime( m_rLosstimeRecord.sStartTime,	m_rLosstimeRecord.tmStartTime );
	CGxUtility::StringToSystemTime( m_rLosstimeRecord.sResetTime,	m_rLosstimeRecord.tmResetTime );

	// 각 항목에 맞도록 분류해서 치환한다.
	if ( m_rLosstimeRecord.sReasonLine != _T("") )
	{
		::AfxExtractSubString( m_rLosstimeRecord.s1stClass,	m_rLosstimeRecord.sReasonLine, 0, DELIMITER_DEFECT);			// 1st class
		::AfxExtractSubString( m_rLosstimeRecord.sMESCode,	m_rLosstimeRecord.sReasonLine, 1, DELIMITER_DEFECT);			// MESCode
		::AfxExtractSubString( m_rLosstimeRecord.s2ndClass,	m_rLosstimeRecord.sReasonLine, 2, DELIMITER_DEFECT);			// 2nd class
		::AfxExtractSubString( m_rLosstimeRecord.s3rdClass,	m_rLosstimeRecord.sReasonLine, 3, DELIMITER_DEFECT);			// 3rd class
	}

}


// 현재 설정된 비가동 정보를 파일로 기록한다.
void CLossTimeBank::SaveLosstimeInfo()
{
	// ini file로 저장하기 위한 변수
	m_rLosstimeRecord.sStartTime = CGxUtility::SystemTimeToString( m_rLosstimeRecord.tmStartTime );						// 문자열로 변경된 시작 시간
	m_rLosstimeRecord.sResetTime = CGxUtility::SystemTimeToString( m_rLosstimeRecord.tmResetTime );						// 문자열로 변경된 인지 시간

	m_iniLosstime.Save();
}

// 비가동 관련 내용이 없다면 내용을 청소해준다.
void CLossTimeBank::ClearLosstimeInfo()
{
	//// ini file 설정 
	//m_rLosstimeRecord.sReasonLine		= _T("");					// 선택된 전체 Code 내용
	//m_rLosstimeRecord.sUserMsg			= _T("");
	//m_rLosstimeRecord.dbAddLossTime	= 0.0;

	//::GetLocalTime( &(m_rLosstimeRecord.tmStartTime) );		// 처리 종료하고 다시 시작 시간
	//::GetLocalTime( &(m_rLosstimeRecord.tmResetTime) );		// 처리 종료하고 다시 시작 시간
	//::GetLocalTime( &(m_rLosstimeRecord.tmEndTime) );			// 처리 종료하고 다시 시작 시간

	////2014-09-11, jhLee, 일단 현재는 정의된 내용이 없다.
	//m_rLosstimeRecord.sTPSystemName	= _T("UNDEFINDED");					// TP System loss name
	//m_rLosstimeRecord.sTPSystemCode	= _T("UNDEFINDED");					// TP System Loss Code

	m_rLosstimeRecord.Clear();										// 내용을 CLear한 후 저장한다.

	SaveLosstimeInfo();
}


void CLossTimeBank::InitialDataFile()
{
	// 새로운 방식의 Cell Log 파일을 기록한다.
	CString sHeader;
	CString sFile;

	// 비가동 Log에 대한 파일 설정을 한다.
	sHeader = _T("EQP ID,MODEL,ID,ALARM TYPE,START TIME,RESET TIME,END TIME,WAIT TIME,REPAIR TIME,DURATION,FIRST CLASS,SECOND CLASS,THIRD CLASS,TP SYSTEM LOSS NAME,TP SYSTEM LOSS CODE,ALARM CODE,REPAIR CODE");
	m_logLosstime.SetHeader( sHeader );

	sFile.Format(_T("Losstime_%s"), theConfigBank.m_System.m_strLineNo);						// 예: 20150413_LOSSTIME_Y01.csv
	m_logLosstime.SetFilePath(theConfigBank.m_System.m_strLossTimeLogPath, _T("MAIN"), sFile);				// Data Log
	
	m_logLosstime.m_bIsUnicode		= FALSE;				// MBCS로 저장한다.
	m_logLosstime.m_bKeepOpen		= TRUE;					// 파일을 열은 채로 처리하여 다른곳에서의 접근을 막는다.
	m_logLosstime.m_bMirrorCreate	= TRUE; 				// 미러 파일을 만든다.
	m_logLosstime.m_bIsHeaderPrint	= TRUE;																		// Header 출력
	m_logLosstime.m_bIsFolderCreate	= TRUE;
	m_logLosstime.m_sExtention		= _T("csv");			// 확장자 지정
	m_logLosstime.m_sMirrorSuffix	= _T("Temp");			// 미러링 파일의 접두사 지정
	m_logLosstime.m_sDeliMiliSec	= _T(":");				// 밀리초는 : 으로 구분해준다.
	// 6, 2, 1 : D:\UB-AMT\Log\Sample\201509\20150904_LogTest.txt	-> Loss Time (비가동 로그) 기록용 설정값
	m_logLosstime.m_nDirNameMode	= CGxLog::eNAME_YearMonth;
	m_logLosstime.m_nDirDatePos		= CGxLog::ePOS_DateLast;
	m_logLosstime.m_nFileDatePos	= CGxLog::ePOS_DateFirst;

}
// 지정한 비가동 레코드 데이터를 파일로 기록한다.
void CLossTimeBank::WriteLossTimeRecord(TLosstimeRecord& rRec, BOOL bPrevDay)
{
	CString sData;

	// Alarm에 의해 중지가 발생 한 경우 
	if ( (rRec.sAlarmType == _T("A")) && (rRec.s1stClass == _T("MD")) )
	{
		// 60초 이상이면 BM으로 전환한다.
		float fFlag = CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmEndTime);		// Duration time

		//2014-11-19, jhLee, 김길성책임 요청, MD/BM 자동 전환시 지정된 시간을 사용한다.
		//OLDif ( fFlag >= 60.0 ) // 지정한 60초를 넘겼다면,
		//if ( fFlag >= (float)(m_rProfileData.nTIME_MDChangeTime) ) // 지정한 시간을 넘겼다면,
		//{
		//	rRec.s1stClass = _T("BM");			// MD -> BM으로 자동 전환된다.
		//}
	}

	//////////////////////////////////////////////////////////////////////////
	// Data 기록

	// EQID;
	m_logLosstime.AddItem(_T("0") );
	m_logLosstime.AddItem(_T("0"));
    m_logLosstime.AddItem( _T("0") );				// 현재 작업자의 사원번호
	m_logLosstime.AddItem( rRec.sAlarmType );						// Alarm여부


	// 지정한 시간 보정값이 존재한다면,
	if ( rRec.dbAddLossTime > 0.0 )
	{
		//보류 CGxUtility::PrevSecondTime(rRec.dbAddLossTime, rRec.tmStartTime);		// 지정한 시간만큼 시작 시간을 당겨준다.
		m_logLosstime.AddItem_DateTime( rRec.tmStartTime , TRUE );		// 시작 시간
		m_logLosstime.AddItem_DateTime( rRec.tmResetTime, TRUE );		// 조치 시간
		m_logLosstime.AddItem_DateTime( rRec.tmEndTime, TRUE );		// 종료 시각
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmResetTime), 1);			// Wait time
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmResetTime, rRec.tmEndTime) + rRec.dbAddLossTime , 1);	// Repair time
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmEndTime) + rRec.dbAddLossTime , 1);	// Duration time
	}
	else
	{
		m_logLosstime.AddItem_DateTime( rRec.tmStartTime , TRUE );		// 시작 시간
		m_logLosstime.AddItem_DateTime( rRec.tmResetTime , TRUE );		// 조치 시간
		m_logLosstime.AddItem_DateTime( rRec.tmEndTime, TRUE );		// 종료 시각
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmResetTime), 1 );	// Wait time
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmResetTime, rRec.tmEndTime), 1 );		// Repair time
		m_logLosstime.AddItem_Double( CGxUtility::DiffSeconds(rRec.tmStartTime, rRec.tmEndTime), 1 );		// Duration time
	}

	m_logLosstime.AddItem( rRec.s1stClass);
	m_logLosstime.AddItem( rRec.s2ndClass);

	// 최종 사유가 사용자 입력이고, 사용자가 입력한 내용이 있으면 해당 내용으로 기록한다.
	sData = rRec.s3rdClass;

	if ( (rRec.s3rdClass == _T("입력")) || (rRec.s3rdClass == ("Input") ) )
	{
		if ( rRec.sUserMsg != _T("") ) 
		{
			sData.Format(_T("%s")	, rRec.sUserMsg);
		}
	}
	m_logLosstime.AddItem( sData );

	m_logLosstime.AddItem( rRec.sTPSystemName);
	m_logLosstime.AddItem( rRec.sTPSystemCode);
	m_logLosstime.AddItem( rRec.sAlarmCode);
	m_logLosstime.AddItem( rRec.sRepairCode);


	// 만약 강제로 어제 날짜로 저장하라고 설정되었다면, 적용한다.
	if ( bPrevDay == TRUE ) 
	{
		// 어제 날짜로 저장
		//!! m_logLosstime.Sa .SetFilenameOption(_T("PREV"));	// 옵션 적용
		m_logLosstime.WriteItem( -1 );			// 어제 날짜로 파일에 기록한다.
	}
	else
		m_logLosstime.WriteItem();			// 파일에 기록한다.

		

}



// 자동으로 생성되는 OP/EQ Loss time 전용 기록 함수.
void CLossTimeBank::WriteAutoLossTimeRecord(TLosstimeRecord& rRec)
{
	// EQID;
	m_logLosstime.AddItem( _T("0"));
	m_logLosstime.AddItem( _T("0"));
	m_logLosstime.AddItem( _T("0") );				// 현재 작업자의 사원번호
	m_logLosstime.AddItem( rRec.sAlarmType );						// Alarm여부


	// 시간 데이터는 계산을 통하지 않고 그대로 기록한다.
	m_logLosstime.AddItem_DateTime( rRec.tmStartTime , TRUE );		// 시작 시간
	m_logLosstime.AddItem_DateTime( rRec.tmResetTime , TRUE );		// 조치 시간
	m_logLosstime.AddItem_DateTime( rRec.tmEndTime, TRUE );			// 종료 시각
	m_logLosstime.AddItem_Double( rRec.fWaitTime, 1 );				// Wait time
	m_logLosstime.AddItem_Double( rRec.fRepairTime, 1 );			// Repair time
	m_logLosstime.AddItem_Double( rRec.fDuration, 1 );				// Duration time, 경과시간은 소숫점 1자리까지만 표기한다.

	m_logLosstime.AddItem( rRec.s1stClass);
	m_logLosstime.AddItem( rRec.s2ndClass);
	m_logLosstime.AddItem( rRec.s3rdClass );

	m_logLosstime.AddItem( rRec.sTPSystemName);
	m_logLosstime.AddItem( rRec.sTPSystemCode);
	m_logLosstime.AddItem( rRec.sAlarmCode);
	m_logLosstime.AddItem( rRec.sRepairCode);

	m_logLosstime.WriteItem();			// 파일에 기록한다.
	
}



//
// Alarm에 의해 강제적으로 비가동 상태가 종료되었다고 저장한다.
// 2015-03-09, jhLee, 바가동 상태로 설비가 멈춘 상태로 라이트커튼등의 감지로 알람이 발생하면 기존 비가동과 알람에 의한 비가동이 섞이게 된다.
// 이를 방지하기 위해 비가동 중지일때 알람이 발생하면, 기존의 비가동은 완료 시켜주고 알람으로 비가동 사유를 대체한다.
//
void CLossTimeBank::WriteLossTimeByAlarm()
{
	/*if ( m_rLosstimeRecord.sReasonLine == _T("") )				// 비가동 사유가 없다면 저장할 것도 없다.
	{
		return;
	}

	if ( m_rLosstimeRecord.sAlarmType == _T("A") )				// Alarm 발생 상태에서 다시 Alarm이 발생하였다 ? 논리상 오류로 이전에 발생한 Alarm 정보는 기록하지 않는다.
	{
		return;
	}


	::GetLocalTime( &(m_rLosstimeRecord.tmResetTime) );			// Reset 시간을 현재 시각으로 맞춘다.
	::GetLocalTime( &(m_rLosstimeRecord.tmEndTime) );			// 처리 종료 시각

	// Login과는 다른 사용자 입력이다.
	m_rLosstimeRecord.sID	= GetCurrentUserID();				// 현재 작업자 ID로 
	m_rLosstimeRecord.sIDNo	= GetCurrentUserIDNo();

	//
	// 중지 사유에 따른 처리
	// Auto RunDown에 의해 중지 되었다면,
	if ( (m_rLosstimeRecord.s2ndClass == _T("AUTO RUN DOWN")) || (m_rLosstimeRecord.s2ndClass == _T("AUTO RUNDOWN") ) )
	{
		// 비가동 Log 저장시 AUTO RUn down 기준 시간을 더하여 기록한다.
		m_rLosstimeRecord.dbAddLossTime =  m_rProfileData.nTIME_RunDownTime;
	}
	else
		m_rLosstimeRecord.dbAddLossTime = 0.0;

	// 파일로 기록을 한다.
	WriteLossTimeRecord( m_rLosstimeRecord, FALSE );			 

	ClearLosstimeInfo();														// 비가동 임시기억 초기화
	*/
}






// 작업자 시간초과 기록 (비가동 로그에 남긴다)
// 작업자 작업 시간이 정해진 시간을 초과하게되면 기록을 한다.
//
void CLossTimeBank::WriteOPLossTime(int nJigNo)
{
/*	if ( (nJigNo < 0) || (nJigNo >= eMAX_JigCount) )  return;		// index 

	TCellData* pCell = m_pCellInfo->GetCellData(nJigNo, eIDX_CH_1 );		// Cell Data 1개를 가져온다. Cell 존재여부와 상관없이 시각은 기록되므로 사용이 가능하다.

	// OP 작업시간이 기준보다 초과되었나 ?
	if ( (pCell->dbMoveTACT[eTACT_OPLossTime] < 1.0 )			// 1초 미만이라면 기록하지 않는다. 기록은 1초 단위의 정수형이므로 0초가 기록되는 경우가 많다.
		|| (pCell->dbMoveTACT[eTACT_OPLossTime] > 120.0 ) )		// 2015-04-10, jhLee, 120 초, 즉 60초를 초과하는 값은 무시하도록 한다.
	{
		// m_pLogSeq->Write(_T("[MAIN] Write OP Loss time Skip : %.3f"), dbOPLoss);			
		return;		// 초과된 시간이 없으므로 기록을 할 필요가 없다.
	}

	TLosstimeRecord	rLossRec;	// OP Loss Log를 남기기 위한 데이터 기록
	float fLossTime = (float)( pCell->dbMoveTACT[eTACT_OPLossTime] / 2.0 );	// 2014-11-27, 김길성 책임 요청, 기준 시간을 더하는것을 취소, + m_rProfileData.nTIME_OPLossTime);	// 작업자 손실 시간

	//2015/03/29 JH-PARK
	//시간별 OPLOSS를 기록한다
	SaveTodayProductData(eIDX_Product_TodayOPLoss, int(fLossTime));
				
	rLossRec.sReasonLine	= _T("");					// 선택된 전체 Code 내용
	rLossRec.s1stClass		= _T("RD");					// 각 Class별 선택 내용
	rLossRec.s2ndClass		= _T("OP LOSS");
	rLossRec.s3rdClass		= _T("OP LOSS");
	rLossRec.sMESCode		= _T("000");				// 상위 보고용 코드, 현재 미사용
	rLossRec.sID			= m_rCurrentUser.sID;		// Stop 시킨 작업자 ID
	rLossRec.sIDNo			= m_rCurrentUser.sIDNo;		// 작업자 사원번호;
	rLossRec.sAlarmType		= _T("-");					// Alarm 여부, 중/경 알람여부
	rLossRec.sAlarmCode		= _T("");					// Alarm Code
	rLossRec.sRepairCode	= _T("");					// MD/BM의 경우 조치내역
	rLossRec.sUserMsg		= _T("");					// 사용자 정의 입력
	rLossRec.fWaitTime		= 0.0;						// 알람발생 ~ 고장조치 시작까지
	rLossRec.fRepairTime	= 0.0;						// 실제 조치가 일어난 시간
	rLossRec.fDuration		= fLossTime;				// 작업자 손실 시간
	rLossRec.dbAddLossTime	= 0.0;						// 추가적인 더하는 시각

	//2014-12-24, jhLee, TPSystem code 지정
	rLossRec.sTPSystemName	= _T("OP LOSS");			// TP System loss name
	rLossRec.sTPSystemCode	= _T("01100");				// TP System Loss Code

	// ::GetLocalTime( &(rLossRec.tmStartTime) );			// 정지 종료시각

	//OLD CGxUtility::PrevSecondTime( (int)(pCell->dbMoveTACT[eTACT_OPLossTime]),  rLossRec.tmStartTime );	// 정지 시작 시각
	//2015-03-12, 현재시각으로 모두 통일 CGxUtility::PrevSecondTime( (int)fLossTime,  rLossRec.tmStartTime );	// 정지 시작 시각

	::GetLocalTime( &(rLossRec.tmStartTime) );			// 시각 시각
	::GetLocalTime( &(rLossRec.tmResetTime) );			// 인지 시각
	::GetLocalTime( &(rLossRec.tmEndTime) );			// 정지 종료시각

	//old WriteLossTimeRecord( rLossRec, FALSE );
	// WriteLossTimeRecord_new( rLossRec, FALSE );
	WriteAutoLossTimeRecord( rLossRec );
	*/
}


// 설비 시간초과 기록 (비가동 로그에 남긴다)
// 설비 작업 시간이 정해진 시간을 초과하게되면 기록을 한다.
//
void CLossTimeBank::WriteEQLossTime(int nJigNo)
{
	/*if ( (nJigNo < 0) || (nJigNo >= eMAX_JigCount) )  return;		// index 

	TCellData* pCell = m_pCellInfo->GetCellData(nJigNo, eIDX_CH_1 );		// Cell Data 1개를 가져온다. Cell 존재여부와 상관없이 시각은 기록되므로 사용이 가능하다.

	// EQ 작업시간이 기준보다 초과되었나 ?
	if ( pCell->dbMoveTACT[eTACT_EQLossTime] < 1.0 )	
	{
		// m_pLogSeq->Write(_T("[MAIN] Write OP Loss time Skip : %.3f"), dbOPLoss);			
		return;		// 초과된 시간이 없으므로 기록을 할 필요가 없다.
	}

	TLosstimeRecord	rLossRec;	// OP Loss Log를 남기기 위한 데이터 기록
	float fLossTime = (float)( pCell->dbMoveTACT[eTACT_EQLossTime] / 2.0 );	// + m_rProfileData.nTIME_EQLossTime);	// 설비 손실 시간

	rLossRec.sReasonLine	= _T("");					// 선택된 전체 Code 내용
	rLossRec.s1stClass		= _T("RD");					// 각 Class별 선택 내용
	rLossRec.s2ndClass		= _T("TACT TIME OVER");
	rLossRec.s3rdClass		= _T("TACT TIME OVER");
	rLossRec.sMESCode		= _T("000");				// 상위 보고용 코드, 현재 미사용
	rLossRec.sID			= m_rCurrentUser.sID;		// Stop 시킨 작업자 ID
	rLossRec.sIDNo			= m_rCurrentUser.sIDNo;		// 작업자 사원번호;
	rLossRec.sAlarmType		= _T("-");					// Alarm 여부, 중/경 알람여부
	rLossRec.sAlarmCode		= _T("");					// Alarm Code
	rLossRec.sRepairCode	= _T("");					// MD/BM의 경우 조치내역
	rLossRec.sUserMsg		= _T("");					// 사용자 정의 입력
	rLossRec.fWaitTime		= 0.0;						// 알람발생 ~ 고장조치 시작까지
	rLossRec.fRepairTime	= 0.0;						// 실제 조치가 일어난 시간
	rLossRec.fDuration		= fLossTime;				// 설비 손실 시간
	rLossRec.dbAddLossTime	= 0.0;						// 추가적인 더하는 시각

	//2014-12-24, jhLee, TPSystem code 지정
	rLossRec.sTPSystemName	= _T("TACT TIME OVER");		// TP System loss name
	rLossRec.sTPSystemCode	= _T("01200");					// TP System Loss Code


	//OLD CGxUtility::PrevSecondTime( (int)(pCell->dbMoveTACT[eTACT_EQLossTime]),  rLossRec.tmStartTime );	// 정지 시작 시각
	// ::GetLocalTime( &(rLossRec.tmStartTime) );			// 정지 시작시각
	//2015-03-12, 현재시각으로 모두 통일, CGxUtility::PrevSecondTime( (int)fLossTime,  rLossRec.tmStartTime );		// 정지 시작 시각

	::GetLocalTime( &(rLossRec.tmStartTime) );			// 시각 시각
	::GetLocalTime( &(rLossRec.tmResetTime) );			// 인지 시각
	::GetLocalTime( &(rLossRec.tmEndTime) );				// 정지 종료시각

	//old WriteLossTimeRecord( rLossRec, FALSE );
	//old WriteLossTimeRecord_new( rLossRec, FALSE );
	WriteAutoLossTimeRecord( rLossRec );
	*/
}




//
// 비가동 로그를 일일 기준으로 마무리를 수행한다.
//
// 호출하는 축에서 이미 설비 State가 AUTO 상태가 아님을 확인하고 본 함수를 호출한다.
//
// CTime timePrev : 이전 날짜
//
void CLossTimeBank::LosstimeDataCloseProcess(BOOL bPrevDay)
{

	// 대분류가 존재하는가 ? 이미 비가동 사유가 보관되고 있다 -> 처리 가능 !
	/*if ( m_rLosstimeRecord.s1stClass != _T("") )
	{
		CTime tmToday		= CTime::GetCurrentTime();		// 실제 현재 시각
		CTimeSpan timeOffset(0, 0, 1, 0);					// 1분전
		tmToday -= timeOffset;								// 1분전 시각을 구한다. 

		//d CTime tmToday	= GetToday();						// 오늘 기준일자

		// 종료 시각을 이전 조가 끝나는 시각으로 지정한다.
		::GetLocalTime( &(m_rLosstimeRecord.tmEndTime) );	// 오늘 기준, 현재 시각 (기본값 채우기)

		// 바로 직전 날짜로 만들어준다.
		m_rLosstimeRecord.tmEndTime.wYear			= (WORD)tmToday.GetYear();
		m_rLosstimeRecord.tmEndTime.wMonth			= (WORD)tmToday.GetMonth();
		m_rLosstimeRecord.tmEndTime.wDay			= (WORD)tmToday.GetDay();

		m_rLosstimeRecord.tmEndTime.wHour			= (WORD)tmToday.GetHour();	// 바로 직전시간	
		m_rLosstimeRecord.tmEndTime.wMinute			= 59;		// 59분
		m_rLosstimeRecord.tmEndTime.wSecond			= 59;		// 59초
		m_rLosstimeRecord.tmEndTime.wMilliseconds	= 999;		// 0.999초

		m_rLosstimeRecord.tmResetTime = m_rLosstimeRecord.tmEndTime;
		m_rLosstimeRecord.dbAddLossTime = 0.0;

		// 기준 날짜가 변경되었기에 이전 날짜의 파일에 기록하도록 한다. 
		WriteLossTimeRecord( m_rLosstimeRecord, bPrevDay );			// 이전 날짜로 기록할것인지 구분하여 기록한다.

		// 새로운 시작시간을 현재의 시각으로 만든다.
		::GetLocalTime( &(m_rLosstimeRecord.tmStartTime) );		// 오늘 기준, 현재 시각으로 만들어준다. (기본값 채우기)

		m_rLosstimeRecord.tmStartTime.wMinute		= 0;		// 0분
		m_rLosstimeRecord.tmStartTime.wSecond		= 0;		// 0초
		m_rLosstimeRecord.tmStartTime.wMilliseconds	= 0;		// 0.000초

		// 일단 동일한 시간으로 통일한다.
		m_rLosstimeRecord.tmResetTime	= m_rLosstimeRecord.tmStartTime;
		m_rLosstimeRecord.tmEndTime		= m_rLosstimeRecord.tmStartTime;


		// Alarm이 발생했던것이라면,
		// 자동으로 작업조별로 구간을 나눌때 Alarm 시작 시간을 새롭게 나뉘어진 시간으로 대입시켜준다.
		// 만약 이 루틴을 타지 않는다면, 최초에 Alarm이 발생했던 시간을 간직하고 있을것이다.
		// 본 값은 DlgRepairAlarm에서 비가동 로그를 기록할 때 사용되어진다.
		if ( m_rLosstimeRecord.sAlarmType == _T("A") )
		{
			// Alarm 발생 시각 자체도 현재시각으로 만들어준다.
			CGxAlarmCtrl* pAlarmCtrl = CGxAlarmCtrl::GetInstance();
			CGxAlarmData* pAlarmData = pAlarmCtrl->GetCurrentAlarmData();		

			pAlarmData->tmLossStart	= m_rLosstimeRecord.tmStartTime;				// 새롭게 계산되어진 시작 시간을 대입한다.
		}

		SaveLosstimeInfo();	// 현재 설정된 비가동 정보를 파일로 기록한다.
		
	}//of if 대분류 존재
	*/
}

