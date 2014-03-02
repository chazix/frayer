#ifndef _ITOOLEDITWNDHANDLE_H_
#define _ITOOLEDITWNDHANDLE_H_

#include "LibIEDllHeader.h"

class _EXPORTCPP IToolEditWndHandle
{
public:
	IToolEditWndHandle(){};
	virtual ~IToolEditWndHandle(){};

	virtual VOID ClearAllCtrl(){};

	//////////////////////////////////
	/*!
		�g���b�N�o�[�̒ǉ�
		@param[in] name �g���b�N�o�[�̖��O
		@param[in] min �ŏ��l
		@param[in] max �ő�l
		@param[in] start �J�n���̒l
		@param[in,out] change_val �l��ύX����ϐ�
	*/
	virtual VOID AddTrackbar(const char* name, int min, int max, int start, int *change_val){};

	//////////////////////////////////
	/*!
		���C���[�����R���{�{�b�N�X�̒ǉ�
		@param[in] name �g���b�N�o�[�̖��O
		@param[in] start_index �J�n���̃C���f�b�N�X
		@param[in,out] change_val �l��ύX����ϐ�
	*/
	virtual VOID AddLayerDrawModeComboBox(const char* name, int start_index, int* change_val){};
};

#endif //ITOOLEDITWNDHANDLE_H_