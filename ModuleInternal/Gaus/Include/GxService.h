#pragma once

#include "Gaus.h"
#include "GxTask.h"
#include "GxProfile.h"

class _GAUS_LIB CGxService : public CGxTask
{
protected:
	UINT ThreadRun1();
	UINT ThreadRun2();
	UINT ThreadRun3();

	// 기능 수행 함수
	virtual void PreServiceRun()	{};			// ServiceRun()을 최초로 수행하기 전에 동작
	virtual void ServiceRun()		= 0;			// 일정하게 Tread 수행하는 반복 호출 함수
	virtual void PostServiceRun()	{};			// ServiceRun() 수행을 마치고 종료하는 시점에 동작

public:
	CGxService(void);
	virtual ~CGxService(void);
	virtual void SetBasePath(CString sPath);							// 기본 경로를 설정하여 파일 경로를 갱신한다.
	
	//  Service 제어
	virtual BOOL Startup(void);											// 기동 시킨다.
	virtual void Shutdown(void);											// 종료처리한다.

	virtual BOOL InitProfile(void);
	virtual BOOL LoadProfile(void);
	virtual BOOL SaveProfile(void);
};

typedef CArray<CGxService, CGxService&> CGxServiceArray;
typedef CTypedPtrArray<CPtrArray, CGxService*> CGxServicePtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxService*> CGxMapStringToService;

