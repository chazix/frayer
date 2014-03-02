#include "StdAfx.h"

#include "IEConfig_Environment.h"
#include "ImgEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IEConfig_Environment::IEConfig_Environment()
{
}

IEConfig_Environment::~IEConfig_Environment()
{
}

void IEConfig_Environment::InitDialog(HWND hDialog, HINSTANCE hInst, IE_CONFIG_DIALOG_DATA *pConfigData)
{
	m_pConfigData = pConfigData;

	RECT grid_rc;
	grid_rc.left = IEENV_PROP_GRID_X;
	grid_rc.top = IEENV_PROP_GRID_Y;
	grid_rc.right = grid_rc.left + IEENV_PROP_GRID_W;
	grid_rc.bottom = grid_rc.top + IEENV_PROP_GRID_H;

	m_wndEnvPropGrid.Create(
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER,
		grid_rc,
		CWnd::FromHandle( hDialog ),
		1);
	m_wndEnvPropGrid.EnableDescriptionArea();
	m_wndEnvPropGrid.SetVSDotNetLook( TRUE );

	CMFCPropertyGridProperty* pEnvPropGroup = new CMFCPropertyGridProperty( _T("���ݒ�") );

	IEColor bg_color = pConfigData->pImgEdit->GetEditWndBGColor();
	m_pBGColorProp = new CMFCPropertyGridColorProperty(
		_T("�G�f�B�b�g�E�B���h�E�w�i�F"),
		RGB(bg_color.r, bg_color.g, bg_color.b),
		NULL,
		_T("�G�f�B�b�g�E�B���h�E�̔w�i�F���w�肷��B"));
	m_pBGColorProp->EnableOtherButton( _T("Other...") );
	m_pBGColorProp->EnableAutomaticButton(_T("Default"), ::GetSysColor(COLOR_3DFACE));
	pEnvPropGroup->AddSubItem( m_pBGColorProp );

	unsigned int numOfHistory = pConfigData->pImgEdit->GetNumOfHistory();
	m_pNumOfHistoryProp = new CMFCPropertyGridProperty(
		_T("�ő�q�X�g����"),
		COleVariant((long)numOfHistory),
		_T("�L�����Ă����ő�̃q�X�g���̐����w�肷��B"));
	pEnvPropGroup->AddSubItem( m_pNumOfHistoryProp );
	m_pNumOfHistoryProp->EnableSpinControl();
	m_wndEnvPropGrid.AddProperty( pEnvPropGroup );

	::UpdateWindow(hDialog);
}

void IEConfig_Environment::Submit()
{
	if( m_pBGColorProp->IsModified() ){
		COLORREF color_ref = m_pBGColorProp->GetColor();
		IEColor bg_color;
		bg_color.b = GetBValue(color_ref);
		bg_color.g = GetGValue(color_ref);
		bg_color.r = GetRValue(color_ref);
		bg_color.a = 255;
		m_pConfigData->pImgEdit->SetEditWndBGColor( bg_color );
	}

	if( m_pNumOfHistoryProp->IsModified() ){
		COleVariant Variant = m_pNumOfHistoryProp->GetValue();
		int numOfHistory = Variant.lVal;
		if(numOfHistory < 0){
			OutputError::Alert("�ő�q�X�g����: 0�ȏ�̐������w�肵�Ă��������B");
		}
		else{
			m_pConfigData->pImgEdit->SetNumOfHistory( numOfHistory );
		}
	}

	//image update
	UPDATE_DATA data;
	data.isAll = true;
	data.update_flag = UPDATE_FLAG::UPDATE_DISPLAY_IMAGE;
	m_pConfigData->pImgEdit->PushUpdateDataToAllFile( &data );
}

BOOL IEConfig_Environment::OnMessage(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg){
		//case AFX_WM_PROPERTY_CHANGED:
		//	{
		//		CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*) lp;
		//	}
		//	return TRUE;
	}

	return FALSE;
}