#pragma once

#include "LibIEDllHeader.h"


class IEffect;

class _EXPORTCPP IEffectEditDialog
{
public:
	IEffectEditDialog(){};
	virtual ~IEffectEditDialog(){};

	//////////////////////////////////
	/*!
		�_�C�A���O�̕\��
		@param[in] hInst
		@param[in] hwndParent
		@param[in] file
		@param[in] effect
	*/
	virtual INT_PTR CreateDialogBox(HINSTANCE hInst, HWND hwndParent, ImgFile_Ptr file, IEffect* effect) =0;

	//////////////////////////////////
	/*!
		�g���b�N�o�[�̒ǉ�
		@param[in] name �g���b�N�o�[�̖��O
		@param[in] min �ŏ��l
		@param[in] max �ő�l
		@param[in] start �J�n���̒l
		@param[in] change_val �l��ύX����ϐ�
	*/
	virtual VOID AddTrackbar(const char* name, int min, int max, int start, int *change_val) =0;

	///////////////////////////////////
	/*!
		�v���O���X�o�[�͈͎̔w��
	*/
	virtual VOID SetProgRange(int min, int max){};

	virtual VOID SetProgStep(int step){};
	virtual VOID ProgStepIt(){};
	virtual VOID SetProgPos(int pos){};
};
