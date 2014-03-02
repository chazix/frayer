#pragma once

#include "ImgLayer.h"
#include "ImgFile.h"
#include "ImgLayerEventListener.h"

#include "ImgEdit_UI.h"
#include "LayerNameEdit.h"

#undef CImage

#define ID_LP_NAME_EDIT 1

#define LP_FONT_NAME "�l�r �S�V�b�N"

#define LAYER_NAME_FONT_SIZE 14
#define LAYER_NAME_FONT_WIDTH 17
#define LAYER_STYLE_FONT_SIZE 12

#define LP_EYE_WIDTH 18
#define LP_EYE_HEIGHT 18

#define LP_LAYER_THM_W 40
#define LP_LAYER_THM_H 40

#define LW_CM_COPY_LAYER  10002				//���C���[����
#define LW_CM_DELETE_LAYER 10003			//���C���[�폜
#define LW_CM_LOCK_PIXEL_ALPHA	10004		//�s�����x�ی삷��
#define LW_CM_UNLOCK_PIXEL_ALPHA 10005		//�s�����x��ی삵�Ȃ�
#define LW_CM_CLIP_WITH_UNDER_LAYER 10006	//���̃��C���[�ŃN���b�s���O����
#define LW_CM_UNCLIP_WITH_UNDER_LAYER 10007	//���̃��C���[�ŃN���b�s���O���Ȃ�
#define LW_CM_MERGE_UNDER_LAYER 10008		//���̃��C���[�ƌ���
#define LW_CM_MERGE_LAYER_GROUP 10009		//���C���[�O���[�v������
#define LW_CM_MERGE_DISPLAY_LAYER 10010		//�\�����C���[������
#define LW_CM_ADJUST_PANEL 10100			//���C���[�p�l���ʒu�A��Ԃ��ĕ\��

class CLayerPanel :
	public ImgLayerEventListener
{
public:
	CLayerPanel(HWND hParent);
	virtual ~CLayerPanel();

	void SetLayer(IImgLayer_Ptr pLayer);
	void SetLayerNameEdit(CLayerNameEdit* pEdit);

	////////////////////////////////////////////
	/*!
		���C���[�X�V�C�x���g���ɌĂяo�����B
	*/
	void OnUpdateLayer();

	void OnChangedLayerName(const char* new_name);
	void OnChangedLayerOpacity(uint8_t new_opacity);
	void OnChangedLayerSynthFourCC(uint32_t new_fourcc);

	/////////////////////////////////////////////////////
	/*!
		�p�l����I����Ԃɂ���B
	*/
	void Select();

	////////////////////////////////////////////////
	/*!
		�p�l�����I����Ԃɂ���B
	*/
	void Newtral();

	////////////////////////////////////////////////
	/*!
		�p�l�����}�E�X�I����Ԃɂ���B
	*/
	void OnMouse();

	////////////////////////////////////////////////
	/*!
		�p�l�����}�E�X�I�t��Ԃɂ���B
	*/
	void LeaveMouse();

	//////////////////////////////////////////////////
	///*!
	//	�s�����x���Z�b�g����
	//	@param[in] alpha �s�����x
	//*/
	//void SetOpacity(int alpha);

	//////////////////////////////////////////////
	/*!
		���C���[�X�^�C���_�C�A���O���J��
	*/
	void OpenLayerStyleDialog(HWND hParent);

	//////////////////////////////////////////////
	/*!
		���C���[�폜
	*/
	void RemoveLayer();

	//////////////////////////////////////////////
	/*!
		�|�b�v�A�b�v���j���[�\��
		@param[in] hParent
		@param[in] lpt �X�N���[�����W
	*/
	virtual void PopupMenu(HWND hParent, const LPPOINT lpt);

	//////////////////////////////////////////////
	/*!
		���C���[�I�u�W�F�N�g�ւ̃|�C���^��Ԃ��B
		@return ���C���[�I�u�W�F�N�g�ւ̃|�C���^
	*/
	inline IImgLayer_Ptr GetLayer(){
		return m_Layer;
	}

	void UpdateLayerThm();


	void SetPanelRect(const LPRECT lprc);
	void GetPanelRect(LPRECT lprc);
	virtual void DrawPanel(CDC* pDC);

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

protected:
	afx_msg void OnTimer(UINT_PTR uIDEvent);

	void DrawBackGround(CDC* pDC);

	HWND m_hParentWnd;
	IImgLayer_Ptr m_Layer;

	RECT m_EyeRect;
	RECT m_LayerThmRect;
	RECT m_LayerStyleTxtRect;
	RECT m_LayerNameTxtRect;
	RECT m_LayerPanelRect;

private: 


	IplImage* m_LayerThm;

	CBitmap m_cEye;	//��
	CBitmap m_cNEye;//�s��

	CFont m_LayerNameFont;
	CLayerNameEdit* m_pLayerNameEdit;
	
	bool m_IsSelect;
	bool m_IsOnMouse;

	UINT_PTR m_nUpdateThmTimer;
	UINT_PTR m_nOffMouseTimer;
};
