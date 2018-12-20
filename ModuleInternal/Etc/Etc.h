//////////////////////////////////////////////////////////////////////////
// 이것 저것 잡다한 함수 모아놓은 class
//
// static void CircleCenter() 함수 추가 [2/24/2016 OSC]
// 
// static LPCTSTR GetLocalIpAddress() 함수 추가 [4/9/2013 OSC]
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "dblPoint.h"

static HANDLE hSleepTimeEvent = NULL;

class CEtc
{
public:
	CEtc(void);
	~CEtc(void);

	//////////////////////////////////////////////////////////////////////////
	// 특정 컨트롤 영역 Capture 함수(Screen Capture 응용)
	// 파일 확장명에 따라 자동으로 File Format 변경됨
	// *.bmp(BMP), *.png(PNG), *.jpg(JPEG), *.gif(GIF)
	// ex) CEtc::CaptureScreen(this.m_hWnd, _T("D:\\a.jpg"));
	static void CaptureScreen(HWND hScreen, LPCTSTR szFileName );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 반올림 함수
	// 소수점 nPos째 자리를 반올림 한다
	//#define round(dTemp) (long(dTemp+(dTemp>0? .5:-.5)))
	static double Round(double dValue, int nPos = 1);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 로그함수	
	// base가 몇승일 때 dValue가 되는지 승을 알아오는 함수
	static double Logbase(double dValue, double base);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Sleep() 대용 함수
	// 검증용
	// CreateSleepTimeEvent();로 이벤트 선언후
	// 실제 Sleep()을 쓸 곳에 SleepTime()을 대신 사용
	// 이후 ClCloseSleepTimeEvent()로 이벤트를 종료한다.
	static void CreateSleepTimeEvent();
	static void SleepTime(DWORD dwMillisecond);
	static void CloseSleepTimeEvent();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Encoder Position Check용 함수
	static BOOL CheckPosition(double dDefaultPos, double dNowPos, double dCheckLimit = 0.05); /* ±0.01mm이상 차이나면 틀리다고 간주 */
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Local IP Address 얻어오는 함수
	static LPCTSTR GetLocalIPAddress();
	//////////////////////////////////////////////////////////////////////////

	static LPCTSTR GetDeviceIPAddress(LPCTSTR szDeviceName);
	static HRESULT GetConnectionIPAddress(LPCTSTR szConnectionName);

	//////////////////////////////////////////////////////////////////////////
	// Unicode, Multibyte 변환
	static void ConvertUnicode( char* szFrom, wchar_t* wszTo, int nToMaxLength );
	static void ConvertMultibyte( wchar_t* wszFrom, char* szTo, int nToMaxLength );
	// 혹시 위에꺼로 처리 못하는게 있다면 아래꺼로 해보도록
	// wchar Length는 whcar기준으로 측정된 길이어야 됨. 아마도...
	static void ConvertUnicode( char* szFrom, int nFromLength, wchar_t* wszTo, int nToMaxLength );
	static void ConvertMultibyte( wchar_t* wszFrom, int nFromLength, char* szTo, int nToMaxLength );
	//////////////////////////////////////////////////////////////////////////

	// 하루를 07시 기준으로 끊거나 할 때 사용하는 함수 [3/15/2014 OSC]
	static void ApplyTimeOffset( CTime &time, int nHour, int nMin );

	//////////////////////////////////////////////////////////////////////////
	// 3개 Point의 중심점 구하는 함수 [2/24/2016 OSC]
	static void CircleCenter(CdblPoint A, CdblPoint B, CdblPoint C, CdblPoint& ptCenter);

};

