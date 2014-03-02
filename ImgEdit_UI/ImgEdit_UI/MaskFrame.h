#pragma once 

#include "ImgFile.h"
#include "MaskPanel.h"
#include "ImgEditEventListener.h"
#include "ImgFileEventListener.h"
#include "ScrollWnd.h"

typedef std::vector<CMaskPanel*> MaskPanel_Vec;

#define MASK_PANEL_H 40

class CMaskFrame :
	public CScrollWnd,
	public ImgEditEventListener,
	public ImgFileEventListener
{
public:
	CMaskFrame();
	~CMaskFrame();

	////////////////////////////////////////////////////
	/*!
		�A�N�e�B�u�ɂȂ����t�@�C�����ς�����Ƃ��ɌĂяo�����C�x���g
		@param[in] file �A�N�e�B�u�ȃt�@�C���I�u�W�F�N�g
	*/
	void OnChangeActiveFile(ImgFile_Ptr pNewFile, ImgFile_Ptr pOldFile);

	/////////////////////////////////////////////////////
	/*!
		�t�@�C�����ǉ����ꂽ�Ƃ��ɌĂяo���C�x���g
		@param[in] pFile �ǉ����ꂽ�t�@�C��
	*/
	void OnAddImgFile(ImgFile_Ptr pFile);

	/////////////////////////////////////////////////////
	/*!
		�}�X�N���ǉ����ꂽ�Ƃ��ɌĂяo�����C�x���g
		@param[in] pMask �ǉ����ꂽ�}�X�N
		@param[in] index �}�X�N�C���f�b�N�X
	*/
	void OnAddMask(ImgMask *pMask, int index);

	/////////////////////////////////////////////////////
	/*!
		�}�X�N���폜�����Ƃ��ɌĂяo�����C�x���g
		@param[in] pMask �폜�����}�X�N
		@param[in] index �}�X�N�C���f�b�N�X
	*/
	void OnDeleteMask(ImgMask *pMask, int index);

	virtual void OnDraw(CDC* pDC);
	virtual void OnInitialUpdate();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnCommand(WPARAM wp, LPARAM lp);

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
	VOID ReSetAllPanel(ImgFile_Ptr pFile);

	////////////////////////////////////////////////////
	/*!
		index�̈ʒu��pMask�̃}�X�N�p�l����ǉ�����B
		@param[in] pMask
		@param[in] index
	*/
	VOID AddMaskPanel(ImgMask* pMask, int index);

	////////////////////////////////////////////////////
	/*!
		index�̈ʒu�̃}�X�N�p�l�����폜����
		@param[in] index
	*/
	VOID DeleteMaskPanel(int index);

	////////////////////////////////////////////////////
	/*!
	*/
	VOID AdjastPanel();

	///////////////////////////////////////////////////
	/*!
		���ׂẴ}�X�N�p�l�����ĕ`�悷��B
	*/
	void AllPanelRePaint();

	///////////////////////////////////////////////////
	/*!
		�E�B���h�E�n���h������}�X�N�p�l����T���Ԃ��B
		@param[in] hWnd �T���}�X�N�p�l���̃n���h��
		@return �}�X�N�p�l���̃|�C���^
	*/
	//LayerPanel* GetLayerPanelFromHWND(HWND hWnd);

	////////////////////////////////////////////////////
	/*!
		�}�E�X�̉��ɂ���p�l���̃|�C���^��Ԃ��B
		@param[in] point �N���C�A���g�}�E�X���W
		@return �}�X�N�p�l���̃|�C���^
	*/
	CMaskPanel* GetMaskPanelFromPos(CPoint point);

	////////////////////////////////////////////////////
	/*!
		�}�X�N�p�l�����폜����B
		@param[in,out] lp �폜����}�X�N�p�l��
	*/
	VOID DeleteMaskPanel(CMaskPanel* lp);

	////////////////////////////////////////////////////
	/*!
		�}�X�N�p�l�������ׂč폜�������[�J��
	*/
	VOID ClearMaskPanel();

	HINSTANCE m_hInst;
	MaskPanel_Vec m_MaskPanel_Vec;	//�}�X�N�p�l���z��
	CMaskPanel* m_SelectMaskPanel;	//�I�𒆃}�X�N�p�l��
	CMaskPanel* m_DragMaskPanel;	//�h���b�O���̃}�X�N�p�l��
	CMaskPanel* m_UnderMaskPanel;	//�h���b�O���}�E�X�J�[�\���̉��ɂ���}�X�N�p�l��
	CMaskPanel* m_PopupMaskPanel;
	
	HIMAGELIST m_hPanelDragImage;		//�h���b�O���̃C���[�W
	bool isPanelDrag;					//�p�l���h���b�O�t���O

	ImgFile_Ptr m_pActiveFile				//���ݕҏW���̃t�@�C���I�u�W�F�N�g

	SCROLLINFO m_FrameScrInfo;

	int m_width;
};