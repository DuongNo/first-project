#pragma once
#include "../../ModuleInternal/MELSECNet/MelsecNet.h"

typedef struct _tagPLC_INFO 
{

	short						m_iStation;
	short						m_iNetWork;
	long						m_lNetPath;
	short						m_iNetMode;
	short						m_iNetChanel;

} PLC_INFO;

#define INT_MNET_PATH				151
#define INT_MNET_MODE				1		//1: ONLINE, 2: OFFLINE, 3:Forward loop test, 4: Reverse loop test
#define INT_MNET_CHANNEL			151
#define INT_MNET_NETWORK			10		//Networt Number
#define INT_MNET_STATION			255		//Station Number

class cDeviceScaraRobot : public CMelsecNet
{
public:
	cDeviceScaraRobot(void);
	~cDeviceScaraRobot(void);

private:
	BOOL m_bIsConnected;

public:
	enum SCARA_G_POINT 
	{ 
		SCARA_WAIT_POS1 =0,		//index0
		SCARA_WAIT_POS2,		//index1
		SCARA_CELL_REVERSE,		//index2
		SCARA_BUFFER_STAGE,		//index3
		SCARA_TRAY1_POS,		//index4
		SCARA_TRAY2_POS			//index5
	};

	enum SCARA_MOVE_TYPE
	{
		TYPE_SPOS_EPOS = 0,
		TYPE_W1_EPOS,
		TYPE_W2_EPOS,
		TYPE_SPOS_W1_W2_EPOS,
		TYPE_SPOS_W2_W1_EPOS
	};

	int m_nMoveType;
	PLC_INFO m_PLCInfo;

	CString m_strRcvBuf;
	CString m_strSndBuf;



	void SetConnected(BOOL bIsConnected)	
	{ 
		m_bIsConnected = bIsConnected; 
	}

	BOOL IsConnected()	
	{ 
		return m_bIsConnected; 
	}

	BOOL Open();
	void Close();

	//bit On 	//MelsecNetBitOn
	void Scara_BitOn(short devno);

	//bit Off	//MelsecNetBitOff
	void Scara_BitOff(short devno);

	//bit read	//MelsecNetBitRecv
	short Scara_ReadBit(short devno);

	//word read	//MelsecNetWordRecvBuf
	void Scara_ReadWord(short devno, short nSizebyByte, PSHORT pData);

	//word write	//MelsecNetWordSend
	CString Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
	
	void	MoveStart();
	void	SetEndPosition(SCARA_G_POINT ePoint);

	void	SetZOffset(int nOffset);				// 픽커가 올라오는 높이 설정. 
	void	SetTOffset(int nOffset);
	void	SetYOffset(int nMode, int nOffset);		// Tray나 Stage의 세로 방향의 offset값을 세팅한다. 
	void	SetXOffset(int nMode, int nOffset);		// Tray나 Stage의 가로 방향의 offset값을 세팅한다.  
	//void	SetPickerPosition();

	int	GetPosition();		// 현재 Scara 픽커의 위치를 알려준다 (Position Index)

	BOOL	CheckFinish();
	void	MoveFinish();

	int	MoveToWaitOnePoint(int nStartPoint, int nEndPoint);
	int	MoveToWaitTwoPoint(int nStartPoint, int nEndPoint);
	int	MoveToCellReverse(int nStartPoint, int nEndPoint);
	int	MoveToBufferStage(int nStartPoint, int nEndPoint);
	int	MoveToTrayOne(int nStartPoint, int nEndPoint);
	int	MoveToTrayTwo(int nStartPoint, int nEndPoint);
};
