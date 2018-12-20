//==================================================================================================================
//
// ȭ�� ǥ�� ó���� �ϸ鼭 �����ϴ� ���� ��� 
//
//==================================================================================================================


#pragma once


// ��/���̾�α� ȭ�� ��ȯ�� �ʿ��� ���� ��� �Ϸ� ��ȣ ����
enum {

	// CFormView, ���� ȭ�� �� 
	eFORM_Default			= 0,			// ���α׷� ���۽� �⺻������ ������ �ִ�  CFormView
	eFORM_Main,								// Main ȭ��
	eFORM_Alarm,							// Alarm ǥ�� ȭ��
	eFORM_PM,								// PM / ��� ȭ��	
	eFORM_OLBDOWN,
	eFORM_Recipe,							// [W] Recipe ȭ��
	eFORM_Report1,							// ���� Report ǥ�� ȭ��
	eFORM_Report2,							
	eFORM_Report3,							
	eFORM_Report4,
	eFORM_Report5,

	eFORM_Teach1_Loader,							// ��⺰ ƼĪ ȭ��
	eFORM_Teach1_PreAlign,
	eFORM_Teach1_LoaderTR,
	eFORM_Teach1_TurnTable,
	eFORM_Teach1_UnloaderTR,
	eFORM_Teach1_GoodTray,
	eFORM_Teach1_NGTray,
	eFORM_Teach1_LDRobot,
	eFORM_Teach1_UDRobot,

	eFORM_Setup1,							// ����̽� ���� ȭ��
	eFORM_Setup2,
	eFORM_Setup3,
	eFORM_Setup4,
	eFORM_Config1,							//	�Ķ���� ���� ȭ��
	eFORM_Config2,				//Options
	eFORM_Config3,			
	eMAX_FORMCount,


	// CDialogBar, ȭ��
	eDLGBAR_None			= 0,			// ���� ����.
	eDLGBAR_Top,							// ���� ����ǥ�� 
	eDLGBAR_Bottom,						// ���� ���� �޴� ǥ��
	eDLGBAR_SubReport,						// Report ���� �޴� ����
	eDLGBAR_SubTeach,						// Teach ���� �޴� ����
	eDLGBAR_SubSetup,						// Setup ���� �޴� ����
	eDLGBAR_SubConfig,						// Config ���� �޴� ����
	eDLGBAR_Jog,							// Motion Jog ���� 
	eMAX_DLGBARCount,
	

	// CDialog, ��ȭ���� 
	eDLG_None				= 0,			// ���� ����. (����)
	eDLG_Confirm,							// ������� Ȯ���� �޴� ȭ��
	eDLG_Inform,							// ���� ���� ǥ�� ȭ��
	eDLG_Initial,							// �ʱ�ȭ ���� ��Ȳ ǥ�� ȭ��
	eDLG_MsgInterlock,					// ���� Interlock �޼��� ǥ�� ȭ��
	eDLG_MsgOPCall,						// ���� Operator Call �޼��� ǥ�� ȭ��
	eDLG_MsgTerminal,						// ���� Terminal �޼��� ǥ�� ȭ��
		
	eDLG_Losstime,						// �񰡵� �Է�ȭ��  20170118 byskcho

	eDLG_JobChange,                     //Trayid �� ü���� �޼���

	eDLG_MsgTMDDownLoad,                 //TMD Download  �޽���
	//eDLG_Door,								// Door open / Safety ���� ȭ��

	eDLG_Statement,                     //���ȭ��   
	eDLG_VersionHistory,
	eDLG_AGVMsg,
	eDLG_PortEmptyMsg,
	eDLG_RETRYINFO,
	eMAX_DLGCount,

};