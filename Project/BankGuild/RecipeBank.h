#pragma once

#include "RecipeCell.h"
#include "RecipeTray.h"

class CRecipeBank
{
public:
	CRecipeBank(void);
	~CRecipeBank(void);

	CString m_strRecipeName;		// 현재 Read되어 있는 Recipe Name
	CRecipeCell m_Cell;
	CRecipeTray m_Tray;

	void ReadLastRecipeFile();	// 마지막 불러왔던 Recipe Read. m_strRecipeName가 자동으로 마지막 불러온 것으로 바뀐다
	void SaveLastRecipeFile();	// 마지막 불러왔던 Recipe Read을 저장한다.
	void SetMainRecipeName(CString strName) { m_strRecipeName = strName; }
	void ReadRecipeFile(CString strRecipeName);	// Name으로 Recipe를 찾아 그걸 불러온다
	// 레시피 버그 수정. 2017/06/13 HSK.
	void SaveRecipeFile(CString strRecipe);
};

