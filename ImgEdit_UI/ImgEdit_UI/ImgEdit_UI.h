
// ImgEdit_UI.h : ImgEdit_UI �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��
#include "ImgEdit.h"
#include "LayerSynthesizerMG.h"
#include "EffectMG.h"
#include "IEToolMG.h"
#include "IECommandMG.h"
#include "ImgFileIOMG.h"
#include "BrushPopupWnd.h"
#include "ImgEdit_UIDoc.h"
#include "LoadFromTWAIN.h"

// CImgEdit_UIApp:
// ���̃N���X�̎����ɂ��ẮAImgEdit_UI.cpp ���Q�Ƃ��Ă��������B
//

class CImgEdit_UIApp : public CWinAppEx
{
public:
	CImgEdit_UIApp();
	~CImgEdit_UIApp();

// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void HookKeyboard() {
		m_isHookKeyboard = true;
	}
	void UnhookKeyBoard(){
		m_isHookKeyboard = false;
	}

	void HookMouse() {
		m_isHookMouse = true;
	}
	void UnhookMouse() {
		m_isHookMouse = false;
	}

// ����
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	void OnFileOpenFromScaner();

	BOOL DoOrgPromptFileName(CString& fileName, UINT nIDSTitle,
		DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);


	afx_msg BOOL OnIdle(LONG lCount);
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	DECLARE_MESSAGE_MAP()
//
private:
	static LRESULT CALLBACK GetMessageHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	//static LRESULT CALLBACK CallWndHookProc(int nCode, WPARAM wParam, LPARAM lParam);
	static LRESULT KeyboardHookProc(UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT MouseWheelHookProc(UINT message, WPARAM wParam, LPARAM lParam);

	CBrushPopupWnd m_BrushPopupWnd;
	LoadFromTWAIN* m_pLoadFromTWAIN;

	static bool m_isHookKeyboard;
	static bool m_isHookMouse;
	static HHOOK m_hGetMessageHook;
	//static HHOOK m_hCallWndHook;
};

extern CImgEdit_UIApp theApp;
