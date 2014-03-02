
#pragma once

#include "ToolPopupPanel.h"
#include <libImgEdit.h>

#define POPUP_TIME 500

typedef std::vector<CToolPopupPanel*> CToolPopupPanel_Vec;

class CToolFrame :
	public CWnd,
	public IEToolMGEventListener
{
public:
	CToolFrame();
	~CToolFrame();

	//
	void OnChangeSelectTool(IIETool* pNewTool, IIETool* pOldTool);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnTipsDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnLButtonDown(UINT nFlagas, CPoint point);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//afx_msg void OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP();

private:
	////////////////////////////
	/*!
		�{�^�����Ĕz�u
	*/
	void AdjastButtonGroup();

	////////////////////////////
	/*!
		�ʒu����{�^���O���[�v�ւ̃|�C���^�𓾂�
	*/
	CToolPopupPanel* GetToolPanelFromPos(CPoint point);

	bool SelectToolButton(const IIETool* pTool);

	//////////////////////////////////////////////////////
	/*!
		�c�[���̎�ނ�ID�Ŏw�肷��
		@param[in] id �w�肷��ID
	*/
	void SetIEToolID(int id) const;

	CToolPopupPanel_Vec m_ToolPopupPanel_Vec;
};