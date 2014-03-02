#pragma once

#include "IEBrush.h"
#include "IIEBrushPopupWnd.h"

#define PROP_BPP_WINPROC "CBrushPopupWnd"

class CBrushPopupWnd : 
	public CWnd,
	public IIEBrushPopupWnd 
{
public:
	CBrushPopupWnd();
	virtual ~CBrushPopupWnd();

	BOOL CreatePopupWnd(CWnd* pOwner, UINT nID);

	//////////////////////////////////
	/*!
		���W���w�肵�ă|�b�v�A�b�v��\��
		@param[in] lpt �X�N���[�����W
	*/
	void ShowPopup();

	//////////////////////////////////
	/*!
		�|�b�v�A�b�v���\��
	*/
	void HidePopup();

	///////////////////////////////////
	/*!
		�ҏW����u���V���Z�b�g����B
	*/
	void SetEditBrush(IEBrush_Ptr pBrush);

	BOOL PreCreateWindow(CREATESTRUCT& cs);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP();

private:

	IEBrush_Ptr m_pEditBrush;

	CFont m_font;
	CStatic m_BrushSizeLabel;
	CSliderCtrl m_BrushSizeSlider;
};