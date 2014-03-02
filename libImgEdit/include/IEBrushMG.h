#ifndef _IEBRUSHMG_H_
#define _IEBRUSHMG_H_

#include "IEBrushGroup.h"
#include "IEBrushTextureMG.h"
#include "IEBrushMGEventListener.h"

typedef std::vector<IEBrushGroup_Ptr> IEBrushGroup_Vec;
typedef std::list<IEBrushMGEventListener* > IEBrushMGEventListener_List;

enum IE_BRUSH_MG_RESOURCE_ID {
	IE_BRUSH_GROUP=0x01,
};


class _EXPORTCPP IEBrushMG
{
public:
	IEBrushMG();
	~IEBrushMG();

	void SetStartDir(const char* path);

	void Init(const char* brushXmlFilePath);
	void End();

	/////////////////////////////////
	/*!
		�C�x���g���X�i�[��o�^����B
		@param[in] pListener �o�^���郊�X�i�[
	*/
	void AddEventListener(IEBrushMGEventListener*  pListener);

	//////////////////////////////////
	/*!
		�C�x���g���X�i�[���폜����
		@param[in] pListener �폜���郊�X�i�[
	*/
	void DeleteEventListener(IEBrushMGEventListener*  pListener);

	void SetSelectBrushGroup(IEBrushGroup_Ptr pBGroup);
	void SetSelectBrushGroup(int index);
	int GetSelectBrushGroupIndex();

	void AddBrushGroup(IEBrushGroup_Ptr pBGroup);
	void DeleteBrushGroup(IEBrushGroup_Ptr ppBGroup);

	////////////////////////////////
	/*!
		�u���V��I������
		@param[in] pBrush �I������u���V�̃|�C���^
	*/
	void SetSelectBrush(IEBrush_Ptr pBrush)
	{
		if (m_pSelectBrushGroup) {
			m_pSelectBrushGroup->SetSelectBrush( pBrush );
		}
	}

	////////////////////////////////
	/*!
		�u���V��I������
		@param[in] index �I������u���V�̃C���f�b�N�X
	*/
	void SetSelectBrush(int index)
	{
		IEBrush_Ptr pBrush = GetBrushAtIndex(index);
		if (pBrush) {
			SetSelectBrush(pBrush);
		}
	}

	IEBrush_Ptr GetSelectBrush() const
	{
		if(m_pSelectBrushGroup){
			return m_pSelectBrushGroup->GetSelectBrush();
		}
		return NULL;
	}

	int GetBrushSize() const
	{
		if (m_pSelectBrushGroup) {
			return m_pSelectBrushGroup->GetBrushSize();
		}
		return NULL;
	}

	////////////////////////////////
	/*!
		�C���f�b�N�X���w�肵�ău���V��Ԃ�
		@param[in] index �u���V�C���f�b�N�X
		@return �w�肳�ꂽ�u���V
	*/
	IEBrush_Ptr GetBrushAtIndex(int index) const
	{
		if (m_pSelectBrushGroup) {
			return m_pSelectBrushGroup->GetBrushAtIndex( index );
		}
		return NULL;
	}

	int GetBrushIndex(const IEBrush_Ptr pBrush) const
	{
		return m_pSelectBrushGroup->GetBrushIndex(pBrush);
	}

	int GetSelectBrushIndex() const
	{
		return m_pSelectBrushGroup->GetSelectBrushIndex();
	}

	////////////////////////////////
	/*!
		�u���V�t�@�C����ǂݍ���
		@param[in] filename �t�@�C���p�X
	*/
	void LoadBrush(const char* filename);

	//////////////////////////////
	/*!
		�u���V��ǉ�����B
		@param[in] pBruhs �ǉ�����u���V
	*/
	void AddBrush(IEBrush_Ptr pBrush)
	{
		if (m_pSelectBrushGroup) {
			return m_pSelectBrushGroup->AddBrush( pBrush );
		}
	}

	//////////////////////////////
	/*!
		�u���V���폜����B
		@param[in] pBrush�@�폜����u���V
	*/
	void DeleteBrush(IEBrush_Ptr pBrush)
	{
		if (m_pSelectBrushGroup) {
			return m_pSelectBrushGroup->DeleteBrush( pBrush );
		}
	}

	////////////////////////////////
	/*!
		�u���V�O���[�v�����R���{�{�b�N�X�ɒǉ�����B
	*/
	void SetBrushGroupComboBox(HWND hCombo) const;
	//void SetBrushGroupComboBox(CComboBox combo);

	int GetBrushTextureIndex(const char* texture_name) const {
		return m_TextureMG.GetBrushTextureIndex(texture_name);
	}

	void GetBrushTexturePath(char* dst_path, int max_path_size, int index) const {
		m_TextureMG.GetBrushTexturePath(dst_path, max_path_size, index);
	}
	
	////////////////////////////////
	/*!
		�u���V�e�N�X�`�������R���{�{�b�N�X�ɒǉ�����B
	*/
	void SetBrushTextureComboBox(HWND hCombo) const {
		m_TextureMG.SetBrushTextureComboBox(hCombo);
	}


	///////////////////////////////////
	inline void ResourceLock(unsigned int id){
		while (IsResourceLock( id ));
		m_LockResourceID |= id;
	}
	inline void ResourceUnlock(unsigned int id){
		m_LockResourceID &= (~id);
	}
	inline bool IsResourceLock(unsigned int id) const {
		return ((m_LockResourceID & id) == id);
	}

private:
	IEBrushMGEventListener_List m_IEBrushMGEventListener_List;

	IEBrushTextureMG m_TextureMG;

	IEBrushGroup_Vec m_IEBrushGroup_Vec;
	IEBrushGroup_Ptr m_pSelectBrushGroup;

	std::string m_IEStartDir;
	std::string m_BrushXmlFilePath;

	bool m_isInit;
	unsigned int m_LockResourceID;
};

#endif //_IEBRUSHMG_H_