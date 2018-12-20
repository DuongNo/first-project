#include "stdafx.h"
#include "MelsecNet.h"
#include "../../ModuleExternal/MELSEC/INCLUDE/Mdfunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMelsecNet::CMelsecNet()
{
}

CMelsecNet::~CMelsecNet()
{
}

BEGIN_MESSAGE_MAP(CMelsecNet, CWnd)
END_MESSAGE_MAP()


short CMelsecNet::MelsecOpen(short chan, short mode, LPLONG path,int nIndex)
{
	m_shortChannel[nIndex]			= chan;
	m_shortMode[nIndex]			= 1;
	m_shortStationNumber[nIndex]	= 255;
	//m_longPath[nIndex]				= *path;
	mode  =-1;
	short ret = mdOpen(chan, mode, &m_longPath[nIndex]);

	return ret;
}

short CMelsecNet::MelsecClose(long path)
{
	short ret = mdClose(path);

	return ret;
}

short CMelsecNet::MelsecDevSet(short numNetNo, short numStationNo, short devtyp, short devno,int nIndex)
{

	//Bit on
	short ret = mdDevSet(m_longPath[nIndex], numStationNo, devtyp, devno);
//	short ret = mdDevSetEx(m_longPath[nIndex],(long)numNetNo,(long) numStationNo, (long)devtyp, (long)devno);

	if(ret)		
	{
		CString strMsg;
		strMsg.Format(_T("Check Melsec(MelsecDevSet Error) Code = %d,"), ret);
	}

	return ret;
}

short CMelsecNet::MelsecDevRst(short numNetNo, short numStationNo, short devtyp, short devno,int nIndex)
{
	//Bit off
	short ret = mdDevRst(m_longPath[nIndex], numStationNo, devtyp, devno);

	if(ret)		
	{
		CString strMsg;
		strMsg.Format(_T("Check Melsec(mdDevRst Error) Code = %d,"), ret);
	}

	return ret;
}

short CMelsecNet::MelsecDevReceive(short numNetNo, short numStationNo, short devtyp, short devno, int* data,int nIndex)
{
	//Bit Read(Receive)
	short devno2, data2, ibit, temp;
	devno2 = data2 = ibit = temp =0;

	if(devno>7)
	{
		devno2 = short(devno/8) * 8;
	}
	else
	{
		devno2 = 0;
	}

	short size = 1;	//This fn. is received by bit, X, Y etc....(so, size is only 1)

	short ret = MelsecReceive(numNetNo, numStationNo, devtyp, devno2, &size, &data2, nIndex); //for pointer variable.. 

	ibit = (devno % 8) +1;

	switch(ibit)
	{
		case 1:	temp = 0x01; break;
		case 2:	temp = 0x02; break;
		case 3:	temp = 0x04; break;
		case 4:	temp = 0x08; break;
		case 5:	temp = 0x10; break;
		case 6:	temp = 0x20; break;
		case 7:	temp = 0x40; break;
		default:temp = 0x80; break;
	}

	int flags = data2 & temp;
	if(flags)
	{ 
		if(nIndex == ROBOT)
		{
			//theLog[LOG_SEQUENCE].AddBuf( _T("TRUE: DevNo %d - Data2 %d, flags %d"), devno,data2,flags);
		}
		flags = 1;		
	}
	else
	{
		if(nIndex == ROBOT)
		{
			//--theLog[LOG_SEQUENCE].AddBuf( _T("FALSE: DevNo %d - Data2 %d, flags %d"), devno,data2,flags);
		}
		flags = 0;
	}
	*data = flags;

	return ret;
}
	
short CMelsecNet::MelsecReceive(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data,int nIndex)
{
	//Word Read
	short ret = mdReceive(m_longPath[nIndex], numStationNo, devtyp, devno, size, data);

	return ret;
}

short CMelsecNet::MelsecSend(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data,int nIndex)
{
	//Word write
	short ret = mdSend(m_longPath[nIndex], numStationNo, devtyp, devno, size, data);

	return ret;
}
