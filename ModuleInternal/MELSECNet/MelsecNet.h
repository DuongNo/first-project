#pragma once

enum DEVICE_TYPE_DEFINE 
{
	DEV_TYP_READ	= 1,	
	DEV_TYP_WRIT	= 2,	
	DEV_TYP_OPTICAL_BIT		= 23,
	DEV_TYP_OPTICAL_WORD	= 24,
	DEV_TYP_CCLINK_WORDW	= 36,	
	DEV_TYP_CCLINK_WORDR	= 37
};

class CMelsecNet : public CWnd
{
public:
	CMelsecNet();
	virtual ~CMelsecNet();

	DECLARE_MESSAGE_MAP()

	short MelsecOpen(short chan, short mode, LPLONG path,int nIndex);
	short MelsecClose(long path);

	//Bit on
	//short mnetDevSet(short numNetNo, short numStationNo, short devtyp, short devno);
	short MelsecDevSet(short numNetNo,short numStationNo, short devtyp, short devno,int nIndex);

	//Bit off
	//short mnetDevRst(short numNetNo, short numStationNo, short devtyp, short devno);
	short MelsecDevRst(short numNetNo,short numStationNo, short devtyp, short devno,int nIndex);

	//Bit Read(Receive)
	//short mnetDevReceive(short numNetNo, short numStationNo, short devtyp, short devno, int* data);
	short MelsecDevReceive(short numNetNo, short numStationNo, short devtyp, short devno, int* data,int nIndex);
	
	//Word Read
	//short mnetReceive(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data);
	short MelsecReceive(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data,int nIndex);

	//Word write
	//short mnetSend(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data);
	short MelsecSend(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data,int nIndex);

	short	m_shortChannel[2];			//slot number.
	short	m_shortMode[2];			//on/off line Mode.
	short	m_shortStationNumber[2];	//station number
	long	m_longPath[2];				//board address.
};