#include "stdafx.h"
#include "GxIDReaderWrapper.h"

#import "GxIDReader.tlb" no_namespace named_guids

CGxIDReaderWrapper::CGxIDReaderWrapper(void)
{
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_GxIDReaderImp, 
								  NULL, 
								  CLSCTX_INPROC_SERVER, 
								  IID_IGxIDReader, 
								  reinterpret_cast<void**>(&mp_CallClass));
	if(FAILED(hr))
	{
		m_isSucceeded = false;
// 		CoUninitialize();
	}
	else
	{
		m_isSucceeded = true;
		OutputDebugString( _T("Succeeded CallClass.\n") );
	}
}

CGxIDReaderWrapper::~CGxIDReaderWrapper(void)
{
	CoUninitialize();
}

// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==

// 각종 이벤트 회신용 설정
void CGxIDReaderWrapper::SetEventHandle(HWND hOwner, int nConnect, int nResult, int nLive)
{
	if(NULL != mp_CallClass)
	{
		mp_CallClass->SetEventHandle((long)hOwner, nConnect, nResult, nLive);
	}

}

// 접속할 IP 주소 대입
void CGxIDReaderWrapper::SetAddress(CString sAddr)
{
	if(NULL != mp_CallClass)
	{
		
		_bstr_t bStrAddr = (LPCTSTR)sAddr;
		mp_CallClass->SetAddress( bStrAddr );
	}
}

// 설정된 주소 조회
CString CGxIDReaderWrapper::GetAddress()
{
	if(NULL != mp_CallClass)
	{
		// _bstr_t = mp_CallClass->GetAddress();
		return (LPCTSTR)mp_CallClass->GetAddress();
	}

	return _T("");
}


// 라이브 이미지 저장 이름
void CGxIDReaderWrapper::SetLiveImageName(CString sName)
{
	if(NULL != mp_CallClass)
	{
		
		_bstr_t bStrName = (LPCTSTR)sName;
		mp_CallClass->SetLiveImageName( bStrName );
	}
}

// 현재 결과 이미지를 저장한다.
void CGxIDReaderWrapper::SaveImage(CString sName)
{
	if(NULL != mp_CallClass)
	{		
		_bstr_t bStrName = (LPCTSTR)sName;
		mp_CallClass->SaveImage( bStrName );
	}
}



void CGxIDReaderWrapper::Connect()
{
	if(NULL != mp_CallClass)
	{
		mp_CallClass->Connect();
	}
}


void CGxIDReaderWrapper::DisConnect()
{
	if(NULL != mp_CallClass)
	{
		mp_CallClass->DisConnect();
	}

}

void CGxIDReaderWrapper::Trigger()
{
	if(NULL != mp_CallClass)
	{
		mp_CallClass->Trigger();
	}

}

BOOL CGxIDReaderWrapper::IsConnect()
{
	if(NULL != mp_CallClass)
	{
		return mp_CallClass->IsConnect();
	}

	return FALSE;
}

void CGxIDReaderWrapper::LiveOn()
{
	if(NULL != mp_CallClass)
	{
		mp_CallClass->LiveOn();
	}

}

void CGxIDReaderWrapper::LiveOff()
{
	if(NULL != mp_CallClass)
	{
		mp_CallClass->LiveOff();
	}

}

BOOL CGxIDReaderWrapper::IsLiveOn()
{
	if(NULL != mp_CallClass)
	{
		return mp_CallClass->IsLiveOn();
	}

	return FALSE;
}


// 리딩 결과 조회
int CGxIDReaderWrapper::GetReadResult()
{
	if(NULL != mp_CallClass)
	{
		return mp_CallClass->GetReadResult();
	}

	return 0;
}

// 결과 데이터 조회
CString CGxIDReaderWrapper::GetReadString()
{
	if(NULL != mp_CallClass)
	{
		return (LPCTSTR)mp_CallClass->GetReadString();
	}

	return _T("");	
}

// 실패시 표기되는 메세지 지정
void CGxIDReaderWrapper::SetFailMessage(CString sMsg)
{
	if(NULL != mp_CallClass)
	{
		_bstr_t bStrMsg = (LPCTSTR)sMsg;

		mp_CallClass->SetFailMessage( bStrMsg );
	}

}



// == == == == == == == == == == == == == == == == == == == == == == == == == == == == == ==



void CGxIDReaderWrapper::ShowVersion()
{
	if(NULL != mp_CallClass)
	{
		mp_CallClass->ShowVersion();
	}
}
//
//int CGxIDReaderWrapper::Add(int a, int b)
//{
//	if(NULL != mp_CallClass)
//	{
//		return mp_CallClass->Add(a, b);
//	}
//	return 0;
//}
//
//void CGxIDReaderWrapper::ShowForm()
//{
//	if(NULL != mp_CallClass)
//	{
//		mp_CallClass->ShowForm();
//	}
//}
//
//
//// 신규 함수 추가
//void CGxIDReaderWrapper::NewFuncCall(CString sName)
//{
//	if(NULL != mp_CallClass)
//	{
//		_bstr_t bstrLabel = (LPCTSTR)sName;
//
//		mp_CallClass->NewFunc( bstrLabel );
//	}
//}
