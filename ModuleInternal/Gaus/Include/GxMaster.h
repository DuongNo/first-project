#pragma once

#include "Gaus.h"
#include "GxDefine.h"
#include "GxThread.h"
#include "GxEvent.h"
#include "File\GxLog.h"			
#include "File\GxIni.h"

#include "GxStationHub.h"
#include "GxAlarmCtrl.h"


class _GAUS_LIB CGxMaster : public CGxThread, public CGxSingleton<CGxMaster>	
{

private:
//	static CGxMaster* theMaster;			// GxMaster 지칭
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -

	CGxEvent			*m_StateEvent[eMAX_EVENTCount];			// Master 상태 Event 배열
	CGxLog			m_log;									// Log 기록용
	CGxLog			m_debug;								// 동작상태 Log 기록용
	CGxIni				m_profile;								// 각종 설정정보 

	int					m_nMasterState;							// Master 현재 상태

	// Slave event 갱신용
	CRITICAL_SECTION	m_csStateUpdate;	// 상태를 갱신하는 객체의 공유 지원
	CGxStringList		m_listSlaveStates[eMAX_EVENTCount];		// 사용되는 이벤트만큼 StringList를 선언한다.
	int					m_nSalveCapacity[eMAX_EVENTCount];		// 각 이벤트별 Slave 수량 지정
	CGxEvent			m_evtSlaves[eMAX_EVENTCount];			// 모든 Slave의 상태 변이를 감지하는 event
	
	CString				m_sStateCaption[eMAX_STATECount];		// 각 상태를 문자열로 설명

protected:
	HWND				m_hMsgTarget;			// Main Frame의 Handle, Message를 보내기 위한 핸들,

	CString				m_sName;					// Master 이름
	CString				m_sBasePath;			// 기본이 되는 경로

	UINT ThreadRun1();
	UINT ThreadRun2();
	UINT ThreadRun3();

	// Master의 실행상태 Check
	void CheckMasterSate(void);		

	// Abort 수행
	void Abort(void);



public:
	CGxMaster(void);
	~CGxMaster(void);

	//	static CGxMaster* GetMaster(void);		// SingleTone, master의 주소 조회
//	static void ReleaseMaster(void);			// SingleTone, Master 객체 파괴
	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	// 메세지를 수신받을 대상 지정
	void SetMsgTargetHandle(HWND hHandle);		// 각종 메세지를 보내게될 핸들을 지정해준다, 주로 MainFrame
	HWND GetMsgTargetHandle(void);				// 각종 메세지를 보내게될 핸들을 조회한다. 주로 MainFrame


	virtual void StartUp(void);					// Thread 생성
	virtual void Shutdown(void);				// Thread 삭제
	void ReleaseProcess(void);					// 종료 처리


	//// debugging용
	//int GetStationAutoSeq(int nIdx);
	//int GetStationState(int nIdx);			// 각 Station의 상태


	// 각 Slave들의 상태 갱신
	bool WaitSateApply(int nState);							// 지정 상태가 적용되었는지 Check
	void EnterSlaveState(CString sName, int nState);		// 지정 이름을 가진 Slave가 해당 상태가 되었음을 신고.
	void ClearSlaveState(int nState);						// 지정 Event의 List를 삭제한다.
	void SetSlaveStateCapacity(int nState, int nCount);		// 지정 Event를 Set 시키는 Slave수를 지정한다.
	void SetSlaveStateCapacity(int nCount);					// 모든 Event에 대하여 Set 시키는 Slave수를 지정한다.
	CEvent* GetSlaveStateEvent(int nState);					// 지정 Slave event의 Pointer를 조회한다.

	// -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
	// Event 관련
	CGxEvent* GetEvents(int nIdx);				// 지정한 순번의 Event를 반환한다.
	CEvent*	GetEventPtr(int nIdx);				// 각종 Event pointer 반환
	void ClearEvents(void);						// 각종 Event를 Reset 해준다.

	// Event 대기 함수
	int WaitInStop(void);						// Stop 상태에서 Event 대기
	int WaitInAuto(void);						// Auto 상태에서 Event 대기
	int WaitInAlarm(void);						// Alar 상태에서 event 대기


	// 상태 설정 / 조회
	int  GetState(void);						// Master 상태 조회
	void SetState(int nState);					// Master를 지정된 상태로 전환시킨다.
	CString GetStateString(void);				// Master의 상태를 문자열로 조회한다.
	CString GetStateString(int nIdx);			// 지정한 상태를 문자열로 조회한다.

	CGxLog* GetLogPtr(void);					// Log 관련 control의 pointer 조회
	CGxLog* GetDebugPtr(void);				// Debug용 Log 관련 control의 pointer 조회

	void SetBasePath(CString sPath);			// 기본 경로 지정
	CString GetBasepath(void);					// 기본 경로 조회
	

	// 동작 상태를 변경시켜준다.
	bool SetStop(void);							// 자동 실행을 중지 시킨다.
	bool SetAuto(void);							// 자동 실행을 진행시킨다
	bool SetManual(void);						// 수동 동작을 수행시킨다.
	bool SetAbort(void);						// 모든 동작을 무효화 시키고 초기 상태로 되돌아간다.
	bool SetAlarm(void);						// Alarm 상태를 적용시킨다.
	bool SetReset(void);						// 초기/Abort시 사용 준비를 해준다.

};

