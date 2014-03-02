#pragma once

#include "../resource1.h"
#include "ImgEdit_UI.h"
#include <libImgEdit.h>

// CNewPaletteColorDialog �_�C�A���O

class CNewPaletteColorDialog : public CDialog
{
	DECLARE_DYNAMIC(CNewPaletteColorDialog)

public:
	CNewPaletteColorDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CNewPaletteColorDialog();

	void SetColor(IEColor color){
		m_color.value = color.value;
	}

	BOOL Create(CWnd* pParentWnd){
		//unhook
		CImgEdit_UIApp* pApp = (CImgEdit_UIApp*)AfxGetApp();
		pApp->UnhookKeyBoard();
		return CDialog::Create(IDD, pParentWnd);
	}

// �_�C�A���O �f�[�^
	enum { IDD = IDD_CREATE_NEW_PALETTE_COLOR };

protected:
	//
	virtual void PostNcDestroy();
	//
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	BOOL OnInitDialog();
	void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	IEColor m_color;
};
