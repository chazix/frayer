// PlayEventCapture.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "PlayEventCapture.h"

// for detect memory leak
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <cstdlib>
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define DISPLAY_NAME "�C�x���g�L���v�`�����Đ�����"

IECOMMAND_EXPORT IEMODULE_ID GetIEModule_ID()
{
	return IEMODULE_ID::COMMAND;
}

IECOMMAND_EXPORT void GetIECommandName(char* dst)
{
	strcpy_s(dst, MAX_IE_MODULE_NAME, "PlayEventCapture");
}

IECOMMAND_EXPORT void GetIECommandDisplayNameJa(char* dst)
{
	strcpy_s(dst, MAX_IE_MODULE_NAME, DISPLAY_NAME);
}

IECOMMAND_EXPORT IIECommand* CreateIECommand()
{
	return new PlayEventCapture;
}

IECOMMAND_EXPORT void ReleaseIECommand(IIECommand** ppCommand)
{
	if (*ppCommand) {
		delete (*ppCommand);
		(*ppCommand) = NULL;
	}
}

PlayEventCapture::PlayEventCapture()
{
  	strcpy_s(m_name, MAX_IE_MODULE_NAME, "PlayEventCapture");
	strcpy_s(m_disp_name, MAX_IE_MODULE_DISPLAY_NAME, DISPLAY_NAME);
}

PlayEventCapture::~PlayEventCapture()
{
}

BOOL PlayEventCapture::Run(ImgEdit *pEdit, void* pvoid)
{
	::OPENFILENAME ofn;
	TCHAR file_path[MAX_PATH] = TEXT("untitled.fed");
	TCHAR file_name[MAX_PATH] = TEXT("test.fed");

	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = ::GetForegroundWindow();
	ofn.lpstrFilter = TEXT("frayer event data(*.fed)\0*.fed\0\0");
	ofn.lpstrFile = file_path;
	ofn.lpstrFileTitle = file_name;
	ofn.nMaxFile = MAX_PATH;
	ofn.nMaxFileTitle = MAX_PATH;
	ofn.Flags = OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt = TEXT("fed");
	ofn.lpstrTitle = TEXT("�Đ�����C�x���g�t�@�C����I�����Ă�������");

	if (GetOpenFileName(&ofn) == 0) {
		return FALSE;
	}

	pEdit->PlayEventCapture(file_path);
	return FALSE;
}