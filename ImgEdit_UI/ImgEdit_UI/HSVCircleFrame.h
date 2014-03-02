
#pragma once

class CHSVCircleFrame :
	public CWnd,
	public ImgEditEventListener,
	public std::enable_shared_from_this<CHSVCircleFrame>
{
public:
	CHSVCircleFrame();
	~CHSVCircleFrame();

	//////////////////////////////
	/*!
		�I��F���ς�����Ƃ��ɌĂт������
		@param[in] color �ύX��̐F
	*/
	void OnChangeSelectColor(IEColor color);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP();

private:
	//H�T�[�N����`��
	void DrawHCircle(IplImage* dst, int center_x, int center_y, int radius, int circle_width);
	//SV��`
	void DrawSVRect(IplImage* dst, int start_x, int start_y, int width, int height);
	//
	void DrawPoint(IplImage* dst, int center_x, int center_y, int radius, int circle_width);

	///////////////////////////////////////////
	/*!
		�F�I���~�̒��S�̈ʒu��Ԃ��B
		@param[out] point ���S�̃N���C�A���g���W
	*/
	void GetCircleCenterPos(CPoint& point);

	//////////////////////////////////
	/*!
		�F�I���~�̔��a��Ԃ��B
	*/
	double GetCircleRadius();

	//////////////////////////////////
	/*!
		SV�l�p�`�̊J�n�ʒu��Ԃ��B
		@param[out] point
	*/
	void GetBeginSVRectPos(CPoint& point);

	//////////////////////////////////
	/*!
		SV�l�p�`�̏I���ʒu��Ԃ��B
		@param[out] point
	*/
	void GetEndSVRectPos(CPoint& point);

	//////////////////////////////////
	/*!
		lpt��H�I���~�ɏ���Ă��邩���肷��B
		@param[in] point �}�E�X�N���C�A���g���W
		@return ����Ă�����H�l �����łȂ��Ȃ�-1.0
	*/
	double isOnColorCircle(CPoint point);

	///////////////////////////////////
	/*!
		lpt��SV�I���l�p�ɏ���Ă��邩���肷��B
		@param[in] lpt �}�E�X�N���C�A���g���W
		@param[out] s �I�����ꂽs�l
		@param[out] v �I�����ꂽv�l
		@return ����Ă��邽��true �����łȂ��Ȃ�false��Ԃ��B
	*/
	double isOnColorRect(CPoint point, double *s, double *v);

	bool m_isHDrag;
	bool m_isSVDrag;
	IEColor m_SelectColor;
	double m_Select_H;
	double m_Select_S;
	double m_Select_V;
	RECT m_ColorCircleRect;
	RECT m_FGColorPanelRect;
	RECT m_BGColorPanelRect;
};