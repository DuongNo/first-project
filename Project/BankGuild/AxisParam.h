#pragma once


class CAxisParam
{
public:

	enum DEVICE_TYPE
	{
		DEVICE_TYPE_NMC,
		DEVICE_TYPE_MP2100,
		DEVICE_TYPE_MAX
	};

	enum ORIGIN_TYPE
	{
		ORIGIN_TYPE_HOME,
		ORIGIN_TYPE_LIMIT_N,
		ORIGIN_TYPE_LIMIT_P,
		ORIGIN_TYPE_Z_PULSE,
		ORIGIN_TYPE_MAX
	};

	enum MOTOR_TYPE
	{
		MOTOR_TYPE_SERVO,
		MOTOR_TYPE_EZ_SERVO,		// Step인데 Encoder가 달린 모터
		MOTOR_TYPE_STEP,
		MOTOR_TYPE_MAX
	};

	CAxisParam(void);
	~CAxisParam(void);

	CString m_strAxisName;
	BOOL m_bUse;
	DEVICE_TYPE m_DeviceType;
	int m_nModuleNo;	// Board No or Device No
	int m_nInnerNo;		// Axis No in Module
	ORIGIN_TYPE m_OriginType;
	MOTOR_TYPE m_MotorType;
	double m_dJogSpeedHigh;
	double m_dJogSpeedMiddle;
	double m_dJogSpeedLow;
	double m_dHomeSpeed;
	double m_dDefaultMoveSpeed;
	double m_dPositiveLimit;
	double m_dNegativeLimit;
	double m_dGearRatio;

	void Init();
	void ReadFile(CString strFilePath);
	void SaveFile(CString strFilePath);
	void SaveECIDFile(CString strFilePath);
};

