#ifndef _IE_TOOL_MG_EVENT_LISTENER_H_
#define _IE_TOOL_MG_EVENT_LISTENER_H_

class IEToolMGEventListener
{
public:
	IEToolMGEventListener():
		m_isLockEvent(false),
		m_isCalledIEToolMG(false){}
	virtual ~IEToolMGEventListener(){}

	//!	�C�x���g�Ăяo�����s��Ȃ��悤�ɂ���
	void LockIEToolMGEvent(){ m_isLockEvent = true; };

	//! �C�x���g�Ăяo�����s���悤�ɂ���
	void UnlockIEToolMGEvent(){ m_isLockEvent = false; };

	//! �C�x���g�Ăяo�����s�����ǂ���
	bool IsLockIEToolMGEvent(){ return m_isLockEvent; };

	////////////////////////////////
	/*!
		�I���c�[�����ς�����Ƃ��ɌĂяo���B
		@param[in] pNewTool �ύX��̃c�[��
		@param[in] pOldTool �ύX�O�̃c�[��
	*/
	virtual void OnChangeSelectTool(
		IIETool* pNewTool, IIETool* pOldTool){};

	bool IsCalledIEToolMG(){ return m_isCalledIEToolMG; };
	void SetCalledIEToolMG(bool bl){ m_isCalledIEToolMG = bl; };

private:
	bool m_isLockEvent;
	bool m_isCalledIEToolMG;
};

#endif //_IE_BURHS_MG_EVENT_LISTENER_H_