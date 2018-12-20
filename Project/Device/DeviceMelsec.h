#pragma once
#include "../../ModuleInternal/MELSECNet/MelsecNet.h"

typedef struct _tagCC_INFO 
{
	short						m_iStation;
	short						m_iNetWork;
	long						m_lNetPath;
	short						m_iNetMode;
	short						m_iNetChanel;

} CC_INFO;

typedef struct _tagOP_INFO 
{
	short						m_iStation;
	short						m_iNetWork;
	long						m_lNetPath;
	short						m_iNetMode;
	short						m_iNetChanel;

	short						m_iStation_Own;
	short						m_iNetWork_Own;


} OP_INFO;

#define ROBOT 0
#define UPPER_EQP 1
#define INT_MNET_CCLINK_PATH		81		//melsec ID	Robot CC Link 
#define INT_MNET_MODE				-1		//1: ONLINE, 2: OFFLINE, 3:Forward loop test, 4: Reverse loop test
#define INT_MNET_CCLINK_CHANNEL		81		//	Robot CC Link 
#define INT_MNET_NETWORK			10		//Networt Number
#define INT_MNET_STATION			255		//Station Number

#define INT_MNET_OPTICAL_CHANNEL	151		//	앞 설비 광케이블
#define INT_MNET_OPTICAL_NETWORK	0		//Networt Number
#define INT_MNET_OPTICAL_STATION	255 	//Station Number:2  앞설비:1
#define INT_MNET_OPTICAL_PATH       151
#define INT_MNET_OPTICAL_MODE    1

//#define INT_MNET_OPTICAL_NETWORK_OWN	0		//Networt Number
//#define INT_MNET_OPTICAL_STATION_OWN	255		//Station Number:2  앞설비:1


#define OPTICAL_XY_RECEIVE_ABLE						0x284
#define OPTICAL_XY_RECEIVE_START					0x285
#define OPTICAL_XY_RECEIVE_COMP						0x286
#define OPTICAL_XY_RECEIVE_IMM_PAUSE				0x287

#define OPTICAL_XY_SEND_ABLE					    0x504
#define OPTICAL_XY_SEND_START				        0x505
#define OPTICAL_XY_SEND_COMP					    0x506
#define OPTICAL_XY_SEND_IMM_PAUSE					0x507



//Optical Bit address
#define OPTICAL_XY_HEARTBEAT						100
#define OPTICAL_XY_MACHINE_PAUSE					101
#define OPTICAL_XY_MACHINE_ABNORMAL					102
#define OPTICAL_XY_RESERVE_03						103



#define OPTICAL_XY_EXCHANGE_FLAG					0x10A
#define OPTICAL_XY_MULTICARRY_FLAG					0x10B
#define OPTICAL_XY_RESERVE_12						0x10C
#define OPTICAL_XY_RESERVE_13						0x10D
#define OPTICAL_X_RESERVE_14_Y_LOADINGSTOP			0x10E
#define OPTICAL_X_RESERVE_15_Y_TRANSFERSTOP			0x10F
#define OPTICAL_XY_PRE_ACTION1						0x110
#define OPTICAL_XY_PRE_ACTION2						0x111
#define OPTICAL_XY_MID_ACTION1						0x112
#define OPTICAL_XY_MID_ACTION2						0x113
#define OPTICAL_XY_POST_ACTION1						0x114
#define OPTICAL_XY_RESERVE21						0x115
#define OPTICAL_X_RESERVE22_Y_RCVREFUSE				0x116
#define OPTICAL_XY_RESERVE23						0x117
#define OPTICAL_XY_RESERVE24						0x118
#define OPTICAL_XY_RESERVE25						0x119
#define OPTICAL_XY_HS_RESUME_REQUEST				0x11A
#define OPTICAL_XY_HS_RESUME_REPLY_ACK				0x11B
#define OPTICAL_XY_HS_RESUME_REPLY_NAK				0x11C
#define OPTICAL_XY_RESERVE29						0x11D
#define OPTICAL_XY_HS_INITIAL						0x11E
#define OPTICAL_XY_HS_ERROR							0x11F

//CCLink bit address
#define	LDROBOT_X_TEACHMODE_Y_EXTSTART					0x00
#define	LDROBOT_X_PLAYMODE_Y_EXTSTOP					0x01
#define	LDROBOT_X_RUNNING_Y_ERRRESET					0x02
#define	LDROBOT_X_STOPPING_Y_EXTMOTORON					0x03
#define	LDROBOT_X_INTERLOCKALARM_Y_EXTMOTOROFF			0x04	
#define	LDROBOT_X_MOTORON_Y_LOWSPEED					0x05
#define	LDROBOT_X_PGMEND_Y_PAUSE						0x06
#define LDROBOT_X_ALARM_Y_EXTRESET						0x07
#define	LDROBOT_X_BATWARN_Y_ORG							0x08
#define	LDROBOT_X_HOMEPOS1_Y_CYCLESTOP					0x09
#define	LDROBOT_X_HOMEPOS2_Y_PGMSTROBE					0x0A
#define LDROBOT_X_PGMACK_Y_PGMSEL1						0x0B
#define	LDROBOT_X_EXTRESET_Y_PGMSEL2					0x0C
#define	LDROBOT_X_STATEOUT1_Y_PGMSEL3					0x0D
#define	LDROBOT_X_STATEOUT2_Y_PGMSEL4					0x0E
#define	LDROBOT_X_UNITREADY_Y_PGMSEL5					0x0F
#define	LDROBOT_X_BUSY_Y_PGMSEL6						0x10
#define	LDROBOT_X_CYCLESTOP_Y_PGMSEL7					0x11
#define	LDROBOT_X_ORGERR_Y_PGMSEL8						0x12
#define	LDROBOT_X_DATAERR_Y_SPEEDIN1					0x13
#define	LDROBOT_X_MEMORYERR_Y_SPEEDIN2					0x14
#define	LDROBOT_Y_SPEEDIN3								0x15
#define	LDROBOT_Y_SPEEDIN4								0x16
#define	LDROBOT_Y_SPEEDIN5								0x17	
#define	LDROBOT_Y_SPEEDIN6								0x18
#define	LDROBOT_Y_SPEEDIN7								0x19
#define	LDROBOT_Y_RETRY									0x1A
#define	LDROBOT_X_MANUALIN_Y_MANUALSELECT				0x1B
#define	LDROBOT_X_MANUALCOMP_Y_MANUALGO					0x1C
//메뉴얼 상태 확인 비트 추가 2017/06/20 HSK.
#define	LDROBOT_X_MANUALMOVE_STANBY						0x20

#define	LDROBOT_XY_VACON								0x22
#define	LDROBOT_XY_VACOFF								0x23
#define	LDROBOT_X_VACERR								0x24
#define	LDROBOT_XY_CVGET								0x31
#define	LDROBOT_XY_VICOMP								0x32
#define	LDROBOT_Y_VIERR									0x33
#define	LDROBOT_X_CVGETERR								0x39
#define	LDROBOT_X_CVGETCOMP								0x3A
#define	LDROBOT_XY_UPPERSTGPUT							0x3B
#define	LDROBOT_XY_UPPERSTGVAC1ON						0x3C
#define	LDROBOT_XY_UPPERSTGVAC1OFF						0x3D
#define	LDROBOT_XY_UPPERSTGVAC2ON						0x3E
#define	LDROBOT_XY_UPPERSTGVAC2OFF						0x3F
//2017/11/08, JSJUNG,  비트 추가 
#define	LDROBOT_Y_UPPERSTGCH1PUT						0x40
#define	LDROBOT_Y_UPPERSTGCH2PUT						0x41

#define	LDROBOT_X_UPPERSTGVAC1ERR						0x42
#define	LDROBOT_X_UPPERSTGVAC2ERR						0x43
#define	LDROBOT_X_UPPERSTGPUTCOMP						0x44
#define	LDROBOT_XY_LOWERSTGPUT							0x45
#define	LDROBOT_XY_LOWERSTGVAC1ON						0x46
#define	LDROBOT_XY_LOWERSTGVAC1OFF						0x47
#define	LDROBOT_XY_LOWERSTGVAC2ON						0x48
#define	LDROBOT_XY_LOWERSTGVAC2OFF						0x49
//2017/11/08, JSJUNG,  비트 추가 
#define	LDROBOT_Y_LOWERSTGCH1PUT						0x4A
#define	LDROBOT_Y_LOWERSTGCH2PUT						0x4B

#define	LDROBOT_X_LOWERSTGVAC1ERR						0x4C
#define	LDROBOT_X_LOWERSTGVAC2ERR						0x4D
#define	LDROBOT_X_LOWERSTGPUTCOMP						0x4E

//2017/11/08, JSJUNG,  비트 추가 
#define	LDROBOT_Y_SQCOMPRTN								0x63
#define	LDROBOT_Y_SQERRRTN								0x64
#define	LDROBOT_Y_SQSKIP								0x65

#define	LDROBOT_XY_LDROBOTINTERLOCK1					0x6E
#define	LDROBOT_XY_LDROBOTINTERLOCK2					0x6F

#define	LDROBOT_PERMIT									0x00

#define	LDROBOT_MANUAL_POS								0x07

//2018/01/26, JSJUNG, LD/UD ROBOT 티칭값/현재 위치값(워드영역/READ 부)
#define	LDROBOT_TEACH_MOVE_XYZT_VAL						0x00
#define	UDROBOT_TEACH_MOVE_XYZT_VAL						0x10

//CCLink bit address
#define	UDROBOT_X_TEACHMODE_Y_EXTSTART					0x80
#define	UDROBOT_X_PLAYMODE_Y_EXTSTOP					0x81
#define	UDROBOT_X_RUNNING_Y_ERRRESET					0x82
#define	UDROBOT_X_STOPPING_Y_EXTMOTORON					0x83
#define	UDROBOT_X_INTERLOCKALARM_Y_EXTMOTOROFF			0x84	
#define	UDROBOT_X_MOTORON_Y_LOWSPEED					0x85
#define	UDROBOT_X_PGMEND_Y_PAUSE						0x86
#define UDROBOT_X_ALARM_Y_EXTRESET						0x87
#define	UDROBOT_X_BATWARN_Y_ORG							0x88
#define	UDROBOT_X_HOMEPOS1_Y_CYCLESTOP					0x89
#define	UDROBOT_X_HOMEPOS2_Y_PGMSTROBE					0x8A
#define UDROBOT_X_PGMACK_Y_PGMSEL1						0x8B
#define	UDROBOT_X_EXTRESET_Y_PGMSEL2					0x8C
#define	UDROBOT_X_STATEOUT1_Y_PGMSEL3					0x8D
#define	UDROBOT_X_STATEOUT2_Y_PGMSEL4					0x8E
#define	UDROBOT_X_UNITREADY_Y_PGMSEL5					0x8F
#define	UDROBOT_X_BUSY_Y_PGMSEL6						0x90
#define	UDROBOT_X_CYCLESTOP_Y_PGMSEL7					0x91
#define	UDROBOT_X_ORGERR_Y_PGMSEL8						0x92
#define	UDROBOT_X_DATAERR_Y_SPEEDIN1					0x93
#define	UDROBOT_X_MEMORYERR_Y_SPEEDIN2					0x94
#define	UDROBOT_Y_SPEEDIN3								0x95
#define	UDROBOT_Y_SPEEDIN4								0x96
#define	UDROBOT_Y_SPEEDIN5								0x97	
#define	UDROBOT_Y_SPEEDIN6								0x98
#define	UDROBOT_Y_SPEEDIN7								0x99
#define	UDROBOT_Y_RETRY									0x9A
#define	UDROBOT_X_MANUALIN_Y_MANUALSELECT				0x9B
#define	UDROBOT_X_MANUALCOMP_Y_MANUALGO					0x9C
//메뉴얼 상태 확인 비트 추가 2017/06/20 HSK.
#define	UDROBOT_X_MANUALMOVE_STANBY						0xA0

#define	UDROBOT_XY_VACON								0xA2
#define	UDROBOT_XY_VACOFF								0xA3
#define	UDROBOT_X_VACERR								0xA4

//2017/11/08, JSJUNG, TRAY VISION 비트 추가 .
#define	UDROBOT_X_TRAYVIREADY_Y_TRAYVIUSE				0xA7
#define	UDROBOT_X_TRAYVIOK_Y_TRAYVISTART				0xA8
#define	UDROBOT_X_TRAYVING_Y_OKNGRTN					0xA9
#define	UDROBOT_X_RBTOVIRETRY_Y_TRAYVISTARTCOMP			0xAA	

#define	UDROBOT_XY_GETSTAGE								0xB1
#define	UDROBOT_XY_STGCH1VACON							0xB2
#define	UDROBOT_XY_STGCH1VACOFF							0xB3
#define	UDROBOT_X_STGCH2VACON_Y_STG1OK					0xB4
#define	UDROBOT_X_STGCH2VACOFF_Y_STG1NG					0xB5
#define	UDROBOT_Y_STG2VACON								0xB6
#define	UDROBOT_X_STG1GETERR_Y_STG2VACOFF				0xB7
#define	UDROBOT_X_STG2GETERR_Y_STG2OK					0xB8
#define	UDROBOT_X_STGGETCOMP_Y_STG2NG					0xB9
#define	UDROBOT_XY_NGTRAYPUT							0xBB
//2017/11/09, JSJUNG,  비트 추가 
#define	UDROBOT_X_STGCH1GET								0xBC
#define	UDROBOT_X_STGCH2GET								0xBD

//2018-05-14,NAM, Add IO for ULD ROBOT Put in Hybrid Conveyor
#define UDROBOT_XY_REQUESTPUT_CONVEYORMODE			  0xBF  //
#define UDROBOT_XY_PUTDOWNCMP_CONVEYORREADY           0xC0

#define	UDROBOT_X_NGTRAYPUTERR							0xC3
#define	UDROBOT_X_NGTRAYPUTCOMP							0xC4
#define	UDROBOT_XY_OKTRAYPUT							0xC5
#define	UDROBOT_X_OKTRAYPUTERR							0xCD
#define	UDROBOT_X_OKTRAYPUTCOMP							0xCE

//2017/11/08, JSJUNG,  비트 추가 
#define	UDROBOT_Y_SQCOMPRTN								0xE3
#define	UDROBOT_Y_SQERRRTN								0xE4
#define	UDROBOT_Y_SQSKIP								0xE5

#define	UDROBOT_X_TRAYDATANG							0xE8
#define	UDROBOT_X_TRAYDATAOK							0xE9
#define	UDROBOT_XY_UDROBOTINTERLOCK1					0xEE
#define	UDROBOT_XY_UDROBOTINTERLOCK2					0xEF

#define	UDROBOT_PERMIT									0x10

#define	UDROBOT_MANUAL_POS								0x17

#define	UDROBOT_TRAYCOUNT								0x18



class cDeviceMelsec : public CMelsecNet					  
{														 
public:													 
	cDeviceMelsec(void);								  
	~cDeviceMelsec(void);								  
														  
	DECLARE_MESSAGE_MAP()								  
														  
private:
	BOOL m_bCCIsConnected;
	BOOL m_bOPIsConnected;

public:
	int m_nMoveType;
	CC_INFO m_CCInfo;
	OP_INFO m_OPInfo;

	CString m_strRcvBuf;
	CString m_strSndBuf;

	void CCSetConnected(BOOL bIsConnected)	
	{ 
		m_bCCIsConnected = bIsConnected; 
	}

	BOOL CCIsConnected()	
	{ 
		return m_bCCIsConnected; 
	}
		
	void OPSetConnected(BOOL bIsConnected)	
	{ 
		m_bOPIsConnected = bIsConnected; 
	}

	BOOL OPIsConnected()	
	{ 
		return m_bOPIsConnected; 
	}

	BOOL Open(BOOL	bRobot);
	void Close(BOOL	bRobot);

	short UpperBitOn(short devno);
	short RobotBitOn(short devno);

	//bit Off	//MelsecNetBitOff
	short UpperBitOff(short devno);
	short RobotBitOff(short devno);

	//bit read	//MelsecNetBitRecv
	short UpperBitRecv(short devno);
	short RobotBitRecv(short devno);

	//word read	//MelsecNetWordRecvBuf
	void UpperWordRecvBuf(short devno, short nSizebyByte, PSHORT pData);
	void RobotWordRecvBuf(short devno, short nSizebyByte, PSHORT pData);
	void RobotWordRecvBuf(short devno, short nSizebyByte, PLONG pData);

	//word write //MelsecNetWordSend
	CString UpperWordSend(short devno, LPVOID data, short nSizebyByte, BOOL bString);
	CString RobotWordSend(short devno, LPVOID data, short nSizebyByte, BOOL bString);
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

