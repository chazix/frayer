#pragma once

#include "ScrollWnd.h"
#include "BrushPanel.h"
#include "ImgEditEventListener.h"


#define BRUSH_PANEL_W 50
#define BRUSH_PANEL_H 50

typedef std::vector<CBrushPanel*> BrushPanel_Vec;


class CBrushSelectFrame :
	public CScrollWnd,
	public IEBrushGroupEventListener
{
public:
	CBrushSelectFrame();
	~CBrushSelectFrame();

	//////////////////////////////////
	/*!
		����u���V�ύX�C�x���g
		@param[in] pNewBrush
		@param[in] pOldBrush
	*/
	void OnChangeSelectBrush(IEBrush_Ptr pNewBrush, IEBrush_Ptr pOldBrush);

	//////////////////////////////////
	/*!
		�u���V�ǉ��C�x���g
		@param[in] pBrush �ǉ������u���V
	*/
	void OnAddBrush(IEBrush_Ptr pBrush);

	///////////////////////////////////
	/*!
		�u���V�폜�C�x���g
	*/
	void OnDeleteBrush(IEBrush_Ptr pBrush);

	void SetBrushGroup(IEBrushGroup_Ptr pBGroup);

	////////////////////////////////////
	/*!
	*/
	virtual void OnDraw(CDC* pDC){
	}
	virtual void OnInitialUpdate(){
	}

protected:
	//DECLARE_DYNCREATE(CBrushSelectFrame)

	BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	//afx_msg BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	void PostNcDestroy();

	DECLARE_MESSAGE_MAP()

private:
	//////////////////////////////////////
	/*!
		�I���p�l����ύX����
		@param[in,out] pPanel �I������p�l��
	*/
	void ChangeSelectPanel(CBrushPanel* pPanel);

	//
	void AddBrushPanel(IEBrush_Ptr pBrush);

	//////////////////////////////////////
	/*!
		�X�N���[���o�[�T�C�Y����
	*/
	void AdjastScrollSizes();

	//////////////////////////////////////
	/*!
		�p�l�����Ĕz�u����B
	*/
	void AdjastPanel();

	//////////////////////////////////////
	/*!
		�}�E�X�̉��ɂ���p�l���̃|�C���^��Ԃ��B
		@param[in] point �N���C�A���g�}�E�X���W
		@return �u���V�p�l���̃|�C���^
	*/
	CBrushPanel* GetBrushPanelFromPos(CPoint point);

	//////////////////////////////////////
	/*!
		IEBrush�|�C���^��ێ����Ă���p�l���̃|�C���^��Ԃ��B
		@param[in] pBrush �u���V�|�C���^
		@return �u���V�p�l���̃|�C���^
	*/
	CBrushPanel* GetBrushPanelFromIEBrush(const IEBrush_Ptr pBrush);

	//////////////////////////////////////
	/*!
		�p�l���z�񂩂�̏������u���V�p�l�����폜����
	*/
	void DeleteBrushPanel(CBrushPanel* pPanel);


	BrushPanel_Vec m_BrushPanel_Vec;	//�\�����Ă���p�l���̔z��
	CBrushPanel *m_pSelectBrushPanel;	//�I�𒆂̃p�l��

	CToolTipCtrl m_ToolTip;

	IEBrushGroup_Ptr m_pViewBrushGroup;

	int m_panel_col; //
	int m_panel_x_index;
};