//
// Alarm data를 보관하는 기본 Class
//
//
#pragma once

#include "Gaus.h"

typedef enum 
{
	MAX_LINE_CAUSE		= 3,				// 알람 발생 원인 최대 줄 수 
	MAX_LINE_SOLVE		= 3,				// 알람 해결 방법 최대 줄 수
	MAX_LANG_COUNT		= 3,				// 최대 지원 언어 종류, 0:한국어, 1:영어, 2:베트남어


	ERR_RESP_NONE     = 0x0000,
	ERR_RESP_RETRY    = 0x0001,
	ERR_RESP_CANCEL   = 0x0002,
	ERR_RESP_IGNORE   = 0x0004,
	ERR_RESP_YES      = 0x0008,
	ERR_RESP_NO	      = 0x0010,
	ERR_RESP_OK       = 0x0020,
	ERR_RESP_CONTINUE = 0x0040,
	ERR_RESP_RESUME	= 0x0080,
	ERR_RESP_INITIAL  = 0x0100,
	ERR_RESP_ABORT    = 0x1000, // 초기화나 메뉴얼 취소등의 이유로 강제 Clear 될때 
                                // Error 띄울때는 사용하지 말것.
}
EErrorResp;


class _GAUS_LIB CGxAlarmData
{
public:
	CString		sTaskName;			// 발생한 Task 이름
	CString		sFileName;			// Error 내용 읽어올 파일명
	CString		sParam;

	BOOL		bIsAuto;			// TRUE:Auto 에러, FALSE:Manual 에러
	BOOL		bIsAllStop;			// TRUE:설비 전체 정지, FALSE:해당 Module만 정지
	BOOL		bIsChecked;			// 에러 발생 후 사용자 인지 여부

	UINT		nCode;				// Alarm Code
	CString		sParts;				// 파트 (안전/PreAlign/....)
	CString		sUnit;				// 부품 단위 (X축/BackLight/....)
	UINT		nLevel;				// Alarm Level

	CString		sSymbol;			// Program내부에서 사용하는 code값, 참고용
	CString		sErrNameCIM;		// CIM (상위) 보고용 영문 알람 이름
	CString		sErrName[MAX_LANG_COUNT];					// 알람 이름 
	CString		sCause[MAX_LANG_COUNT][MAX_LINE_CAUSE];		// 알람 발생 원인
	CString		sSolve[MAX_LANG_COUNT][MAX_LINE_SOLVE];		// 알람 해결 방법
	CString		sDetailMsg;			// 상세한 Alarm 원인 : Motion 제어측에서 지정된 내용

	// COleDateTime m_tmStart;		// Occur Time
	// COleDateTime m_tmEnd;		// Recovery Time
	SYSTEMTIME	tmStart;			// 발생시각, msec 관리를 위해 type 변경
	SYSTEMTIME	tmEnd;				// 종료시각 (인지시각 ?)

	//2015-03-09, jhLee,				// 비가동 기록용, 순수 Alarm Log에는 해당 ALarm의 실제 발생시각을 남겨놓지만,
	//								비가동 로그에는 근무 조별로 통계를 나누기 위해 변경된 알람 발생시각을 두어야 중복된 시간 집계를 피할 수 있다.
	SYSTEMTIME	tmLossStart;		// Loss Time 기록용 새로운 시작 시각, 조변 나눈기가 없다면, 알람 발생시각 그대로 사용되어진다.


	// 화면에 표시를 위한 데이터
	CString		sPartsImg;			// 파트 이미지
	CString		sUnitImg;			// 상세 단위 이미지
	CString		sUnitImg_R;			// 상세 단위 반전 이미지
	UINT		nUint_x;			// 상세 단위 이미지 x 축 위치
	UINT		nUint_y;			// 상세 단위 이미지 y 축 위치


	// 비가동 로그에 관련된 항목
	CString		sMESCode;			// 상위 보고용 코드, 현재 미사용
	CString		s1stClass;			// Frist / Second / Third Class
	CString		s2ndClass;
	CString		s3rdClass;
	CString		sRepairCode;		// 조치내역 코드


public:
	CGxAlarmData();
	virtual ~CGxAlarmData();

	void Reset();
	// COleDateTimeSpan GetElapseTime();

	const CGxAlarmData& operator =( const CGxAlarmData& src);
};

// typedef CTypedPtrMap<CMapStringToPtr, CString, CGxAlarmData*> CMapStringToErrData;


