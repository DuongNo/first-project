#pragma once

#include "RecipeCell.h"
#include "RecipeTray.h"

class CRecipeBank
{
public:
	CRecipeBank(void);
	~CRecipeBank(void);

	CString m_strRecipeName;		// ���� Read�Ǿ� �ִ� Recipe Name
	CRecipeCell m_Cell;
	CRecipeTray m_Tray;

	void ReadLastRecipeFile();	// ������ �ҷ��Դ� Recipe Read. m_strRecipeName�� �ڵ����� ������ �ҷ��� ������ �ٲ��
	void SaveLastRecipeFile();	// ������ �ҷ��Դ� Recipe Read�� �����Ѵ�.
	void SetMainRecipeName(CString strName) { m_strRecipeName = strName; }
	void ReadRecipeFile(CString strRecipeName);	// Name���� Recipe�� ã�� �װ� �ҷ��´�
	// ������ ���� ����. 2017/06/13 HSK.
	void SaveRecipeFile(CString strRecipe);
};

