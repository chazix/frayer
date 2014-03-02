
#pragma once

#include "ScrollWnd.h"

typedef std::list<IplImage*> PanelImg_List;

class CPaletteFrame :
	public CScrollWnd,
	public IEPaletteEventListener
{
public:
	CPaletteFrame();
	~CPaletteFrame();

	////////////////////////////////////
	/*!
		�I��F���ύX�C�x���g
	*/
	void OnChangeSelectColor(int new_select_color_index);

	///////////////////////////////////
	/*!
		�F���ǉ��C�x���g
	*/
	void OnAddColor(IEPaletteNode_Ptr pNode);

	///////////////////////////////////
	/*!
		�F���폜�C�x���g
	*/
	void OnDeleteColor(int index, IEPaletteNode_Ptr pNode);

	void SetPalette(IEPalette_Ptr pPalette);

	virtual void OnDraw(CDC* pDC){};
	virtual void OnInitialUpdate(){};

	CString m_tool_tip_txt;

protected:

	BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	DECLARE_MESSAGE_MAP()

private:

	void ResetAllPanel();

	//////////////////////////////////////
	/*!
		�X�N���[���o�[�T�C�Y����
	*/
	void AdjastScrollSizes();

	IEPalette_Ptr m_pViewPalette;
	PanelImg_List m_PanelImg_List;

	CToolTipCtrl m_ToolTip;

	int m_select_panel_index;
	int m_panel_col; //
};