#ifndef _IECOLORDIALOG_H_
#define _IECOLORDIALOG_H_

#include "LibIEDllHeader.h"
#include "IEColor.h"
#include <windows.h>

class _EXPORTCPP IEColorDialog
{
public:
	//
	IEColorDialog(IEColor defColor);

	//�_�C�A���O�̕\��
	INT_PTR DoModal();

	/////////////////////
	/*!
		�I��F��Ԃ��B
		@return 
	*/
	IEColor GetColor();

	static LRESULT CALLBACK IEColorDialogProc(HWND hDlgWnd, UINT msg,
															WPARAM wp, LPARAM lp);

private:
	IEColor m_SelectColor;
};

#endif