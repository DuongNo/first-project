/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CGxMotionCmd_NMC2 : PAIX NMC2 Control class
//
// 2014-12-15, jhLee, 베트남向 모션 제어기
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gaus.h"
#include "Motion\GxMotionCmd.h"

#if defined(_ORIGIN_SOURCE) || defined(_DLL_EXPORTS)
	#include "Motion\NMC2\NMC2.h"
	#pragma comment(lib, "NMC2.lib")
#else
	#include "Motion\DLL_Include\NMC2_Struct.H"			// 구조체 선언 부분만 발췌하여 사용한다.
#endif


// #include "MotionDefine.h"


#define NMC2_AXIS_COUNT		8			// 최대 축수

//
//// 다축 지정 정지를 위한 축 지정용 구조체
//typedef struct _NMC2_MULTI_STOP
//{
//	short nAxisSelect[NMC2_AXIS_COUNT];				// 최대 축에 대한 선택
//
//	_NMC2_MULTI_STOP() 
//	{ for(int i = 0; i < NMC2_AXIS_COUNT; i++) nAxisSelect[i] = 0; };
//
//} NMC2_MULTI_STOP;
//
//

// Motion에 대한 실제적인 API 사용을 담은 Class, CGxMotionCmd에서 파생되었으며 CGxAxis에서 사용되어 진다.
class _GAUS_LIB CGxMotionCmd_NMC2 : public CGxMotionCmd
{
public:
	CGxMotionCmd_NMC2(void);
	~CGxMotionCmd_NMC2(void);

	enum
	{
		eMAX_AXIS_CNT = NMC2_AXIS_COUNT,
	};


public :

	// TEST用
	void TestCall()	{ AfxMessageBox(_T("CGxMotionCmd_NMC2 is Called !") ); };


	// Cmd 데엍 정보 관리 
	TGxCmdRecordPtrArray		m_arrCmdRecord;				// Cmd 정보를 담아두는 Pointer 


	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==
	void ReleaseInstance();								// 선언 되었던 각종 데이터들을 해제시켜준다. 소멸자 역할

	// 정보 지정과 조회 함수
	TPGxCmdRecord GetCmdRecord(int nIdx);					// 지정 순번의 Cmd Record 주소 획득
	TPGxCmdRecord GetCmdRecordByName(CString sName);		// 지정 이름을 가지는 Cmd Record 주소 획득

	int AddControlCmd(TGxCmdRecord &rNewCmd);				// 새로운 보드를 추가한다.	GxMotionCmdMgr에서 호출
	int GetControlCmdCount();								// 지정된 Control Cmd의 수량을 되돌려준다.


	// 제어기 열기와 닫기
	BOOL	OpenDevice();										// 등록된 모든 제어기 열기
	BOOL	CloseDevice();										// 등록된 모든 제어기 닫기


	BOOL	OpenDevice(int nCtrlIdx);							// 장치를 활성화하고 사용가능하게 만든다.
	BOOL	CloseDevice(int nCtrlIdx);							// 장치를 닫고 더이상 사용하지 않는다.


	// 현 제어기 모든축의 정보를 조회한다.
	BOOL	GetAxisInfo(int nCtrlIdx, NMCAXESEXPR* pInfo);				// 축 종합정보 정보 조회
	BOOL	GetHomeInfo(int nCtrlIdx, NMCHOMEFLAG* pInfo);				// 원점찾기 정보 조회
	BOOL	GetOutputInfo(int nCtrlIdx, NMCAXESMOTIONOUT* pInfo);	// 각종 출력값 정보 조회


	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


	//--- Check Function
	BOOL	IsCheckBoard(int nCtrlIdx);							// 지정 모션 보드가 정상적인가 ?
	BOOL	IsOpen(int nCtrlIdx);
	BOOL	IsServoOn(int nCtrlIdx, int nAxisNo);				// Servo On 출력 여부
	BOOL	IsCurrentOn(int nCtrlIdx, int nAxisNo);				// Step motor Amp On 출력 여부
	BOOL	IsServoReady(int nCtrlIdx, int nAxisNo);			// Servo가 동작 가능 상태인가 ?
	BOOL	IsAlarm(int nCtrlIdx, int nAxisNo);					// Alarm 발생여부
	BOOL	IsInpos(int nCtrlIdx, int nAxisNo);					// 축 정지 여부
	BOOL	IsBusy(int nCtrlIdx, int nAxisNo);					// 명령 지령중 여부
	BOOL	IsHomeSearch(int nCtrlIdx, int nAxisNo);			// 원점 찾기 완료 여부
	BOOL	IsHomeSearchMoving(int nCtrlIdx, int nAxisNo);		// 원점 찾기 수행중인가 ?
	BOOL	IsReady(int nCtrlIdx, int nAxisNo);					// 이동이 가능한지 여부
	BOOL	IsPLimit(int nCtrlIdx, int nAxisNo);				// Positive 센서 감지 여부
	BOOL	IsNLimit(int nCtrlIdx, int nAxisNo);				// Negative 센서 감지 여부
	BOOL	IsOrg(int nCtrlIdx, int nAxisNo);					// 원점(Near)센서 감지 여부


	BOOL	SetGearRatio(int nCtrlIdx, int nAxisNo, double dbRatio);	// 기어비 설정
	BOOL	GetCmdPos(int nCtrlIdx, int nAxisNo, double& dbPos);		// 지령 위치값 조회
	BOOL	GetEncPos(int nCtrlIdx, int nAxisNo, double& dbPos);		// 엔코더 위치값 조회
	BOOL	SetCmdPos(int nCtrlIdx, int nAxisNo, double dbNewPos);		// Command position 값을 지정한다.
	BOOL	SetEncPos(int nCtrlIdx, int nAxisNo, double dbNewPos);		// Encoder position 값을 지정한다.
	BOOL	SetPosition(int nCtrlIdx, int nAxisNo, double dbPos);		// 위치값 적용
	BOOL	SetSyncToEncPos(int nCtrlIdx, int nAxisNo);					// 엔코더 값으로 Command position을 일치화 시킨다.

	//--- ServoAmp Control
	BOOL	ServoON(int nCtrlIdx, int nAxisNo);					// Servo On 지령
	BOOL	ServoOFF(int nCtrlIdx, int nAxisNo);				// Servo Off 지령
	BOOL	CurrentON(int nCtrlIdx, int nAxisNo);				// Step motor Amp의 Power On 지령
	BOOL	CurrentOFF(int nCtrlIdx, int nAxisNo);				// Step motor Amp의 Power Off 지령
	BOOL	AlarmClearON(int nCtrlIdx, int nAxisNo);			// Servo AlarmClear 신호 ON
	BOOL	AlarmClearOFF(int nCtrlIdx, int nAxisNo);			// Servo AlarmClear 신호 OFF


	//--- 이동 및 정지 명령
	BOOL	SetHomeSearchSpeed(int nCtrlIdx, int nAxisNo, double dbHigh, double dbLow);					// 원점 이동속도 지정
	BOOL	MoveHomeSearch(int nCtrlIdx, int nAxisNo, int nHomeMode, int nEndMode, double dbOffset);	// 원점 이동 명령

	BOOL	SetMoveSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbDrive);		// 이동 속도 설정 (사다리꼴)
	BOOL	SetMoveSCurveSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbDrive);	// 이동 속도 설정 (S-커브)

	BOOL	MoveAbs(int nCtrlIdx, int nAxisNo, double dbPos);			// 지정 위치로 절대좌표 이동을 지령한다.
	BOOL	MoveRel(int nCtrlIdx, int nAxisNo, double dbPos);			// 지정 위치로 상대좌표 이동을 지령한다.
	BOOL	MoveJog(int nCtrlIdx, int nAxisNo, int nDir);				// 지정된 방향으로 JOG 이동을 지령한다.

	// V2.5.8, 2016-06-21, jhLee, 미사용
	// BOOL	MoveAbsSpeedChange(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbPos1, double dbSpeed1, double dbPos2, double dbSpeed2);	// 2지점 속도 변경 이동


	BOOL	MoveStop(int nCtrlIdx, int nAxisNo);						// 이동을 정지 시킨다. (감속 정지)
	BOOL	MoveJogStop(int nCtrlIdx, int nAxisNo);						// Jog 이동을 정지 시킨다.
	BOOL	MoveEmgStop(int nCtrlIdx, int nAxisNo);						// 이동을 비상 정지 시킨다.
	BOOL	MoveStopAll(int nCtrlIdx);									// 모든 축을 감속 정지 시킨다.
	BOOL	MoveEmgStopAll(int nCtrlIdx);								// 모든 축을 비상정지 시킨다.

	// BOOL	MoveMultiStop(int nCtrlIdx, int nCount, NMC2_MULTI_STOP *pSelect, int nMode);		// 지정 다축을 정지 시킨다.

};

