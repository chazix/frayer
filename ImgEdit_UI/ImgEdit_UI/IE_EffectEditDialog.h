#ifndef _IE_EFFECTEDITDIALOG_H_
#define _IE_EFFECTEDITDIALOG_H_

#include <vector>
#include "IEffectEditDialog.h"

#define PROP_EED_WINPROC "EditWindowProc" //�N���X�����ʂ��镶����

typedef struct _TRACKBAR_DATA{
	int id;
	HWND hWnd;
	HWND hLabel;
	HWND hValLabel;
	ImgFile_Ptr file;
	int *change_val;
}TRACKBAR_DATA;

typedef std::vector<TRACKBAR_DATA*> TRACKBAR_DATA_VEC;


class IE_EffectEditDialog : public IEffectEditDialog
{
public:
	IE_EffectEditDialog();
	~IE_EffectEditDialog();

	//////////////////////////////////
	/*!
		�_�C�A���O�̕\��
		@param[in] hInst
		@param[in] hwndParent
	*/
	INT_PTR CreateDialogBox(HINSTANCE hInst, HWND hwndParent, ImgFile_Ptr file, IEffect* effect);

	//////////////////////////////////
	/*!
		�g���b�N�o�[�̒ǉ�
		@param[in] name �g���b�N�o�[�̖��O
		@param[in] min �ŏ��l
		@param[in] max �ő�l
		@param[in] start �J�n���̒l
		@param[in] change_val �l��ύX����ϐ�
	*/
	VOID AddTrackbar(const char* name, int min, int max, int start, int *change_val);

	VOID SetProgRange(int min, int max);

	VOID SetProgStep(int step);
	VOID ProgStepIt();

	VOID SetProgPos(int pos);

	//////////////////////////////////
	/*!
		�_�C�A���O�̃v���V�[�W��
	*/
	static INT_PTR CALLBACK DialogProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

private:
	//////////////////////////////////
	/*!
	*/
	VOID SetHWND(HWND hWnd);

	VOID SetDialogToEffect();

	VOID EffectEdit();

	//////////////////////////////////
	/*!
		�g���b�N�o�[�̃C�x���g��
		@param[in] hTrac �g���b�N�o�[�̃n���h��
		@param[in] wp
		@param[in] val �g���b�N�o�[�̒l
	*/
	VOID CallTrackbar(HWND hTrack, WPARAM wp, int val);

	//////////////////////////////////
	/*!
	*/
	VOID ClearDialog(); 


	HWND m_hWnd;
	HINSTANCE m_hInst;

	ImgFile_Ptr m_pEditFile;
	IEffect *m_pEditEffect;

	int m_Ctrl_Num;
	int m_IdIndex;

	TRACKBAR_DATA_VEC m_TrackBar_DataVec;

};

#endif