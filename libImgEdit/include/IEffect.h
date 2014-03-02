#ifndef _IEFFECT_H_
#define _IEFFECT_H_

#include "LibIEDllHeader.h"

#include "IEModules.h"
#include "IIECommand.h"
#include "IEffectEditDialog.h"
#include "ImgFile.h"

class _EXPORTCPP IEffect
{
public:
	IEffect(){};
	virtual ~IEffect(){};

	////���O��Ԃ�
	//virtual std::string GetName() = 0;

	////////////////////////////////
	/*!
		�G�t�F�N�g�J�n���Ɉ�x�Ă΂��
	*/
	virtual bool Init(ImgFile_Ptr wpFile){
		return true;
	}
	////////////////////////////////
	/*!
		�G�t�F�N�g�I�����Ɉ�x�Ă΂��
	*/
	virtual void End(ImgFile_Ptr pFile){};

	//���C���[�ɃG�t�F�N�g��������B
	virtual void Edit(ImgFile_Ptr pFile) = 0;
	
	//�G�f�B�b�g�E�B���h�E��ݒ肷��
	virtual void SetEditDialog(IEffectEditDialog *pEditDialog) = 0;

private:
	std::string name;

};

#ifdef __cplusplus
#define EFFECT_EXPORT extern "C" __declspec (dllexport)
#else
#define EFFECT_EXPORT __declspec (dllexport)
#endif

EFFECT_EXPORT IEffect* CreateIEEffect();
EFFECT_EXPORT void ReleaseIEEffect(IEffect** ppEffect);
EFFECT_EXPORT IEMODULE_ID GetIEModule_ID();

EFFECT_EXPORT void GetEffectDisplayNameJa(char* dst);

#endif //_IEFFECT_H_
