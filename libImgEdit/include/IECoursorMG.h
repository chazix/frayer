#ifndef _IECOURSORMG_H_
#define _IECOURSORMG_H_

#include "IEBrush.h"

class IECoursorMG
{
public:
	/////////////////////////////////////
	/*!
		�}�E�X�J�[�\�����u���V�̌`��ɕύX����B
		@param[in] IEBrush �u���V�̃|�C���^
		@param[in] scale �u���V�̃T�C�Y�{��
	*/
	static bool SetBrushCoursor(HWND hWnd, const IEBrush_Ptr pBrush, double scale);
};

#endif //_IECOURSORMG_H_