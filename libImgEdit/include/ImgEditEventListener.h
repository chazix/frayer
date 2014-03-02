#ifndef _IMGEDITEVENTLISTENER_H_
#define _IMGEDITEVENTLISTENER_H_

#include "IEColor.h"
#include "ImgFile.h"
#include "IEBrush.h"


/*!
	ImgEdit�̃C�x���g���ɌĂяo�����N���X�C���^�[�t�F�C�X
	�p�����Ďg�p����B
*/
class ImgEditEventListener {
public:
	ImgEditEventListener():
	  m_isLockEvent(false),
	  m_isCalledImgEdit(false) {};
	virtual ~ImgEditEventListener(){};

	//!	�C�x���g�Ăяo�����s��Ȃ��悤�ɂ���
	void LockImgEditEvent(){ m_isLockEvent = true; };

	//! �C�x���g�Ăяo�����s���悤�ɂ���
	void UnlockImgEditEvent(){ m_isLockEvent = false; };

	//! �C�x���g�Ăяo�����s�����ǂ���
	bool IsLockImgEditEvent(){ return m_isLockEvent; };

	//////////////////////////////
	/*!
		�I��F���ς�����Ƃ��ɌĂяo���B
		@param[in] color �ύX��̐F
	*/
	virtual void OnChangeSelectColor(IEColor color){};

	///////////////////////////////
	/*!
		�ҏW���̃t�@�C�����؂�ւ�����Ƃ��ɌĂяo���B
		@param[in] pNewFile �؂�ւ���̃t�@�C��
		@param[in] pOldFile �؂�ւ���̃t�@�C��
	*/
	virtual void OnChangeActiveFile(ImgFile_Ptr pNewFile, ImgFile_Ptr pOldFile){};

	////////////////////////////////
	/*!
		�ҏW����摜�t�@�C����ǉ�����
		@param[in] pFile �ǉ������t�@�C��
	*/
	virtual void OnAddImgFile(ImgFile_Ptr pFile){};

	//////////////////////////////////
	/*!
		�c�[�����ύX���ꂽ�Ƃ��ɌĂяo��
		@param[in] pNewTool �ύX��̃c�[�� 
		@param[in] pOldTool �ύX�O�̃c�[��
	*/
	virtual void OnChangeSelectTool(IIETool* pNewTool, IIETool* pOldTool){};

	bool IsCalledImgEdit(){ return m_isCalledImgEdit; };
	void SetCalledImgEdit(bool bl){ m_isCalledImgEdit = bl; };
private:
	bool m_isLockEvent;
	bool m_isCalledImgEdit;
};

#endif //_IMGEDITEVENTLISTENER_H_