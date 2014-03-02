#include "stdafx.h"
#include "InputReviseMG.h"

#include "IInputRevise.h"
#include "InputNoRevise.h"
#include "InputMovingAverageRevise.h"
#include "InputGaussianRevise.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


static InputNoRevise noRevise;
static InputMovingAverageRevise MA4Revise(4);
static InputMovingAverageRevise MA8Revise(8);
static InputGaussianRevise G4Revise(4, 5.0);
static InputGaussianRevise G8Revise(8, 5.0);


InputReviseMG::InputReviseMG()
{
	m_pSelectInputRevise = &noRevise;
}

InputReviseMG::~InputReviseMG()
{
}

//////////////////////////////
/*!
	�␳���@�̑I�������܂��B
*/
void InputReviseMG::SelectInputRevise(const char *name)
{
	if(strcmp(name, "�␳����") == 0){
		m_pSelectInputRevise = &noRevise;
	}
	else if(strcmp(name, "���ϕ␳4") == 0){
		m_pSelectInputRevise = &MA4Revise;
	}
	else if(strcmp(name, "���ϕ␳8") == 0){
		m_pSelectInputRevise = &MA8Revise;
	}
	else if(strcmp(name, "G�␳4") == 0){
		m_pSelectInputRevise = &G4Revise;
	}
	else if(strcmp(name, "G�␳8") == 0){
		m_pSelectInputRevise = &G8Revise;
	}
}

//////////////////////////////
/*!
	�I�𒆂̕␳���@�̖��O�𓾂�B
*/
void InputReviseMG::GetInputReviseName(char* name)
{
	if(m_pSelectInputRevise == &noRevise){
		strcpy(name, "�␳����");
	}
	else if(m_pSelectInputRevise == &MA4Revise){
		strcpy(name, "���ϕ␳4");
	}
	else if(m_pSelectInputRevise == &MA8Revise){
		strcpy(name, "���ϕ␳8");
	}
	else if(m_pSelectInputRevise == &G4Revise){
		strcpy(name, "G�␳4");
	}
	else if(m_pSelectInputRevise == &G8Revise){
		strcpy(name, "G�␳8");
	}
}

//////////////////////////////
/*!
	�I�𒆂̕␳���@�N���X��Ԃ��B
	@return
*/
IInputRevise* InputReviseMG::GetInputRevise()
{
	return m_pSelectInputRevise;
}

///////////////////////////////
/*!
	�␳���@��I������R���{�{�b�N�X���Z�b�g����
*/
void InputReviseMG::SetInputReviseComboBox(HWND hCombo)
{
	//add item
	::SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"�␳����");
	::SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"���ϕ␳4");
	::SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"���ϕ␳8");
	::SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"G�␳4");
	::SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)"G�␳8");
	
	//select item
	char name[256];
	GetInputReviseName(name);
	int select_index = ::SendMessage(hCombo, CB_FINDSTRINGEXACT, 0, (LPARAM)name);
	::SendMessage(hCombo, CB_SETCURSEL, select_index, 0);
}
