#ifndef _IECOMMAND_H_
#define _IECOMMAND_H_

#include "LibIEDllHeader.h"
#include "IIEGuiFrame.h"

class ImgEdit;

//class _EXPORTCPP IIECommand : public IIEModule
class _EXPORTCPP IIECommand : public IIEGuiFrame
{
public:
	IIECommand(){};
	virtual ~IIECommand(){};


	////////////////////////////////
	/*!
		�R�}���h�J�n���Ɉ�x�Ă΂��
	*/
	virtual bool Init(ImgEdit* pEdit){
		return true;
	}

	////////////////////////////////
	/*!
		�R�}���h�I�����Ɉ�x�Ă΂��
	*/
	virtual void End(ImgEdit* pEdit){}

	////////////////////////////////
	/*!
		�A�C�h��������Ăяo��
		@param[in] pEdit
		@return FALSE��Ԃ��ƃR�}���h���I������ATRUE�̏ꍇ�͌p������
	*/
	virtual BOOL Run(ImgEdit* pEdit, void* pvoid){ return FALSE; };

	//////////////////////////////////
	/*!
		�ҏW���̉摜�E�B���h�E�`�掞�ɌĂяo��
		@param[in] hdc �`�悷��f�o�C�X�R���e�L�X�g
	*/
	virtual void OnDraw(HDC hdc){};
};


#ifdef __cplusplus
#define IECOMMAND_EXPORT extern "C" __declspec (dllexport)
#else
#define IECOMMAND_EXPORT __declspec (dllexport)
#endif

IECOMMAND_EXPORT IEMODULE_ID GetIEModule_ID();

IECOMMAND_EXPORT void GetIECommandName(char* dst);
IECOMMAND_EXPORT void GetIECommandDisplayNameJa(char* dst);

IECOMMAND_EXPORT IIECommand* CreateIECommand();
IECOMMAND_EXPORT void ReleaseIECommand(IIECommand** ppCommand);

#endif //_IECOMMAND_H_