#ifndef _IEBRUSHGROUPEVENTLISTENRE_H_
#define _IEBRUSHGROUPEVENTLISTENRE_H_

class IEBrushGroupEventListener
{
public:
	IEBrushGroupEventListener():
		m_isLockEvent(false),
		m_isCalledIEBrushGroup(false) {};

	virtual ~IEBrushGroupEventListener(){};

	//!	�C�x���g�Ăяo�����s��Ȃ��悤�ɂ���
	void LockIEBrushGroupEvent(){ m_isLockEvent = true; };

	//! �C�x���g�Ăяo�����s���悤�ɂ���
	void UnlockIEBrushGroupEvent(){ m_isLockEvent = false; };

	//! �C�x���g�Ăяo�����s�����ǂ���
	bool IsLockIEBrushGroupEvent(){ return m_isLockEvent; };

	////////////////////////////////
	/*!
		�I���u���V���ς�����Ƃ��ɌĂяo���B
		@param[in] pNewBrush �ύX��̃u���V
		@param[in] pOldBrush �ύX�O�̃u���V
	*/
	virtual void OnChangeSelectBrush(IEBrush_Ptr pNewBrush, IEBrush_Ptr pOldBrush){};

	/////////////////////////////////
	/*!
		�u���V���ǉ����ꂽ�Ƃ��ɌĂяo��
		@param[in] pBrush �ǉ����ꂽ�u���V
	*/
	virtual void OnAddBrush(IEBrush_Ptr pBrush){};

	//////////////////////////////////
	/*!
		�u���V���폜���ꂽ�Ƃ��ɌĂяo��
		@param[in] pBrish �폜�����u���V
	*/
	virtual void OnDeleteBrush(IEBrush_Ptr pBrush){};

	bool IsCalledIEBrushGroup(){ return m_isCalledIEBrushGroup; };
	void SetCalledIEBrushGroup(bool bl){ m_isCalledIEBrushGroup = bl; };
private:
	bool m_isLockEvent;
	bool m_isCalledIEBrushGroup;
};

#endif //_IEBRUSHGROUPEVENTLISTENER_H_