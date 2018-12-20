#pragma once

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
	#define NMC_CONTI_BUF_FULL      -15	/* Unlimited Interpolation Error - UCI */
	#define NMC_CONTI_BUF_EMPTY     -14	/* Unlimited Interpolation Error - UCI */
	#define NMC_INTERPOLATION       -13
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

//2014-12-17, jhLee, DataPC와의 Struct size 호환을 위해 본 library만 1Byte으로 지정하고 컴파일한다.
//#pragma pack(show)
#pragma pack(push, 1)				
//#pragma pack(show)


typedef struct NMC_PARA_LOGIC
{
   //-------------------------------------------------------------
	short nEmg;				// EMG
	short nEncCount;		// 엔코더 카운트 모드

	short nEncDir;			// 엔코더 카운트 방향
	short nEncZ;			// 엔코더 Z

	short nNear;			// NEAR(HOME)
	short nMLimit;			// - Limit

	short nPLimit;			// + Limit
	short nAlarm;			// Alarm

	short nInp;				// INPOSITION
	short nSReady;			// Servo Ready

	short nPulseMode;		// 1p/2p Mode
	//-------------------------------------------------------------

	short nLimitStopMode;	// Limit stop mode
	short nBusyOff;			// Busy off mode

	short nSWEnable;		// sw limit 활성화 여부
   //-------------------------------------------------------------
	double  	dSWMLimitPos;
	double  	dSWPLimitPos;
}PARALOGIC,*PPARALOGIC;


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
	short nEmg;				// EMG
	short nMLimit;			// 엔코더 카운트 모드

	short nPLimit;			// 엔코더 체
	short nAlarm;			// 엔코더 Z

	short nNear;			// NEAR
	short nEncZ;			// - Limit
}STOPMODE,*PSTOPMODE;

typedef struct NMC2_HOME_FLAG{
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
//-------------------------------------------------------------------------------------------------
// NMC 상태 정보
//old #pragma pack(1)
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
//old #pragma pack()

//---------------------------------------------------------------------------------------------------
//old #pragma pack(1)
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

//2014-11-03, jhLee, DataPC와의 Struct size 호환을 위해 유일하게 1Byte를 사용하는 MP2100 library만 1Byte으로 지정하고 컴파일한다.
// 원래 사용하던 Align size를 복구한다.
#pragma pack(pop)		//of #pragma pack(push, 1)				
//#pragma pack(show)
