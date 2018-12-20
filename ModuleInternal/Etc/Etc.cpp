#include "StdAfx.h"
#include "Etc.h"
#include <math.h>
#include <atlimage.h>	// CImage

// GetAdaptersInfo()
#include <ipHlpapi.h>	
#pragma comment(lib, "ipHlpapi.lib")

// GetIPAddress()
#include <NetCon.h>	

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CEtc::CEtc(void)
{
}

CEtc::~CEtc(void)
{
}

void CEtc::CaptureScreen( HWND hScreen, LPCTSTR szFileName )
{
	/*//////////////////////////////////////////////////////////////////////////
	// 원본 소스 
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HWND hDesktopWnd = GetDesktopWindow();
	HDC hDesktopDC = GetDC(hDesktopWnd);
	HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
		nScreenWidth, nScreenHeight);
	SelectObject(hCaptureDC,hCaptureBitmap); 
	BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight,
		hDesktopDC,0,0,SRCCOPY|CAPTUREBLT); 
	SaveCapturedBitmap(hCaptureBitmap); //Place holder - Put your code
	//here to save the captured image to disk
	ReleaseDC(hDesktopWnd,hDesktopDC);
	DeleteDC(hCaptureDC);
	DeleteObject(hCaptureBitmap);
	*///////////////////////////////////////////////////////////////////////////

	CRect rect;
	::GetWindowRect(hScreen, &rect);
	int nScreenWidth = rect.Width();
	int nScreenHeight = rect.Height();

	HDC hScreenDC = ::GetDC(hScreen);
	HDC hCaptureDC = ::CreateCompatibleDC(hScreenDC) ;
	HBITMAP hCaptureBitmap = ::CreateCompatibleBitmap(hScreenDC, nScreenWidth, nScreenHeight);
	::SelectObject(hCaptureDC,hCaptureBitmap); 
	::BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight,	hScreenDC,0,0,SRCCOPY); 

	if(hCaptureBitmap)
	{
		CImage image;
		image.Attach(hCaptureBitmap);
		image.Save(szFileName);
	}
	if(hScreenDC)	::ReleaseDC(hScreen,hScreenDC);
	if(hCaptureDC)	::DeleteDC(hCaptureDC);
	if(hCaptureBitmap)	::DeleteObject(hCaptureBitmap);
}

double CEtc::Round( double dValue, int nPos /*= 1*/ )
{ 
	// 소수점 nPos째 자리를 반올림 한다
	// Round(12.15923, 3)		return value = 12.16
	dValue*=pow(10.0, nPos-1); 
	dValue=(dValue>0) ?  floor(dValue+0.5) : ceil(dValue-0.5); 
	dValue*=pow(10.0, -(nPos-1)); 
	return dValue; 
	// 소숫점을 버리는 반올림 소스
	// 	return ((dValue>0) ? floor(dValue+.5) : ceil(dValue-.5)); 
} 
// 
double CEtc::Logbase(double dValue, double base)
{
	return log(dValue) / log(base);
}

void CEtc::CreateSleepTimeEvent()
{
	hSleepTimeEvent= CreateEvent(NULL, TRUE, FALSE, NULL);
}

void CEtc::SleepTime(DWORD dwMillisecond)
{
	WaitForSingleObject(hSleepTimeEvent, dwMillisecond);
}

void CEtc::CloseSleepTimeEvent()
{
	CloseHandle(hSleepTimeEvent);

	if(hSleepTimeEvent)
		hSleepTimeEvent = NULL;
}

BOOL CEtc::CheckPosition( double dDefaultPos, double dNowPos, double dCheckLimit /*= 0.05*/ )	/* 0.01mm이상 차이나면 틀리다고 간주 */
{
	if(max(dDefaultPos+dCheckLimit, dNowPos) == dNowPos)	return FALSE;
	if(min(dDefaultPos-dCheckLimit, dNowPos) == dNowPos)	return FALSE;

	return TRUE;
}


LPCTSTR CEtc::GetLocalIPAddress()
{
	char name[200];
	char* LocalHostName;
	if( gethostname( name, 200) == 0 )
	{
		PHOSTENT hostinfo = gethostbyname( name );

		if( hostinfo != NULL)
		{
			LocalHostName = inet_ntoa( *( struct in_addr* ) *hostinfo->h_addr_list );
			return (LPCTSTR)LocalHostName;
		}
	}
	return NULL;
}

LPCTSTR CEtc::GetDeviceIPAddress(LPCTSTR szDeviceName)
{
	DWORD dwStatus;

	IP_ADAPTER_INFO *pAdapterInfo = NULL;

	ULONG ulSizeAdapterInfo = 0;



	dwStatus = GetAdaptersInfo(pAdapterInfo,&ulSizeAdapterInfo);

	//***********************************************************************
	//버퍼 오버 플로우 일때 ulSizeAdapterInfo 크기로 메모리를 할당하고
	//다시 GetAdaptersInfo를 호출한다.
	if( dwStatus == ERROR_BUFFER_OVERFLOW)
	{
		pAdapterInfo = new IP_ADAPTER_INFO;
// 		if( !(pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulSizeAdapterInfo)) )
// 		{
// 			AfxMessageBox("Get Gateway Address Error");
// 		}

		dwStatus = GetAdaptersInfo(pAdapterInfo,&ulSizeAdapterInfo);
	}

	char Gateway_Addr[16];

	do
	{
#ifdef UNICODE
		wchar_t  wszConnectionName[MAX_ADAPTER_NAME_LENGTH + 4];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pAdapterInfo->AdapterName, strlen(pAdapterInfo->AdapterName), wszConnectionName, MAX_ADAPTER_NAME_LENGTH + 4);
		if(_tcscmp(wszConnectionName, szDeviceName) == 0)
#else
		if(_tcscmp(pAdapterInfo->AdapterName, szDeviceName) == 0)
#endif
		{
			for(int x=0; x<16; x++)
				Gateway_Addr[x] = pAdapterInfo->GatewayList.IpAddress.String[x];
		}

		pAdapterInfo = pAdapterInfo->Next;
	}
	while(pAdapterInfo);
	//	free(pAdapterInfo);
	delete pAdapterInfo;
#ifdef UNICODE
	wchar_t  wszBuf[255];
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Gateway_Addr, strlen(Gateway_Addr), wszBuf, 255);
	LPCTSTR szIPAddress = wszBuf;
#else
	LPCTSTR szIPAddress = Gateway_Addr;
#endif
	return szIPAddress;
}

HRESULT CEtc::GetConnectionIPAddress( LPCTSTR szConnectionName )
{
	HRESULT hr = E_FAIL;
	CoInitialize(NULL);
	INetConnectionManager *pNetConnectionManager = NULL;
	hr = CoCreateInstance(CLSID_ConnectionManager,
		NULL,
		CLSCTX_LOCAL_SERVER | CLSCTX_NO_CODE_DOWNLOAD,
		IID_INetConnectionManager,
		reinterpret_cast<LPVOID *>(&pNetConnectionManager) );
	if (SUCCEEDED(hr))
	{
		/*Get an enumurator for the set of connections on the system */
		IEnumNetConnection* pEnumNetConnection;
		pNetConnectionManager->EnumConnections(NCME_DEFAULT, &pEnumNetConnection);
		ULONG ulCount = 0;
		BOOL fFound = FALSE;
		hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		HRESULT hrT = S_OK;

		/*
		Enumurate through the list of adapters on the system and look for the one we want
		NOTE: To include per-user RAS connections in the list, you need to set the COM Proxy Blanket on all the interfaces. This is not needed for All-user RAS connections or LAN connections.
		*/
		do
		{
			NETCON_PROPERTIES* pProps = NULL;
			INetConnection *pConn;

			/*
			Find the next (or first connection)
			*/
			hrT = pEnumNetConnection->Next(1, &pConn, &ulCount);

			if (SUCCEEDED(hrT) && 1 == ulCount)
			{
				/*
				Get the connection properties
				*/
				hrT = pConn->GetProperties(&pProps);

				if (S_OK == hrT)
				{
					wchar_t  wszConnectionName[255];
					ZeroMemory(wszConnectionName, sizeof(wchar_t)*255);
#ifndef UNICODE
					MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szConnectionName, _tcslen(szConnectionName), wszConnectionName, 255);
					LPWSTR a = wszConnectionName;
					if(pProps->pszwName == a)
#else
					if(pProps->pszwName == szConnectionName)
#endif
					{
						GetDeviceIPAddress((LPCTSTR)pProps->pszwDeviceName);
					}
				}

				pConn->Release();
				pConn = NULL;
			}

		}
		while (SUCCEEDED(hrT) && 1 == ulCount && !fFound);

		if (FAILED(hrT))
		{
			hr = hrT;
		}

		pEnumNetConnection->Release();
	}

	if (FAILED(hr) && hr != HRESULT_FROM_WIN32(ERROR_RETRY))
	{
		printf("Could not enable or disable connection (0x%08x)\r\n", hr);
	}
	pNetConnectionManager->Release();
	CoUninitialize();
	return hr;
}

void CEtc::ConvertUnicode( char* szFrom, wchar_t* wszTo, int nToMaxLength )
{
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFrom, -1, wszTo, nToMaxLength);
}

void CEtc::ConvertUnicode( char* szFrom, int nFromLength, wchar_t* wszTo, int nToMaxLength )
{
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szFrom, nFromLength, wszTo, nToMaxLength);
}

void CEtc::ConvertMultibyte( wchar_t* wszFrom, char* szTo, int nToMaxLength )
{
	::WideCharToMultiByte(CP_ACP, 0, wszFrom, -1, szTo, nToMaxLength, 0, 0);
}

void CEtc::ConvertMultibyte( wchar_t* wszFrom, int nFromLength, char* szTo, int nToMaxLength )
{
	::WideCharToMultiByte(CP_ACP, 0, wszFrom, nFromLength, szTo, nToMaxLength, 0, 0);
}

void CEtc::ApplyTimeOffset( CTime &time, int nHour, int nMin )
{
	// 하루를 07시 기준으로 끊거나 할 때 사용하는 함수 [3/15/2014 OSC]
	CTimeSpan dayOffset(1, 0, 0, 0);
	if (time.GetHour() == nHour)
	{
		if(time.GetMinute() < nMin)
		{
			time -= dayOffset; 
		}
	}
	else if(time.GetHour() < nHour)
	{
		time -= dayOffset; 
	}
}

void CEtc::CircleCenter( CdblPoint A, CdblPoint B, CdblPoint C, CdblPoint& ptCenter )
{
	double bax = B.x-A.x;
	double bay = B.y-A.y;
	double cax = C.x-A.x;
	double cay = C.y-A.y;

	double E = bax*(A.x+B.x) + bay*(A.y+B.y);
	double F = cax*(A.x+C.x) + cay*(A.y+C.y);
	double G = 2.*(bax*(C.y-B.y)-bay*(C.x-B.x));

	ptCenter.x = (cay*E - bay*F)/G;
	ptCenter.y = (bax*F - cax*E)/G;
}
