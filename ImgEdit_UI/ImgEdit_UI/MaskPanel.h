#pragma once

#include "ImgMask.h"
#include "ImgFile.h"

#undef CImage

#define MASK_NAME_FONT_SIZE 12
#define MASK_NAME_FONT_NAME "�l�r �S�V�b�N"


class CMaskPanel :
	public CWnd
{
public:
	CMaskPanel();
	~CMaskPanel();


	void SetMask(ImgMask* pMask);

	////////////////////////////////////////////
	/*!
		�}�X�N�X�V�C�x���g���ɌĂяo�����B
	*/
	void OnUpdateMask();

	////////////////////////////////////////////
	/*!
		�h���b�O���̃}�E�X���p�l���̏�ɗ����Ƃ��ɌĂяo��
		@param[in] lpt �}�E�X�̃X�N���[�����W
	*/
	void OnDragMouse(LPPOINT lpt);

	/////////////////////////////////////////////////////
	/*!
		�p�l����I��F�ɂ���B
	*/
	void SelectPanel();

	////////////////////////////////////////////////
	/*!
		�p�l���𕁒ʂ̐F�ɂ���B
	*/
	void NewtralPanel();

	////////////////////////////////////////////////
	/*!
		�h���b�O���̃C���[�W���쐬����
		@param[out] hImageList �o�͐�̃C���[�W���X�g
	*/
	void CreateDragImage(HIMAGELIST hImageList);

	//////////////////////////////////////////////
	/*!
		�}�X�N�X�^�C���_�C�A���O���J��
	*/
	void OpenMaskStyleDialog(HWND hParent);

	//////////////////////////////////////////////
	/*!
		�}�X�N�폜
	*/
	void DeleteMask();

	//////////////////////////////////////////////
	/*!
		�|�b�v�A�b�v���j���[�\��
	*/
	void PopupMenu(HWND hParent, const LPPOINT lpt);

	//////////////////////////////////////////////
	/*!
		�}�X�N�I�u�W�F�N�g�ւ̃|�C���^��Ԃ��B
		@return �}�X�N�I�u�W�F�N�g�ւ̃|�C���^
	*/
	ImgMask* GetMask();

	void UpdateMaskThm();

	///////////////////////////////////////////
	/*!
		�p�l���̕`��
		@param[in] pDC �`�悷��f�o�C�X�R���e�L�X�g
	*/
	void Draw(CDC *pDC);

	//////////////////////////////////////////////
	/*!
		�}�X�N�X�^�C���_�C�A���O�̃v���V�[�W��
	*/
	static LRESULT CALLBACK MaskStyleDialogProc(HWND, UINT, WPARAM, LPARAM);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType,int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP();

private:

	/////////////////////////////////////////////
	/*!
		���C���[�X�^�C���_�C�A���O�̏�����
	*/
	void InitMaskStyleDialog(HWND hDlgWnd);

	//////////////////////////////////////////////
	/*!
		�`�惂�[�h�̑I�����ڂ��ύX���ꂽ�Ƃ��Ăяo��
	*/
	void OnChangeDrawModeCombo(HWND hDlgWnd);


	HINSTANCE m_hInst;

	ImgMask *m_Mask;

	IplImage* m_MaskThm;

	
	RECT m_EyeRect;
	
	RECT m_MaskThmRect;

	int m_DrawModeCombo_buf;

	bool isSelectMask; //�I��\�������邩�ǂ���
	bool isInsertOver;
	bool isInsertUnder;
};