#pragma once

#include "File\GxIni.h"
#include "File\GxLogData.h"			// Cell Log를 위한 class

#include "BankGuild\MCDefine.h"


class CLossTimeBank
{
public:
	CLossTimeBank(void);
	~CLossTimeBank(void);


		// 비가동 항목 관련 데이터 -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	TLosstimeList		m_rLosstimeList;						// 비가동 항목과련 데이터를 관리하는 구조체
	TLosstimeRecord		m_rLosstimeRecord;							// 비가동 Log를 남기기 위한 데이터 기록

	CString             m_sTPSystemCode;                         //2017-03-25,skcho 추가
	CString             m_sTPSystemName;
	
	CGxIni				m_iniLosstime;			// 비가동 정보를 임시 기억한다.	
	CGxLogData			m_logLosstime;						// 비가동 로그

	BOOL LoadLosstimeItemList();
	
	// 비가동 Log
//	CGxLogData	m_logLosstimeData;						// 비가동 Log를 기록한다. (새로운 방식)
	void SetLosstimeLogPath(void* pValue);			// 비가동 Log 파일이름 생성 callback 함수
	void WriteLossTimeRecord(TLosstimeRecord& rRec, BOOL bPrevDay);		//새로운 방식으로 지정한 비가동 레코드 데이터를 파일로 기록한다.
	void WriteAutoLossTimeRecord(TLosstimeRecord& rRec);

	void InitLosstimeInfo();					// 비가동 정보를 임시 기억하기위한 초기화
	void LoadLosstimeInfo();					// 파일에 저장된 비가동 정보를 읽어온다. (Program 실행시)
	void SaveLosstimeInfo();					// 현재 설정된 비가동 정보를 파일로 기록한다.
	void ClearLosstimeInfo();					// 비가동 관련 내용이 없다면 내용을 청소해준다.

	void WriteLossTimeByAlarm();				// Alarm에 의해 강제적으로 비가동 상태가 종료되었다고 저장한다.
	void WriteOPLossTime(int nJigNo);		// 작업자 시간초과 기록 (비가동 로그에 남긴다)
	void WriteEQLossTime(int nJigNo);		// 설비 시간초과 기록 (비가동 로그에 남긴다)

	// 비가동 로그의 일별, 조별 마무리를 수행해준다.
	void LosstimeDataCloseProcess(BOOL bPrevDay);

	//비가동로그 파일 설정
	void InitialDataFile();
};

