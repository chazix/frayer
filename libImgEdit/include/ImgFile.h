#ifndef _IMGFILE_H_
#define _IMGFILE_H_

#include "LibIEDllHeader.h"

#include "IEffect.h"
#include "IIETool.h"
#include "IEColor.h"
#include "ImgUpdateData.h"
#include "ImgLayer.h"
#include "ImgLayerGroup.h"
#include "ImgMask.h"
#include "ImgFileHandleMG.h"

#include <list>
#include <string>
#include <vector>

#define IMG_FILE_COUNT_TIME //

//typedef std::vector<ImgMask*> ImgMask_Vec;
typedef std::list<ImgFileHandle*> ImgFileHandle_List;
typedef std::list<ImgFileEventListener* > ImgFileEventListener_List;

_EXPORTCPP ImgFile_Ptr CreateIEImgFile();
//_EXPORT void ReleaseIEImgFile(ImgFile_Ptr* ppFile);

enum{
	MASK_DOT_LINE_UPDATE_TIME = 1000,
	MASK_DOT_LINE_SIZE = 3,
};

enum IE_MASK_DRAW_ID {
	MASK_SOLID_DRAW,
	MASK_DOT_LINE_DRAW,
};

enum IE_FILE_RESOURCE_ID {
	IE_DRAW_IMG=0x01,
	IE_DRAW_MASK_IMG=0x02,
	IE_DISPLAY_IMG=0x04,
	IE_UPDATE_DATA=0x08,
	IE_LAYER=0x010,
	IE_MASK=0x020,
	IE_EVENTLISTENER=0x040,
};


//�ЂƂ̉摜�t�@�C����\���N���X
class _EXPORTCPP ImgFile :
	public std::enable_shared_from_this<ImgFile>
{
public:
	ImgFile();
	virtual ~ImgFile();

	/////////////////////////////////
	/*!
		�C�x���g���X�i�[��o�^����B
		@param[in] pListener �o�^���郊�X�i�[
	*/
	void AddEventListener(ImgFileEventListener*  pListener);

	//////////////////////////////////
	/*!
		�C�x���g���X�i�[���폜����
		@param[in] pListener �폜���郊�X�i�[
	*/
	void DeleteEventListener(ImgFileEventListener*  pListener);

	//
	inline bool IsInit(){
		return isInit;
	}

	//////////////////////////////////////
	/*
		����������
		�摜�T�C�Y��^���ď�����
	*/
	bool Init(CvSize size);

	//////////////////////////////////////
	/*
		����������
		�摜�T�C�Y��^���ď�����
	*/
	bool InitCanvasSize(CvSize size);

	void Close();

	//////////////////////////////////////
	/*!
		�w�i�F��ݒ肷��B
		@param[in] color �ݒ肷��F
	*/
	void SetBGColor(IEColor color){
		m_BackGroundColor = color;
	}

	void SetNumOfHistory(unsigned int num);

	//////////////////////////////////////
	/*!
		�t�@�C���̃p�X���Z�b�g����B
		@param[in] path �p�X
	*/
	void SetPath(const char* path);

	//////////////////////////////////////
	/*!
		�t�@�C���̃p�X��Ԃ��B
		@return �p�X��Ԃ�
	*/
	std::string GetPath() const
	{
		return m_path;
	}

	///////////////////////////////////////
	/*!	
		�}�X�N�̕\���@��ݒ肷��B
		@param[in] id �\���@������id
	*/
	void SetMaskDrawType(int id)
	{
		m_MaskDrawType = id;
	}

	///////////////////////////////////////
	/*!	
		�}�X�N�̕\���@��ID��Ԃ��B
		@return id �\���@������id
	*/
	int GetMaskDrawType() const
	{
		return m_MaskDrawType;
	}

	///////////////////////////////////////
	/*!
		�摜�̃T�C�Y��Ԃ�
		@return �T�C�Y��Ԃ�
	*/
	CvSize GetImgSize() const
	{
		return m_size;
	}

	//////////////////////////////////////
	/*!
		�摜���`�悳��Ă���E�B���h�E�̃N���C�A���g�͈̔͂�
		�X�N���[�����W�œn���B
		@param[out] lrect
	*/
	void GetDrawWndClientRect(LPRECT lrect) const;

	ImgFileHandle* CreateImgFileHandle(int handle_id) {
		return ImgFileHandleMG::CreateImgFileHandle(this->shared_from_this(), handle_id);
	}
	void ReleaseImgFileHandle(ImgFileHandle* pHandle) {
		ImgFileHandleMG::ReleaseImgFileHandle(pHandle);
	}

	/////////////////////////////////////
	/*!
		�t�@�C������N���X��K�p���A���X�g�ɒǉ�
		@param[in] pHandle �t�@�C������N���X
		@return
	*/
	bool DoImgFileHandle(ImgFileHandle* pHandle);

	/////////////////////////////////////
	/*!
		������i�߂�
		@param[in] i �߂������̌�
	*/
	void Undo(int i);

	//////////////////////////////////////
	/*!
		������i�i�߂�
		@param[in] i �i�߂鏈���̌�
	*/
	void Redo(int i);

	size_t GetUndoHandleSize();
	const ImgFileHandle* GetUndoHandleAt(int index);

	size_t GetRedoHandleSize();
	const ImgFileHandle* GetRedoHandleAt(int index);

	/////////////////////////////////////
	/*!
		�摜�X�V�f�[�^���L��
		@param[in] data �摜�X�V�f�[�^�t���O
	*/
	LPUPDATE_DATA CreateUpdateData();

	/////////////////////////////////////
	/*!
		�摜�X�V�f�[�^���L��
		@param[in] data �摜�X�V�f�[�^�t���O
	*/
	void PushUpdateData(LPUPDATE_DATA data);

	//////////////////////////////////////
	/*!
		�摜�X�V�f�[�^���폜
	*/
	void ClearUpdateData();

	/////////////////////////////////////
	/*!
		�t���O�������Ă�����̂̃C���[�W���Đ�������
		@param[in] enable_lprc �X�V�ł���͈́ANULL�Ȃ�S�͈͍X�V
		@param[out] updated_lprc �X�V�����͈�
		@return �X�V������Ȃ�true,�����łȂ��Ȃ�false
	*/
	bool Update(const LPRECT enable_lprc, LPRECT updated_lprc);

	void LockUpdateData();
	void UnlockUpdateData();

	///////////////////////////////////////
	/*!
		�o�͉摜���R�s�[����B
		@param[in] image �R�s�[�� 4ch BGRA
	*/
	void CopyFileImage(IplImage *image);
	void CopyFileImageThumbnail(IplImage *image);

	///////////////////////////////////////
	/*!
		�}�X�N���R�s�[����B
		@param[in] start_x
		@param[in] start_y
		@param[in] width
		@param[in] height
		@param[in] dst
		@param[in] dst_x
		@param[in] dst_y
	*/
	void CopyFileMask(int start_x, int start_y, int width, int height,
		IplImage* dst, int dst_x, int dst_y);
	void CopyFileMask(int start_x, int start_y, int width, int height,
		IplImageExt* dst, int dst_x, int dst_y);

	/////////////////////////////////
	/*!
		�\���T�C�Y���Z�b�g����(1�`800%)
		@param[in] size �\���T�C�Y
		@param[in] lpt ���S���W
	*/
	void SetViewSize(int size);
	void SetViewSize(int size, LPPOINT lpt);
	void UpViewSize();
	void DownViewSize();

	/////////////////////////////////
	/*!
		�\���T�C�Y��Ԃ�
		@return �\���T�C�Y
	*/
	int GetViewSize() const {
		return m_view_size;
	}

	/////////////////////////////////
	/*!
		�\���摜��]�p���Z�b�g����B
		@param[in] rot ��]�p (0 ~ 360)
	*/
	void SetViewRot(double rot);

	//////////////////////////////////
	/*!
		�\���摜�̉�]�p��Ԃ��B
		@return ��]�p (0 ~ 360)
	*/
	double GetViewRot() const {
		return m_view_rot;
	}

	///////////////////////////////////
	/*!
		�\���J�n�ʒu���Z�b�g�@���㌴�_
	*/
	void SetViewPos(const CvPoint2D64f *viewPos);

	///////////////////////////////////
	/*!
		�\���J�n�ʒu��Ԃ��B
	*/
	void GetViewPos(CvPoint2D64f* viewPos) const;

	///////////////////////////////////
	/*!
		�\�����S�ʒu(�摜���W)��Ԃ�
	*/
	CvPoint2D64f GetViewCenterPos();

	///////////////////////////////////////
	/*!
		�E�B���h�E�T�C�Y���ύX���ꂽ�Ƃ��Ăяo��
		@param[in] lrect �E�B���h�E�͈̔�
	*/
	void OnWindowReSize(LPRECT lrect);

	CvSize GetDisplaySize();

	///////////////////////////////////////
	/*!
		display image flip horizontal
	*/
	inline bool IsViewFlipH() {
		return m_isViewFlipH;
	}

	void SetViewFlipH(bool bl);

	///////////////////////////////////////
	/*!
		display image flip vertical
	*/
	inline bool IsViewFlipV() {
		return m_isViewFlipV;
	}

	void SetViewFlipV(bool bl);

	///////////////////////////////////////
	/*!
		add layer
		@param[in] pLayer �ǉ����郌�C���[�̃|�C���^
		@param[in] index �ǉ���C���f�b�N�X
	*/
	void AddLayer(IImgLayer_Ptr pLayer);
	void AddLayer(IImgLayer_Ptr pLayer, int index);
	void AddLayer(ImgLayerGroup_Ptr pLayerGroup);

	///////////////////////////////////////
	/*!
		remove layer
		@param[in,out] pLayer ��菜�����C���[�̃|�C���^
		@param[in] index ��菜�����C���[�̃C���f�b�N�X
	*/
	void RemoveLayer(IImgLayer_Ptr pLayer);

	///////////////////////////////////////
	/*!
		���C���[�̕��т�ς���
	*/
	void ChangeLayerLine(int from_index, int to_index);

	///////////////////////////////////////
	/*!
		�t�@�C�����ێ����Ă��郌�C���[�̐���Ԃ�
		@return number of layer
	*/
	int GetLayerSize() const;

	///////////////////////////////////////
	/*!
		get layer index
		@param[in] pLayer to get index of layer
		@return layer index, if don't exist layer return -1
	*/
	inline int GetLayerIndex(IImgLayer_weakPtr pLayer) const
	{
		assert(m_pRootLayerGroup);
		return m_pRootLayerGroup->GetLayerIndex(pLayer);
	}

	///////////////////////////////////////
	/*!
		get layer from layer index
		@param[in] index of get layer
		@return layer pointer, if don't exist layer return NULL
	*/
	inline IImgLayer_Ptr GetLayerAtIndex(int index) const
	{
		assert(m_pRootLayerGroup);
		return m_pRootLayerGroup->GetLayerAtIndex(index);
	}

	////////////////////////////////////////
	/*!
	*/
	inline ImgLayerGroup_Ptr const GetRootLayerGroup() const {
		return m_pRootLayerGroup;
	}

	/////////////////////////////////////////
	/*!
	*/
	void SetRootLayerGroup(ImgLayerGroup_Ptr pRootLayerGroup);

	///////////////////////////////////////
	/*!
		�I�𒆂̃��C���[�I�u�W�F�N�g�̃|�C���^�[��Ԃ�
		@return ���C���[�I�u�W�F�N�g�̃|�C���^
	*/
	inline IImgLayer_weakPtr const GetSelectLayer() const {
		return m_pSelectLayer;
	}

	///////////////////////////////////////
	/*!
		�I�����C���[���Z�b�g����B
		@param[in] layer �I�����郌�C���[
	*/
	void SetSelectLayer(IImgLayer_weakPtr layer);
	void UnsetSelectLayer() {
		m_pSelectLayer.expired();
	}

	/////////////////////////////////////////
	///*!
	//	�}�X�N��ǉ�����B
	//	@param[in] pMask �ǉ�����}�X�N�̃|�C���^
	//	@param[in] index �ǉ���C���f�b�N�X
	//*/
	//void AddMask(ImgMask *pMask);
	//void AddMask(ImgMask *pMask, int index);

	/////////////////////////////////////////
	///*!
	//	�}�X�N���폜����B
	//	@param[in,out] pMask �폜����}�X�N�̃|�C���^
	//	@param[in] index �폜����}�X�N�̃C���f�b�N�X
	//*/
	//void DeleteMask(ImgMask *pMask);
	//void DeleteMask(int index);

	/////////////////////////////////////////
	///*!
	//	�t�@�C�����ێ����Ă���}�X�N�̐���Ԃ�
	//	@return �}�X�N����
	//*/
	//int GetMaskSize() const;

	/////////////////////////////////////////
	///*!
	//	�C���f�b�N�X���w�肵�ă}�X�N�I�u�W�F�N�g�ւ̃|�C���^��Ԃ�
	//	@return �}�X�N�I�u�W�F�N�g�̃|�C���^
	//*/
	//ImgMask* const GetMaskAtIndex(int index) const;

	/////////////////////////////////////////
	///*!
	//	�w�肵���}�X�N�I�u�W�F�N�g�̃C���f�b�N�X��Ԃ�
	//	@param[in] mask �w�肷��}�X�N�ւ̃|�C���^
	//	@return �w�肵���}�X�N�̃C���f�b�N�X
	//*/
	//int GetMaskIndex(const ImgMask* mask) const;

	///////////////////////////////////////
	/*!
		�I�𒆂̃}�X�N�I�u�W�F�N�g�̃|�C���^�[��Ԃ�
		@return �}�X�N�I�u�W�F�N�g�̃|�C���^
	*/
	ImgMask_Ptr GetSelectMask() const {
		return m_pMask;
	}
	//int GetSelectMaskIndex() const;

	/////////////////////////////////////////
	///*!
	//	�I���}�X�N���Z�b�g����
	//	@param[in] mask �I������}�X�N
	//*/
	//void SetSelectMask(ImgMask* mask);
	//void SetSelectMask(int index);

	void GetDrawImgPixel(int x, int y, UCvPixel* dst) const {
		if( ((0 <= x) && (x < m_DrawImg->width)) &&
			((0 <= y) && (y < m_DrawImg->height)) ){
			GetPixelFromBGRA(m_DrawImg, x, y, dst);
		}
		else{
			dst->value = 0;
		}
	}
	void GetMaskData(int x, int y, uint8_t* dat) const {
		if( ((0 <= x) && (x < m_DrawImg->width)) &&
			((0 <= y) && (y < m_DrawImg->height)) ){
			(*dat) = GetMaskDataPos(m_DrawMaskImg, x, y);
		}
		else{
			dat = 0;
		}
	}

	///////////////////////////////////////
	/*!
		return draw image
		@return draw image
	*/
	const IplImage* GetDrawImg() const {
		assert( isInit );
		return m_DrawImg;
	}

	///////////////////////////////////////
	/*!
		return draw mask image
		@return draw mask image
	*/
	const IplImage* GetDrawMaskImg() const {
		assert( isInit );
		return m_DrawMaskImg;
	}

	///////////////////////////////////////
	/*!
		return display image
		@return display image
	*/
	const IplImage* GetDisplayImg() const {
		assert( isInit );
		return m_DisplayImg;
	}



	///////////////////////////////////////
	/*!
		�}�X�N�̏�Ԃ�Ԃ�
		@return
	*/
	inline unsigned int GetMaskState() const {
		return m_pMask->GetMaskState();
	}

	///////////////////////////////////////
	/*!
		�}�X�N�͈̔͂�Ԃ�
		@param[out] lprc �}�X�N�͈̔�
	*/
	inline void GetMaskRect(LPRECT lprc) {
		ResourceLock(IE_FILE_RESOURCE_ID::IE_MASK);
		ucvGetMaskRect(m_DrawMaskImg, lprc);
		ResourceUnlock(IE_FILE_RESOURCE_ID::IE_MASK);
	}

	///////////////////////////////////////
	/*!
		�S�Ẵ}�X�N��s���ɂ���B
	*/
	void AllMaskInvisible() const {
	}

	////////////////////////////////
	/*!
		�I��̈�̏�Ƀ}�E�X�����邩�ǂ����H
		@param[in] lpt �}�E�X�̓��͏��
		@return �}�E�X���I��̈�̏�Ȃ�true �����łȂ��Ȃ�false
	*/
	bool IsOnSelectImage(LPPOINT lpt);

	//////////////////////////////////////
	/*!
		�E�B���h�E�N���C�A���g���W���摜�̍��W�ɕϊ�����B
		@param[in,out] lpd ���́A�o��
	*/
	void ConvertPosWndToImg(LPIE_INPUT_DATA lpd) const;
	void ConvertPosWndToImg(const LPPOINT src, LPPOINT dst) const;
	void ConvertPosWndToImg(const CvPoint* src, CvPoint* dst) const;
	void ConvertPosWndToImg(const CvPoint2D64f* src, CvPoint2D64f* dst) const;
	void ConvertPosWndToImg(const LPRECT src_rc, LPRECT dst_rc) const;

	//////////////////////////////////////
	/*!
		�摜�̍��W���E�B���h�E�N���C�A���g���W�ɕϊ�����B
		@param[in,out] lpd ���́A�o��
	*/
	void ConvertPosImgToWnd(LPIE_INPUT_DATA lpd) const;
	void ConvertPosImgToWnd(const LPPOINT src, LPPOINT dst) const;
	void ConvertPosImgToWnd(const CvPoint* src, CvPoint* dst) const;
	void ConvertPosImgToWnd(const CvPoint2D64f* src, CvPoint2D64f* dst) const;
	void ConvertPosImgToWnd(const CvPoint2D64f* src, CvPoint2D64f* dst, int size) const;
	void ConvertPosImgToWnd(const LPRECT src_rc, LPRECT dst_rc) const;

private:
	////////////////////////////////////////
	inline void ResourceLock(unsigned int id){
#ifdef _WIN32
		//switch(id){
		//	case IE_FILE_RESOURCE_ID::IE_DRAW_IMG:
		//		::EnterCriticalSection( &m_csDrawImg );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_DRAW_MASK_IMG:
		//		::EnterCriticalSection( &m_csDrawMaskImg );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_DISPLAY_IMG:
		//		::EnterCriticalSection( &m_csDisplayImg );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_UPDATE_DATA:
		//		::EnterCriticalSection( &m_csUpdateData );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_LAYER:
		//		::EnterCriticalSection( &m_csIELayer );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_MASK:
		//		::EnterCriticalSection( &m_csIEMask );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_EVENTLISTENER:
		//		::EnterCriticalSection( &m_csIEEventListener );
		//		break;
		//	default:
		//		assert( 0 );
		//		break;
		//}
		switch(id){
			case IE_FILE_RESOURCE_ID::IE_DRAW_IMG:
				::WaitForSingleObject(m_csDrawImg, INFINITE);
				break;
			case IE_FILE_RESOURCE_ID::IE_DRAW_MASK_IMG:
				::WaitForSingleObject(m_csDrawMaskImg, INFINITE);
				break;
			case IE_FILE_RESOURCE_ID::IE_DISPLAY_IMG:
				::WaitForSingleObject(m_csDisplayImg, INFINITE);
				break;
			case IE_FILE_RESOURCE_ID::IE_UPDATE_DATA:
				::WaitForSingleObject(m_csUpdateData, INFINITE);
				break;
			case IE_FILE_RESOURCE_ID::IE_LAYER:
				::WaitForSingleObject(m_csIELayer, INFINITE);
				break;
			case IE_FILE_RESOURCE_ID::IE_MASK:
				::WaitForSingleObject(m_csIEMask, INFINITE);
				break;
			case IE_FILE_RESOURCE_ID::IE_EVENTLISTENER:
				::WaitForSingleObject(m_csIEEventListener, INFINITE);
				break;
			default:
				assert( 0 );
				break;
		}
#else
		while((m_LockResourceID & id) == id);
		m_LockResourceID |= id;
#endif
	}
	inline void ResourceUnlock(unsigned int id){
#ifdef _WIN32
		//switch(id){
		//	case IE_FILE_RESOURCE_ID::IE_DRAW_IMG:
		//		::LeaveCriticalSection( &m_csDrawImg );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_DRAW_MASK_IMG:
		//		::LeaveCriticalSection( &m_csDrawMaskImg );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_DISPLAY_IMG:
		//		::LeaveCriticalSection( &m_csDisplayImg );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_UPDATE_DATA:
		//		::LeaveCriticalSection( &m_csUpdateData );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_LAYER:
		//		::LeaveCriticalSection( &m_csIELayer );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_MASK:
		//		::LeaveCriticalSection( &m_csIEMask );
		//		break;
		//	case IE_FILE_RESOURCE_ID::IE_EVENTLISTENER:
		//		::LeaveCriticalSection( &m_csIEEventListener );
		//		break;
		//	default:
		//		assert( 0 );
		//		break;
		//}
		switch(id){
			case IE_FILE_RESOURCE_ID::IE_DRAW_IMG:
				::ReleaseMutex(m_csDrawImg);
				break;
			case IE_FILE_RESOURCE_ID::IE_DRAW_MASK_IMG:
				::ReleaseMutex(m_csDrawMaskImg);
				break;
			case IE_FILE_RESOURCE_ID::IE_DISPLAY_IMG:
				::ReleaseMutex(m_csDisplayImg);
				break;
			case IE_FILE_RESOURCE_ID::IE_UPDATE_DATA:
				::ReleaseMutex(m_csUpdateData);
				break;
			case IE_FILE_RESOURCE_ID::IE_LAYER:
				::ReleaseMutex(m_csIELayer);
				break;
			case IE_FILE_RESOURCE_ID::IE_MASK:
				::ReleaseMutex(m_csIEMask);
				break;
			case IE_FILE_RESOURCE_ID::IE_EVENTLISTENER:
				::ReleaseMutex(m_csIEEventListener);
				break;
			default:
				assert( 0 );
				break;
		}
#else
		m_LockResourceID &= (~id);
#endif
	}

	/////////////////////////////////////
	/*!
		�ŏI�\���摜�̍Đ���
		@param[in] lprc �X�V�͈� �摜���W
	*/
	void UpdateDisplayImage(const LPRECT lprc);

	/////////////////////////////////////
	/*!
		�`��C���[�W�̍Đ���
		@param[in] lprc �X�V�͈� �摜���W
	*/
	void UpdateDrawImage(const LPRECT lprc);

	/////////////////////////////////////
	/*!
		�`��C���[�W�̍Đ���
		@param[in] lprc �X�V�͈� �摜���W
	*/
	void UpdateDrawImageFromDisplayScan(const LPRECT lprc);

	/////////////////////////////////////
	/*!
		�I��͈͂����ꂽ�`��C���[�W�̍Đ���
		@param[in] lprc �X�V�͈� NULL�Ȃ�S�͈�
	*/
	void UpdateMaskImage(const LPRECT lprc);

	//////////////////////////////////////
	/*!
		���s�ړ�
	*/
	void UpdateViewParallelMove(int d_x, int d_y);

	///////////////////////////////////////
	/*!
		update affin transform matrix
	*/
	void UpdateTransformMat();

	///////////////////////////////////////
	/*!
		�I�t�X�N���[���o�b�t�@�[���쐬����B
		@param[in] lprect �X�N���[����
	*/
	void ReGenOffScreenBuffer(LPRECT lprect);


	ImgFileHandle_List m_UndoHandle_List; //Undo�t�@�C������N���X���X�g
	ImgFileHandle_List m_RedoHandle_List; //Redo�t�@�C������N���X���X�g

	ImgFileEventListener_List m_ImgFileEventListener_List; //�C�x���g���X�i�[���X�g
	
	char* m_path;				//�t�@�C���̃p�X
	CvSize m_size;				//�摜�T�C�Y
	RECT m_ClientRect;			//

	int m_view_size;				//�摜�\���T�C�Y(1�`800%)
	double m_view_rot;				//�摜�\���p�@�x��
	CvPoint2D64f m_view_startPt;	//�摜�\���J�n���㌴�_
	CvPoint2D64f m_view_centerPt;	//�f�B�X�v���C�̒��S�_�̍��W �摜���W
	CvMat* m_TransformMat;
	CvMat* m_TransformInvMat;

	bool m_isViewFlipH;	//���E���]
	bool m_isViewFlipV;	//�㉺���]
	int m_MaskDrawType;

	IplImage* m_DrawImg;	//�\���摜	4ch BGR
	IplImage* m_DrawMaskImg;//�\���}�X�N    1ch
	IplImage* m_DisplayImg; //�ŏI�\���摜 (�I��͈́{�\�����摜) 3ch BGR
	
	LPUpdateData_List* m_pUpdateDataForeGrnd_List; //�X�V�����i�[����
	LPUpdateData_List* m_pUpdateDataBackGrnd_List; //

	ImgLayerGroup_Ptr m_pRootLayerGroup;

	//ImgMask_Vec m_Mask_Vec;	//�I��͈͔z��
	ImgMask_Ptr m_pMask;	//

	IImgLayer_weakPtr m_pSelectLayer;	//�I�𒆂̃��C���[
	//ImgMask* m_pSelectMask;

	IEColor m_BackGroundColor;
	unsigned int m_NumOfHistory;

	bool isInit;

#ifdef _WIN32
	//CRITICAL_SECTION m_csDrawImg;
	//CRITICAL_SECTION m_csDrawMaskImg;
	//CRITICAL_SECTION m_csDisplayImg;
	//CRITICAL_SECTION m_csUpdateData;
	//CRITICAL_SECTION m_csIELayer;
	//CRITICAL_SECTION m_csIEMask;
	//CRITICAL_SECTION m_csIEEventListener;
	HANDLE m_csDrawImg;
	HANDLE m_csDrawMaskImg;
	HANDLE m_csDisplayImg;
	HANDLE m_csUpdateData;
	HANDLE m_csIELayer;
	HANDLE m_csIEMask;
	HANDLE m_csIEEventListener;
#else
	unsigned int m_LockResourceID;
#endif //_WIN32

#ifdef IMG_FILE_COUNT_TIME
public:
	double m_UpdateMaskTime;
	double m_UpdateLayerTime;
	double m_UpdateDrawImageTime;
	double m_UpdateMaskImageTime;
	double m_UpdateDisplayImageTime;
#endif //_IMG_FILE_COUNT_TIME
};

#endif
