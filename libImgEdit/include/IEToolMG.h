#ifndef _IETOOLMG_H_
#define _IETOOLMG_H_

#include "IEToolGroup.h"
#include "IToolEditWndHandle.h"
#include "IEToolMGEventListener.h"

class ImgEdit;

typedef std::vector<IEToolGroup*> IEToolGroup_Vec;
typedef std::list<IEToolMGEventListener* > IEToolMGEventListener_List;

typedef IIETool* (*_CreateIETool)();
typedef void (*_ReleaseIETool)(IIETool*);

enum IE_TOOL_MG_RESOURCE_ID {
	IE_SELECT_TOOL=0x01,
};

class _EXPORTCPP IEToolMG
{
public:
	IEToolMG();
	~IEToolMG();

	void SetImgEdit(ImgEdit* pImgEdit){
		m_pImgEdit = pImgEdit;
	}

	void SetStartDir(const char* path);

	//////////////////////////////
	/*!
		����������
	*/
	void Init(const char* path);

	//////////////////////////////
	/*!
		�I������
	*/
	void End();

	/////////////////////////////////
	/*!
		�C�x���g���X�i�[��o�^����B
		@param[in] pListener �o�^���郊�X�i�[
	*/
	void AddEventListener(IEToolMGEventListener*  pListener);

	//////////////////////////////////
	/*!
		�C�x���g���X�i�[���폜����
		@param[in] pListener �폜���郊�X�i�[
	*/
	void DeleteEventListener(IEToolMGEventListener*  pListener);

	////////////////////////////////
	/*!
		�c�[�����w�肷��
		@param[in] id �w�肷��c�[���̃|�C���^
	*/
	void SetSelectTool(IIETool* pTool);

	////////////////////////////////
	/*!
		�I������Ă���c�[����Ԃ�
		@return �I������Ă���c�[���ւ̃|�C���^
	*/
	IIETool* GetSelectTool(){
		return m_pSelectTool;
	}

	void ReturnPrevSelectTool(){
		if(m_pPrevSelectTool){
			SetSelectTool( m_pPrevSelectTool );
		}
	}

	////////////////////////////////
	/*!
		�c�[���G�f�B�b�g�E�B���h�E�n���h�����Z�b�g
		�c�[���G�e�B�b�g�E�B���h�E�ɒǉ�����c�[���G�f�B�b�g
		�R���g���[���𑗂�B
		@param[in,out] pHandle
	*/
	void SetToolEditWndHandle(IToolEditWndHandle* pHandle);

	//////////////////////////////
	/*!
		id�ɑΉ�����c�[���I�u�W�F�N�g�̃|�C���^��Ԃ��B
		@param[in] id
		@return �c�[���I�u�W�F�N�g
	*/
	//IIETool* GetIEToolFromIndex(int group_index, int tnode_index);

	//////////////////////////////
	/*!
		�c�[�����ɑΉ�����c�[���I�u�W�F�N�g�̃|�C���^��Ԃ��B
		@param[in] tool_name �c�[����
		@return �c�[���I�u�W�F�N�g
	*/
	IIETool* GetIEToolFromName(const char* tool_name);

	////////////////////////////////
	///*!
	//	�c�[���\�����ɑΉ�����c�[���I�u�W�F�N�g�̃|�C���^��Ԃ��B
	//	@param[in] disp_name �c�[���\����
	//	@return �c�[���I�u�W�F�N�g
	//*/
	//IIETool* GetIEToolFromDisplayName(const char* disp_name);

	void AddToolGroup(IEToolGroup* pTGroup);

	///////////////////////////////
	/*!
		���c�[���O���[�v����Ԃ��B
		@return 
	*/
	int GetSizeOfToolGroup(){
		return m_IEToolGroup_Vec.size();
	}

	////////////////////////////////
	/*!
		�C���f�b�N�X���w�肵�ăc�[���O���[�v��Ԃ�
	*/
	IEToolGroup* GetToolGroupAtIndex(int index){
		return m_IEToolGroup_Vec[index];
	}

	///////////////////////////////
	/*!
		�w�肵���O���[�v�Ɋ܂܂��c�[���̐���Ԃ��B
		@param[in] group_index �O���[�v�̃C���f�b�N�X
		@return 
	*/
	int GetSizeOfToolNode(int group_index);

	/////////////////////////////////
	///*!
	//	�O���[�v�C���f�b�N�X,�c�[���m�[�h�C���f�b�N�X���w�肵
	//	���̃c�[����ID��Ԃ��B
	//	@param[in] group_index �O���[�v�̃C���f�b�N�X
	//	@param[in] tnode_index �c�[���̃C���f�b�N�X
	//	@return
	//*/
	//int GetToolID(int group_index, int tnode_index);

	///////////////////////////////
	/*!
		�O���[�v�C���f�b�N�X,�c�[���m�[�h�C���f�b�N�X���w�肵
		���̃c�[���̕\������dst�ɏ�������
		@param[in] group_index �O���[�v�̃C���f�b�N�X
		@param[in] tnode_index �c�[���̃C���f�b�N�X
		@param[in] dst �o�͐�A�h���X
		@return
	*/
	bool GetToolName(int group_index, int tnode_index, char* dst);

	///////////////////////////////
	/*!
		�c�[�������擾����
		@param[in] group_index �O���[�v�̃C���f�b�N�X
		@param[in] tnode_index �c�[���̃C���f�b�N�X
		@param[in] dst �o�͐�A�h���X
		@return
	*/
	bool GetToolName(const char* display_name, char* dst);

	///////////////////////////////
	/*!
		�O���[�v�C���f�b�N�X,�c�[���m�[�h�C���f�b�N�X���w�肵
		���̃c�[���̕\������dst�ɏ�������
		@param[in] group_index �O���[�v�̃C���f�b�N�X
		@param[in] tnode_index �c�[���̃C���f�b�N�X
		@param[in] dst �o�͐�A�h���X
		@return
	*/
	bool GetDisplayName(int group_index, int tnode_index, char* dst);

	/////////////////////////////////
	/*!
		�c�[���̕\�������擾����
		@param[in] tool_name �擾����c�[���̖��O���w�肷��
		@param[in] dst �o�͐�A�h���X
	*/
	bool GetDisplayName(const char* tool_name, char* dst);

	///////////////////////////////////
	inline void ResourceLock(unsigned int id){
		while(IsResourceLock( id ));
		m_LockResourceID |= id;
	}
	inline void ResourceUnlock(unsigned int id){
		m_LockResourceID &= (~id);
	}
	inline bool IsResourceLock(unsigned int id) const {
		return ((m_LockResourceID & id) == id);
	}
private:

	void MakeDefaultToolXmlFile(const char* tool_xml_path);

	IEToolMGEventListener_List m_IEToolMGEventListener_List;

	std::string m_IEStartDir;
	std::string m_ToolXmlFilePath;

	ImgEdit* m_pImgEdit;
	IIETool* m_pSelectTool;
	IIETool* m_pPrevSelectTool;

	IToolEditWndHandle* m_pIToolEditWndHandle;

	IEToolGroup_Vec m_IEToolGroup_Vec;

	bool m_isInit;
	unsigned int m_LockResourceID;
};

#endif //_IETOOLMG_H_