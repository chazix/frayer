#ifndef _IEIMGBRUSH_H_
#define _IEIMGBRUSH_H_

#include "IEBrush.h"

//
//
class _EXPORTCPP IEImgBrush :
	public IEBrush
{
public:
	IEImgBrush(){
		m_brush_type_id = IE_IMG_BRUSH;
		m_brushOrgImg = NULL;
		m_brushImg = NULL;
	}
	~IEImgBrush();

	////////////////////////////////////
	/*!
		XML�`���Ńu���V�O���[�v�̃f�[�^����������
	*/
	void WriteBrushToXml(FILE* fp, int indt);

	/////////////////////////////
	/*!
		���u���V�f�[�^��Ԃ�
		@return �u���V�f�[�^
	*/
	inline const IplImage* GetOrgBrushData() const{
		return m_brushOrgImg;
	}

	/////////////////////////////
	/*!
		���u���V�f�[�^��Ԃ�
		@return �u���V�f�[�^
	*/
	inline const IplImage* GetBrushData() const{
		return m_brushImg;
	}

	////////////////////////////
	/*!
		�u���V�̒��S���W��Ԃ��B
	*/
	inline void GetBrushCenter(LPPOINT lpt){
		RECT brushRect;
		ucvGetMaskRect(m_brushImg, &brushRect);
		int brush_width = brushRect.right - brushRect.left;
		int brush_height = brushRect.bottom - brushRect.top;

		lpt->x = brush_width/2 + brushRect.left;
		lpt->y = brush_height/2 + brushRect.top;
	}

	////////////////////////////
	/*!
		�u���V�̃T�C�Y��Ԃ�
		@return �u���V�̃T�C�Y
	*/
	inline CvSize GetBrushSize() const{
		return cvGetSize(m_brushImg);
	}

	//////////////////////////////
	/*!
		�u���V�̐[�x��Ԃ��B
		@return �u���V�[�x
	*/
	inline int GetBrushDepth() const{
		return m_brushOrgImg->depth;
	}

	//////////////////////////////
	/*!
		�摜�����Ƀu���V���쐬����B
		@param[in] path �摜�̃p�X
	*/
	bool LoadBrushImg(const char* path);

	/////////////////////////////
	/*!
		1ch�f�[�^�����Ƀu���V���쐬����
		@param[in] buffer ���ɂȂ�f�[�^
		@param[in] width �u���V��
		@param[in] height �u���V����
		@param[in] depth �f�[�^�[�x
	*/
	void CreateBrush(const uchar* buffer, int width, int height, int depth);

	/////////////////////////////
	/*!
		�u���V�f�[�^��dst�ɓ]������
		@param[in,out] dst 1ch image
		@param[in] center_lpd center input data
		@param[out] blted_rc
		@return
	*/
	void BltBrushData(IplImage* dst, const LPIE_INPUT_DATA center_lpd, LPRECT blted_rc);

	/////////////////////////////
	/*!
		�u���V�f�[�^��dst�ɓ]������
		@param[in] copyOp
		@param[in,out] dst image, 4ch or 1ch
		@param[in] center_lpd center input data
		@param[out] blted_rc
		@return
	*/
	template <class _CBltOp>
	void BltBrushData(
		const _CBltOp& copyOp,
		IplImageExt* dst,
		const LPIE_INPUT_DATA center_lpd,
		LPRECT blted_rc);

	template <class _CBltOp>
	void BltBrushData(
		const _CBltOp& copyOp,
		IplImage* dst,
		const LPIE_INPUT_DATA center_lpd,
		LPRECT blted_rc);

	/////////////////////////////
	/*!
		�T�C�Y(width, height)�̃T���v���摜�𐶐����ĕԂ��B
		@return �쐬����IplImage��Ԃ�
	*/
	IplImage* CreateBrushSampleImg(int width, int height);

	/////////////////////////////
	/*!
		�u���V�v���r���[��dst�ɕ`�悷��
		@param[in] dst
		@return
	*/
	void DrawBrushPreview(IplImage* dst);

	//////////////////////////////
	/*!
		�u���V�̗L���͈͔��a���Z�b�g����B
	*/
	void SetBrushRadius(int r);

private:
	std::string m_imgPath;

	IplImage* m_brushOrgImg;	//�u���V���f�[�^
	IplImage* m_brushImg;		//�u���V�T�C�Y�ύX��f�[�^
};

_EXPORTCPP IEImgBrush_Ptr CreateIEImgBrush();

template <class _CCopyOp>
class ImgBrushEdit1chOp {
public:
	ImgBrushEdit1chOp(const _CCopyOp& copyOp)
	{
		m_copyOp = copyOp;
	}
	~ImgBrushEdit1chOp(){};

	void SetParam(
		double center_x,
		double center_y,
		double rad,
		double in_d,
		double out_d,
		uint8_t brush_alpha,
		IplImage* texture)
	{
		m_center_x = center_x;
		m_center_y = center_y;
		m_rad = rad;
		m_in_d = in_d;
		m_out_d = out_d;
		m_brush_alpha = brush_alpha;
		m_texture = texture;


		if (m_texture) {
			assert(m_texture->width == m_texture->height);
			assert((m_texture->width == 256) || (m_texture->width == 512) || (m_texture->width == 1024));

			if (m_texture->width == 256) {
				m_texture_shift_count = 8;
				m_texture_mod_bit = 0xff;
			}
			else if (m_texture->width == 512) {
				m_texture_shift_count = 9;
				m_texture_mod_bit = 0x1ff;
			}
			else if (m_texture->width == 1024) {
				m_texture_shift_count = 10;
				m_texture_mod_bit = 0x3ff;
			}
		}
	}

	inline void operator()(void* dst, int posX, int posY, int edit_width) const
	{
	}

private:

	_CCopyOp m_copyOp;
	double m_center_x;
	double m_center_y;
	double m_rad;
	double m_in_d;
	double m_out_d;
	uint8_t m_brush_alpha;
	IplImage* m_texture;
	int m_texture_shift_count;
	int m_texture_mod_bit;
};

template <class _CCopyOp>
class ImgBrushEdit4chOp {
public:
	ImgBrushEdit4chOp(const _CCopyOp& copyOp)
	{
		m_copyOp = copyOp;
	}
	~ImgBrushEdit4chOp(){};

	void SetParam(
		double center_x,
		double center_y,
		double rad,
		double in_d,
		double out_d,
		uint8_t brush_alpha,
		IplImage* texture)
	{
		m_center_x = center_x;
		m_center_y = center_y;
		m_rad = rad;
		m_in_d = in_d;
		m_out_d = out_d;
		m_brush_alpha = brush_alpha;
		m_texture = texture;


		if (m_texture) {
			assert(m_texture->width == m_texture->height);
			assert((m_texture->width == 256) || (m_texture->width == 512) || (m_texture->width == 1024));

			if (m_texture->width == 256) {
				m_texture_shift_count = 8;
				m_texture_mod_bit = 0xff;
				}
			else if (m_texture->width == 512) {
				m_texture_shift_count = 9;
				m_texture_mod_bit = 0x1ff;
			}
			else if (m_texture->width == 1024) {
				m_texture_shift_count = 10;
				m_texture_mod_bit = 0x3ff;
			}
		}
	}

	inline void operator()(void* dst, int posX, int posY, int edit_width) const
	{
	}

private:

	_CCopyOp m_copyOp;
	double m_center_x;
	double m_center_y;
	double m_rad;
	double m_in_d;
	double m_out_d;
	uint8_t m_brush_alpha;
	IplImage* m_texture;
	int m_texture_shift_count;
	int m_texture_mod_bit;
};

template <class _CCopyOp>
void IEImgBrush::BltBrushData(
	const _CCopyOp& copyOp,
	IplImageExt* dst,
	const LPIE_INPUT_DATA center_lpd,
	LPRECT blted_rc)
{
	BRUSH_CTRL brush_ctrl;
	GetBrushCtrl(&brush_ctrl, center_lpd);

	// check brush radius
	if (brush_ctrl.rad <= 0.0) {
		blted_rc->top = blted_rc->bottom = 0;
		blted_rc->left = blted_rc->right = 0;
		return;
	}

	// check brush interval
	double pow_brush_dist = (brush_ctrl.center_x - m_prev_draw_pt.x) * (brush_ctrl.center_x - m_prev_draw_pt.x) +
		(brush_ctrl.center_y - m_prev_draw_pt.y) * (brush_ctrl.center_y - m_prev_draw_pt.y);
	if (brush_ctrl.interval * brush_ctrl.interval > pow_brush_dist) {
		blted_rc->top = blted_rc->bottom = 0;
		blted_rc->left = blted_rc->right = 0;
		return;
	}
	m_prev_draw_pt.x = brush_ctrl.center_x;
	m_prev_draw_pt.y = brush_ctrl.center_y;

	//
	double out_aa = 1.5;
	double in_aa = brush_ctrl.rad * (1.0 - m_hardness) + 1.5;
	if (brush_ctrl.rad < in_aa)
		in_aa = brush_ctrl.rad;
	
	blted_rc->top = brush_ctrl.center_y - brush_ctrl.rad - 2.0;
	blted_rc->left = brush_ctrl.center_x - brush_ctrl.rad - 2.0;
	blted_rc->bottom = brush_ctrl.center_y + brush_ctrl.rad + 2.0;
	blted_rc->right = brush_ctrl.center_x + brush_ctrl.rad + 2.0;

	//clipping
	CvSize img_size = dst->GetSize();
	RECT img_rect;
	img_rect.left = 0;
	img_rect.top = 0;
	img_rect.right = img_size.width;
	img_rect.bottom = img_size.height; 
	AndRect(blted_rc, &img_rect, blted_rc);

	//
	uint8_t brush_alpha = brush_ctrl.alpha * 255;

	if (dst->GetImgNChannel() == 4) {
		ImgBrushEdit4chOp<_CCopyOp> editOp(copyOp);
		editOp.SetParam(
			brush_ctrl.center_x,
			brush_ctrl.center_y,
			brush_ctrl.rad,
			in_aa,
			out_aa,
			brush_alpha,
			m_texture);
		dst->OperateEdit(
			editOp,
			blted_rc->left,
			blted_rc->top,
			blted_rc->right - blted_rc->left,
			blted_rc->bottom - blted_rc->top);
	} else if (dst->GetImgNChannel() == 1) {
		ImgBrushEdit1chOp<_CCopyOp> editOp(copyOp);
		editOp.SetParam(
			brush_ctrl.center_x,
			brush_ctrl.center_y,
			brush_ctrl.rad,
			in_aa,
			out_aa,
			brush_alpha,
			m_texture);
		dst->OperateEdit(
			editOp,
			blted_rc->left,
			blted_rc->top,
			blted_rc->right - blted_rc->left,
			blted_rc->bottom - blted_rc->top);
	} else {
		assert(0);
	}
}

template <class _CCopyOp>
void IEImgBrush::BltBrushData(
	const _CCopyOp& copyOp,
	IplImage* dst,
	const LPIE_INPUT_DATA center_lpd,
	LPRECT blted_rc)
{
	BRUSH_CTRL brush_ctrl;
	GetBrushCtrl(&brush_ctrl, center_lpd);

	// check brush radius
	if (brush_ctrl.rad <= 0.0) {
		blted_rc->top = blted_rc->bottom = 0;
		blted_rc->left = blted_rc->right = 0;
		return;
	}

	// check brush interval
	double pow_brush_dist = (brush_ctrl.center_x - m_prev_draw_pt.x) * (brush_ctrl.center_x - m_prev_draw_pt.x) +
		(brush_ctrl.center_y - m_prev_draw_pt.y) * (brush_ctrl.center_y - m_prev_draw_pt.y);
	if (brush_ctrl.interval * brush_ctrl.interval > pow_brush_dist) {
		blted_rc->top = blted_rc->bottom = 0;
		blted_rc->left = blted_rc->right = 0;
		return;
	}
	m_prev_draw_pt.x = brush_ctrl.center_x;
	m_prev_draw_pt.y = brush_ctrl.center_y;

	//
	double out_aa = 1.5;
	double in_aa = brush_ctrl.rad * (1.0 - m_hardness) + 1.5;
	if (brush_ctrl.rad < in_aa)
		in_aa = brush_ctrl.rad;
	
	blted_rc->top = brush_ctrl.center_y - brush_ctrl.rad - 2.0;
	blted_rc->left = brush_ctrl.center_x - brush_ctrl.rad - 2.0;
	blted_rc->bottom = brush_ctrl.center_y + brush_ctrl.rad + 2.0;
	blted_rc->right = brush_ctrl.center_x + brush_ctrl.rad + 2.0;

	//clipping
	RECT img_rect;
	img_rect.left = 0;
	img_rect.top = 0;
	img_rect.right = dst->width;
	img_rect.bottom = dst->height; 
	AndRect(blted_rc, &img_rect, blted_rc);

	//
	uint8_t brush_alpha = brush_ctrl.alpha * 255;

	if (dst->nChannels == 4) {
		ImgBrushEdit4chOp<_CCopyOp> editOp(copyOp);
		editOp.SetParam(
			brush_ctrl.center_x,
			brush_ctrl.center_y,
			brush_ctrl.rad,
			in_aa,
			out_aa,
			brush_alpha,
			m_texture);
		IplImageExt::OperateEdit(
			editOp,
			dst,
			blted_rc->left,
			blted_rc->top,
			blted_rc->right - blted_rc->left,
			blted_rc->bottom - blted_rc->top);
	} else if (dst->nChannels == 1) {
		ImgBrushEdit1chOp<_CCopyOp> editOp(copyOp);
		editOp.SetParam(
			brush_ctrl.center_x,
			brush_ctrl.center_y,
			brush_ctrl.rad,
			in_aa,
			out_aa,
			brush_alpha,
			m_texture);
		IplImageExt::OperateEdit(
			editOp,
			dst,
			blted_rc->left,
			blted_rc->top,
			blted_rc->right - blted_rc->left,
			blted_rc->bottom - blted_rc->top);
	} else {
		assert(0);
	}
}


#endif //_IEIMGBRUSH_H_