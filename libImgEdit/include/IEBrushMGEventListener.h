#ifndef _IE_BRUSH_MG_EVENT_LISTENER_H_
#define _IE_BRUSH_MG_EVENT_LISTENER_H_

class IEBrushMGEventListener
{
public:
	IEBrushMGEventListener():
		m_isLockEvent(false),
		m_isCalledIEBrushMG(false){}
	virtual ~IEBrushMGEventListener(){}

	//!	�C�x���g�Ăяo�����s��Ȃ��悤�ɂ���
	void LockIEBrushMGEvent(){ m_isLockEvent = true; };

	//! �C�x���g�Ăяo�����s���悤�ɂ���
	void UnlockIEBrushMGEvent(){ m_isLockEvent = false; };

	//! �C�x���g�Ăяo�����s�����ǂ���
	bool IsLockIEBrushMGEvent(){ return m_isLockEvent; };

	////////////////////////////////
	/*!
		�I���u���V�O���[�v���ς�����Ƃ��ɌĂяo���B
		@param[in] pNewBrushGruop �ύX��̃u���V�O���[�v
		@param[in] pOldBrushGroup �ύX�O�̃u���V�O���[�v
	*/
	virtual void OnChangeSelectBrushGroup(
		IEBrushGroup_Ptr pNewBrushGroup, IEBrushGroup_Ptr pOldBrushGroup){};

	/////////////////////////////////
	/*!
		�u���V�O���[�v���ǉ����ꂽ�Ƃ��ɌĂяo��
		@param[in] pBGroup to add brush group
	*/
	virtual void OnAddBrushGroup(IEBrushGroup_Ptr pBGroup){};

	//////////////////////////////////
	/*!
		�u���V�O���[�v���폜���ꂽ�Ƃ��ɌĂяo��
		@param[in] pBGroup to delete brush group
	*/
	virtual void OnDeleteBrushGroup(IEBrushGroup_Ptr pBGroup){};

	bool IsCalledIEBrushMG(){ return m_isCalledIEBrushMG; };
	void SetCalledIEBrushMG(bool bl){ m_isCalledIEBrushMG = bl; };

private:
	bool m_isLockEvent;
	bool m_isCalledIEBrushMG;
};

#endif //_IE_BURHS_MG_EVENT_LISTENER_H_