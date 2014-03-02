#ifndef _IE_PALETTE_H_
#define _IE_PALETTE_H_

#include "LibIEDllHeader.h"

#include "IEColor.h"

#define IE_PALETTE_NON_SELECT (-1)

typedef struct _IEPaletteNode {
	IEColor color;
	char name[ MAX_IE_PALETTE_NODE_NAME ];
} IEPaletteNode;

typedef std::shared_ptr<IEPaletteNode> IEPaletteNode_Ptr;
typedef std::shared_ptr<const IEPaletteNode> const_IEPaletteNode_Ptr;

#include "IEPaletteEventListener.h"

typedef std::vector<IEPaletteNode_Ptr> IEPaletteNode_Vec;
typedef std::list<IEPaletteEventListener* > IEPaletteEventListener_List;

enum IE_PALETTE_RESOURCE_ID {
	IE_PALETTE_NODE=0x01,
};

class _EXPORTCPP IEPalette
{
public:
	IEPalette();
	~IEPalette();

	/////////////////////////////////////
	/*!
		XML�`���Ńp���b�g�f�[�^��������
	*/
	void WritePaletteToXml(FILE* fp, int indt);

	/////////////////////////////////
	/*!
		�C�x���g���X�i�[��o�^����B
		@param[in] pListener �o�^���郊�X�i�[
	*/
	void AddEventListener(IEPaletteEventListener*  pListener);

	//////////////////////////////////
	/*!
		�C�x���g���X�i�[���폜����
		@param[in] pListener �폜���郊�X�i�[
	*/
	void DeleteEventListener(IEPaletteEventListener*  pListener);

	///////////////////////////////////////
	///*!
	//	�p���b�g�f�[�^��ǂݍ���
	//*/
	//bool LoadPalette(const char* path);

	int GetPaletteSize() const {
		return m_IEPaletteNode_Vec.size();
	}

	const_IEPaletteNode_Ptr GetColorAtIndex(int index) const {
		assert(0 <= index && index < m_IEPaletteNode_Vec.size());
		return m_IEPaletteNode_Vec[index];
	}

	//////////////////////////////////////
	/*!
		�C���f�b�N�X���w�肵�đI���m�[�h��ݒ�
	*/
	void SetSelectColor(int index);
	int GetSelectColorIndex(){
		return m_SelectPaletteNodeIndex;
	}

	//////////////////////////////////////
	/*!
		�p���b�g�ɐF����ǉ�
		@param[in] name �F�̖��O
		@param[in] color �F�f�[�^
	*/
	void AddColor(const char* name, IEColor color);

	//////////////////////////////////////
	/*!
		�C���f�b�N�X���w�肵�ĐF�����폜
	*/
	void DeleteColor(int index);

	/////////////////////////////////////
	/*!
		palette name
	*/
	inline const char* GetName() const {
		return m_name;
	}
	inline void GetName(char* dst) const {
		strcpy_s(dst, MAX_IE_PALETTE_NAME, m_name);
	}
	inline void SetName(const char* src){
		strcpy_s(m_name, MAX_IE_PALETTE_NAME, src);
	}



private:
	///////////////////////////////////
	inline void ResourceLock(unsigned int id){
#ifdef _WIN32
		switch( id ){
			case IE_PALETTE_RESOURCE_ID::IE_PALETTE_NODE:
				::EnterCriticalSection( &m_csPaletteNode );
				break;
			default:
				assert( 0 );
				break;
		}
#else
		while((m_LockResourceID & id) == id);
		m_LockResourceID |= id;
#endif //_WIN32
	}
	inline void ResourceUnlock(unsigned int id){
#ifdef _WIN32
		switch( id ){
			case IE_PALETTE_RESOURCE_ID::IE_PALETTE_NODE:
				::EnterCriticalSection( &m_csPaletteNode );
				break;
			default:
				assert( 0 );
				break;
		}
#else
		m_LockResourceID &= (~id);
#endif //_WIN32
	}
	
	//void SavePalette();

	IEPaletteEventListener_List m_IEPaletteEventListener_List;

	char m_name[MAX_IE_PALETTE_NAME];

	int m_SelectPaletteNodeIndex;
	IEPaletteNode_Ptr m_pSelectPaletteNode;

	IEPaletteNode_Vec m_IEPaletteNode_Vec;

#ifdef _WIN32
	CRITICAL_SECTION m_csPaletteNode;
#else
	unsigned int m_LockResourceID;
#endif
};

_EXPORTCPP IEPalette_Ptr CreateIEPalette();

#endif //_IE_PALETTE_H_