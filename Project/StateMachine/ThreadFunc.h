#pragma once
//스칼라
//트레이
//로더
//검사부
//언로더
UINT MainSequenceThr(LPVOID lpParam);
UINT CellRcvThr(LPVOID lpParam);		// Load Conv는 타이밍제어를 해야 해서 별도 Thread로 뺀다 [9/28/2016 OSC]
UINT AGVInterfaceThr(LPVOID lpParam);		
UINT IOCheckThr(LPVOID lpParam);
UINT LoadVTFileThr(LPVOID lpParam);
UINT FDCMonitorThr( LPVOID lpParam );

//2017-09-17,SKCHO, 메뉴얼 투입 스래드
UINT GoodTrayManualThr(LPVOID lpParam);
UINT VAC_FDCMonitorThr( LPVOID lpParam );  //2017-06-04,SKCHO, 검사부 압력 측정 전용 스레드
UINT TMDDownLoadThr(LPVOID lpParam);	//2017-03-08, skcho, TMD DOWNLAOD 스레드 생성

//UINT MelSecMonitorThr(LPVOID lpParam);  //2018-03-03,SKCHO, 추가

//2017-09-26,SKCHO ,T5 RESTART 스레드
//UINT T5ReStartThr(LPVOID lpParam);

//각 AD 보드에서 사용하고 있는 슬롯의 갯수
#define LD_AD_USE_SLOT 1  //miain 공압만 사용하여 1슬롯만 사용
#define UD_AD_USE_SLOT 3

//20170129 byskcho
//0,1 슬롯만 사용하게 변경
//#define INSP_AD_USE_SLOT 4
#define INSP_AD_USE_SLOT 2

