#pragma once
//��Į��
//Ʈ����
//�δ�
//�˻��
//��δ�
UINT MainSequenceThr(LPVOID lpParam);
UINT CellRcvThr(LPVOID lpParam);		// Load Conv�� Ÿ�̹���� �ؾ� �ؼ� ���� Thread�� ���� [9/28/2016 OSC]
UINT AGVInterfaceThr(LPVOID lpParam);		
UINT IOCheckThr(LPVOID lpParam);
UINT LoadVTFileThr(LPVOID lpParam);
UINT FDCMonitorThr( LPVOID lpParam );

//2017-09-17,SKCHO, �޴��� ���� ������
UINT GoodTrayManualThr(LPVOID lpParam);
UINT VAC_FDCMonitorThr( LPVOID lpParam );  //2017-06-04,SKCHO, �˻�� �з� ���� ���� ������
UINT TMDDownLoadThr(LPVOID lpParam);	//2017-03-08, skcho, TMD DOWNLAOD ������ ����

//UINT MelSecMonitorThr(LPVOID lpParam);  //2018-03-03,SKCHO, �߰�

//2017-09-26,SKCHO ,T5 RESTART ������
//UINT T5ReStartThr(LPVOID lpParam);

//�� AD ���忡�� ����ϰ� �ִ� ������ ����
#define LD_AD_USE_SLOT 1  //miain ���и� ����Ͽ� 1���Ը� ���
#define UD_AD_USE_SLOT 3

//20170129 byskcho
//0,1 ���Ը� ����ϰ� ����
//#define INSP_AD_USE_SLOT 4
#define INSP_AD_USE_SLOT 2

