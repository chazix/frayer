#pragma once

#include "IIEConfig_DialogComponent.h"

#define IE_CONFIG_TXT_ENV "���ݒ�"
#define IE_CONFIG_TXT_INPUT_REVISE "���͕␳"
#define IE_CONFIG_TXT_KEY_CONFIG "�L�[�R���t�B�O"
#define IE_CONFIG_TXT_SHORTCUT "�V���[�g�J�b�g"
#define IE_CONFIG_TXT_TOOL_SHIFT "�c�[���V�t�g"

class IEConfigDialog
{
public:
	IEConfigDialog();
	virtual ~IEConfigDialog();

	void ShowDialog(HINSTANCE hInst, HWND howner, ImgEdit *pImgEdit);

	//////////////////////////////////
	/*!
		�_�C�A���O�̃v���V�[�W��
	*/
	static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:

	///////////////////////////////////
	/*!
		�_�C�A���O�̏�����
	*/
	void InitDialog(HWND hDlg);

	///////////////////////////////////
	/*!
		�ݒ荀�ڑI���c���[�r���[�̃C�x���g
	*/
	void DialogTreeEvent(HWND hDlg, LPARAM lp, IE_CONFIG_DIALOG_DATA* piecData);

	////////////////////////////////////
	/*!
	*/
	void DialogCmpntSubmit();

	BOOL DialogCmpntOnMessage(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


	HINSTANCE m_hInst;
	CFont m_DialogFont;
	IIEConfig_DialogComponent *m_pDialogCmpnt;
};
