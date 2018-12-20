#pragma once

struct CONST_IO
{
	CString strName;	
	CString strNote;	

	// 1. IP1 = Input Port
	// 2. OP1 = Output Port

	enum IID
	{
		X_CYLINDER_FWD_SEN,
		X_001,
		X_002,
		X_003,
		X_004,
		X_005,
		X_006,
		X_007,
		X_008,
		X_009,
		X_00A,
		X_00B,
		X_00C,
		X_00D,
		X_00E,
		X_00F,

		X_010,
		X_011,
		X_012,
		X_013,
		X_014,
		X_015,
		X_016,
		X_017,
		X_018,
		X_019,
		X_01A,
		X_01B,
		X_01C,
		X_01D,
		X_01E,
		X_01F,

		MAX_IN_ID
	};
	enum OID
	{
		Y_CYLINDER_FWD_SOL,
		Y_001,
		Y_002,
		Y_003,
		Y_004,
		Y_005,
		Y_006,
		Y_007,
		Y_008,
		Y_009,
		Y_00A,
		Y_00B,
		Y_00C,
		Y_00D,
		Y_00E,
		Y_00F,

		Y_010,
		Y_011,
		Y_012,
		Y_013,
		Y_014,
		Y_015,
		Y_016,
		Y_017,
		Y_018,
		Y_019,
		Y_01A,
		Y_01B,
		Y_01C,
		Y_01D,
		Y_01E,
		Y_01F,

		MAX_OUT_ID,
	};
};
