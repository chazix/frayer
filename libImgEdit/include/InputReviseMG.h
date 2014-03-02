#ifndef _INPUTREVISEMG_H_
#define _INPUTREVISEMG_H_

#include "LibIEDllHeader.h"

class IInputRevise;

class _EXPORTCPP InputReviseMG {
public:
	InputReviseMG();
	virtual ~InputReviseMG();

	//////////////////////////////
	/*!
		�␳���@�̑I�������܂��B
	*/
	void SelectInputRevise(const char* name);

	//////////////////////////////
	/*!
		�I�𒆂̕␳���@�̖��O�𓾂�B
	*/
	void GetInputReviseName(char* name);

	//////////////////////////////
	/*!
		�I�𒆂̕␳���@�N���X��Ԃ��B
		@return
	*/
	IInputRevise* GetInputRevise();

	///////////////////////////////
	/*!
		�␳���@��I������R���{�{�b�N�X���Z�b�g����
	*/
	void SetInputReviseComboBox(HWND hCombo);

private:
	IInputRevise* m_pSelectInputRevise;
};

#endif //_INPUTREVISEMG_H_