#ifndef _IEBRUSHGROUP_H_
#define _IEBRUSHGROUP_H_

#include "IEBrush.h"
#include "IEBrushGroupEventListener.h"

typedef std::vector<IEBrush_Ptr> IEBrush_Vec;
typedef std::list<IEBrushGroupEventListener* > IEBrushGroupEventListener_List;

enum IE_BRUSH_GROUP_RESOURCE_ID {
	IE_BRUSH=0x01,
};

class _EXPORTCPP IEBrushGroup
{
public:
	IEBrushGroup();
	~IEBrushGroup();

	////////////////////////////////////
	/*!
		XML�`���Ńu���V�O���[�v�̃f�[�^����������
	*/
	void WriteBrushGroupToXml(FILE* fp, int indt);

	/////////////////////////////////
	/*!
		�C�x���g���X�i�[��o�^����B
		@param[in] pListener �o�^���郊�X�i�[
	*/
	void AddEventListener(IEBrushGroupEventListener*  pListener);

	//////////////////////////////////
	/*!
		�C�x���g���X�i�[���폜����
		@param[in] pListener �폜���郊�X�i�[
	*/
	void DeleteEventListener(IEBrushGroupEventListener*  pListener);

	void SetSelectBrush(IEBrush_Ptr pBrush);

	IEBrush_Ptr GetSelectBrush() const {
		return m_pSelectBrush;
	}

	int GetBrushSize() const {
		return m_IEBrush_Vec.size();
	}

	IEBrush_Ptr GetBrushAtIndex(int index) const {
		if (0 <= index && index < m_IEBrush_Vec.size()) {
			return m_IEBrush_Vec[ index ];
		}
		return NULL;
	}

	/////////////////////////////////////
	/*!
		pBrush�̃C���f�b�N�X��Ԃ��B
		pBrush���O���[�v�ɑ��݂��Ȃ��ꍇ��-1��Ԃ�
		@param[in] pBrush �C���f�b�N�X�𒲂ׂ�u���V�ւ̃|�C���^
	*/
	int GetBrushIndex(const IEBrush_Ptr pBrush) const;

	int GetSelectBrushIndex() const {
		return GetBrushIndex(m_pSelectBrush);
	}

	void AddBrush(IEBrush_Ptr pBrush);
	void DeleteBrush(IEBrush_Ptr pBrush);

	//////////////////////////////
	/*!
		brush group name
	*/
	inline const char* GetName() const {
		return m_name;
	}
	inline void GetName(char* dst) const {
		strcpy_s(dst, MAX_IE_BRUSH_GROUP_NAME, m_name);
	}
	inline void SetName(const char* src){
		strcpy_s(m_name, MAX_IE_BRUSH_GROUP_NAME, src);
	}



private:
	///////////////////////////////////
	inline void ResourceLock(unsigned int id){
#ifdef _WIN32
		switch( id ){
			case IE_BRUSH_GROUP_RESOURCE_ID::IE_BRUSH:
				::EnterCriticalSection( &m_csIEBrush );
				break;
			default:
				assert( 0 );
				break;
		}
#else
		while((m_LockResourceID & id) == id); //is_lock ?
		m_LockResourceID |= id;
#endif //_WIN32
	}
	inline void ResourceUnlock(unsigned int id){
#ifdef _WIN32
		switch( id ){
			case IE_BRUSH_GROUP_RESOURCE_ID::IE_BRUSH:
				::LeaveCriticalSection( &m_csIEBrush );
				break;
			default:
				assert( 0 );
				break;
		}
#else
		m_LockResourceID &= (~id);
#endif
	}


	IEBrushGroupEventListener_List m_IEBrushGroupEventListener_List;

	char m_name[MAX_IE_BRUSH_GROUP_NAME];
	IEBrush_Vec m_IEBrush_Vec;
	IEBrush_Ptr m_pSelectBrush;

#ifdef _WIN32
	CRITICAL_SECTION m_csIEBrush;
#else
	unsigned int m_LockResourceID;
#endif //_WIN32
};

_EXPORTCPP IEBrushGroup_Ptr CreateIEBrushGroup();
//_EXPORT void ReleaseIEBrushGroup(IEBrushGroup_Ptr* ppBGroup);

#endif //_IEBRUSHGROUP_H_