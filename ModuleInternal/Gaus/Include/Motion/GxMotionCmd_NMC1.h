/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// CGxMotionCmd_NMC1 : PAIX NMC1 Control class
//
// 2015-05-28, jhLee, Gaus2용 라이브러리 재작성
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gaus.h"
#include "Motion\GxMotionCmd.h"

#if defined(_ORIGIN_SOURCE) || defined(_DLL_EXPORTS)
	#include "Motion\NMC1\NMC420S.h"
	#pragma comment(lib, "NMC420S.lib")
#else
	#include "Motion\DLL_Include\NMC420S_Struct.H"			// 구조체 선언 부분만 발췌하여 사용한다.
#endif


// #include "MotionDefine.h"


#define NMC1_AXIS_COUNT		8			// 최대 축수

// 원점 찾기 동작에 대한 상수 값 선언, NMC2.h에 있는 내용을 토대로 NMC1으로 재구성 함.
#ifndef _NMC1_HOME_MODE_
#define _NMC1_HOME_MODE_
	#define NMC1_HOME_LIMIT_P					0
	#define NMC1_HOME_LIMIT_M					1
	#define NMC1_HOME_NEAR_P					2
	#define NMC1_HOME_NEAR_M					3
	#define NMC1_HOME_Z_P						4
	#define NMC1_HOME_Z_M						5

	#define	NMC1_HOME_USE_Z						0x80			// Z상 (C상) 사용 여부 OR 연산으로 지정

	// 원점 찾기 후 동작 mode 지정
	#define NMC1_END_NONE						0x00
	#define NMC1_END_CMD_CLEAR_A_OFFSET			0x01
	#define NMC1_END_ENC_CLEAR_A_OFFSET			0x02
	#define NMC1_END_CMD_CLEAR_B_OFFSET			0x04
	#define NMC1_END_ENC_CLEAR_B_OFFSET			0x08
#endif



// Motion에 대한 실제적인 API 사용을 담은 Class, CGxMotionCmd에서 파생되었으며 CGxAxis에서 사용되어 진다.
class _GAUS_LIB CGxMotionCmd_NMC1 : public CGxMotionCmd
{
public:
	CGxMotionCmd_NMC1(void);
	~CGxMotionCmd_NMC1(void);

	enum
	{
		eMAX_AXIS_CNT = NMC1_AXIS_COUNT,
	};

// friend class CMotionMgr;

public :

	// TEST用
	void TestCall()	{ AfxMessageBox(_T("CGxMotionCmd_NMC1 is Called !") ); };




	// Cmd  정보 관리 
	TGxCmdRecordPtrArray		m_arrCmdRecord;			// Cmd 정보

	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

	void ReleaseInstance();										// 선언 되었던 각종 데이터들을 해제시켜준다. 소멸자 역할


	TPGxCmdRecord GetCmdRecord(int nIdx);						// 지정 순번의 Cmd Record 주소 획득
	TPGxCmdRecord GetCmdRecordByName(CString sName);			// 지정 이름을 가지는 Cmd Record 주소 획득

	int AddControlCmd(TGxCmdRecord &rNewCmd);					// 새로운 보드를 추가한다.	GxMotionCmdMgr에서 호출
	int GetControlCmdCount();									// 지정된 Control Cmd의 수량을 되돌려준다.



	// 제어기 열기와 닫기
	BOOL	OpenDevice();										// 등록된 모든 제어기 열기
	BOOL	CloseDevice();										// 등록된 모든 제어기 닫기


	BOOL	OpenDevice(int nCtrlIdx);							// 장치를 활성화하고 사용가능하게 만든다.
	BOOL	CloseDevice(int nCtrlIdx);							// 장치를 닫고 더이상 사용하지 않는다.




	// 기어비 설정, NMC1만의 함수
	BOOL	SetPulsePerRevolution(int nCtrlIdx, int nAxisNo, double dbValue);		// 1회전당 펄스 수 지정
	BOOL	SetUnitPerRevolution(int nCtrlIdx, int nAxisNo, double dbValue);		// 1회전당 이동거리 mm 지정


	// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==


	//--- Check Function
	BOOL	IsCheckBoard(int nCtrlIdx);							// 지정 모션 보드가 정상적인가 ?
	BOOL	IsOpen(int nCtrlIdx);								// 지정 보드가 열려져있는가 ?
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


	BOOL	SetGearRatio(int nCtrlIdx, int nAxisNo, double dbRatio);		// 기어비 설정
	BOOL	GetCmdPos(int nCtrlIdx, int nAxisNo, double& dbPos);				// 지령 위치값 조회
	BOOL	GetEncPos(int nCtrlIdx, int nAxisNo, double& dbPos);				// 엔코더 위치값 조회
	BOOL	SetCmdPos(int nCtrlIdx, int nAxisNo, double dbNewPos);		// Command position 값을 지정한다.
	BOOL	SetEncPos(int nCtrlIdx, int nAxisNo, double dbNewPos);		// Encoder position 값을 지정한다.
	BOOL	SetPosition(int nCtrlIdx, int nAxisNo, double dbPos);			// 위치값 적용
	BOOL	SetSyncToEncPos(int nCtrlIdx, int nAxisNo);				// 엔코더 값으로 Command position을 일치화 시킨다.

	//--- ServoAmp Control
	BOOL	ServoON(int nCtrlIdx, int nAxisNo);							// Servo On 지령
	BOOL	ServoOFF(int nCtrlIdx, int nAxisNo);							// Servo Off 지령
	BOOL	CurrentON(int nCtrlIdx, int nAxisNo);							// Step motor Amp의 Power On 지령
	BOOL	CurrentOFF(int nCtrlIdx, int nAxisNo);						// Step motor Amp의 Power Off 지령
	BOOL	AlarmClearON(int nCtrlIdx, int nAxisNo);						// Servo AlarmClear 신호 ON
	BOOL	AlarmClearOFF(int nCtrlIdx, int nAxisNo);					// Servo AlarmClear 신호 OFF


	//--- 이동 및 정지 명령
	BOOL	SetHomeSearchSpeed(int nCtrlIdx, int nAxisNo, double dbHigh, double dbLow);							// 원점 이동속도 지정
	BOOL	MoveHomeSearch(int nCtrlIdx, int nAxisNo, int nHomeMode, int nEndMode, double dbOffset);			// 원점 이동 명령

	BOOL	SetMoveSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbDrive);		// 이동 속도 설정 (사다리꼴)
	BOOL	SetMoveSCurveSpeed(int nCtrlIdx, int nAxisNo, double dbAcc, double dbDec, double dbDrive);	// 이동 속도 설정 (S-커브)

	BOOL	MoveAbs(int nCtrlIdx, int nAxisNo, double dbPos);			// 지정 위치로 절대좌표 이동을 지령한다.
	BOOL	MoveRel(int nCtrlIdx, int nAxisNo, double dbPos);			// 지정 위치로 상대좌표 이동을 지령한다.
	BOOL	MoveJog(int nCtrlIdx, int nAxisNo, int nDir);				// 지정된 방향으로 JOG 이동을 지령한다.

	BOOL	MoveStop(int nCtrlIdx, int nAxisNo);							// 이동을 정지 시킨다. (감속 정지)
	BOOL	MoveJogStop(int nCtrlIdx, int nAxisNo);						// Jog 이동을 정지 시킨다.
	BOOL	MoveEmgStop(int nCtrlIdx, int nAxisNo);						// 이동을 비상 정지 시킨다.
	BOOL	MoveStopAll(int nCtrlIdx);										// 모든 축을 감속 정지 시킨다.
	BOOL	MoveEmgStopAll(int nCtrlIdx);									// 모든 축을 비상정지 시킨다.

	// BOOL	MoveMultiStop(int nCtrlIdx, int nCount, NMC2_MULTI_STOP *pSelect, int nMode);		// 지정 다축을 정지 시킨다.

};

