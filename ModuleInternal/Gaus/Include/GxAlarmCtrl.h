//
// Alarm 처리를 관장하는 Class
//
//
#pragma once

#include "Gaus.h"
#include "GxSingleton.h"
#include "GxDefine.h"
#include "GxAlarmData.h"
#include "GxService.h"

#include "File\GxIniFile.h"
#include "GxEvent.h"


class _GAUS_LIB CGxAlarmCtrl :  public CGxSingleton<CGxAlarmCtrl>			
{
private:
	// 미사용 CRITICAL_SECTION	m_csDataUpdate;			// 데이터를 갱신하는 객체의 공유 지원

	CGxIniFile	m_iniData;

public:
	CGxAlarmCtrl(void);
	~CGxAlarmCtrl(void);

	CString		m_sName;							// 자신의 이름
	CString		m_sBasePath;						// 기본적인 경로
	CString		m_sDataFilename;					// Alarm Data를 읽어들일 파일 이름
	CGxLog		m_debug;							// Debug용 log file



	CGxEvent	m_evtAlarm;							// Alarm 지정, 다른 Class에서 이 이벤트를 참조한다.

	int			m_nErrorCode;						// 발생한 Alarm Code
	int			m_nQuery;							// 선택을 요청한 응답
	int			m_nResponse;						// 사용자의 응답
	CString		m_sQueryCustom;						// m_nQuery에 eQUERY_CUSTOM이 포함된 경우 표시할 사용자 선택사항

	int			m_nDetailCode;						// 상세한 Alarm 원인의 Alarm Code, 정합성 검증용
	CString		m_sDetailMsg;						// 상세한 Alarm 원인 : Motion 제어측에서 지정된 내용

	BOOL		m_bEmgStop;							// 비상 정지를 해야 하는가 ?
	BOOL		m_bAlarmFlag;						// Alarm이 설정되었는가 ? 다른 Class의 실행 조건에서 이 변수를 참고한다.
	BOOL		m_bCycleStop;						// Cycle Stop을 수행할 것인가?

	// 특별히 지정된 이미지를 사용하도록 지정하였는가 ?
	BOOL		m_bCustomImage;					// 특별 지정 이미지를 사용하는가 ?
	CString		m_sCustomPartsImage;
	CString		m_sCustomUnitImage;


	CGxAlarmData	m_CurrentAlarm;				// 현재 발생한 Alarm 정보

	void SetDebugOut(BOOL bFlag);					// Debug 기록을 제어한다

public:
//	static CGxAlarmCtrl* GetAlarmCtrl();		// Instance 획득
//	static void ReleaseAlarmCtrl(void);			// SingleTone, Instance 객체 파괴
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	CGxEvent*	GetAlarmEvent(void);				// Alarm Event를 획득한다.
	BOOL		IsEmgStop(void);					// 비상정지를 해야 하는가 ?
	BOOL		IsCycleStop();						// Cycle Stop을 수행할 것인가 ?
	void		SetCycleStop(BOOL bFlag);		// Cycle Stop 여부를 지정한다. CMCMain에서 지정해준다.
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


	void SetError(UINT nECode, int nQuery = eQUERY_RETRY, BOOL bEmgStop = FALSE);	// Error code를 지정한다.
	void SetCustomQuery(CString sQuery);			// 사용자 지정 응답을 등록한다.
	void SetResponse(int nRes);						// 응답을 지정한다.
	int GetQuery(void);								// 사용자가 지정한 선택사항 조회
	int GetResponse(void);							// 응답 조회
	CString GetCustomQuery();						// 사용자 지정 응답 조회
	int GetAlarmCode();								// 현재 발생된 Alarm Code 조회

	void ClearDetailMsg();							// 상세 발생원인을 초기화 한다.
	void SetDetailMsg(int nCode, CString sMsg);		// 지정 Alarm에 대한 상세 발생원인을 지정한다.
	CString GetDetailMsg(int nCode);				// 지정되어있는 상새 발생원인을 조회한다.


	void ClearError(void);							// 발생한 Error를 청소한다.
	void SetBasePath(CString sPath);				// 기본 경로를 설정하여 파일 경로를 갱신한다.

	void SetCustomImage(CString sPats, CString sUnit);		// 사용자 지정 파츠/유닛 이미지 지정
	BOOL GetIsCustomImage();							// 사용자 지정 이미지 사용 여부

	CGxAlarmData* GetCurrentAlarmData(void);	// 현재 발생한 Alarm 정보
	void ReadErrorData(UINT nECode);				// 지정 Error code에 대한 정보를 읽어온다.

	void SetUseDebugLog(BOOL bFlag);				// Debug Log를 사용 할 것인가 ?
};

