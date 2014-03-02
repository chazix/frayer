#ifndef _UTILOPENCVFIGURE_H_
#define _UTILOPENCVFIGURE_H_

#include "LibDllHeader.h"
#include "UtilOpenCV.h"
#include "UtilOpenCV2D.h"
#include "UtilOpenCVPixel.h"
#include "IplImageExt.h"

class IplImageExt;


///////////////////////////////
/*!
	�����̃}�X�N�����B
	@param[out] mask �}�X�N�̏o�͐�
	@param[in] setnum ���߂�l
	@param[in] start �J�n�ʒu
	@param[in] end �I���ʒu
*/
_EXPORT void LineMask(IplImage* mask, uint8_t setnum, const CvPoint2D64f *start, const CvPoint2D64f *end);


_EXPORT void DrawLine(CvArr* img, CvPoint pt1, CvPoint pt2,
              CvScalar color, int thickness=1,
              int line_type=8, int shift=0);

_EXPORT void DrawDotLine(CvArr* img, CvPoint pt1, CvPoint pt2,
				 CvScalar color, int dot_length, int thickness = 1,
				 int line_type = 8, int shift = 0);

//�l�p�`�̃}�X�N�����B
_EXPORT void ucvFillRect(IplImage* dst, int num, int x, int y, int width, int height);
_EXPORT void FillRectMask(IplImage* mask, uint8_t setnum, int x, int y, int width, int height);
_EXPORT void IplExtFillRectMask(IplImageExt* mask, uint8_t setnum, int x, int y, int width, int height);

//�ȉ~�̃}�X�N�����B
_EXPORT void FillEllipseMask(IplImage* mask, uint8_t setnum, int x, int y, int width, int height);
_EXPORT void IplExtFillEllipseMask(IplImageExt* mask, uint8_t setnum, int x, int y, int width, int height);

//////////////////////////////////
/*!
	���~�̃}�X�N�����B
	@param[out] mask �}�X�N�̏o�͐�
	@param[in] setnum ���߂�l
	@param[in] x0
	@param[in] y0
	@param[in] r
*/
_EXPORT void FillCircleMask(IplImage* mask, uint8_t setnum, int x0, int y0, int r);
///////////////////////////////////
/*!
	create antialiasing circle mask
	@param[out] dst
	@param[in] center_x
	@param[in] center_y
	@param[in] r
	@param[in] div
	@param[in] uint8_t setnum
	@param[in] alapha
*/
_EXPORT void FillCircleMaskAA(IplImage* dst, double center_x, double center_y, 
							 double r, int div, uint8_t setnum, uint8_t alpha);
_EXPORT void AddFillCircleMaskAA(IplImage* dst, double center_x, double center_y, 
							 double r, int div, uint8_t setnum, uint8_t alpha);

_EXPORT void FillCircleMaskAA2(IplImage* dst, double center_x, double center_y,
					  double r,  double aa_d, uint8_t setnum, uint8_t alpha);
_EXPORT void FillCircleMaskAA2e(IplImage* dst, double center_x, double center_y,
					  double r,  double aa_d, uint8_t setnum, uint8_t alpha);
_EXPORT void AddFillCircleMaskAA2(IplImage* dst, double center_x, double center_y,
					  double r,  double aa_d, uint8_t setnum, uint8_t alpha);

_EXPORT void FillCircleMaskAA3(IplImage* dst, double center_x, double center_y,
					  double r,  double aa_d, uint8_t setnum, uint8_t alpha);
_EXPORT void AddFillCircleMaskAA3(IplImage* dst, double center_x, double center_y,
					  double r,  double aa_d, uint8_t setnum, uint8_t alpha);

_EXPORT void SoftCircleMask(IplImage* dst, double center_x, double center_y,
					  double r, uint8_t setnum, uint8_t alpha);
_EXPORT void AddSoftCircleMask(IplImage* dst, double center_x, double center_y,
					  double r, uint8_t setnum, uint8_t alpha);
//float�ɂ��Ă݂����ǁA�x���Ȃ��c
_EXPORT void AddSoftCircleMaskf(IplImage* dst, float center_x, float center_y,
					  float r, uint8_t setnum, uint8_t alpha);

////////////////////////////////////
/*!
	�s���͗l����������
	@param[in,out] img �������ݐ� 3or4 ch
	@param[in] color1 �F1 rgb
	@parma[in] color2 �F2 rgb
	@parma[in] block_size �͗l�̃u���b�N�T�C�Y
*/
_EXPORT void CheckerImage(IplImage* img, const UCvPixel* color1, const UCvPixel* color2, int block_size);

/////////////////////////////////////
/*!
	�s���͗l����������
	@param[in,out] img �������ݐ� 3or4 ch
	@param[in] color1 �F1 rgb
	@param[in] color2 �F2 rgb
	@param[in] block_size �͗l�̃u���b�N�T�C�Y
	@param[in] lprc �������ݐ�͈�
*/
_EXPORT void CheckerImageRect(IplImage* img, const UCvPixel* color1, const UCvPixel* color2, int block_size, LPRECT lprc);
_EXPORT void CheckerImageRect2(IplImage* img, const UCvPixel* color1, const UCvPixel* color2, int block_size, LPRECT lprc);

/////////////////////////////////////
/*!
	�s���͗l����������
	@param[in,out] img �������ݐ� 3or4 ch
	@param[in] color1 �F1 rgb
	@param[in] color2 �F2 rgb
	@param[in] block_size �͗l�̃u���b�N�T�C�Y
	@param[in] lprc �������ݐ�͈�
*/
_EXPORT void CheckerImageExtRect(IplImageExt* img, const UCvPixel* color1, const UCvPixel* color2, int block_size, LPRECT lprc);

/////////////////////////////////////
/*!
	�摜�ɑ���line_weight�A�Fcolor�̘g����������
	@param[in,out] img �������ݐ�3or4 ch
	@param[in] line_weight �g�̑���
	@param[in] color �g�̐F rgb
*/
_EXPORT void AddRectToImage(IplImage* img, int line_weight, const UCvPixel* color);



#endif // _UTILOPNECVFIGURE_H_