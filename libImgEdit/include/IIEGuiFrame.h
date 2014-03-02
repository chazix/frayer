#ifndef _IIEGUIFRAME_H_
#define _IIEGUIFRAME_H_

#include "LibIEDllHeader.h"

#include "InputData.h"
#include "IEKeyCord.h"
#include "IEModules.h"

enum IE_INPUT_DATA_FLAG{
	IMAGE_POS,			//�摜��̍��W
	WINDOW_POS,			//�E�B���h�E�N���C�A���g���W
};

//GUI�C�x���g�Ăяo���p�C���^�[�t�F�[�X
class _EXPORTCPP IIEGuiFrame : public IIEModule
{
public:
	IIEGuiFrame(){};
	virtual ~IIEGuiFrame(){};

	///////////////////////////////
	/*!
		���͂̕�ԁA�␳�����邩���Ȃ�����Ԃ��B
		true�̏ꍇ�͂���Afalse�̏ꍇ�͂��Ȃ�
		�f�t�H���g�ł�false
	*/
	virtual bool isRevise(){ return false; }

	////////////////////////////////
	/*!
		���͂��Ԃ���ꍇ�̓_�̊Ԋu(pixel�P�ʂ�)
		�f�t�H���g�ł�1.0
	*/
	virtual double GetReviseStepSize(const LPIE_INPUT_DATA){ return 1.0; }

	////////////////////////////////
	/*!
		�}�E�X�̓��͍��W�Ƃ��đ����Ă�����̍��W�n�����߂�B
		�f�t�H���g�ł̓E�B���h�E�̃N���C�A���g���W
	*/
	virtual IE_INPUT_DATA_FLAG GetInputDataFlag(){ return WINDOW_POS; }

	//////////////////////////////////
	/*!
		�ҏW���̉摜�E�B���h�E�`�掞�ɌĂяo��
		@param[in] hdc �`�悷��f�o�C�X�R���e�L�X�g
	*/
	virtual void OnDraw(HDC hdc){};

	//////////////////////////////////
	/*!
		�}�E�X���������Ƃ��ɌĂяo��
		@param[in] lpd �}�E�X���
	*/
	virtual void OnMouseMove(UINT nFlags, const LPIE_INPUT_DATA lpd){};

	//////////////////////////////////
	/*!
		�}�E�X���{�^���������ɌĂяo���B
		@param[in] lpd �}�E�X���
	*/
	virtual void OnMouseLButtonDown(UINT nFlags, const LPIE_INPUT_DATA lpd){};
	
	//////////////////////////////////
	/*!
		�}�E�X���{�^���𗣂������ɌĂяo���B
		@param[in] lpd �}�E�X���
	*/
	virtual void OnMouseLButtonUp(UINT nFlags, const LPIE_INPUT_DATA lpd){};

	///////////////////////////////////
	/*!
		�}�E�X���{�^���h���b�O���ɌĂяo���B
		@param[in] lpd �}�E�X���
	*/
	virtual void OnMouseLDrag(UINT nFlags, const LPIE_INPUT_DATA lpd){};
	
	///////////////////////////////////
	/*!
		�}�E�X�E�{�^���������ɌĂяo��
		@param[in] lpd �}�E�X���
	*/
	virtual void OnMouseRButtonDown(UINT nFlags, const LPPOINT lpt){};

	//////////////////////////////////
	/*!
		�}�E�X�E�{�^���h���b�O���ɌĂяo���B
	*/
	virtual void OnMouseRDrag(UINT nFlags, const LPPOINT lpt){};

	//////////////////////////////////
	/*!
		�}�E�X���{�^���𗣂������ɌĂяo���B
		@param[in] lpd �}�E�X���
	*/
	virtual void OnMouseRButtonUp(UINT nFlags, const LPPOINT lpt){};

	////////////////////////////////
	/*!
		�L�[�{�[�h�̃L�[�������ꂽ�Ƃ��ɌĂяo��
		@param[in] nChar �����ꂽ�L�[�̉��z�R�[�h
		@param[in] optChar �����ɉ�����Ă���Ctrl,Shift�L�[�Ȃǂ̏��
	*/
	virtual void OnKeyboardDown(UINT nChar, UINT optChar){};
};

#endif //_IIEGUIFRAME_H_