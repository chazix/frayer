#ifndef _IMGLAYER_H_
#define _IMGLAYER_H_

#include "IImgLayer.h"
#include "EditNode.h"


_EXPORTCPP ImgLayer_Ptr CreateIEImgLayer(ImgFile_Ptr parent_file);
//_EXPORT void ReleaseIEImgLayer(ImgLayer_Ptr* ppLayer);

//
class _EXPORTCPP ImgLayer :
	public IImgLayer
{
public:
	ImgLayer(ImgFile_Ptr parent_file)
		:IImgLayer(parent_file)
	{
		m_pEditQueueForeGrnd = NULL;
		m_pEditQueueBackGrnd = NULL;
	}

	virtual ~ImgLayer()
	{
		End();
	}

	inline int GetLayerType(){
		return IE_LAYER_TYPE::NORMAL_LAYER;
	}

	////////////////////////////////////
	/*!
		layer initialize
		@param lprc layer rect
	*/
	virtual void Init(const LPRECT lprc);

	////////////////////////////////////
	/*!
	*/
	virtual void End();

	bool isAlphaMask() const;

	//////////////////////////
	/*!
		���̃��C���[�̃G�f�B�b�g�m�[�h�𐶐����|�C���^��n���B
		@param[in] lprc �G�f�B�b�g�m�[�h�̓\��t����͈�
		@return �G�f�B�b�g�m�[�h�̃|�C���^
	*/
	EditNode* CreateEditNode(const LPRECT lprc);

	///////////////////////////
	/*!
		���̃��C���[���쐬����EditNode���폜����B
		@parma[in] pEditNode �폜����EditNode
	*/
	void DestroyEditNode(EditNode* pEditNode);

	void MoveLayer(int move_x, int move_y);

	//////////////////////////
	/*!
		�X�^�b�N�ɃG�f�B�b�g�m�[�h��ς�
		@param[in] pNode �X�^�b�N�ɐςރG�f�B�b�g�m�[�h
	*/
	void PushAddEditNodeQueue(IEditNode* pNode);
	void PushSubEditNodeQueue(IEditNode* pNode);

	//////////////////////////
	bool Update(const LPRECT enable_lprc, LPRECT updated_lprc);

	void ClearUpdateData();
	void LockUpdateData();
	void UnlockUpdateData();

	//////////////////////////////////
	/*!
		���C���[�摜�ɏ�������
		@param[in] x
		@param[in] y
		@param[in] pixel
	*/
	void SetPixel(int x, int y, const UCvPixel* pixel);

	//////////////////////////////////
	/*!
		���C���[�摜��h��Ԃ�
		@param[in] posX �h��Ԃ��J�nX���W
		@param[in] posY �h��Ԃ��J�nX���W
		@param[in] width �h��Ԃ���
		@param[in] height �h��Ԃ�����
		@param[in] fillcolor �h��Ԃ��F
	*/
	void RectFillColor(int posX, int posY, int width, int height, const UCvPixel* fillcolor);

	void RectFillAlpha(int posX, int posY, int width, int height, uint8_t alpha);

	//////////////////////////////////
	/*!
		���C���[�ɉ摜��]��
		@param[in] posX ���C���[�]����X���W
		@param[in] posY ���C���[�]����X���W
		@param[in] width �]����
		@param[in] height �]������
		@param[in] src �]���摜
		@param[in] startX �]���摜�J�nX���W
		@param[in] startY �]���摜�J�nY���W
	*/
	void ImgBltToLayer(int posX, int posY, int width, int height,
		const IplImage* src, int startX, int startY);

	//////////////////////////////////
	/*!
		pLayer�Ɏ��g���R�s�[����B
		@return ���������true�A�����łȂ����false��Ԃ��B
	*/
	bool CopyImgLayer(ImgLayer_Ptr pLayer) const;

	void Split(IplImage* dst0, IplImage* dst1,
		IplImage* dst2, IplImage* dst3) const;

	void Merge(int posX, int posY, int width, int height,
		const IplImage* src0, const IplImage* src1, const IplImage* src2, const IplImage* src3);

	///////////////////////////////////
	/*!
	*/
	void SetLockPixelAlpha(bool bl){
		m_IsLockPixelAlpha = bl;
	}
	bool IsLockPixelAlpha() const {
		return m_IsLockPixelAlpha;
	}

	///////////////////////////////////
	/*!
	*/
	void SetClipWithUnderLayer(bool bl){
		m_IsClipWithUnderLayer = bl;
	}
	bool IsClipWithUnderLayer() const {
		return m_IsClipWithUnderLayer;
	}

	virtual void BltRectSynthPixel(IplImageExt* dst, int d_x, int d_y, int s_x, int s_y, int width, int height) const;

private:

	/////////////////////////
	/*!
		�`��C���[�W�̃A�b�v�f�[�g
		@param[in] lprc �A�b�v�f�[�g����͈�
	*/
	void UpdateEditNode(const LPRECT lprc);

	///////////////////////////////////
	/*!
		pNode�����Z����B
		@param[in] pNode
		@parma[in] lprc �X�V�͈�
	*/
	void AddEditNode(EditNode* pNode, const LPRECT lprc);

	/////////////////////////////////////
	/*!
		pNode�����Z����
		@param[in] pNode
		@param[in] lprc �X�V�͈�
	*/
	void SubEditNode(EditNode* pNode, const LPRECT lprc);

	EditQueue* m_pEditQueueForeGrnd;
	EditQueue* m_pEditQueueBackGrnd;

	bool m_IsLockPixelAlpha;	//�s�����x�����b�N���邩�ǂ���
	bool m_IsLockVisiblePixel;
	bool m_IsLockLayerPosition;
	bool m_IsClipWithUnderLayer;
};


//
class WritePixelMark {
public:
	inline void operator()(void* dst, const void* src, const void* mask, int width) const
	{
		assert(dst);
		assert(mask);
		assert(src == NULL);

		uint8_t* pdst = (uint8_t*) dst;
		UCvPixel* pmask = (UCvPixel*) mask;

//#ifdef IE_USE_WIN32_SSE
//		if (IS_16BYTE_ALIGNMENT(pdst) && IS_16BYTE_ALIGNMENT(pmask)) {
//			SubLineOp_SSE(pdst, pmask, width);
//			return;
//		}
//
//		int dif_d = DIFF_16BYTE_ALIGNMENT(pdst);
//		int dif_s = DIFF_16BYTE_ALIGNMENT(pmask);
//		assert((dif_s & 0x3) == 0); //1pixel�A4byte�Ȃ̂�4�̔{���ɂȂ�͂�
//
//		if (dif_d == dif_s) {
//			int dif_npix = dif_s >> 2; //�A���C�����g�������Ă��Ȃ��s�N�Z�����𓾂�
//			LineOp(pdst, pmask, dif_npix);
//			SubLineOp_SSE(
//				pdst + dif_npix,
//				pmask + dif_npix,
//				width - dif_npix);
//		} else {
//			//�ǂݍ��ݐ�̃A���C�����g�����킹��B
//			int dif_npix = dif_s >> 2;
//			LineOp(pdst, pmask, dif_npix);
//			//
//			SubLineOp_SSE_dst_unaligned(
//				pdst + dif_npix,
//				pmask + dif_npix,
//				width - dif_npix);
//		}
//#else
		LineOp(pdst, pmask, width);
//#endif //IE_USE_WIN32_SSE
	}

	inline void LineOp(uint8_t* pdst, const UCvPixel* pmask, int width) const
	{
		int i;
		for(i=0; i<width; i++){
			if(pmask->a != 0)
				(*pdst) = pmask->a;
			pdst++;
			pmask++;
		}
	}

	inline void SubLineOp_SSE(uint8_t* pdst, const UCvPixel* pmask, int width) const
	{
		assert(IS_16BYTE_ALIGNMENT(pdst) == IS_16BYTE_ALIGNMENT(pmask));

		int nloop = width >> 4; //width / 16

		__m128i xmzero = _mm_setzero_si128();

		int i;
		for (i = 0; i < nloop; i++) {
			__m128i xmmask0 = _mm_load_si128((const __m128i*)pmask);
			__m128i xmmask1 = _mm_load_si128((const __m128i*)pmask);
			__m128i xmmask2 = _mm_load_si128((const __m128i*)pmask);
			__m128i xmmask3 = _mm_load_si128((const __m128i*)pmask);

			xmmask0 = _mm_srli_epi32(xmmask0, 24);
			xmmask1 = _mm_srli_epi32(xmmask1, 24);
			__m128i xmmask_lo16 = _mm_packs_epi32(xmmask0, xmmask1);

			xmmask2 = _mm_srli_epi32(xmmask2, 24);
			xmmask3 = _mm_srli_epi32(xmmask3, 24);
			__m128i xmmask_hi16 = _mm_packs_epi32(xmmask2, xmmask3);

			__m128i xmdst = _mm_packs_epi16(xmmask_lo16, xmmask_hi16);

			_mm_store_si128((__m128i*)pdst, xmdst);

			pdst += 16;
			pmask += 16;
		}

		int bn = width & 0xf; //width % 16
		LineOp(pdst, pmask, bn);
	}

	inline void SubLineOp_SSE_dst_unaligned(uint8_t* pdst, const UCvPixel* pmask, int width) const
	{
		assert(IS_16BYTE_ALIGNMENT(pmask));

		int nloop = width >> 4; //width / 16

		__m128i xmzero = _mm_setzero_si128();

		int i;
		for (i = 0; i < nloop; i++) {
			__m128i xmmask0 = _mm_load_si128((const __m128i*)pmask);
			__m128i xmmask1 = _mm_load_si128((const __m128i*)pmask);
			__m128i xmmask2 = _mm_load_si128((const __m128i*)pmask);
			__m128i xmmask3 = _mm_load_si128((const __m128i*)pmask);

			xmmask0 = _mm_srli_epi32(xmmask0, 24);
			xmmask1 = _mm_srli_epi32(xmmask1, 24);
			__m128i xmmask_lo16 = _mm_packs_epi32(xmmask0, xmmask1);

			xmmask2 = _mm_srli_epi32(xmmask2, 24);
			xmmask3 = _mm_srli_epi32(xmmask3, 24);
			__m128i xmmask_hi16 = _mm_packs_epi32(xmmask2, xmmask3);

			__m128i xmdst = _mm_packs_epi16(xmmask_lo16, xmmask_hi16);

			_mm_storeu_si128((__m128i*)pdst, xmdst);

			pdst += 16;
			pmask += 16;
		}

		int bn = width & 0xf; //width % 16
		LineOp(pdst, pmask, bn);
	}
};

//blt undo image operator
class BltLayerUndoOp
{
public:
	inline void operator()(void* dst, const void* src, const void* mask, int width) const
	{
		if(src == NULL)
			return;

		if(mask == NULL){
			memcpy(dst, src, width * sizeof(uint32_t));
			return;
		}

		UCvPixel* pdst = (UCvPixel*) dst;
		UCvPixel* psrc = (UCvPixel*) src;
		uint8_t* pmask = (uint8_t*) mask;

		int i;
		for(i=0; i<width; i++){
			if((*pmask) == 0)
				pdst->value = psrc->value;
			pdst++;
			psrc++;
			pmask++;
		}
	}
};

#endif