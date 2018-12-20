#include "stdafx.h"
#include "CQueue.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
void CQueue::Clear()
{
	m_iHead = m_iTail = 0;
	memset(buff,0,SERIAL_MAX_BUFFER);
}

CQueue::CQueue()
{
	Clear();
}

int CQueue::GetSize()
{
	return (m_iHead - m_iTail + SERIAL_MAX_BUFFER) % SERIAL_MAX_BUFFER;
}

BOOL CQueue::PutByte(BYTE b)
{
	if(GetSize() == (SERIAL_MAX_BUFFER-1)) return FALSE;
	buff[m_iHead++] = b;
	m_iHead %= SERIAL_MAX_BUFFER;
	return TRUE;
}

BOOL CQueue::GetByte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	*pb = buff[m_iTail++];
	m_iTail %= SERIAL_MAX_BUFFER;
	return TRUE;
}

BOOL CQueue::GetWord(short *pb)
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

BOOL CQueue::Get8Byte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	if(GetSize() < 8) return FALSE;

	memcpy(pb, &buff[m_iTail], 8);
//	*pb = buff[m_iTail];

	m_iTail+=8;
	m_iTail %= SERIAL_MAX_BUFFER;
	return TRUE;
}

BOOL CQueue::Get12Byte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	if(GetSize() < 12) return FALSE;

	memcpy(pb, &buff[m_iTail], 12);
	//	*pb = buff[m_iTail];

	m_iTail+=12;
	m_iTail %= SERIAL_MAX_BUFFER;
	return TRUE;
}


int CQueue::GetData(BYTE* pByte, int nSize)
{
	if(GetSize() == 0) return FALSE;
	if(GetSize() < nSize) return FALSE;

	memcpy(pByte, &buff[m_iTail], nSize);
	//	*pb = buff[m_iTail];

	m_iTail+=nSize;
	m_iTail %= SERIAL_MAX_BUFFER;

	return 0;
}

int CQueue::GetSTXPos()
{
	int nSize = GetSize();
	if(nSize == 0) return -1;

	int nIndex = m_iTail;
	for (int i=0; i<nSize; i++)
	{
		if(buff[nIndex] == STX)
			return nIndex;
		nIndex++;
		nIndex %= SERIAL_MAX_BUFFER;
	}

	return -1;
}

int CQueue::GetETXPos()
{
	int nSize = GetSize();
	if(nSize == 0) return -1;

	int nIndex = m_iTail;
	for (int i=0; i<nSize; i++)
	{
		if(buff[nIndex] == ETX)
			return nIndex;
		nIndex++;
		nIndex %= SERIAL_MAX_BUFFER;
	}

	return -1;
}

int CQueue::GetSizeSTX_To_ETX()
{
	int nStxPos = GetSTXPos();
	int nEtxPos = GetETXPos();
	if( (nStxPos == -1) || (nEtxPos == -1) )
		return -1;

	return ( (nEtxPos+1) - nStxPos + SERIAL_MAX_BUFFER) % SERIAL_MAX_BUFFER;
}

int CQueue::GetDataSTX_To_ETX( BYTE *pb, int nMaxBuffer )
{
	int nSize = GetSizeSTX_To_ETX();
	if(nSize <= 0)	return -1;

	BYTE tempbuf[SERIAL_MAX_BUFFER];

	if(m_iHead < m_iTail)
	{
		int nSize1, nSize2;
		nSize1 = SERIAL_MAX_BUFFER - m_iTail;
		memcpy(&tempbuf[0], &buff[m_iTail], nSize1);
		nSize2 = m_iHead+1;
		memcpy(&tempbuf[nSize1], &buff[0], nSize2);
	}
	else
	{
		memcpy(&tempbuf[0], &buff[m_iTail], nSize);
	}

	memcpy(pb, tempbuf, min(nSize, nMaxBuffer));

	m_iTail+=nSize;
	m_iTail %= SERIAL_MAX_BUFFER;

	return (nSize <= nMaxBuffer) ? nSize:nMaxBuffer;
}
