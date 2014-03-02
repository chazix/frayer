#pragma once

#include "ImgFile.h"
#include "LayerPanel.h"
#include "LayerGroupPanel.h"
#include "ImgEditEventListener.h"
#include "ImgFileEventListener.h"
#include "ScrollWnd.h"

typedef std::vector<CLayerPanel*> LayerPanel_Vec;

#define UM_DROPLAYER 10 //���C���[�p�l�����h���b�v�����Ƃ��̃��b�Z�[�W


class CLayerFrame :
	public CScrollWnd,
	public ImgEditEventListener,
	public ImgFileEventListener
{
public:
	CLayerFrame();
	~CLayerFrame();

	////////////////////////////////////////////////////
	/*!
		�A�N�e�B�u�ɂȂ����t�@�C�����ς�����Ƃ��ɌĂяo�����C�x���g
		@param[in] file �A�N�e�B�u�ȃt�@�C���I�u�W�F�N�g
	*/
	void OnChangeActiveFile(ImgFile_Ptr pNewFile, ImgFile_Ptr pOldFile);

	/////////////////////////////////////////////////////
	/*!
		���C���[���ǉ����ꂽ�Ƃ��ɌĂяo�����C�x���g
		@param[in] pLayer �ǉ����ꂽ���C���[
		@param[in] index ���C���[�C���f�b�N�X
	*/
	void OnAddLayer(IImgLayer_Ptr pLayer, int index);

	/////////////////////////////////////////////////////
	/*!
		���C���[���폜�����Ƃ��ɌĂяo�����C�x���g
		@param[in] pLayer �폜����郌�C���[
		@param[in] index ���C���[�C���f�b�N�X
	*/
	void OnDeleteLayer(IImgLayer_Ptr pLayer, int index);

	void OnChangeLayerLine(int from_index, int to_index);

	//////////////////////////////
	/*!
		�I�����C���[���ς�����Ƃ��ɌĂяo��
		@param[in] pLayer �I�����ꂽ���C���[
	*/
	void OnSelectLayer(IImgLayer_weakPtr pLayer);

	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();

protected:

	BOOL PreCreateWindow(CREATESTRUCT& cs);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnCommand(WPARAM wp, LPARAM lp);

	afx_msg void OnLayerNameEditKillFocus();

	DECLARE_MESSAGE_MAP();

private:

	//////////////////////////////////////
	/*!
		�X�N���[���o�[�T�C�Y����
	*/
	void AdjastScrollSizes();

	////////////////////////////////////////////////////
	/*!
	*/
	VOID CreatePanel(ImgLayerGroup_Ptr pLGroup);

	////////////////////////////////////////////////////
	/*!
	*/
	VOID ReSetAllPanel(ImgFile_Ptr pFile);

	////////////////////////////////////////////////////
	/*!
		reset panel position annd state
	*/
	VOID AdjastPanel();

	////////////////////////////////////////////////////
	/*!
		index�̈ʒu�̃��C���[�p�l�����폜����
		@param[in] index
	*/
	VOID DeleteLayerPanel(int index);

	////////////////////////////////////////////////////
	/*!
		@param[in] from_index
		@param[in] to_index
	*/
	VOID InsertLayerPanel(int from_index, int to_index);

	///////////////////////////////////////////////////
	/*!
		���ׂẴ��C���[�p�l�����ĕ`�悷��B
	*/
	void AllPanelRePaint();

	////////////////////////////////////////////////////
	/*!
		�}�E�X�̉��ɂ���p�l���̃|�C���^��Ԃ��B
		@param[in] point �N���C�A���g�}�E�X���W
		@return ���C���[�p�l���̃|�C���^
	*/
	CLayerPanel* GetLayerPanelFromPos(CPoint point);
	
	////////////////////////////////////////////////////
	/*!
		�}�E�X�̉��ɂ���p�l���̃C���f�b�N�X��Ԃ��B
		@param[in] point �N���C�A���g�}�E�X���W
		@return ���C���[�p�l���̃C���f�b�N�X�A������Ȃ����(-1)
	*/
	int GetLayerPanelIndexFromPos(CPoint point);

	////////////////////////////////////////////////////
	/*!
		���C���[�p�l�����폜����B
		@param[in,out] lp �폜���郌�C���[�p�l��
	*/
	VOID DeleteLayerPanel(CLayerPanel* lp);

	////////////////////////////////////////////////////
	/*!
		���C���[�p�l�������ׂč폜�������[�J��
	*/
	VOID ClearLayerPanel();

	bool isPanelDrag;					//�p�l���h���b�O�t���O
	LayerPanel_Vec m_LayerPanel_Vec;	//���C���[�p�l�����X�g
	CLayerPanel* m_OnMousePanel;		//�}�E�X�̉��ɂ���p�l��
	CLayerPanel* m_SelectLayerPanel;	//�I�𒆃��C���[�p�l��

	int m_DragLayerPanelInsertIndex;
	int m_DragLayerPanelIndex;
	CLayerPanel* m_DragLayerPanel;		//�h���b�O���̃��C���[�p�l��
	CLayerPanel* m_PopupLayerPanel;

	RECT m_InsertCursorRect;

	CFont m_LayerNameFont;
	CLayerNameEdit m_LayerNameEdit;

	ImgFile_Ptr m_pActiveFile;				//���ݕҏW���̃t�@�C���I�u�W�F�N�g

	SCROLLINFO m_FrameScrInfo;
};