
#pragma once

//#define WM_COMM_READ (WM_USER +1)
#define SERIAL_MAX_BUFFER	6500


//	통신 클래스	CCommThread 

// 포트에서 읽기 :
//   포트를 연 후에 포트에 자료가 도착하면 WM_COMM_READ 메시지가 메인 
//   윈도우에 전달된다. ON_MESSAGE 매크로를 이용, 함수를 연결하고
//   m_ReadData String에 저장된 데이터를 이용 읽기
// 포트에 쓰기 : WriteComm(buff, 30)과 같이 버퍼와 그 크기를 건네면 된다.
class CQueue
{
public:
	BYTE buff[SERIAL_MAX_BUFFER];//큐 버퍼
	int m_iHead, m_iTail;//큐 버퍼에 데이터를 넣고 빼 올때 사용할 변수 
	CQueue();
	void Clear();//버퍼를 초기화 시킴
	int GetSize();//현재 버퍼에 들어있는 데이터의 size를 리턴
	BOOL PutByte(BYTE b);//큐버퍼에 1바이트를 넣음
	BOOL GetByte(BYTE *pb);//큐버퍼에서 1바이트를 빼 옴
	BOOL GetWord(short *pb);
	BOOL CQueue::Get8Byte(BYTE *pb);
	BOOL CQueue::Get12Byte(BYTE *pb);
	int GetData(BYTE* pByte, int nSize);

	int GetSTXPos();
	int GetETXPos();
	int GetSizeSTX_To_ETX();
	int GetDataSTX_To_ETX(BYTE *pb, int nMaxBuffer);	// STX부터 ETX까지 데이터 빼옴.

};

