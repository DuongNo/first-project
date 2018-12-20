#pragma once

#define MAX_PULSE_VALUE		2147483647
#define MIN_PULSE_VALUE		-2147483646

// 모든 함수의 리턴값 
#ifndef PAIX_RETURN_VALUE1
#define PAIX_RETURN_VALUE1
	#define	NMC1_ICMP_LOAD_FAIL	-11
	#define	NMC1_NOT_EXISTS		-10
	#define	NMC1_CMDNO_ERROR	-9
	#define	NMC1_NOTRESPONSE	-8
	#define NMC1_BUSY			-7
	#define NMC1_COMMERR		-6
	#define NMC1_SYNTAXERR   	-5
	#define NMC1_INVALID		-4
	#define NMC1_UNKOWN			-3
	#define NMC1_SOCKINITERR 	-2
	#define NMC1_NOTCONNECT 	-1
	#define NMC1_OK				0
#endif

// Bit 연산 정의
#ifndef _PAIX_BITOPERATION_
	#define _PAIX_BITOPERATION_
	#define BITSET(val,bit)					(val) |= (1<<(bit))
	#define BITCLEAR(val,bit)				(val) &= (~(1<<(bit)))
	#define BITTOGGLE(val,bit)				(val) ^= (1<<(bit))
	#define BITVALUE(val, sdata, bit)		(val) = (((sdata)>> bit) & 0x01)
	#define GETBIT(bytes, bit_no)			(((bytes)>>(bit_no))&0x01)
	#define SETBIT(bytes, bit_no, val)    { (bytes) &=~(1<<(bit_no)); (bytes) |= ((val) << (bit_no)); }
#endif
	
	// 내부 홈 동작 플래그
	#define HOME_READY			0	// 홈 동작 완료
	#define HOME_START			1	// 홈 동작 시작
	#define HOME_NEAR			2	// NEAR 서치중
	#define HOME_EDGE			3	// EDGE 서치중 ( Near or Limit 센서 )
	#define HOME_ESTOP			4	// 센서 온시 즉시 스탑
	#define HOME_HOME			5	// Z 상 서치중
    #define HOME_END			6
	#define HOME_NONE			7
	#define HOME_RE_EDGE		8
	#define HOME_OFFSET			9
	
	#define HOME_DELAY			10

	#define HOME_EMG			11
	#define HOME_ALARM			12
	
	#define HOME_NEAR_DELAY		13
	#define HOME_EDGE_DELAY		14
	#define HOME_RE_EDGE_DELAY	15
	#define HOME_END_DELAY		16
	#define HOME_DELAY_DONE		17
	#define HOME_ESTOP_DELAY	18
	#define HOME_ON_NEAR		19
	#define HOME_ON_NEAR_DELAY	20

    
// 2009.07.13 추가
struct NMC_OUT
{
	unsigned short 	MotorOn0:1;		// Motor OnOff
	unsigned short 	ServoOn0:1;		// Servo OnOff
	unsigned short 	ResetDCC0:1;	// Reset DCC
	unsigned short 	ResetAlm0:1;	// Reset Alarm

	unsigned short 	MotorOn1:1;
	unsigned short 	ServoOn1:1;
	unsigned short 	ResetDCC1:1;
	unsigned short 	ResetAlm1:1;

	unsigned short 	MotorOn2:1;
	unsigned short 	ServoOn2:1;
	unsigned short 	ResetDCC2:1;
	unsigned short 	ResetAlm2:1;

	unsigned short 	MotorOn3:1;
	unsigned short 	ServoOn3:1;
	unsigned short 	ResetDCC3:1;
	unsigned short 	ResetAlm3:1;
	
	unsigned short 	MotorOn4:1;		// Motor OnOff
	unsigned short 	ServoOn4:1;		// Servo OnOff
	unsigned short 	ResetDCC4:1;	// Reset DCC
	unsigned short 	ResetAlm4:1;	// Reset Alarm

	unsigned short 	MotorOn5:1;
	unsigned short 	ServoOn5:1;
	unsigned short 	ResetDCC5:1;
	unsigned short 	ResetAlm5:1;

	unsigned short 	MotorOn6:1;
	unsigned short 	ServoOn6:1;
	unsigned short 	ResetDCC6:1;
	unsigned short 	ResetAlm6:1;

	unsigned short 	MotorOn7:1;
	unsigned short 	ServoOn7:1;
	unsigned short 	ResetDCC7:1;
	unsigned short 	ResetAlm7:1;
}; 

typedef union NMC_OUTSTATUS{	//WR3
   long            all;
   struct NMC_OUT		bit;
}NMCOUTSTATUS,*PNMCOUTSTATUS;
//	2008.05.06 - LIGIC READ 추가
typedef struct NMCLOGIC{
	UINT nOrg0:1;
	UINT nOrg1:1;
	UINT nOrg2:1;
	UINT nOrg3:1;

	UINT nInpo0:1;
	UINT nInpo1:1;
	UINT nInpo2:1;
	UINT nInpo3:1;//8
	//---------------1 BYte ---------------	1		
	UINT nAlarm0:1;
	UINT nAlarm1:1;
	UINT nAlarm2:1;
	UINT nAlarm3:1;

	UINT nMLimit0:1;
	UINT nMLimit1:1;
	UINT nMLimit2:1;
	UINT nMLimit3:1;//16
	//---------------1 BYte ---------------	2  -0
	UINT nPLimit0:1; 
	UINT nPLimit1:1;
	UINT nPLimit2:1;
	UINT nPLimit3:1;

	UINT nNear0:1;
	UINT nNear1:1;
	UINT nNear2:1;
	UINT nNear3:1;//24
	//---------------1 BYte ---------------	3		
	UINT nZ0:1;
	UINT nZ1:1;
	UINT nZ2:1;
	UINT nZ3:1;

	UINT nEmer:1;
	UINT nReserve1:3; //32

	//---------------1 BYte --------------- 4 -1
	UINT nEncCount0:2;
	UINT nEncCount1:2;
	UINT nEncCount2:2;
	UINT nEncCount3:2;
	//---------------1 BYte --------------- 5
	UINT nEncInput0:2;
	UINT nEncInput1:2;
	UINT nEncInput2:2;
	UINT nEncInput3:2;
	//---------------1 BYte --------------- 6 -2
	UINT nPulse0:1;
	UINT nPulse1:1;
	UINT nPulse2:1;
	UINT nPulse3:1;
	
	UINT nClock0:1;
	UINT nClock1:1;
	UINT nClock2:1;
	UINT nClock3:1;
	//---------------1 BYte --------------- 7
    UINT nPulseDir0:1;
    UINT nPulseDir1:1;
    UINT nPulseDir2:1;
    UINT nPulseDir3:1;
	
	UINT nEndBusyMode0:1;
	UINT nEndBusyMode1:1;	
	UINT nEndBusyMode2:1;
	UINT nEndBusyMode3:1;	
	//---------------1 BYte --------------- 8 -3
}NMCLOGICBIT,*PNMCLOGICBIT;


typedef struct NMCBit{
	UINT nBusy0:1;
	UINT nBusy1:1;
	UINT nBusy2:1;
	UINT nBusy3:1;  //4
			
	UINT nError0:1;
	UINT nError1:1;
	UINT nError2:1;
	UINT nError3:1;  //8
	//---------------1 BYte ---------------	1		
	UINT nNear0:1;
	UINT nNear1:1;
	UINT nNear2:1;
	UINT nNear3:1;  //12
			
	UINT nPLimit0:1;
	UINT nNLimit0:1;
	UINT nAlarm0:1;  //15
			
	UINT nPLimit1:1;
	//---------------1 BYte ---------------	2
	UINT nNLimit1:1;
	UINT nAlarm1:1;  //18
			
	UINT nPLimit2:1;
	UINT nNLimit2:1;
	UINT nAlarm2:1;  //21
			
	UINT nPLimit3:1;
	UINT nNLimit3:1;
	UINT nAlarm3:1;  //24
	//---------------1 BYte ---------------	3		
	UINT nEmer:1;    //25
	
	UINT nPointMove:1; //28
	UINT nReserve1:2; //28
		
	UINT nSwPLimit0:1; //29
	UINT nSwPLimit1:1; //30
	UINT nSwPLimit2:1; //31
	UINT nSwPLimit3:1; //32
	//---------------1 BYte --------------- 4

	UINT nInpo0:1;
	UINT nInpo1:1;
	UINT nInpo2:1;
	UINT nInpo3:1;  //36

	UINT nHome0:1;
	UINT nHome1:1;
	UINT nHome2:1;
	UINT nHome3:1;  //40
	//---------------1 BYte --------------- 1
	UINT nEncZ0:1;
	UINT nEncZ1:1;
	UINT nEncZ2:1;
	UINT nEncZ3:1; //44

	UINT nOrg0:1;
	UINT nOrg1:1;
	UINT nOrg2:1;
	UINT nOrg3:1; //48
	//---------------1 BYte --------------- 2
	UINT nSReady0:1;
	UINT nSReady1:1;
	UINT nSReady2:1;
	UINT nSReady3:1; //52

	// 2 축에서만 사용, 4축에서는 무시
	//INPUT 8BIT 값
	UINT nInput0:1;
	UINT nInput1:1;
	UINT nInput2:1;
	UINT nInput3:1;
	//---------------1 BYte --------------- 3
	UINT nInput4:1;  //56
	UINT nInput5:1;
	UINT nInput6:1;
	UINT nInput7:1;

	UINT nSwMLimit0:1; //29
	UINT nSwMLimit1:1; //30
	UINT nSwMLimit2:1; //31
	UINT nSwMLimit3:1; //32
	//---------------1 BYte --------------- 3
}NMCBIT,*PNMCBIT;

typedef struct NMCData
{
	NMCBIT nmcBit;
	long Enc[4];
	long Cmd[4];
	double fEnc[4];
	double fCmd[4];
}NMCDATA,*PNMCDATA;

typedef struct NMCIOBit{
	UINT nBit0:1; 	UINT nBit1:1;	UINT nBit2:1;	UINT nBit3:1;
	UINT nBit4:1;	UINT nBit5:1;	UINT nBit6:1;	UINT nBit7:1;	
	UINT nBit8:1;	UINT nBit9:1;	UINT nBit10:1;	UINT nBit11:1;
	UINT nBit12:1;	UINT nBit13:1;	UINT nBit14:1;	UINT nBit15:1;
}NMCIOBIT,*PNMCIOBIT;
// 32 점 IO struct		
typedef struct NMCIOBit32{
	UINT nBit0:1;	UINT nBit1:1;	UINT nBit2:1;	UINT nBit3:1;
	UINT nBit4:1;	UINT nBit5:1;	UINT nBit6:1;	UINT nBit7:1;	
	UINT nBit8:1;	UINT nBit9:1;	UINT nBit10:1;	UINT nBit11:1;
	UINT nBit12:1;	UINT nBit13:1;	UINT nBit14:1;	UINT nBit15:1;
	UINT nBit16:1;	UINT nBit17:1;	UINT nBit18:1;	UINT nBit19:1;
	UINT nBit20:1;	UINT nBit21:1;	UINT nBit22:1;	UINT nBit23:1;
	UINT nBit24:1;	UINT nBit25:1;	UINT nBit26:1;	UINT nBit27:1;
	UINT nBit28:1;	UINT nBit29:1;	UINT nBit30:1;	UINT nBit31:1;
}NMCIOBIT32,*PNMCIOBIT32;

typedef struct NMC_STATUS{
	// 1 축
	UINT nStopOrg0:1;
	UINT nStopNear0:1;
	UINT nStopEncZ0:1;
	UINT nStopSReady0:1;
	UINT nStopPLimit0:1;
	UINT nStopMLimit0:1;
	UINT nStopAlarm0:1;
	UINT nStopEmg0:1;
	// 2축
	UINT nStopOrg1:1;
	UINT nStopNear1:1;
	UINT nStopEncZ1:1;
	UINT nStopSReady1:1;
	UINT nStopPLimit1:1;
	UINT nStopMLimit1:1;
	UINT nStopAlarm1:1;
	UINT nStopEmg1:1;

	// 3 축
	UINT nStopOrg2:1;
	UINT nStopNear2:1;
	UINT nStopEncZ2:1;
	UINT nStopSReady2:1;
	UINT nStopPLimit2:1;
	UINT nStopMLimit2:1;
	UINT nStopAlarm2:1;
	UINT nStopEmg2:1;
	
	// 4축
	UINT nStopOrg3:1;
	UINT nStopNear3:1;
	UINT nStopEncZ3:1;
	UINT nStopSReady3:1;
	UINT nStopPLimit3:1;
	UINT nStopMLimit3:1;
	UINT nStopAlarm3:1;
	UINT nStopEmg3:1;

	// 5 축
	UINT nStopOrg4:1;
	UINT nStopNear4:1;
	UINT nStopEncZ4:1;
	UINT nStopSReady4:1;
	UINT nStopPLimit4:1;
	UINT nStopMLimit4:1;
	UINT nStopAlarm4:1;
	UINT nStopEmg4:1;
	// 6축
	UINT nStopOrg5:1;
	UINT nStopNear5:1;
	UINT nStopEncZ5:1;
	UINT nStopSReady5:1;
	UINT nStopPLimit5:1;
	UINT nStopMLimit5:1;
	UINT nStopAlarm5:1;
	UINT nStopEmg5:1;

	// 7 축
	UINT nStopOrg6:1;
	UINT nStopNear6:1;
	UINT nStopEncZ6:1;
	UINT nStopSReady6:1;
	UINT nStopPLimit6:1;
	UINT nStopMLimit6:1;
	UINT nStopAlarm6:1;
	UINT nStopEmg6:1;
	// 8축
	UINT nStopOrg7:1;
	UINT nStopNear7:1;
	UINT nStopEncZ7:1;
	UINT nStopSReady7:1;
	UINT nStopPLimit7:1;
	UINT nStopMLimit7:1;
	UINT nStopAlarm7:1;
	UINT nStopEmg7:1;
}NMC_STATUSBIT,*PNMC_STATUSBIT;

//{	
struct NMC_OUT_EXT
{
	short MotorOn[8];		// Motor OnOff
	short ServoOn[8];		// Servo OnOff
	short ResetDCC[8];		// Reset DCC
	short ResetAlm[8];		// Reset Alarm
};

//	2008.05.06 - LIGIC READ 추가
struct NMCLOGIC_EXT{
	short nOrg[8];
	short nInpo[8];
	short nAlarm[8];
	short nMLimit[8];
	short nPLimit[8]; 
	short nNear[8];
	short nZ[8];
	short nEmer[2];
	short nEncCount[8];
	short nEncInput[8];
	short nPulse[8];
	short nClock[8];
    short nPulseDir[8];
	short nEndBusyMode[8];
};
// Size 424
struct NMCDATA_EXT{
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
	// 2 축에서만 사용, 4축에서는 무시
	//INPUT 8BIT 값
	short nInput[8];
	short nSwMLimit[8]; 

	long Enc[8];
	long Cmd[8];
	double fEnc[8];
	double fCmd[8];
	char dummy[4];
};

struct NMC_STATUS_EXT{
	// 1 축
	short nStopOrg[8];
	short nStopNear[8];
	short nStopEncZ[8];
	short nStopSReady[8];
	short nStopPLimit[8];
	short nStopMLimit[8];
	short nStopAlarm[8];
	short nStopEmg[8];
};
struct NMC_HOME_FLAG{
    short nSrchFlag[8];
    short nStatusFlag[8];
};

struct NMC_SAM_STATUS_EXT{
	// 1 축
	short nRun[8];
	short nStatus[8];
	long  lMotorRot[8];
	long  lMotorPulse[8];
};

struct NMC_CONT_STATUS_EXT
{
	short nLineNo;
	short nStatus;
	short nError;
};
struct NMC_RING_FLAG
{
	short nRingMode[8];
	long lRingCmd[8];
	long lRingEnc[8];
};
//}

    // Angle 동작 완료
	#define SON_READY			0
    // Angle 동작 시작
	#define SON_START			1
    // Angle 센서 OFF 서치
	#define SON_NEAROFF			2
    // Angle 센서 On 서치
	#define SON_NEAR			3
    // Angle 정지
	#define SON_STOP			4
    // Angle 설정된 펄스값 모션 이동 대기
	#define SON_WAIT_FOR_READY	5
 
