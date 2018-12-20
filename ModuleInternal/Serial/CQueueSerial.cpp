#include "stdafx.h"
#include "CQueueSerial.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
void CQueueSerial::Clear()
{
	m_iHead = m_iTail = 0;
	memset(buff,0,SERIAL_MAX_BUFFER);
}

CQueueSerial::CQueueSerial()
{
	Clear();
	m_cStart = STX;
	m_cEnd = ETX;
}

int CQueueSerial::GetSize()
{
	return (m_iHead - m_iTail + SERIAL_MAX_BUFFER) % SERIAL_MAX_BUFFER;
}

BOOL CQueueSerial::PutByte(BYTE b)
{
	if(GetSize() == (SERIAL_MAX_BUFFER-1)) return FALSE;
	buff[m_iHead++] = b;
	m_iHead %= SERIAL_MAX_BUFFER;
	return TRUE;
}

BOOL CQueueSerial::GetByte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	*pb = buff[m_iTail++];
	m_iTail %= SERIAL_MAX_BUFFER;
	return TRUE;
}

BOOL CQueueSerial::GetWord(short *pb)
{
	if(GetSize() < 2) return FALSE;
	
	if( m_iTail+1 >= SERIAL_MAX_BUFFER)
	{
		BYTE TempBuff[2];

		memcpy(TempBuff, &buff[m_iTail], 1);
		memcpy(&TempBuff[1], &buff[0], 1);
		memcpy(pb, TempBuff, 2);
	}
	else
		memcpy(pb, &buff[m_iTail], 2);
	
	m_iTail+=2;
	m_iTail %= SERIAL_MAX_BUFFER;

	*pb = ((*pb >> 8) & 0x00ff) | ((*pb << 8) & 0xFF00);
	
	return TRUE;
}

BOOL CQueueSerial::Get8Byte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	if(GetSize() < 8) return FALSE;

	memcpy(pb, &buff[m_iTail], 8);
//	*pb = buff[m_iTail];

	m_iTail+=8;
	m_iTail %= SERIAL_MAX_BUFFER;
	return TRUE;
}

BOOL CQueueSerial::Get12Byte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	if(GetSize() < 12) return FALSE;

	memcpy(pb, &buff[m_iTail], 12);
	//	*pb = buff[m_iTail];

	m_iTail+=12;
	m_iTail %= SERIAL_MAX_BUFFER;
	return TRUE;
}


int CQueueSerial::GetData(BYTE* pByte, int nSize)
{
	if(GetSize() == 0) return FALSE;
	if(GetSize() < nSize) return FALSE;

	memcpy(pByte, &buff[m_iTail], nSize);
	//	*pb = buff[m_iTail];

	m_iTail+=nSize;
	m_iTail %= SERIAL_MAX_BUFFER;

	return 0;
}

int CQueueSerial::GetSTXPos()
{
	int nSize = GetSize();
	if(nSize == 0) return -1;

	int nIndex = m_iTail;
	for (int i=0; i<nSize; i++)
	{
		if(buff[nIndex] == m_cStart)
			return nIndex;
		nIndex++;
		nIndex %= SERIAL_MAX_BUFFER;
	}

	return -1;
}

int CQueueSerial::GetETXPos()
{
	int nSize = GetSize();
	if(nSize == 0) return -1;

	int nIndex = m_iTail;
	int nCharPos = -1;
	for (int i=0; i<nSize; i++)
	{
		if(nCharPos < 0)
		{
			if(buff[nIndex] == m_cEnd)
				nCharPos = nIndex;
		}
		else
		{
			// Checksum 등으로 EndChar가 연속으로 나올경우 마지막 꺼를 선택한다. [12/8/2016 OSC]
			if(buff[nIndex] == m_cEnd)
				nCharPos = nIndex;
			else
				return nCharPos;
		}
		nIndex++;
		nIndex %= SERIAL_MAX_BUFFER;
	}

	return -1;
}

int CQueueSerial::GetDataSTX_To_ETX()
{
	int nStxPos = GetSTXPos();
	int nEtxPos = GetETXPos();
	if( (nStxPos == -1) || (nEtxPos == -1) )
		return -1;

	int nSize = ( (nEtxPos+1) - nStxPos + SERIAL_MAX_BUFFER) % SERIAL_MAX_BUFFER;
	// Size가 0이면 초기화해버린다 [12/8/2016 OSC]
	if(nSize == 0)	Clear();
	return nSize;
}

int CQueueSerial::GetDataSTX_To_ETX( BYTE *pb, int nMaxBuffer )
{
	int nSize = GetDataSTX_To_ETX();
	if(nSize <= 0)	return -1;
	int nStxPos = GetSTXPos();
	int nEtxPos = GetETXPos();

	BYTE tempbuf[SERIAL_MAX_BUFFER];
	memset(tempbuf, NULL, SERIAL_MAX_BUFFER);

	if(nEtxPos < nStxPos)
	{
		int nSize1, nSize2;
		nSize1 = SERIAL_MAX_BUFFER - nStxPos;
		memcpy(&tempbuf[0], &buff[nStxPos], nSize1);
		nSize2 = nEtxPos+1;
		memcpy(&tempbuf[nSize1], &buff[0], nSize2);
	}
	else
	{
		memcpy(&tempbuf[0], &buff[nStxPos], nSize);
	}

	memcpy(pb, tempbuf, min(nSize, nMaxBuffer));

	m_iTail = nEtxPos+1;
	m_iTail %= SERIAL_MAX_BUFFER;

	return (nSize <= nMaxBuffer) ? nSize:nMaxBuffer;
}
void CQueueSerial::SetSTX(char cStart) 
{
	m_cStart = cStart;
}
void CQueueSerial::SetETX(char cEnd) 
{
	m_cEnd = cEnd;
}