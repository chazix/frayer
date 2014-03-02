#ifndef _IMGFILEEVENTLISTENER_H_
#define _IMGFILEEVENTLISTENER_H_

#include "IImgLayer.h"

/*!
	ImgFile�̃C�x���g���ɌĂяo�����N���X�C���^�[�t�F�C�X
	�p�����Ďg�p����B
*/
class ImgFileEventListener {
public:
	ImgFileEventListener():
	  m_isLockEvent(false),
	  m_isCalledImgFile(false) {};
	virtual ~ImgFileEventListener(){};

	//!	�C�x���g�Ăяo�����s��Ȃ��悤�ɂ���
	void LockFileEvent(){ m_isLockEvent = true; };

	//! �C�x���g�Ăяo�����s���悤�ɂ���
	void UnlockFileEvent(){ m_isLockEvent = false; };

	//! �C�x���g�Ăяo�����s�����ǂ���
	bool IsLockFileEvent(){ return m_isLockEvent; };

	bool IsCalledImgFile(){ return m_isCalledImgFile; };
	void SetCalledImgFile(bool bl){ m_isCalledImgFile = bl; };

	////////////////////////////
	/*!
		���C���[���ǉ����ꂽ��ɌĂяo���B
		@param[in] pLayer �ǉ�����郌�C���[
		@param[in] index �ǉ����ꂽ���C���[�̃C���f�b�N�X
	*/
	virtual void OnAddLayer(IImgLayer_Ptr pLayer, int index){};

	/////////////////////////////
	/*!
		���C���[���폜����O�ɌĂяo��
		@param[in] pLyaer �폜����郌�C���[
		@param[in] index �폜���ꂽ���C���[�̃C���f�b�N�X
	*/
	virtual void OnDeleteLayer(IImgLayer_Ptr pLayer, int index){};

	virtual void OnChangeLayerLine(int from_index, int to_index){};

	//////////////////////////////
	/*!
		�I�����C���[���ς�����Ƃ��ɌĂяo��
		@param[in] pLayer �I�����ꂽ���C���[
	*/
	virtual void OnSelectLayer(IImgLayer_weakPtr pLayer){};

	////////////////////////////
	/*!
		�}�X�N���ǉ����ꂽ��ɌĂяo���B
		@param[in] pMask �ǉ������}�X�N
		@param[in] index �ǉ����ꂽ�}�X�N�̃C���f�b�N�X
	*/
	virtual void OnAddMask(ImgMask* pMask, int index){};

	/////////////////////////////
	/*!
		�}�X�N���폜����O�ɌĂяo��
		@param[in] pMask �폜�����}�X�N
		@param[in] index �폜���ꂽ�}�X�N�̃C���f�b�N�X
	*/
	virtual void OnDeleteMask(ImgMask* pMask, int index){};

	//////////////////////////////
	/*!
		�I���}�X�N���ς�����Ƃ��ɌĂяo��
		@param[in] pMask �I�����ꂽ�}�X�N
	*/
	virtual void OnSelectMask(ImgMask* pMask){};

	//////////////////////////////
	/*!
		�\���ʒu���ς�����Ƃ��ɌĂяo��
		@param[in] size �\���ʒu
	*/
	virtual void OnChangeViewPos(const CvPoint2D64f *pos){};

	//////////////////////////////
	/*!
		�\���T�C�Y���ς�����Ƃ��ɌĂяo��
		@param[in] size �\���T�C�Y
	*/
	virtual void OnChangeViewSize(int size){};

	///////////////////////////////
	/*!
		�\���p�x���ς�����Ƃ��ɌĂяo��
		@param[in] rot �\���p�x
	*/
	virtual void OnChangeViewRot(int rot){};

	///////////////////////////////
	/*!
		�\���̍��E���]���؂�ւ�����Ƃ��ɌĂяo��
		@param[in] state
	*/
	virtual void OnChangeViewFlipH(double state){};

	///////////////////////////////
	/*!
		�\���p�x���ς�����Ƃ��ɌĂяo��
		@param[in] state
	*/
	virtual void OnChangeViewFlipV(double state){};

	///////////////////////////////
	/*!
	*/
	virtual void OnUpdate(){};

	///////////////////////////////
	/*!
		�t�@�C���n���h���A�ǉ����ɌĂяo��
	*/
	virtual void OnAddUndoImgFileHandle(const ImgFileHandle* pHandle){};
	///////////////////////////////
	/*!
		�Â��t�@�C���n���h���A�폜���ɌĂяo��
	*/
	virtual void OnRemoveOldUndoImgFileHandle(int num){};

	virtual void OnUndoImgFileHandle(const ImgFileHandle* pHandle){};
	virtual void OnRedoImgFileHandle(const ImgFileHandle* pHandle){};

	///////////////////////////////
	/*!
	*/
	virtual void OnImgFilePushUpdateData(const LPUPDATE_DATA data){};

private:
	bool m_isLockEvent;
	bool m_isCalledImgFile;
};

#endif //_IMGFILEEVENTLISTENER_H_