#pragma once
#include "BankGuild\CommonDefineBank.h"

#define LOWER_MCR_CAMERA_IP		 _T("192.168.10.160")
#define UPPER_MCR_CAMERA_IP		 _T("192.168.10.161")

#define SOCKET_MAIN		_T("MAIN")		// Socket Index

//kjpark 20161006 MCR 상하 수정
#define SOCKET_LOWER_UVMCR		_T("LOWER_UVMCR")		
#define SOCKET_UPPER_UVMCR		_T("UPPER_UVMCR")	

#define SOCKET_PGPC				_T("PGPC")		

#define SOCKET_DATAPC			_T("DATAPC")	

#define SOCKET_ACTIVE1			_T("ACTIVE1")			
#define SOCKET_ACTIVE2			_T("ACTIVE2")		

#define SOCKET_MITALIGN			_T("MITALIGN")

#define SOCKET_MITEDGEALIGN		_T("MITEDGEALIGN")

#define SOCKET_ROBOTALIGN		_T("ROBOTALIGN")

#define SOCKET_LD_ROBOT			_T("LDROBOT")
#define SOCKET_UD_ROBOT			_T("UDROBOT")

#define SOCKET_DCR				_T("DCR")		

#define SOCKET_TRAY_ALIGN		_T("TRAY_ALIGN")

#define	SOCK_PARS_FAIL			-1
#define SOCK_PARS_SYNTEXT_ERR	-2
//////////////////////////////////////////////////////////////////////////
// 화상 검사
#define CMD_GRAB_START			500
#define CMD_CLASS_END			501
#define CMD_VT_RECEIPE_CHANGE	502
//////////////////////////////////////////////////////////////////////////
// PG
#define MSG_DISPLAY_RATE	0.2314	// D-Zone 화면에 표시되는 글자 비율. Cell별로 해상도가 틀리기에 비율로 계산한다

#define CODEAMT		0x50
#define CODEPGPC	0x80

#define PG_JUDGE_GOOD		_T("GOOD")
#define PG_JUDGE_NG			_T("NG")

#define	TURN_ON				_T("TURNON")
#define	TURN_OFF			_T("TURNOFF")
#define	RESET				_T("RESET")
#define	NEXT				_T("NEXT")
#define	BACK				_T("BACK")
#define SLEEP_IN			_T("SLEEPIN")
#define SLEEP_OUT			_T("SLEEPOUT")

// 공통
#define CELL_LOADING		_T("CELL_LOADING")
#define MTP_WRITE			_T("MTP_WRITE")
#define MTP_VERIFY			_T("MTP_VERIFY")
#define TMD_INFO			_T("TMD_INFO")	// TMD File Name [3/12/2013 OSC]
#define TSP_START			_T("TSP_START")
#define TSP_START2			_T("TSP_START2")
#define ID_CHECK			_T("ID_CHECK")	// WHITE CURRENT 대용인듯?
#define DZONE_OFF			_T("DZONE_OFF")	// D존에서 다 끝나고 Contact 푸는 명령어
#define TE_CHECK			_T("TE_CHECK")	// 2015-03-27, jhLee, TE_Check 기능 추가
#define PATTERN_SELECT		_T("PTRN")		// 2015-06-01, jhLee, Pattern Select 기능 추가
#define OTP_REG_CHECK		_T("OTP_REG_CHECK")		// 2015-06-01, jhLee, Pattern Select 기능 추가
#define ZONE_FIN			_T("ZONE_FIN")

// Send Only
#define SET_ZONE_A			_T("ZONE_A")
#define SET_ZONE_B			_T("ZONE_B")
#define SET_ZONE_C			_T("ZONE_C")
#define SET_ZONE_D			_T("ZONE_D")
#define SLEEP_CURRENT_SND	_T("SLEEP_CURRENT_CHECK")
#define WHITE_CURRENT_SND	_T("WHITE_CURRENT_CHECK")	// ID_CHECK로 대체됨 [8/3/2013 OSC]
#define PG_CIRCLE			_T("CIRCLE")
#define PG_MSG				_T("MSG")
#define PG_CONTACT_OK_MSG	_T("CONTACT_OK_MSG")		// 2014-11-27, jhLee, A-Zone에서 Loading시 Contact Current GOOD 판정이 수신되었음을 표시
#define PG_TURN_START		_T("TURN_START")			// 미사용 [8/15/2013 OSC]
#define PG_TURN_FINISH		_T("TURN_FINISH")			// 미사용

// Receive Only
#define REG_PIN				_T("REG_PIN")		// CONTACT 저항 [3/20/2013 OSC]
#define TSP_PIN				_T("TSP_PIN")	// TSP Pin 상태 [3/12/2013 OSC]
#define CURRENT_A_ACK		_T("A_CURRENT")
#define CURRENT_B_ACK		_T("B_CURRENT")
#define CURRENT_C_ACK		_T("C_CURRENT")
#define CURRENT_D_ACK		_T("D_CURRENT")
#define SLEEP_CURRENT_ACK	_T("SLEEP_CURRENT")
#define WHITE_CURRENT_ACK	_T("WHITE_CURRENT")
#define CONTACT_CURRENT		_T("CONTACT_CURRENT")
#define CURRENT_ALRAM		_T("CURRENT_ALRAM")
#define VOLTAGE_ALRAM		_T("VOLTAGE_ALRAM")
#define PG_ACK				_T("ACK")
#define PG_LINECHECK		_T("LineCheck")
#define PG_CHECK			_T("CHECK")
#define PG_PIN_REG			_T("PIN_REG")
#define PG_SYSTEM_ERROR		_T("SYSTEM_ERROR")
#define PG_DOWN				_T("DOWN")
#define TSP_HOVER			_T("TSP_HOVER")
#define PG_RUN				_T("RUN")
#define DIC_CHECK           _T("DIC_CHECK")    //2018-05-17,SKCHO


//2017-02-25, jhLee, Job file download 관련 신호기 명령 추가
#define PG_ROUTE						_T("ROUTE")							// MES server의 model file이 저장된 경로명 전송
#define PG_TMD							_T("TMD")							// MES Server의 Model file이 저장된 다운로드 할 TMD 파일과 DLL 명과 각 파일의 Checksum 전송
#define PG_TMD_COPY_START			_T("TMD_COPY_START")				// 지정 경로에 있는 Model file을 신호기PC C:\TMD 폴더로 복사
#define PG_TMD_CHECKSUM_START		_T("TMD_CHECKSUM_START")		// 신호기 PC에 C:\TMD 폴더로 복사한 파일의 Checksum값을 운영PC로 전송
#define PG_CLIENT_CHECKSUM_START	_T("CLIENT_CHECKSUM_START")	// 신호기의 checksum 값을 확인하여 MD5에 있는 Checksum 값이 틀린지 확인
#define PG_TMD_DOWNLOAD_START		_T("TMD_DOWNLOAD_START")		// 각 신호기로 download한 파일을 전송한다.
#define PG_TMD_MD5					_T("TMD_MD5")						// 각 신호기에서 사용되는 TMD의 Checksum 값을 전송받는다.


//////////////////////////////////////////////////////////////////////////
// 수동검사 [4/22/2013 OSC]
#define MANUAL_ZONE_C		_T("C")
#define MANUAL_ZONE_D		_T("D")
#define MANUAL_START		_T("START")
#define MANUAL_LOGIN		_T("LOGIN")
#define MANUAL_LOGOUT		_T("LOGOUT")
#define MANUAL_ERROR_MSG	_T("MSG")
#define MANUAL_RESULT		_T("RESULT")
#define MANUAL_CANCEL		_T("CANCEL")
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// MIT  Align
enum ALIGN_TRY_FLAG
{
	FG_CONTACT_L	= 0x01,
	FG_CONTACT_R	= 0x02,
	FG_VISION_A		= 0x04,
	FG_VISION_B		= 0x08,
};

enum SMID_CONTACT_ALIGN	// Socket Message ID
{
	SMID_ALIGN_START=1,
	SMID_ALIGN_END,
	SMID_ALIGN_ERROR,
	SMID_HALF_ALIGN_1_START,
	SMID_HALF_ALIGN_1_END,
	SMID_HALF_ALIGN_2_START,
	SMID_HALF_ALIGN_2_END,
	SMID_ALIGN_IMG_SAVE_START,
	SMID_ALIGN_IMG_SAVE_END,
	SMID_ALIGN_MARK_POS_SAVE,
	SMID_ALIGN_MARK_POS_SAVE_RESULT,
	SMID_VISION_MARK_POS_SAVE,
	SMID_VISION_MARK_POS_SAVE_RESULT,
	SMID_GRAB_MODE
};
// Socket Message Packet

struct SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_CONTACT_ALIGN_HEAD()
	{
		cSTX = STX;
	};
	char	cSTX;
	int		nMsgID;
};
struct SMPACK_CONTACT_ALIGN_START : public SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_CONTACT_ALIGN_START()
	{
		cETX = ETX;
		nMsgID = SMID_ALIGN_START;
		memset(cRcpName, NULL, 20);
		for(int i = 0; i < 4; i++)
		{
			memset(szInnerID[i], NULL, 10);
			memset(szCellID[i], NULL, 20);
		}
		dMarkDistance = 10.;
		Jig = JIG_ID_A;
		Ch = JIG_CH_1;
	};
	long	lTryAlign;//
	char	cRcpName[20];
	double	dMarkDistance;
	char	szInnerID[4][10];
	char	szCellID[4][20];
	JIG_ID	Jig;
	JIG_CH	Ch;
	char cETX;
};
struct SMPACK_HALF_ALIGN_START : public SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_HALF_ALIGN_START()
	{
		cETX = ETX;
		nMsgID = SMID_HALF_ALIGN_1_START;
		memset(cRcpName, NULL, 20);
		for(int i = 0; i < 4; i++)
		{
			memset(szInnerID[i], NULL, 10);
			memset(szCellID[i], NULL, 20);
		}
		dMarkDistance = 10.;
		Ch = JIG_CH_1;
	};
	long	lTryAlign;//
	char	cRcpName[20];
	double	dMarkDistance;
	char	szInnerID[4][10];
	char	szCellID[4][20];
	JIG_CH	Ch;
	char cETX;
};
#pragma pack(push, 1)
struct SMPACK_CONTACT_ALIGN_END : public SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_CONTACT_ALIGN_END()
	{
		cETX = ETX;
		nMsgID = SMID_ALIGN_END;
		for(int i=0; i<4; i++)
		{
			Result[i].nResult = ALIGN_RESULT_NONE;
			Result[i].dX = 0.;
			Result[i].dY = 0.;
			Result[i].dTH = 0.;
			Result[i].dScore[E_MARK_LEFT] = 0.;
			Result[i].dScore[E_MARK_RIGHT] = 0.;
		}
	};
	struct
	{
		int		nResult;// Fail, Good, Skip
		double dX;
		double dY;
		double dTH;
		double dScore[E_MARK_MAX];
	}Result[4];
	char cETX;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct SMPACK_HALF_ALIGN_END : public SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_HALF_ALIGN_END()
	{
		cETX = ETX;
		nMsgID = SMID_HALF_ALIGN_1_END;
		for(int i=0; i<4; i++)
		{
			Result[i].nResult = ALIGN_RESULT_NONE;
			Result[i].dX = 0.;
			Result[i].dY = 0.;
			Result[i].dTH = 0.;
			Result[i].dScore = 0.;
		}
	};
	struct
	{
		int		nResult;
		double dX;	
		double dY;	
		double dTH;	
		double dScore;
	}Result[4];
	char cETX;
};
#pragma pack(pop)

struct SMPACK_ALIGN_MARK_POS_SAVE : public SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_ALIGN_MARK_POS_SAVE()
	{
		cETX = ETX;
		nMsgID = SMID_ALIGN_MARK_POS_SAVE;
		Jig = JIG_ID_A;
		Ch = JIG_CH_1;
	};
	JIG_ID	Jig;
	JIG_CH	Ch;
	long	lTrySave;//
	char cETX;
};

struct SMPACK_ALIGN_MARK_POS_RESULT : public SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_ALIGN_MARK_POS_RESULT()
	{
		cETX = ETX;
		nMsgID = SMID_ALIGN_MARK_POS_SAVE_RESULT;
		bSuccess = FALSE;
	};
	BOOL bSuccess;
	char cETX;
};

struct SMPACK_VISION_MARK_POS_SAVE : public SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_VISION_MARK_POS_SAVE()
	{
		cETX = ETX;
		nMsgID = SMID_VISION_MARK_POS_SAVE;
		Ch = JIG_CH_1;
		bFirst = TRUE;
	};
	long	lTrySave;//
	JIG_CH	Ch;
	BOOL bFirst;
	char cETX;
};

struct SMPACK_VISION_MARK_POS_RESULT : public SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_VISION_MARK_POS_RESULT()
	{
		cETX = ETX;
		nMsgID = SMID_VISION_MARK_POS_SAVE_RESULT;
		bSuccess = FALSE;
	};
	BOOL bSuccess;
	char cETX;
};

struct SMPACK_GRAB_MODE : public SMPACK_CONTACT_ALIGN_HEAD
{
	SMPACK_GRAB_MODE()
	{
		cETX = ETX;
		nMsgID = SMID_GRAB_MODE;
	};
	char cETX;
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// MIT Edge Align
// Send Only
#define LA_ALIGN_REQ		_T("ALIGN_REQ")
#define LA_CHG_RECIPE		_T("CHG_RECIPE")
#define LA_ALIGN_TEACHING	_T("ALIGN_TEACHING")
// Recive Only
#define LA_CURRENT_STATE	_T("CURRENT_STATE")
#define LA_ALIGN_RES		_T("ALIGN_RES")
#define LA_CHG_RECIPE_RES	_T("CHG_RECIPE_RES")
#define LA_ALIGN_TEACHING_RES	_T("ALIGN_TEACHING_RES")
/////////////////////////////////////////////////////////////////////////
// Tray Align
// Send Only
#define TRAY_ALIGN_REQ		_T("ALIGN_REQ")
#define TRAY_CHG_RECIPE		_T("CHG_RECIPE")
#define TRAY_ALIGN_TEACHING	_T("ALIGN_TEACHING")
// Recive Only
#define TRAY_CURRENT_STATE	_T("CURRENT_STATE")
#define TRAY_ALIGN_RES		_T("ALIGN_RES")
#define TRAY_CHG_RECIPE_RES	_T("CHG_RECIPE_RES")
#define TRAY_ALIGN_TEACHING_RES	_T("ALIGN_TEACHING_RES")
/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// ROBOT
// Send Only
#define		MAX_ROBOT_DATA	2
struct ROBOT_DATA
{
	ROBOT_DATA()
	{
		for(int i=0; i<MAX_ROBOT_DATA; i++)
		{
			nData[i] = 0;
		}
	};

	DWORD	nData[MAX_ROBOT_DATA];
};
//////////////////////////////////////////////////////////////////////////

enum
{
	TIMER_RE_CONNECT = 0,
	TIMER_MAX
};


static const unsigned short crc16tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};