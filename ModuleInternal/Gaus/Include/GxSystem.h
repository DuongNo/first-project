#pragma once

#include "Gaus.h"
#include "GxTask.h"
#include "GxProfile.h"

class _GAUS_LIB CGxSystem :	public CGxTask
{
protected:
	UINT ThreadRun1();
	UINT ThreadRun2();
	UINT ThreadRun3();

	// 기능 수행 함수
	// 초기화 및 종료 처리를 실행시켜준다.

	virtual void PreSystemRun()	{};			// SystemRun()을 최초로 수행하기 전에 동작
	virtual void SystemRun()		= 0;			// System적으로 
	virtual void PostSystemRun()	{};			// SystemRun() 수행을 마치고 종료하는 시점에 동작

public:
	CGxSystem(void);
	virtual ~CGxSystem(void);

	virtual void SetBasePath(CString sPath);							// 기본 경로를 설정하여 파일 경로를 갱신한다.

	// System을 제어한다.
	virtual BOOL Startup(void);									// 기동 시킨다.
	virtual void Shutdown(void);								// 종료처리한다.

	virtual BOOL InitProfile(void);
	virtual BOOL LoadProfile(void);
	virtual BOOL SaveProfile(void);
};

typedef CArray<CGxSystem, CGxSystem&> CGxSystemArray;
typedef CTypedPtrArray<CPtrArray, CGxSystem*> CGxSystemPtrArray;
typedef CTypedPtrMap<CMapStringToPtr, CString, CGxSystem*> CGxMapStringToSystem;



