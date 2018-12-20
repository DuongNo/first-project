//
// MCMain : Gaus의 모든 Control을 관리하는 Main class
//
// 2015-06-02, jhLee, 기존 CMCMain의 내용을 별도의 Frame으로 모아서 관리하도록 하였다.

#pragma once

#include "GxDefine.h"
#include "GxSingleton.h"

#include "GxMaster.h"
#include "GxServiceHub.h"
#include "GxSystemHub.h"
#include "GxStationHub.h"
#include "GxAlarmCtrl.h"

#include "File\GxLog.h"
#include "File\GxLogMgr.h"
#include "Motion\GxMotionCmdMgr.h"
#include "Motion\GxAxisMgr.h"
#include "Motion\GxRobotMgr.h"
#include "DIO\GxDIOMgr.h"
#include "Comm\GxSocketMgr.h"


class _GAUS_LIB CGxGausFrame : public  CGxSingleton<CGxGausFrame>				// Singleton으로 만든다. 
{
public:
	CGxGausFrame(void);
	~CGxGausFrame(void);

	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
	CString				m_sBasePath;			// 가장 근간이 되는 Base 경로
	BOOL				m_bLogFlag;				// 각종 실행 과정의 Log를 남길것인가 ?


	HWND				m_pOwnerHandle;		// 소유한 객체 핸들

	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
	// Gaus
	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
	CGxMaster*			m_pMaster;				// 자동처리의 중심이되는 Master
	CGxServiceHub*		m_pServiceHub;			// GxService들을 연계
	CGxSystemHub*		m_pSystemHub;			// GxSystem들을 연계
	CGxStationHub*		m_pStationHub;			// GxStation들을 연계

	// Motion 관련
	CGxMotionCmdMgr*	m_pMotionCmdMgr;		// MotionCmd를 관리하는 class
	CGxAxisMgr*			m_pAxisMgr;				// 축 관리
	CGxRobotMgr*		m_pRobotMgr;			// Robot 관리

	// DIO 관련
	CGxDIOMgr*			m_pDIOMgr;				// Digital I/O 관리 class

	// Alarm 관련
	CGxAlarmCtrl*		m_pAlarmCtrl;			// Alarm 처리 class


	// Log 기록 관련
	CGxLog*				m_pDebug;				// debug용 기록 log
	CGxLogMgr*			m_pLogMgr;				// Log 객체 관리 class

	// Socket 관리 
	CGxSocketMgr*		m_pSocketMgr;			// Socket 관리자 class

	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

	CString GetDllVersion();					// Gaus DLL의 Version 조회
	CString GetBasePath();						// 기본 경로를 조회한다.
	
	void SetOwnerHandle(HWND hHandle);		// 소유자의 핸들을 대입시켜준다.

	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
	// Guas에 속한 System을 생성 및 삭제한다.
	// 각종 데이터 및 인스턴스 생성/삭제
	//
	// 생성 순서는
	//		CreateSystem -> InitialSystem -> StartupSystem
	// 종료 순서는
	//		ShutdownSystem -> ReleaseSystem
	// 순으로 진행한다.
	//
	void CreateSystem(CString sPath);		// 시스템을 생성한다.
	void InitialSystem();						// 시스템의 데이터 초기화를 수행한다. (설정파일 읽기)
	void StartupSystem();						// 시스템을 기동시킨다. (Device Open)

	void ShutdownSystem();						// 구동중인 시스템을 정지시킨다. (Device Close)
	void ReleaseSystem();						// 시스템을 파괴한다.
	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


	BOOL CheckYMCMotionState();					//  Yaskawa MP2100의 정의 상태를 조사한다.

	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
	// Station / Service / System 생성 및 추가 / 조회 함수
	void AddStation(CGxStation* pStation);		// Station을 추가한다.
	void AddSystem(CGxSystem* pSystem);			// System을 추가한다.
	void AddService(CGxService* pService);		// Service를 추가한다.

	CGxStation* GetStation(CString sName);		// 지정 이름의 Station의 포인터를 획득한다.
	CGxSystem* GetSystem(CString sName);		// 지정 이름의 System의 포인터를 획득한다.
	CGxService* GetService(CString sName);		// 지정 이름의 Service의 포인터를 획득한다.

	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==




};

