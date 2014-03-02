#ifndef _IE_PALETTE_EVENT_LISTENER_H_
#define _IE_PALETTE_EVENT_LISTENER_H_

class IEPaletteEventListener
{
public:
	IEPaletteEventListener():
		m_isLockEvent(false),
		m_isCalledIEPalette(false) {};
	virtual ~IEPaletteEventListener(){};

	//!	�C�x���g�Ăяo�����s��Ȃ��悤�ɂ���
	void LockIEPaletteEvent(){ m_isLockEvent = true; };

	//! �C�x���g�Ăяo�����s���悤�ɂ���
	void UnlockIEPaletteEvent(){ m_isLockEvent = false; };

	//! �C�x���g�Ăяo�����s�����ǂ���
	bool IsLockIEPaletteEvent(){ return m_isLockEvent; };

	////////////////////////////////////
	/*!
		�I�����Ă���F��񂪕ς�����Ƃ��ɌĂяo��
		@param[in] new_select_node_index
	*/
	virtual void OnChangeSelectColor(int new_select_node_index){};

	////////////////////////////////////
	/*!
		�F��񂪒ǉ����ꂽ�Ƃ��ɌĂяo��
		@param[in] pNode
	*/
	virtual void OnAddColor(IEPaletteNode_Ptr pNode){};

	////////////////////////////////////
	/*!
		�F��񂪍폜���ꂽ�Ƃ��ɌĂяo��
		@parma[in] index
		@param[in] pNode
	*/
	virtual void OnDeleteColor(int index, IEPaletteNode_Ptr pNode){};

	bool IsCalledIEPalette(){ return m_isCalledIEPalette; };
	void SetCalledIEPalette(bool bl){ m_isCalledIEPalette = bl; };
private:
	bool m_isLockEvent;
	bool m_isCalledIEPalette;
};

#endif //_IE_PALETTE_EVENT_LISTENER_H_