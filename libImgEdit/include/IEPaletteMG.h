#ifndef _IE_PALETTE_MG_H_
#define _IE_PALETTE_MG_H_

#include "IEPalette.h"
#include "IEPaletteMGEventListener.h"

typedef std::vector<IEPalette_Ptr> IEPalette_Vec;
typedef std::list<IEPaletteMGEventListener* > IEPaletteMGEventListener_List;

enum IE_PALETTE_MG_RESOURCE_ID {
	IE_PALETTE=0x01,
};


class _EXPORTCPP IEPaletteMG
{
public:
	IEPaletteMG();
	~IEPaletteMG();

	void Init(const char* path);
	void End();

	/////////////////////////////////
	/*!
		�C�x���g���X�i�[��o�^����B
		@param[in] pListener �o�^���郊�X�i�[
	*/
	void AddEventListener(IEPaletteMGEventListener*  pListener);

	//////////////////////////////////
	/*!
		�C�x���g���X�i�[���폜����
		@param[in] pListener �폜���郊�X�i�[
	*/
	void DeleteEventListener(IEPaletteMGEventListener*  pListener);

	void AddPalette(IEPalette_Ptr pPalette);
	void DeletePalette(IEPalette_Ptr pPalette);

	void SetSelectPalette(IEPalette_Ptr pPalette);

	///////////////////////////////
	/*!
		index���w�肵�ăp���b�g���w�肷��
		@param[in] index
	*/
	void SetSelectPalette(int index);

	////////////////////////////////
	/*!
		�I������Ă���p���b�g���ێ����Ă���F����Ԃ�
		@return �ێ����Ă���F��
	*/
	int GetSelectPaletteSize() const {
		if( m_pSelectPalette ){
			return m_pSelectPalette->GetPaletteSize();
		}
		return -1;
	}

	//////////////////////////////
	/*!
		�F����ǉ�����B
		@param[in] pNode �ǉ�����F���
	*/
	void AddColor(const char* name, IEColor color){
		if( m_pSelectPalette ){
			m_pSelectPalette->AddColor(name, color);
		}
	}

	//////////////////////////////
	/*!
		�F�����폜����B
		@param[in] index�@�폜����F�̃C���f�b�N�X
	*/
	void DeleteColor(int index){
		if( m_pSelectPalette ){
			m_pSelectPalette->DeleteColor( index );
		}
	}

	///////////////////////////////
	/*!
		���ׂẴp���b�g�����R���{�{�b�N�X�ɒǉ�����
		@param[in] hCombo
	*/
	void SetPaletteComboBox(HWND hCombo);

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
	IEPaletteMGEventListener_List m_IEPaletteMGEventListener_List;

	IEPalette_Vec m_IEPalette_Vec;
	IEPalette_Ptr m_pSelectPalette;

	std::string m_PaletteXmlFilePath;

	bool m_isInit;
	unsigned int m_LockResourceID;
};

#endif //_IE_PALETTE_MG_H_