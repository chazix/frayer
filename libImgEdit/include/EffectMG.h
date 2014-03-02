#ifndef _EFFECTMG_H_
#define _EFFECTMG_H_

#include <map>

#include "IEffect.h"

typedef std::map<std::string, std::string> EffectMap;
typedef std::shared_ptr<EffectMap> EffectMap_Ptr;
typedef std::map<std::string, EffectMap_Ptr> EffectGroupMap;
typedef std::map<IEffect*, HANDLE> CreateEffectMap;

typedef IEffect* (*_CreateIEEffect)();
typedef void (*_ReleaseIEEffect) (IEffect**);
typedef void (*_GetEffectDisplayName)(char*);

class _EXPORTCPP EffectMG {
public:

	void SetEffectDir(const char* path);
	void SetEffectDat(const char* path);

	void SetStartDir(const char* path);

	void Init(const char* path);
	void End();

	//////////////////////////////
	/*!
		�c���[�r���[�ɃA�C�e����ǉ�����B
		@param[in] hTree �c���[�r���[�̃n���h��
	*/
	void SetEditTree(HWND hTree);

	///////////////////////////////
	/*!
		�c���[�r���[�ɃA�C�e����ǉ�����B
		@param[in] pTree �c���[�r���[�R���g���[���N���X
	*/
	//static void SetEditTree(CTreeCtrl *pCTree);

	///////////////////////////////
	/*!
		�G�t�F�N�g�̖��O��^���A�G�t�F�N�g�I�u�W�F�N�g�𐶐����|�C���^��Ԃ��B
		���������I�u�W�F�N�g�� EffectMG::ReleaseEffect() �ŊJ������
		@param[in] effect_name �G�t�F�N�g��
	*/
	IEffect* const CreateEffect(const char* effect_name);

	///////////////////////////////
	/*!
		���������G�t�F�N�g�̊J��
		@param[in] pEffect �J������G�t�F�N�g
	*/
	void ReleaseEffect(IEffect *pEffect);

private:

	void MakeDefaultEffectlXmlFile(const char* effect_xml_path);

	std::string m_IEStartDir;
	std::string m_EffectXmlFilePath;

	EffectGroupMap m_EffectGroupMap;
	CreateEffectMap m_CreateEffectMap; //���������G�t�F�N�g�ƃ��W���[���̃}�b�v
};

#endif
