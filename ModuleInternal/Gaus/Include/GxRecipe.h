/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  
// [ Gaus Project ]
//
// GxRecipe : 생산에 필요한 각종 파라메터/설정변수들의 관리 Class
//
// 2014. 02. 27, jhLee (semsemi)
// 
// 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Gaus.h"
#include "File\GxIni.h"

class _GAUS_LIB CGxRecipe : public CGxIni 
{
public:
	CGxRecipe();
	virtual ~CGxRecipe();

	void Load();
	void Load(LPCTSTR pszFile);
	void Save();
	void Save(LPCTSTR pszFile);
};

typedef CArray<CGxRecipe, CGxRecipe&> CGxRecipeArray;
typedef CTypedPtrArray<CPtrArray, CGxRecipe*> CGxRecipePtrArray;




