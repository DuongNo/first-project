#ifndef _NMC2_H_
#define _NMC2_H_

#ifdef __cplusplus
	extern "C" {
#endif

// NMC2 Equip Type
#define		NMC2_220S			0
#define		NMC2_420S			1
#define		NMC2_620S			2
#define		NMC2_820S			3

#define		NMC2_220_DIO32		4
#define		NMC2_220_DIO64		5
#define		NMC2_420_DIO32		6
#define		NMC2_420_DIO64		7
#define		NMC2_820_DIO32		8
#define		NMC2_820_DIO64		9

#define		NMC2_DIO32			10
#define		NMC2_DIO64			11
#define		NMC2_DIO96			12
#define		NMC2_DIO128			13

#define		NMC2_220			14
#define		NMC2_420			15
#define		NMC2_620			16
#define		NMC2_820			17
#define		NMC2_620_DIO32		18
#define		NMC2_620_DIO64		19

// NMC2 Enum Type
	#define EQUIP_TYPE_NMC_2_AXIS		0x0001
	#define EQUIP_TYPE_NMC_4_AXIS		0x0003
	#define EQUIP_TYPE_NMC_6_AXIS		0x0007
	#define EQUIP_TYPE_NMC_8_AXIS		0x000F
	// 16/16
	#define EQUIP_TYPE_NMC_IO_32		0x0010
	// 32/32
	#define EQUIP_TYPE_NMC_IO_64		0x0030
	// 48/48
	#define EQUIP_TYPE_NMC_IO_96		0x0070
	// 64/64
	#define EQUIP_TYPE_NMC_IO_128		0x00F0
	// 80/80
	#define EQUIP_TYPE_NMC_IO_160		0x01F0
	// 96/96
	#define EQUIP_TYPE_NMC_IO_192		0x03F0
	// 112/112
	#define EQUIP_TYPE_NMC_IO_224		0x07F0
	// 128/128
	#define EQUIP_TYPE_NMC_IO_256		0x0FF0

	#define EQUIP_TYPE_NMC_IO_IE		0x1000
	#define EQUIP_TYPE_NMC_IO_OE		0x2000

	#define EQUIP_TYPE_NMC_M_IO_8		0x4000


// 모든 함수의 리턴값 
#ifndef PAIX_RETURN_VALUE
#define PAIX_RETURN_VALUE
	#define NMC_CANNOT_APPLY		-18		/* 현재 진행 중인 모션에서 지원하지 않는 명령어를 보냈을 경우 */
	#define NMC_NO_CONSTSPDDRV		-17		/* 정속 구간이 아님 */
	#define NMC_SET_1ST_SPDPPS		-16		/* 속도 프로파일을 먼저 설정 하시오 */
	#define NMC_CONTI_BUF_FULL      -15		/* Unlimited Interpolation Buffer Full - UCI */
	#define NMC_CONTI_BUF_EMPTY     -14		/* Unlimited Interpolation Buffer Empty - UCI */
	#define NMC_INTERPOLATION       -13		/* 보간 구동 중에 다른 동작 명령어를 보냈을 경우 */
	#define NMC_FILE_LOAD_FAIL      -12
	#define NMC_ICMP_LOAD_FAIL      -11
	#define NMC_NOT_EXISTS          -10
	#define NMC_CMDNO_ERROR         -9
	#define NMC_NOTRESPONSE         -8
	#define NMC_BUSY                -7
	#define NMC_COMMERR             -6
	#define NMC_SYNTAXERR           -5
	#define NMC_INVALID             -4
	#define NMC_UNKOWN              -3
	#define NMC_SOCKINITERR         -2
	#define NMC_NOTCONNECT          -1
	#define NMC_OK                  0
#endif

// Bit 연산 정의
#ifndef _PAIX_BITOPERATION_
	#define _PAIX_BITOPERATION_
	#define BITSET(val,bit)					(val) |= (1<<(bit))
	#define BITCLEAR(val,bit)				(val) &= (~(1<<(bit)))
	#define BITTOGGLE(val,bit)				(val) ^= (1<<(bit))
	#define BITVALUE(val, sdata, bit)		(val) = (((sdata)>> bit) & 0x01)
	#define GETBIT(bytes, bit_no)			(((bytes)>>(bit_no))&0x01)
	#define SETBIT(bytes, bit_no, val)		{ (bytes) &=~(1<<(bit_no)); (bytes) |= ((val) << (bit_no)); }
#endif
	
 
#ifndef _NMC_STOP_MODE_
#define _NMC_STOP_MODE_
	#define NMC_STOP_OK         0
	#define NMC_STOP_EMG 		1
	#define NMC_STOP_MLIMIT     2
	#define NMC_STOP_PLIMIT   	3
	#define NMC_STOP_ALARM   	4
	#define NMC_STOP_NEARORG   	5
	#define NMC_STOP_ENCZ   	6
#endif

#ifndef _NMC_HOME_MODE_
#define _NMC_HOME_MODE_
	#define NMC_HOME_LIMIT_P					0
	#define NMC_HOME_LIMIT_M					1
	#define NMC_HOME_NEAR_P						2
	#define NMC_HOME_NEAR_M						3
	#define NMC_HOME_Z_P						4
	#define NMC_HOME_Z_M						5

	#define	NMC_HOME_USE_Z						0x80

	#define NMC_END_NONE						0x00
	#define NMC_END_CMD_CLEAR_A_OFFSET			0x01
	#define NMC_END_ENC_CLEAR_A_OFFSET			0x02
	#define NMC_END_CMD_CLEAR_B_OFFSET			0x04
	#define NMC_END_ENC_CLEAR_B_OFFSET			0x08
#endif

#define NMC_LOG_NONE			0
#define NMC_LOG_DEV				0x01
#define NMC_LOG_MO_MOV			0x02	// 모션함수중 MOVE
#define NMC_LOG_MO_SET			0x04	// 모션함수중 GET
#define NMC_LOG_MO_GET			0x08	// 모션함수중 SET
#define NMC_LOG_MO_EXPRESS		0x10	// 모션함수중 각종 상태값 읽는(빈번히 발생)
#define NMC_LOG_IO_SET			0x20
#define NMC_LOG_IO_GET			0x40

#pragma pack(1)
typedef struct NMC_PARA_LOGIC
{
   //-------------------------------------------------------------
	short	nEmg;					// EMG
	short	nEncCount;				// 엔코더 카운트 모드

	short	nEncDir;				// 엔코더 카운트 방향
	short	nEncZ;					// 엔코더 Z

	short	nNear;					// NEAR(HOME)
	short	nMLimit;				// - Limit

	short	nPLimit;				// + Limit
	short	nAlarm;					// Alarm

	short	nInp;					// INPOSITION
	short	nSReady;				// Servo Ready

	short	nPulseMode;				// 1p/2p Mode
	//-------------------------------------------------------------

	short	nLimitStopMode;			// Limit stop mode
	short	nBusyOff;				// Busy off mode

	short	nSWEnable;				// sw limit 활성화 여부
   //-------------------------------------------------------------
	double  dSWMLimitPos;
	double  dSWPLimitPos;
}PARALOGIC,*PPARALOGIC;

typedef struct NMC_PARA_LOGIC_EX
{
   //-------------------------------------------------------------
	short	nEmg;					// EMG
	short	nEncCount;				// 엔코더 카운트 모드

	short	nEncDir;				// 엔코더 카운트 방향
	short	nEncZ;					// 엔코더 Z

	short	nNear;					// NEAR(HOME)
	short	nMLimit;				// - Limit

	short	nPLimit;				// + Limit
	short	nAlarm;					// Alarm

	short	nInp;					// INPOSITION
	short	nSReady;				// Servo Ready

	short	nPulseMode;				// 1p/2p Mode
	//-------------------------------------------------------------

	short	nLimitStopMode;			// Limit stop mode
	short	nBusyOff;				// Busy off mode

	short	nSWEnable;				// sw limit 활성화 여부
	//-------------------------------------------------------------
	double	dSWMLimitPos;
	double	dSWPLimitPos;

	//-------------------------------------------------------------
	// 원점 완료상태 해지 사용여부
	short	nHDoneCancelAlarm;		// Alarm 발생 시 사용여부
	short	nHDoneCancelServoOff;	// Servo Off 시 사용여부
	short	nHDoneCancelCurrentOff;	// Current Off 시 사용여부
	short	nHDoneCancelServoReady;	// Servo Ready 해제 시 사용여부

	short	nDummy[10];				// 예약
}PARALOGICEX,*PPARALOGICEX;

typedef struct NMC_PARA_SPEED{
	double	dStart;
	double	dAcc;
	double	dDec;
	double	dDrive;
	double	dJerkAcc;
	double	dJerkDec;
}PARASPEED,*PPARASPEED;

//{
typedef struct NMC_AXES_MOTION_OUT
{
	short nCurrentOn[8];		// Motor OnOff
	short nServoOn[8];			// Servo OnOff
	short nDCCOn[8];			// Reset DCC
	short nAlarmResetOn[8];		// Reset Alarm
}NMCAXESMOTIONOUT,*PNMCAXESMOTIONOUT;


typedef struct NMC_AXES_EXPR{
	short nBusy[8];
	short nError[8];
	short nNear[8];
	short nPLimit[8];
	short nMLimit[8];

	short nAlarm[8];
	short nEmer[2];
	short nSwPLimit[8];
	short nInpo[8];
	short nHome[8];

	short nEncZ[8];
	short nOrg[8];
	short nSReady[8];

	short nContStatus[2];			// 연속 보간 실행 여부(1:Run, 0:Stop)
	short nDummy[6];
	short nSwMLimit[8]; 

	int lEnc[8];
	int lCmd[8];
	double dEnc[8];
	double dCmd[8];
	char dummy[4];
}NMCAXESEXPR,*PNMCAXESEXPR;

typedef struct NMC_STOPMODE
{
	short nEmg;				
	short nMLimit;			

	short nPLimit;			
	short nAlarm;			

	short nNear;			
	short nEncZ;			
}STOPMODE,*PSTOPMODE;

typedef struct NMC_HOME_FLAG{
    short nSrchFlag[8];
    short nStatusFlag[8];
}NMCHOMEFLAG,*PNMCHOMEFLAG;

typedef struct NMC_CONT_STATUS
{
	short nStatus[2];
	short nExeNodeNo[2];
}NMCCONTSTATUS,*PNMCCONTSTATUS;

typedef struct TNMC_EQUIP_LIST
{
	/*
	192.168.0.11
		BIT0~BIT7  : 192
		BIT8~BIT15  : 168
		BIT16~BIT23  : 0
		BIT24~BIT31  : 11
	*/
	int lIP[200];
	int lModelType[200];
	short nMotionType[200];
	short nDioType[200];
	short nEXDio[200];
	short nMDio[200];

}NMCEQUIPLIST,*PNMCEQUIPLIST;

typedef struct TNMC_MAP_DATA
{
	int		nMapCount;
	int	lMapData[52];
	double	dMapData[52];
}NMCMAPDATA,*PNMCMAPDATA;
#pragma pack()

//------------------------------------------------------------------------------
/**
@breif  NMC2 장치와의 연결을 한다.
@return  0 이면 성공, 다른 경우 실패
@param  nNmcNo NMC2 장치 번호
@see nmc_CloseDevice
*/
short WINAPI nmc_OpenDevice(short nNmcNo);
short WINAPI nmc_OpenDeviceEx(short nNmcNo, long lWaitTime);

/**
@breif  NMC2 장치와의 연결을 해제 한다.
@return  없음
@param  nNmcNo NMC2 장치 번호
@see nmc_OpenDevice
*/
void WINAPI nmc_CloseDevice(short nNmcNo);
//------------------------------------------------------------------------------
/**
@breif  NMC2 축의 현재 로직 정보를 읽어 온다.
@return  0 이면 성공, 다른 경우 실패
@param  nNmcNo NMC2 장치 번호\n
	nAxisNo 로직 정보를 읽어 올 축 번호\n
	pLogic 로직 정보가 들어갈 PARALOGIC 포인터 변수 
@see NMC_PARA_LOGIC
*/
short WINAPI nmc_GetParaLogic(short nNmcNo, short nAxisNo, PPARALOGIC pLogic);
short WINAPI nmc_GetParaLogicEx(short nNmcNo, short nAxisNo, PPARALOGICEX pLogicEx);

/**
@breif  NMC2 축의 로직 정보를 설정한다.
@return  0 이면 성공, 다른 경우 실패
@param  nNmcNo NMC2 장치 번호\n
	nAxisNo 로직 정보를 설정할 축 번호\n
	pLogic 로직 정보가 들어갈 PARALOGIC 포인터 변수 
@see NMC_PARA_LOGIC
*/
short WINAPI nmc_SetParaLogic(short nNmcNo, short nAxisNo, PPARALOGIC pLogic);
short WINAPI nmc_SetParaLogicEx(short nNmcNo, short nAxisNo, PPARALOGICEX pLogicEx);

/**
@breif  NMC2 8축의 로직 정보를 지정한 파일에서 설정한다.
@return  0 이면 성공, 다른 경우 실패
@param  nNmcNo NMC2 장치 번호\n
	szFileName 8축 로직정보가 들어있는 파일이름 
@see NMC_PARA_LOGIC
*/
short WINAPI nmc_SetParaLogicFile(short nNmcNo, char *szFileName);

/**
@breif  NMC2 축의 현재 속도 정보를 읽어 온다.
@return  0 이면 성공, 다른 경우 실패
@param  nNmcNo NMC2 장치 번호\n
	nAxisNo 속도 정보를 읽어 올 축 번호\n
	pSpeed 속도 정보가 들어갈 PARASPEED 포인터 변수 
@see NMC_PARA_SPEED
*/
short WINAPI nmc_GetParaSpeed(short nNmcNo, short nAxisNo,PPARASPEED pSpeed);

/**
@breif  NMC2 축의 링카운트 정보를 읽어 온다.
@return  0 이면 성공, 다른 경우 실패
@param  nNmcNo NMC2 장치 번호\n
	nAxisNo 속도 정보를 읽어 올 축 번호\n
	plMaxPulse 링카운트 펄스 최대값을 읽어 올 포인터 변수\n
	plMaxEncoder 링카운트 엔코더 최대값을 읽어 올 포인터 변수\n 
	pnRingMode 링카운트 활성화 여부를 읽어 올 포인터 변수 0 : Disable, 1 : Enable
@see nmc_SetRingCountMode\n
	nmc_MoveRing
*/
short WINAPI nmc_GetRingCountMode(short nNmcNo, short nAxisNo,int *plMaxPulse,int *plMaxEncoder, short *pnRingMode);
short WINAPI nmc_GetParaTargetPos(short nNmcNo, short nAxisNo,double *pdTargetPos);
short WINAPI nmc_GetAxesMotionOut(short nNmcNo, PNMCAXESMOTIONOUT pOutStatus);
	
//------------------------------------------------------------------------------
short WINAPI nmc_GetDriveAxesSpeed(short nNmcNo, double *pDrvSpeed);
	short WINAPI nmc_GetAxesCmdSpeed(short nNmcNo, double *pDrvSpeed); // 사용안함
	short WINAPI nmc_GetAxesEncSpeed(short nNmcNo, double *pdEncSpeed); // 사용안함
short WINAPI nmc_GetAxesCmdEncSpeed(short nNmcNo, double *pdCmdSpeed, double *pdEncSpeed);
//------------------------------------------------------------------------------
short WINAPI nmc_SetEmgLogic(short nNmcNo, short nGroupNo, short nLogic);
short WINAPI nmc_SetEmgEnable(short nNmcNo, short nEnable);
short WINAPI nmc_SetPlusLimitLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetMinusLimitLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetSWLimitLogic(short nNmcNo, short nAxisNo, short nUse,double dSwMinusPos, double dSwPlusPos);
short WINAPI nmc_SetSWLimitLogicEx(short nNmcNo, short nAxisNo, short nUse,double dSwMinusPos, double dSwPlusPos,short nOpt);
short WINAPI nmc_SetAlarmLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetNearLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetInPoLogic(short nNmcNo, short nAxisNo, short nLogic);
short WINAPI nmc_SetSReadyLogic(short nNmcNo, short nAxisNo, short nLogic);
//------------------------------------------------------------------------------
// ENCODER 관련
short WINAPI nmc_SetEncoderCount(short nNmcNo, short nAxisNo, short nCountMode);
short WINAPI nmc_SetEncoderDir(short nNmcNo, short nAxisNo, short nCountDir);
	 short WINAPI nmc_SetEncoderMode(short nNmcNo, short nAxisNo, short nMode); // 사용안함
short WINAPI nmc_SetEncoderZLogic(short nNmcNo, short nAxisNo, short nLogic);
//------------------------------------------------------------------------------
short WINAPI nmc_SetPulseLogic(short nNmcNo, short nAxisNo, short nLogic);
	short WINAPI nmc_SetPulseMode(short nNmcNo, short nAxisNo, short nMode); // 사용안함
	short WINAPI nmc_Set2PulseDir(short nNmcNo, short nAxisNo, short nDir);  // 사용안함
	short WINAPI nmc_Set1PulseDir(short nNmcNo, short nAxisNo, short nDir);  // 사용안함
	short WINAPI nmc_SetPulseActive(short nNmcNo, short nAxisNo, short nPulseActive);  // 사용안함
//------------------------------------------------------------------------------
	short WINAPI nmc_GetHomeDoneAutoCancel(short nNmcNo, short nAxisNo, short *pnAlarm, short *pnServoOff, short *pnCurrentOff, short *pnServoReady);
	short WINAPI nmc_SetHomeDoneAutoCancel(short nNmcNo, short nAxisNo, short nAlarm, short nServoOff, short nCurrentOff, short nServoReady);
//------------------------------------------------------------------------------
short WINAPI nmc_SetSCurveSpeed(short nNmcNo, short nAxisNo,double dStartSpeed, 
									double dAcc,double dDec ,double dDriveSpeed);
short WINAPI nmc_SetSpeed(short nNmcNo, short nAxisNo,double dStartSpeed, 
						  double dAcc,double dDec ,double dDriveSpeed);
short WINAPI nmc_SetOverrideRunSpeed(short nNmcNo, short nAxisNo,double dAcc,double dDec ,double dDriveSpeed);
short WINAPI nmc_SetOverrideDriveSpeed(short nNmcNo,short nAxisNo, double dDriveSpeed);
short WINAPI nmc_SetAccSpeed(short nNmcNo, short nAxisNo, double dAcc);
short WINAPI nmc_SetDecSpeed(short nNmcNo, short nAxisNo, double dDec);
//------------------------------------------------------------------------------
short WINAPI nmc_AbsMove(short nNmcNo, short nAxisNo,double dPos);
short WINAPI nmc_RelMove(short nNmcNo, short nAxisNo,double dAmount);
short WINAPI nmc_VelMove(short nNmcNo, short nAxisNo, double dPos, double dDrive,short nMode);

short WINAPI nmc_AbsOver(short nNmcNo, short nAxisNo,double dPos);

short WINAPI nmc_VarRelMove(short nNmcNo, short nAxisCount,short *pnAxisList,double *pdAmount);
short WINAPI nmc_VarAbsMove(short nNmcNo, short nAxisCount,short *pnAxisList,double *pdPosList);
short WINAPI nmc_VarAbsOver(short nNmcNo, short nAxisCount,short *pnAxisList,double *pdPosList);
//------------------------------------------------------------------------------
short WINAPI nmc_JogMove(short nNmcNo, short nAxis, short nDir);
//------------------------------------------------------------------------------
short WINAPI nmc_SuddenStop(short nNmcNo, short nAxisNo);
short WINAPI nmc_DecStop(short nNmcNo, short nAxisNo);

short WINAPI nmc_AllAxisStop(short nNmcNo, short nMode);
short WINAPI nmc_MultiAxisStop(short nNmcNo,short nAxisCount,short *pnAxisSelect,short nMode);
//------------------------------------------------------------------------------
short WINAPI nmc_GetAxesExpress(short nNmcNo, PNMCAXESEXPR  pNmcData);
short WINAPI nmc_GetStopInfo(short nNmcNo, short *pnStopInfo);
//------------------------------------------------------------------------------
short WINAPI nmc_SetCmdPos(short nNmcNo, short nAxisNo, double dPos);
short WINAPI nmc_SetEncPos(short nNmcNo, short nAxisNo, double dPos);
short WINAPI nmc_SetCmdEncPos(short nNmcNo, short nAxisNo, double dPos);
//------------------------------------------------------------------------------
short WINAPI nmc_HomeMove(short nNmcNo, short nAxisNo, short nHomeMode,short nHomeEndMode,double dOffset, short nReserve);
short WINAPI nmc_GetHomeStatus(short nNmcNo, PNMCHOMEFLAG pHomeFlag);
short WINAPI nmc_SetHomeSpeed(short nNmcNo, short nAxisNo,
							  double dHomeSpeed0,double dHomeSpeed1,double dHomeSpeed2);
short WINAPI nmc_SetHomeSpeedEx(short nNmcNo, short nAxisNo,
							  double dHomeSpeed0,double dHomeSpeed1,double dHomeSpeed2,double dOffsetSpeed);
short WINAPI nmc_SetHomeSpeedAccDec(short nNmcNo, short nAxisNo, double dHomeSpeed0, double dStart0, double dAcc0, double dDec0, double dHomeSpeed1, double dStart1, double dAcc1, double dDec1,
							  double dHomeSpeed2, double dStart2, double dAcc2, double dDec2, double dOffsetSpeed, double dOffsetStart, double dOffsetAcc, double dOffsetDec);
short WINAPI nmc_HomeDoneCancel(short nNmcNo, short nAxisNo);
short WINAPI nmc_SetHomeDelay(short nNmcNo,int nHomeDelay);
//------------------------------------------------------------------------------
short WINAPI nmc_Interpolation2Axis(short nNmcNo, short nAxisNo0, double dPos0,
									short nAxisNo1, double dPos1,short nOpt);
short WINAPI nmc_Interpolation3Axis(short nNmcNo, short nAxisNo0, double dPos0,
			short nAxisNo1, double dPos1, short nAxisNo2, double dPos2,short nOpt);
//------------------------------------------------------------------------------
short WINAPI nmc_InterpolationArc(short nNmcNo, short nAxisNo0, short nAxisNo1, 
								  double dCenter0,double dCenter1, double dAngle,short nOpt);

	short WINAPI nmc_InterpolationRelCircle (short nNmcNo, short nAxisNo0, double CenterPulse0,double EndPulse0, // 사용안함
				short nAxisNo1, double CenterPulse1, double EndPulse1,short nDir);	// 사용안함
	short WINAPI nmc_InterpolationAbsCircle (short nNmcNo, short nAxisNo0, double CenterPulse0,double EndPulse0, // 사용안함
				short nAxisNo1, double CenterPulse1, double EndPulse1,short nDir); // 사용안함

//------------------------------------------------------------------------------
short WINAPI nmc_SetCurrentOn (short nNmcNo, short nAxisNo,short nOut);
short WINAPI nmc_SetServoOn (short nNmcNo, short nAxisNo,short nOut);
short WINAPI nmc_SetAlarmResetOn(short nNmcNo, short nAxisNo,short nOut);
short WINAPI nmc_SetDccOn (short nNmcNo, short nAxisNo,short nOut);
//------------------------------------------------------------------------------
short WINAPI nmc_SetMultiCurrentOn(short nNmcNo, short nAxisCount,short *pnAxisSelect,short nOut);
short WINAPI nmc_SetMultiServoOn(short nNmcNo, short nAxisCount,short *pnAxisSelect,short nOut);
short WINAPI nmc_SetMultiAlarmResetOn(short nNmcNo, short nAxisCount,short *pnAxisSelect,short nOut);
short WINAPI nmc_SetMultiDccOn(short nNmcNo, short nAxisCount,short *pnAxisSelect,short nOut);
//------------------------------------------------------------------------------
short WINAPI nmc_SetEnableNear(short nNmcNo, short nAxisNo,short nUse);
short WINAPI nmc_SetEnableEncZ(short nNmcNo, short nAxisNo,short nMode);
short WINAPI nmc_SetLimitStopMode (short nNmcNo, short nAxisNo,short nStopMode);
short WINAPI nmc_SetBusyOffMode(short nNmcNo, short nAxisNo, short nMode);

//------------------------------------------------------------------------------
short WINAPI nmc_SetRingCountMode (short nNmcNo, short nAxisNo,int lMaxPulse,int lMaxEncoder, short nRingMode);
short WINAPI nmc_MoveRing (short nNmcNo, short nAxisNo,double dPos, short nMoveMode);
	//------------------------------------------------------------------------------
	short WINAPI nmc_SetSyncSetup(short nNmcNo, short nMainAxisNo,  // 사용안함
								short nSubAxisNoEnable0, short nSubAxisNoEnable1, short nSubAxisNoEnable2); // 사용안함
	short WINAPI nmc_SetSync(short nNmcNo, short nGroupNo,  // 사용안함
								short *pnSyncGrpList0, short *pnSyncGrpList1); // 사용안함
	short WINAPI nmc_SyncFree(short nNmcNo, short nGroupNo); // 사용안함
//------------------------------------------------------------------------------
short WINAPI nmc_GetMDIOInput(short nNmcNo, short *pnInStatus);
short WINAPI nmc_GetMDIOOutput(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_SetMDIOOutput(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_SetMDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);
short WINAPI nmc_SetMDIOOutTogPin(short nNmcNo, short nPinNo);
short WINAPI nmc_SetMDIOOutPins(short nNmcNo, short nCount, short *pnPinNo, short *pnStatus);
short WINAPI nmc_SetMDIOOutTogPins(short nNmcNo, short nCount, short *pnPinNo);

	short WINAPI nmc_GetMDIOInPin(short nNmcNo, short nPinNo,short *pnInStatus); //사용안함
	short WINAPI nmc_SetMDIOOutputTog(short nNmcNo, short nBitNo); //사용안함
	short WINAPI nmc_GetMDIOInputBit(short nNmcNo, short nBitNo,short *pnInStatus); //사용안함
	short WINAPI nmc_SetMDIOOutputBit(short nNmcNo, short nBitNo, short nOutStatus); //사용안함
	short WINAPI nmc_SetMDIOOutputAll(short nNmcNo,  short *pnOnBitNo, short *pnOffBitNo); //사용안함
	short WINAPI nmc_SetMDIOOutputTogAll(short nNmcNo, short *pnBitNo); //사용안함
	short WINAPI nmc_GetMDIOInput32(short nNmcNo, int *plInStatus); //사용안함
	short WINAPI nmc_GetMDIOOutput32(short nNmcNo, int *plOutStatus); //사용안함
	short WINAPI nmc_SetMDIOOutput32(short nNmcNo, int lOutStatus); //사용안함
//-----------------------------------------------------------------------------
short WINAPI nmc_GetDIOInput(short nNmcNo, short *pnInStatus);
short WINAPI nmc_GetDIOInput128(short nNmcNo, short *pnInStatus);
short WINAPI nmc_GetDIOOutput(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_GetDIOOutput128(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_SetDIOOutput(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_SetDIOOutput128(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_SetDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);
short WINAPI nmc_SetDIOOutTogPin(short nNmcNo, short nPinNo);
short WINAPI nmc_SetDIOOutPins(short nNmcNo, short nCount, short *pnPinNo, short *pnStatus);
short WINAPI nmc_SetDIOOutTogPins(short nNmcNo, short nCount, short *pnPinNo);

	short WINAPI nmc_GetDIOInPin(short nNmcNo, short nPinNo,short *pnInStatus); //사용안함
	short WINAPI nmc_GetDIOInputBit(short nNmcNo, short nBitNo,short *pnInStatus); //사용안함
	short WINAPI nmc_SetDIOOutputBit(short nNmcNo, short nBitno, short nOutStatus); //사용안함
	short WINAPI nmc_SetDIOOutputTog(short nNmcNo, short nBitNo); //사용안함
	short WINAPI nmc_SetDIOOutputAll(short nNmcNo, short *pnOnBitNo, short *pnOffBitNo); //사용안함
	short WINAPI nmc_SetDIOOutputTogAll(short nNmcNo, short *pnBitNo); //사용안함
	short WINAPI nmc_GetDIOInput64(short nNmcNo, LONG64 *plInStatus); //사용안함
	short WINAPI nmc_GetDIOOutput64(short nNmcNo, LONG64 *plOutStatus); //사용안함
	short WINAPI nmc_SetDIOOutput64(short nNmcNo, LONG64 lOutStatus); //사용안함
	short WINAPI nmc_GetDIOInput32(short nNmcNo, short nIndex, int *plInStatus); //사용안함
	short WINAPI nmc_GetDIOOutput32(short nNmcNo, short nIndex, int *plOutStatus); //사용안함
	short WINAPI nmc_SetDIOOutput32(short nNmcNo, short nIndex, int lOutStatus); //사용안함
//-----------------------------------------------------------------------------
short WINAPI nmc_GetEXDIOInput(short nNmcNo, short *pnInStatus);
short WINAPI nmc_SetEXDIOOutput(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_GetEXDIOOutput(short nNmcNo, short *pnOutStatus);
short WINAPI nmc_SetEXDIOOutPin(short nNmcNo, short nPinNo, short nOutStatus);
short WINAPI nmc_SetEXDIOOutTogPin(short nNmcNo, short nPinNo);
short WINAPI nmc_SetEXDIOOutPins(short nNmcNo, short nCount, short *pnPinNo, short *pnStatus);
short WINAPI nmc_SetEXDIOOutTogPins(short nNmcNo, short nCount, short *pnPinNo);

	short WINAPI nmc_GetEXDIOInPin(short nNmcNo, short nPinNo,short *pnInStatus); //사용안함
	short WINAPI nmc_GetEXDIOInputBit(short nNmcNo, short nBitNo,short *pnInStatus); //사용안함
	short WINAPI nmc_SetEXDIOOutputBit(short nNmcNo, short nBitNo, short nOutStatus); //사용안함
	short WINAPI nmc_SetEXDIOOutputTog(short nNmcNo, short nBitNo); //사용안함
	short WINAPI nmc_SetEXDIOOutputAll(short nNmcNo, short *pnOnBitNo, short *pnOffBitNo); //사용안함
	short WINAPI nmc_SetEXDIOOutputTogAll(short nNmcNo, short *pnBitNo); //사용안함
	short WINAPI nmc_GetEXDIOInput32(short nNmcNo, int *plInStatus); //사용안함
	short WINAPI nmc_GetEXDIOOutput32(short nNmcNo, int *plOutStatus); //사용안함
	short WINAPI nmc_SetEXDIOOutput32(short nNmcNo, int lOutStatus); //사용안함
//-----------------------------------------------------------------------------
short WINAPI nmc_SetOutLimitTimePin(short nNmcNo, short nIoType, short nPinNo, short nOutStatus, int nTime);
short WINAPI nmc_GetOutLimitTimePin(short nNmcNo,short nIoType, short nPinNo,
									short *pnSet,  short *pnStatus,  short *pnOutStatus, 
									int *pnRemainTime);
//-----------------------------------------------------------------------------
short WINAPI nmc_GetFirmVersion(short nNmcNo,char *pStr); //사용안함
//-----------------------------------------------------------------------------
short WINAPI nmc_SetUnitPerPulse(short nNmcNo, short nAxisNo, double dRatio);
	double WINAPI nmc_GetUnitPerPulse(short nNmcNo, short nAxisNo); //사용안함
//-----------------------------------------------------------------------------
void WINAPI nmc_SetProtocolMethod(short nNmcNo, short nMethod);
	short WINAPI nmc_GetProtocolMethod(short nNmcNo); //사용안함
//-----------------------------------------------------------------------------
void WINAPI nmc_SetIPAddress(short nNmcNo,short nField0,short nField1,short nField2);
short WINAPI nmc_WriteIPAddress(short nNmcNo, short *pnIP, short *pnSubNet,short nGateway);
	bool WINAPI nmc_GetIPAddress(short *pnField0,short *pnField1,short *pnField2,short *pnField3); //사용안함
	short WINAPI nmc_SetDefaultIPAddress(short nNmcNo); //사용안함
//-----------------------------------------------------------------------------
short WINAPI nmc_GetDeviceType(short nNmcNo, int *plDeviceType);

short WINAPI nmc_GetDeviceInfo(short nNmcNo, short *pnMotionType, short *pnDioType, short *pnEXDio, short *pnMDio);
int WINAPI nmc_GetEnumList(short *pnIp,PNMCEQUIPLIST pNmcList);
short WINAPI nmc_GetDIOInfo(short nNmcNo, short *pnInCount, short *pnOutCount);
//-----------------------------------------------------------------------------

short WINAPI nmc_MotCfgSaveToROM(short nNmcNo, short nMode);
short WINAPI nmc_MotCfgSetDefaultROM(short nNmcNo, short nMode);
short WINAPI nmc_MotCfgLoadFromROM(short nNmcNo, short nMode);

//------------------------------------------------------------------------------
	short WINAPI nmc_AccOffsetCount(short nNmcNo, short nAxisNo, int lPulse); //사용안함
//------------------------------------------------------------------------------
short WINAPI nmc_PingCheck(short nNmcNo, int lWaitTime);
//------------------------------------------------------------------------------
short WINAPI nmc_GetBusyStatus(short nNmcNo, short nAxisNo, short *pnBusyStatus);
short WINAPI nmc_GetBusyStatusAll(short nNmcNo, short *pnBusyStatus);
//------------------------------------------------------------------------------
	short WINAPI nmc_SetTriggerCfg(short nNmcNo, short nAxis, short nCompMode, int lCmpAmount, double dDioOnTime,  //사용안함
									short nPinNo, short nDioType, short nReserve); //사용안함
short WINAPI nmc_SetTriggerEnable(short nNmcNo, short nAxis,short nEnable); //사용안함
//------------------------------------------------------------------------------
short WINAPI nmc_GetMapIO(short nNmcNo,short *pnMapInStatus);
short WINAPI nmc_MapMove(short nNmcNo, short nAxis, double dPos,short nMapIndex, short nOpt);
short WINAPI nmc_MapMoveEx(short nNmcNo, short nAxis, double dPos,short nMapIndex,short nOpt, short nPosType);
short WINAPI nmc_GetMapData(short nNmcNo,short nMapIndex, PNMCMAPDATA pNmcMapData);
short WINAPI nmc_GetMapDataEx(short nNmcNo, short nMapIndex, short nDataIndex, PNMCMAPDATA pNmcMapData);
//------------------------------------------------------------------------------
short WINAPI nmc_SetGantryAxis(short nNmcNo, short nGroupNo,short nMain, short nSub);
short WINAPI nmc_SetGantryEnable(short nNmcNo, short nGroupNo, short nEnable);
short WINAPI nmc_GetGantryInfo(short nNmcNo,short *pnEnable,short *pnMainAxes,short *pnSubAxes);
//------------------------------------------------------------------------------
// 전원 리셋 횟수 읽기
short WINAPI nmc_GetPowerResetCount(short nNmcNo, long *plResetCount);
// MPG모드
short WINAPI nmc_SetMpgMode(short nNmcNo, short nAxisNo, short nMode, long lPulse);
//------------------------------------------------------------------------------
//-------------연속보간-------------------------------------------------------
short WINAPI nmc_ContRun(short nNmcNo, short nGroupNo, short nRunMode);
short WINAPI nmc_GetContStatus (short nNmcNo, PNMCCONTSTATUS pContStatus);
short WINAPI nmc_SetContNodeLine(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, double dPos0, double dPos1, double dStart,double dAcc, double dDec , double dDriveSpeed);
short WINAPI nmc_SetContNodeLineIO(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, double dPos0, double dPos1, double dStart,double dAcc, double dDec , double dDriveSpeed, short nOnOff);
short WINAPI nmc_SetContNode3Line(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, short nAxisNo2, double dPos0, double dPos1, double dPos2, double dStart,double dAcc, double dDec , double dDriveSpeed);
short WINAPI nmc_SetContNode3LineIO(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, short nAxisNo2, double dPos0, double dPos1, double dPos2, double dStart,double dAcc, double dDec , double dDriveSpeed, short nOnOff);
short WINAPI nmc_SetContNodeArc(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, double dCenter0, double dCenter1, double dAngle, double dStart,double dAcc, double dDec, double dDriveSpeed);
short WINAPI nmc_SetContNodeArcIO(short nNmcNo, short nGroupNo, short nNodeNo, short nAxisNo0, short nAxisNo1, double dCenter0, double dCenter1, double dAngle, double dStart,double dAcc, double dDec, double dDriveSpeed, short nOnOff);
short WINAPI nmc_ContNodeClear(short nNmcNo,short nGroupNo);
short WINAPI nmc_ContSetIO(short nNmcNo, short nGroupNo, short nIoType, short nIoPinNo, short nEndNodeOnOff);
//------------------------------------------------------------------------------
short WINAPI nmc_GetCmdPos(short nNmcNo, short nAxis,int *plCmdPos);
short WINAPI nmc_GetEncPos(short nNmcNo, short nAxis,int *plEncPos);
//------------------------------------------------------------------------------
short WINAPI nmc_SetDisconnectedStopMode(short nNmcNo, int lTimeInterval, short nStopMode);
	short WINAPI nmc_SetDisconectedStopMode(short nNmcNo, int lTimeInterval, short nStopMode); // 사용안함
//------------------------------------------------------------------------------

//20140515
//------------------------------------------------------------------------------
#ifndef _PAIX_SERIAL_
#define _PAIX_SERIAL_
	#define B_9600			0
	#define B_19200			1
	#define B_38400			2

	#define STOP_1				0
	#define STOP_2				1

	#define DATA_1				0
	#define DATA_2				1
	#define DATA_3				2
	#define DATA_4				3
	#define DATA_5				4
	#define DATA_6				5
	#define DATA_7				6
	#define DATA_8				7

	#define P_NONE 				0
	#define P_ODD				1
	#define P_EVEN				2
#endif
short WINAPI nmc_SetSerialConfig(short nNmcNo, short nBaud, short nData, short nStop, short nParity);
short WINAPI nmc_SetSerialMode(short nNmcNo, short nMode);
short WINAPI nmc_SerialWrite(short nNmcNo, short nLen, char *pStr);
short WINAPI nmc_SerialRead(short nNmcNo, short *pnReadLen,char *pReadStr);
//-------------------------------------------------------------------------------------------------
// NMC 상태 정보
#pragma pack(1)
typedef struct TNMC_STATE_INFO
{
	NMCAXESEXPR			NmcAxesExpr;
	NMCAXESMOTIONOUT	NmcAxesMotOut;
	NMCHOMEFLAG			HomeFlag;
	short				nStopInfo[8];

	short				nInDio[64];			//
	short				nInExDio[16];		//
	short				nInMDio[8];			//

	short				nOutDio[64];		//
	short				nOutExDio[16];		//
	short				nOutMDio[8];		//
}NMCSTATEINFO,*PNMCSTATEINFO;
#pragma pack()

short WINAPI nmc_GetStateInfo(short nNmcNo,PNMCSTATEINFO pNmcStateInfo,int nStructSize);

//---------------------------------------------------------------------------------------------------
#pragma pack(1)
typedef struct TNMC_GANTRY
{
	short nEnable;
	short nMainAxis;
	short nSubAxis;
	short nRsvd;
}NMCGANTRYINFO,*PNMCGANTRYINFO;

typedef struct TNMC_SETUP_INFO
{
	NMCGANTRYINFO NmcGantryInfo[2];
	
}NMCSETUPINFO,*PNMCSETUPINFO;
#pragma pack()

short WINAPI nmc_GetSetupInfo(short nNmcNo,PNMCSETUPINFO pNmcSetupInfo,int nStructSize);

// ---------------------------------------------------------------------------------------------------------- //
// Unlimited Continuous Interpolation
// ------------------------------------------------- Start -------------------------------------------------- //
#pragma pack(1)
/* 무제한 연속 보간 상태 체크 시 응답 구조체 - UCI */
typedef struct NMC_CONTI_STATUS
{
	short nContiRun[2];                     // 연속 보간 실행 여부(1:Run, 0:Stop)
	short nContiWait[2];                    // 1: 큐버퍼의 노드를 모두 소진하여 다음 노드 전송을 대기, 0:큐버퍼에 실행 할 Data가 있음
	short nContiRemainBuffNum[2];           // 비어 있는 큐버퍼의 수(0 ~ 1024)
	unsigned long nContiExecutionNum[2];    // 실행 중인 연속 보간의 수(0 ~ 4,294,967,295)
	short nDummy[6];                        // dummy buff.
}NMCCONTISTATUS,*PNMCCONTISTATUS;
#pragma pack()

/* 무제한 연속 보간 큐버퍼 초기화 함수 - UCI */
short WINAPI nmc_ContiSetNodeClear(short nNmcNo,short nGroupNo);
/* 무제한 연속 보간 초기 설정 함수 - UCI */
short WINAPI nmc_ContiSetMode(short nNmcNo,short nGroupNo,short nAVTRIMode,short nEmptyMode,short n1stAxis,short n2ndAxis,short n3rdAxis,double dMaxDrvSpeed,short nIoType,int nIoCtrlPinMask,int nIoCtrlEndVal);
/* 무제한 연속 보간 상태 체크 함수 - UCI */
short WINAPI nmc_ContiGetStatus(short nNmcNo, PNMCCONTISTATUS pContiStatus);
/* 무제한 연속 보간 2축 직선 보간 함수 - UCI */
short WINAPI nmc_ContiAddNodeLine2Axis(short nNmcNo,short nGroupNo,double dPos0,double dPos1,double dStart,double dAcc,double dDec,double dDrvSpeed,int nIoCtrlVal);
/* 무제한 연속 보간 3축 직선 보간 함수 - UCI */
short WINAPI nmc_ContiAddNodeLine3Axis(short nNmcNo,short nGroupNo,double dPos0,double dPos1,double dPos2,double dStart,double dAcc,double dDec,double dDrvSpeed,int nIoCtrlVal);
/* 무제한 연속 보간 2축 원호 보간 함수 - UCI */
short WINAPI nmc_ContiAddNodeArc(short nNmcNo,short nGroupNo,double dCenter0,double dCenter1,double dAngle,double dStart,double dAcc,double dDec,double dDrvSpeed,int nIoCtrlVal);
/* 무제한 연속 보간 노드 추가 종료 함수 - UCI */
short WINAPI nmc_ContiSetCloseNode(short nNmcNo,short nGroupNo);
/* 무제한 연속 보간 실행/정지 함수 - UCI */
short WINAPI nmc_ContiRunStop(short nNmcNo, short nGroupNo, short nRunMode);

// -------------------------------------------------  End  -------------------------------------------------- //
// Unlimited Continuous Interpolation
// ---------------------------------------------------------------------------------------------------------- //
short WINAPI nmc_AVTRISetMode(short nNmcNo,short nAxis,short nAVTRIMode);
short WINAPI nmc_AVTRIGetMode(short nNmcNo,short nAxis,short *pnAVTRIMode);
short WINAPI nmc_GetGantryAxis(short nNmcNo, short *pnMainAxes, short *pnSubAxes);

// ---------------------------------------------------------------------------------------------------------- //
void WINAPI nmc_SetWaitTime(int nNmcNo,long lWaitTime);										//20160418 han 통신대기 시간

#ifdef __cplusplus
	}
#endif

#endif