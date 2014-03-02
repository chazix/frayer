#ifndef _IMGLAYEREVENTLISTENER_H_
#define _IMGLAYEREVENTLISTENER_H_

#include "ImgUpdateData.h"

class ImgLayerEventListener {
public:
	ImgLayerEventListener():
	  m_isLockEvent(false),
	  m_isCalledImgLayer(false) {};
	virtual ~ImgLayerEventListener() {};

	//!	�C�x���g�Ăяo�����s��Ȃ��悤�ɂ���
	void LockLayerEvent(){ m_isLockEvent = true; };

	//! �C�x���g�Ăяo�����s���悤�ɂ���
	void UnlockLayerEvent(){ m_isLockEvent = false; };

	//! �C�x���g�Ăяo�����s�����ǂ���
	bool IsLockLayerEvent(){ return m_isLockEvent; };

	bool IsCalledImgLayer(){ return m_isCalledImgLayer; };
	void SetCalledImgLayer(bool bl){ m_isCalledImgLayer = bl; };

	//////////////////////////////
	/*!
		���C���[���X�V���ꂽ�Ƃ��ɌĂяo���B
	*/
	virtual void OnUpdateLayer(){};

	///////////////////////////////
	/*!
	*/
	virtual void OnImgLayerPushUpdateData(const LPUPDATE_DATA data){};

	virtual void OnChangedLayerName(const char* new_name){}
	virtual void OnChangedLayerOpacity(uint8_t new_opacity){}
	virtual void OnChangedLayerSynthFourCC(uint32_t new_fourcc){}
private:
	bool m_isLockEvent;
	bool m_isCalledImgLayer;
};

#endif //_IMGLAYEREVENTLISTENER_H_