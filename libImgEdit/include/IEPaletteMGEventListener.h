#ifndef _IE_PALETTE_MG_EVENT_LISTENER_H_
#define _IE_PALETTE_MG_EVENT_LISTENER_H_

class IEPaletteMGEventListener
{
public:
	IEPaletteMGEventListener():
		m_isLockEvent(false),
		m_isCalledIEPaletteMG(false) {};
	virtual ~IEPaletteMGEventListener(){};

	//!	�C�x���g�Ăяo�����s��Ȃ��悤�ɂ���
	void LockIEPaletteMGEvent(){ m_isLockEvent = true; };

	//! �C�x���g�Ăяo�����s���悤�ɂ���
	void UnlockIEPaletteMGEvent(){ m_isLockEvent = false; };

	//! �C�x���g�Ăяo�����s�����ǂ���
	bool IsLockIEPaletteMGEvent(){ return m_isLockEvent; };

	////////////////////////////////////
	/*!
		�p���b�g���ǉ����ꂽ�Ƃ��ɌĂяo��
		@param[in] pPalette
	*/
	virtual void OnAddPalette(IEPalette_Ptr pPalette){};

	////////////////////////////////////
	/*!
		�p���b�g���폜���ꂽ�Ƃ��ɌĂяo��
		@param[in] pPalette
	*/
	virtual void OnDeletePalette(IEPalette_Ptr pPalette){};

	/////////////////////////////////////
	/*!
		�I���p���b�g���ύX���ꂽ�Ƃ��ɌĂяo��
		@param[in] pNewPalette �ύX��̃p���b�g
		@param[in] pOldPalette �ύX�O�̃p���b�g
	*/
	virtual void OnChangeSelectPalette(IEPalette_Ptr pNewPalette, IEPalette_Ptr pOldPalette){};

	bool IsCalledIEPaletteMG(){ return m_isCalledIEPaletteMG; };
	void SetCalledIEPaletteMG(bool bl){ m_isCalledIEPaletteMG = bl; };
private:
	bool m_isLockEvent;
	bool m_isCalledIEPaletteMG;
};

#endif //_IE_PALETTE_EVENT_LISTENER_H_