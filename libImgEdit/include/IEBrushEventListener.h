#ifndef _IEBRUSHEVENTLISTENER_H_
#define _IEBRUSHEVENTLISTENER_H_

class IEBrushEventListener {
public:
	IEBrushEventListener():
	  m_isLockBrushEvent(false),
	  m_isCalledIEBrush(false) {};
	virtual ~IEBrushEventListener(){};

	//! �C�x���g�Ăяo�����s��Ȃ��悤�ɂ���
	void LockBrushEvent(){ m_isLockBrushEvent = true; };

	//! �C�x���g�Ăяo�����s���悤�ɂ���
	void UnlockBrushEvent(){ m_isLockBrushEvent = false; };

	//! �C�x���g�Ăяo�����s�����ǂ���
	bool IsLockBrushEvent(){ return m_isLockBrushEvent; };

	bool IsCalledIEBrush(){ return m_isCalledIEBrush; };
	void SetCalledIEBrush(bool bl){ m_isCalledIEBrush = bl; };

	////////////////////////
	/*!
		�u���V�T�C�Y���ς�����Ƃ��ɌĂяo��
	*/
	virtual void OnChangeRadius(int old_rad, int new_rad){};

	virtual void OnChangeOrgRadius(int old_rad, int new_rad){};

	virtual void OnChangeBrushShape(){};
	virtual void OnChangeBrushStroke(){};
private:
	bool m_isLockBrushEvent;
	bool m_isCalledIEBrush;
};

#endif //_IEBRUSHEVENTLISTENER_H_