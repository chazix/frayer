#include "stdafx.h"

#include "IEBrushGroup.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

_EXPORTCPP IEBrushGroup_Ptr CreateIEBrushGroup()
{
	return IEBrushGroup_Ptr(new IEBrushGroup);
}

//_EXPORT void ReleaseIEBrushGroup(IEBrushGroup_Ptr* ppBGroup)
//{
//	if(*ppBGroup){
//		delete (*ppBGroup);
//		(*ppBGroup) = NULL;
//	}
//}

IEBrushGroup::IEBrushGroup()
{
	m_pSelectBrush = NULL;

#ifdef _WIN32
	::InitializeCriticalSection( &m_csIEBrush );
#else
	m_LockResourceID = 0;
#endif
}

IEBrushGroup::~IEBrushGroup()
{
	//
	if( !m_IEBrush_Vec.empty() ){
		//IEBrush_Vec::iterator itr = m_IEBrush_Vec.begin();
		//for(; itr != m_IEBrush_Vec.end(); itr++){
		//	IEBrush_Ptr pBrush = (*itr);
		//	ReleaseIEBrush( &pBrush );
		//}
		m_IEBrush_Vec.clear();
	}

#ifdef _WIN32
	::DeleteCriticalSection( &m_csIEBrush );
#else
	m_LockResourceID = 0;
#endif
}

////////////////////////////////////
/*!
	XML�`���Ńu���V�O���[�v�̃f�[�^����������
*/
void IEBrushGroup::WriteBrushGroupToXml(FILE* fp, int indt)
{
	assert(indt >= 0);

	int i;
	for(i=0; i<indt; i++)
		fprintf(fp, "  ");
	fprintf(fp, "<brush_group name=\"%s\">\n", m_name);

	IEBrush_Vec::iterator itr = m_IEBrush_Vec.begin();
	for(; itr != m_IEBrush_Vec.end(); itr++){
		(*itr)->WriteBrushToXml(fp, indt + 1);
	}

	for(i=0; i<indt; i++)
		fprintf(fp, "  ");
	fprintf(fp, "</brush_group>\n");
}

/////////////////////////////
/*!
	�C�x���g���X�i�[��o�^����B
	@param[in] pListener �o�^���郊�X�i�[
*/
void IEBrushGroup::AddEventListener(IEBrushGroupEventListener*  pListener)
{
	m_IEBrushGroupEventListener_List.push_back( pListener );
}

/////////////////////////////
/*!
	�C�x���g���X�i�[���폜����B
	@param[in] pListener �폜���郊�X�i�[
*/
void IEBrushGroup::DeleteEventListener(IEBrushGroupEventListener*  pListener)
{
	IEBrushGroupEventListener_List::iterator itr = m_IEBrushGroupEventListener_List.begin();
	for(; itr != m_IEBrushGroupEventListener_List.end(); itr++){
		if((*itr) == pListener){
			m_IEBrushGroupEventListener_List.erase(itr);
			return;
		}
	}
}

void IEBrushGroup::SetSelectBrush(IEBrush_Ptr pBrush)
{
	ResourceLock( IE_BRUSH_GROUP_RESOURCE_ID::IE_BRUSH );

	//�I���u���V�؂�ւ�
	if(m_pSelectBrush){
		m_pSelectBrush->ClearBrushImgBufMap();
	}
	m_pSelectBrush = pBrush;

	ResourceUnlock( IE_BRUSH_GROUP_RESOURCE_ID::IE_BRUSH );
	
	IEBrushGroupEventListener_List::iterator itr = m_IEBrushGroupEventListener_List.begin();
	for(; itr != m_IEBrushGroupEventListener_List.end(); itr++){
		if((*itr)->IsLockIEBrushGroupEvent() == false && (*itr)->IsCalledIEBrushGroup() == false){
			//�����ɃC�x���g�Ăяo���������Ȃ��悤�Ƀt���O�𗧂Ă�
			(*itr)->SetCalledIEBrushGroup(true);
			//
			(*itr)->OnChangeSelectBrush(pBrush, m_pSelectBrush);
		}

		(*itr)->SetCalledIEBrushGroup(false);
	}
}

int IEBrushGroup::GetBrushIndex(const IEBrush_Ptr pBrush) const
{
	int index=0;
	IEBrush_Vec::const_iterator itr = m_IEBrush_Vec.begin();
	for(; itr != m_IEBrush_Vec.end(); itr++){
		if(pBrush == (*itr)){
			return index;
		}
		index++;
	}
	return -1;
}

void IEBrushGroup::AddBrush(IEBrush_Ptr pBrush)
{
	ResourceLock( IE_BRUSH_GROUP_RESOURCE_ID::IE_BRUSH );
	//�u���V�z��ɒǉ�
	m_IEBrush_Vec.push_back( pBrush );
	ResourceUnlock( IE_BRUSH_GROUP_RESOURCE_ID::IE_BRUSH );

	IEBrushGroupEventListener_List::iterator itr = m_IEBrushGroupEventListener_List.begin();
	for(; itr != m_IEBrushGroupEventListener_List.end(); itr++){
		if((*itr)->IsLockIEBrushGroupEvent() == false && (*itr)->IsCalledIEBrushGroup() == false){
			//�����ɃC�x���g�Ăяo���������Ȃ��悤�Ƀt���O�𗧂Ă�
			(*itr)->SetCalledIEBrushGroup( true );
			//
			(*itr)->OnAddBrush( pBrush );
		}

		(*itr)->SetCalledIEBrushGroup(false);
	}
}

void IEBrushGroup::DeleteBrush(IEBrush_Ptr pBrush)
{
	if(pBrush == NULL) return;

	IEBrushGroupEventListener_List::iterator itr = m_IEBrushGroupEventListener_List.begin();
	for(; itr != m_IEBrushGroupEventListener_List.end(); itr++){
		if((*itr)->IsLockIEBrushGroupEvent() == false && (*itr)->IsCalledIEBrushGroup() == false){
			//�����ɃC�x���g�Ăяo���������Ȃ��悤�Ƀt���O�𗧂Ă�
			(*itr)->SetCalledIEBrushGroup(true);
			//
			(*itr)->OnDeleteBrush(pBrush);
		}

		(*itr)->SetCalledIEBrushGroup(false);
	}

	if(m_pSelectBrush == pBrush){
		SetSelectBrush(NULL);
	}

	ResourceLock( IE_BRUSH_GROUP_RESOURCE_ID::IE_BRUSH );

	//�u���V�z�񂩂�폜
	IEBrush_Vec::iterator itr_br = m_IEBrush_Vec.begin();
	for(; itr_br != m_IEBrush_Vec.end(); itr_br++){
		if(pBrush == (*itr_br)){
			m_IEBrush_Vec.erase(itr_br);
			break;
		}
	}

	////�������J��
	//ReleaseIEBrush(ppBrush);

	ResourceUnlock( IE_BRUSH_GROUP_RESOURCE_ID::IE_BRUSH );
}