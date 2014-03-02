
#pragma once

#include "ImgEditEventListener.h"

class CBrushPreviewFrame :
	public CWnd,
	public IEBrushEventListener,
	public IEBrushGroupEventListener
{
public:
	CBrushPreviewFrame();
	~CBrushPreviewFrame();

	void OnChangeOrgRadius(int old_rad, int new_rad);
	void OnChangeBrushShape();
	void OnChangeBrushStroke();

	////////////////////////////////
	/*!
		�I���u���V���ς�����Ƃ��ɌĂяo���B
		@param[in] pNewBrush �ύX��̃u���V
		@param[in] pOldBrush �ύX�O�̃u���V
	*/
	void OnChangeSelectBrush(IEBrush_Ptr pNewBrush, IEBrush_Ptr pOldBrush);

	void SetBrushGroup(IEBrushGroup_Ptr pBGroup);

protected:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nTypde, int cx, int cy);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()

private:
	void UpdatePreviewImg(IEBrush_Ptr pBrush);

	IEBrush_Ptr m_pViewBrush;
	IEBrushGroup_Ptr m_pViewBrushGroup;
	IplImage* m_PreviewImg;
};