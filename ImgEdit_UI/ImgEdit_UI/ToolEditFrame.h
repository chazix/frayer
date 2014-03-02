#pragma once

#include "IToolEditWndHandle.h"
#include "ScrollWnd.h"

#include "EditSliderCtrl.h"
#include "LabelComboBox.h"

typedef struct _TE_CTRL{
	CWnd* pWnd;
	int width;
	int height;
	int CtrlType;
} TE_CTRL;

typedef std::list<TE_CTRL> TE_CTRL_LIST;

class CToolEditFrame :
	public CScrollWnd,
	public IToolEditWndHandle
{
public:
	CToolEditFrame();
	~CToolEditFrame();

	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();

	VOID ClearAllCtrl();

	//////////////////////////////////
	/*!
		�g���b�N�o�[�̒ǉ�
		@param[in] name �g���b�N�o�[�̖��O
		@param[in] min �ŏ��l
		@param[in] max �ő�l
		@param[in] start �J�n���̒l
		@param[in] change_val �l��ύX����ϐ�
	*/
	VOID AddTrackbar(const char* name, int min, int max, int start, int* change_val);

	//////////////////////////////////
	/*!
		���C���[�����R���{�{�b�N�X�̒ǉ�
		@param[in] name �g���b�N�o�[�̖��O
		@param[in] start_index �J�n���̃C���f�b�N�X
		@param[in] change_val �l��ύX����ϐ�
	*/
	VOID AddLayerDrawModeComboBox(const char* name, int start_index, int* change_val);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP();

private:

	//////////////////////////////////////
	/*!
		�X�N���[���o�[�T�C�Y����
	*/
	void AdjastScrollSizes();

	//////////////////////////////////////
	/*!
		�R���g���[���̍Ĕz�u
	*/
	void AdjastControll();

	SCROLLINFO m_FrameScrInfo;

	int m_ctrl_count;
	TE_CTRL_LIST m_te_ctrl_list;
};