//
// 통신에 사용되는 각종 상수 및 데이터 처리기 
//
//
#pragma once

#include <afxsock.h>

#include "Gaus.h"
#include "GxDataStructure.h"

//=============================================================================
// COMMON Constance
//=============================================================================

#define ASCII_NUL	0x00	// null
#define ASCII_SOH	0x01	// start of heading
#define ASCII_STX	0x02	// start of text
#define ASCII_ETX	0x03	// end of text
#define ASCII_EOT	0x04	// end of transmission
#define ASCII_ENQ	0x05	// enquiry
#define ASCII_ACK	0x06	// acknowledge
#define ASCII_BEL	0x07	// bell
#define ASCII_BS	0x08	// backspace
#define ASCII_TAB	0x09	// horizontal tab
#define ASCII_LF	0x0A	// NL line feed, new line
#define ASCII_VT	0x0B	// vertical tab
#define ASCII_FF	0x0C	// NP form feed, new page
#define ASCII_CR	0x0D	// carriage return
#define ASCII_SO	0x0E	// shift out
#define ASCII_SI	0x0F	// shift in
#define ASCII_DLE	0x10	// data link escape
#define ASCII_XON	0x11	// device control 1
#define ASCII_DC2	0x12	// device control 2
#define ASCII_XOFF	0x13	// device control 3
#define ASCII_DC4	0x14	// device control 4
#define ASCII_NAK	0x15	// negative acknowledge
#define ASCII_SYN	0x16	// synchronous idle
#define ASCII_ETB	0x17	// end of trans block
#define ASCII_CAN	0x18	// cancel
#define ASCII_EM	0x19	// end of medium
#define ASCII_SUB	0x1A	// substitute
#define ASCII_ESC	0x1B	// escape
#define ASCII_FS	0x1C	// file separator
#define ASCII_GS	0x1D	// group separator
#define ASCII_RS	0x1E	// record separator
#define ASCII_US	0x1F	// unit separator


// 통신시 발생하는 Event
#define GxCOMM_EVENT_CONNECT		1				// 연결
#define GxCOMM_EVENT_CLOSE			2				// 연결 종료
#define GxCOMM_EVENT_RECEIVE		3				// 데이터 수신
#define GxCOMM_EVENT_ACCEPT		4				// Accept 요청 발생 (Server)


// 통신시 발생하는 Message
#define GxCOMM_MSG_BASE				(WM_USER + 8500)
#define GxCOMM_MSG_CONNECT			(GxCOMM_MSG_BASE + 1)
#define GxCOMM_MSG_CLOSE			(GxCOMM_MSG_BASE + 2)
#define GxCOMM_MSG_RECEIVE			(GxCOMM_MSG_BASE + 3)
#define GxCOMM_MSG_ACCEPT			(GxCOMM_MSG_BASE + 4)



#define GxCOMM_MAX_BUFFER			4096			// 수신 버퍼 최대 크기
#define GxCOMM_MAX_QUEUE			8192			// 수신 데이터 큐 최대 크기
#define GxCOMM_LEN_SIZE				4				// 기본적인 패킷 크기 정보 길이, 4 BYTE, eGxCOMM_PKT_SIZE_A, eGxCOMM_PKT_SIZE_B 일때 

// 수신 데이터의 패킷을 구분하는 방법, CGxCommQueue에서 사용
enum {
	eGxCOMM_PKT_NONE	= 0,		// 수신된 데이터 그대로를 패킷으로 인정한다.
	eGxCOMM_PKT_COUNT,				// 지정한 데이터 수신 수량으로 패킷의 기준을 삼는다.
	eGxCOMM_PKT_TOKEN,				// 지정한 데이터를 패킷의 기준으로 삼는다.
	eGxCOMM_PKT_SIZE_AD,				// 가변길이, 선두 길이 정보가 ASCII 10진수 형식인 패킷   '0010' -> 길이 10 Bytes
	eGxCOMM_PKT_SIZE_AH,				// 가변길이, 선두 길이 정보가 ASCII 16진수 형식인 패킷   '0010' -> 길이 16 Bytes
	eGxCOMM_PKT_SIZE_B,				// 가변길이, 선두 길이 정보가 Binary 데이터인 패킷 0x0000000a -> 길이 10 Bytes
};


// 오류 감지 함수 ( 전역 함수 )
static BYTE CRC8_Packet1(BYTE* packet, int size);
static BYTE CRC8_Packet2(BYTE* packet, int size);
static BYTE BCC_Packet(BYTE* packet, int size);
static WORD CRC16_Packet1(BYTE *packet, int size);
static WORD CRC16_Packet2(BYTE *packet, int size);
static WORD CRC16_CCITT(BYTE *packet, int size );			// for SanKyo Motor, 2015-11-05, jhLee


//=============================================================================
// COMMUNICATION QUEUE
//=============================================================================

class _GAUS_LIB CGxCommQueue : public CGxQueue <BYTE, BYTE>
{
protected:

	// UINT m_nMode : 패킷을 구분하는 방법
	//	eGxCOMM_PKT_NONE	= 0,		// 수신된 데이터 그대로를 패킷으로 인정한다.
	//	eGxCOMM_PKT_COUNT,				// 지정한 데이터 수신 수량으로 패킷의 기준을 삼는다.
	//	eGxCOMM_PKT_TOKEN,				// 지정한 데이터를 패킷의 기준으로 삼는다.
	//	eGxCOMM_PKT_SIZE_A,				// 가변길이, 선두 길이 정보가 ASCII 형식인 패킷   '0010' -> 길이 10 Bytes
	//	eGxCOMM_PKT_SIZE_B,				// 가변길이, 선두 길이 정보가 Binary 데이터인 패킷 0x0000000a -> 길이 10 Bytes
	//
	// CByteArray* m_arrToken : eGxCOMM_PKT_TOKEN 모든인 경우 토큰 문자들 배열 포인터
	//
	// int m_nIndex  = 기준 위치
	//			eGxCOMM_PKT_TOKEN : BCC/CRC가 토큰 문자 기준으로 어느 위치에 존재하는가 ?
	//								0 : BCC/CRC가 존재하지 않는다.								
	//								음수 : 토큰 문자 이전에 값이 온다.
	//								양수 : 토큰 문자 이후에 값이 온다.
	//			eGxCOMM_PKT_SIZE_A, eGxCOMM_PKT_SIZE_A : 길이 정보의 시작 위치
	//
	// UINT m_nLen  = 정보 길이
	//			eGxCOMM_PKT_COUNT : 패킷으로 인정할 데이터의 길이
	//			eGxCOMM_PKT_TOKEN : BCC/CRC 의 길이
	//			eGxCOMM_PKT_SIZE_A, eGxCOMM_PKT_SIZE_A : 길이 정보의 크기
	//

	UINT			m_nMode;				// 패킷 구분 모드
	CByteArray	m_arrToken;			// 토큰 정보
	int			m_nIndex;			// BCC/CRC or Size Info Block 의 pos
	UINT			m_nLen;				// BCC/CRC or Size Info Block 의 size, 길이 지정 패킷의 길이

public:
	CGxCommQueue();
	~CGxCommQueue();
	
	UINT PutData(BYTE* pbyData, int nSize);
	UINT GetData(BYTE* pbyData, int nSize);

	void Clear();
	BOOL IsEmpty();
	BOOL IsFull();

	UINT GetPacketMode(int& nIdx, int& nLen);						// 패킷 구분자 모드 및 정보
	UINT IsPacket();
	UINT GetPacket(BYTE* pbyData);
	UINT GetPacket(BYTE* pbyData, UINT nLen);

	void SetPacketInfo(UINT nMode, CByteArray* parrToken=NULL, int nIndex=0, UINT nLen=GxCOMM_LEN_SIZE);		// 패킷 정보 지정

	// eGxCOMM_PKT_SIZE_A, eGxCOMM_PKT_SIZE_B의 적용을 위한 Utility 함수
	static void SetAsciiValueDec(BYTE* pArray, UINT uSize, int nLen);			// 지정 크기를 해당 Byte 배열에 ASCII 10진수 형태로 저장한다.
	static void SetAsciiValueHex(BYTE* pArray, UINT uSize, int nLen);			// 지정 크기를 해당 Byte 배열에 ASCII 16진수 형태로 저장한다.
	static void SetBinaryValue(BYTE* pArray, UINT uSize, int nLen);			// 지정 크기를 해당 Byte 배열에 Binary 형태로 저정한다.
	static UINT GetAsciiValueDec(BYTE* pArray, int nLen);							// 지정 Byte 배열에서 Ascii 형태의 수치형 값을 얻는다.
	static UINT GetAsciiValueHex(BYTE* pArray, int nLen);							// 지정 Byte 배열에서 Ascii 16진수 형태의 수치형 값을 얻는다.
	static UINT GetBinaryValue(BYTE* pArray, int nLen);							// 지정 Byte 배열에서 Binay 형태의 수치형 값을 얻는다.
};
