#ifndef _UTILOPENCVTRANSFORM_H_
#define _UTILOPENCVTRANSFORM_H_

#include "LibDllHeader.h"
#include "stdafx.h"

class IplImageExt;

/////////////////////////////////////
/*!
	�A�t�B���ϊ��s��̋t�ϊ��s������Ƃ߂�
	@param[in] src 2x3�̃A�t�B���ϊ��s��
	@param[out] dst 2x3�̋t�s��o�͐�
*/
_EXPORT inline void InvertAffineMat(const CvMat* src, CvMat* dst)
{
	double det_src = cvmGet(src, 0, 0)*cvmGet(src, 1, 1) - cvmGet(src, 0, 1)*cvmGet(src, 1, 0);
	double inv_det_src = 1.0/det_src;

#ifdef _DEBUG
	double tmp;
	tmp = cvmGet(src, 1, 1)*inv_det_src;
	cvmSet(dst, 0, 0, tmp);
	tmp = -cvmGet(src, 0, 1)*inv_det_src;
	cvmSet(dst, 0, 1, tmp);
	tmp = (cvmGet(src, 0, 1)*cvmGet(src, 1, 2) - cvmGet(src, 0, 2)*cvmGet(src, 1, 1))*inv_det_src;
	cvmSet(dst, 0, 2, tmp);

	tmp = -cvmGet(src, 1, 0)*inv_det_src;
	cvmSet(dst, 1, 0, tmp);
	tmp = cvmGet(src, 0, 0)*inv_det_src;
	cvmSet(dst, 1, 1, tmp);
	tmp = (cvmGet(src, 0, 2)*cvmGet(src, 1, 0) - cvmGet(src, 0, 0)*cvmGet(src, 1, 2))*inv_det_src;
	cvmSet(dst, 1, 2, tmp);
#else
	cvmSet(dst, 0, 0, cvmGet(src, 1, 1)*inv_det_src);
	cvmSet(dst, 0, 1, -cvmGet(src, 0, 1)*inv_det_src);
	cvmSet(dst, 0, 2,(cvmGet(src, 0, 1)*cvmGet(src, 1, 2) - cvmGet(src, 0, 2)*cvmGet(src, 1, 1))*inv_det_src);
	
	cvmSet(dst, 1, 0, -cvmGet(src, 1, 0)*inv_det_src);
	cvmSet(dst, 1, 1, cvmGet(src, 0, 0)*inv_det_src);
	cvmSet(dst, 1, 2, (cvmGet(src, 0, 2)*cvmGet(src, 1, 0) - cvmGet(src, 0, 0)*cvmGet(src, 1, 2))*inv_det_src);
#endif //_DEBUG
}

/////////////////////////////////////
/*!
	�䗦�ɂ�鎩�R�ό`
	@param[out] dst �o�͉摜
	@param[in] src ���͉摜
	@param[in] d_pts �o�͐�ό`�_(���v���̏���4�_���w�肷��)
*/
_EXPORT bool FreeDeformationByRatio(IplImage* dst, const IplImage* src, const CvPoint2D64f* d_pts);

/////////////////////////////////////
/*!
	�A�t�B���ϊ�
	@param[out] dst �o�͐�摜
	@param[in] src ���͉摜
	@param[in] matrix �ϊ��s��
	@param[in] fillval 
*/
_EXPORT void WarpAffine(IplImage* dst, const IplImageExt* src, const CvMat* matrix, int flag, const UCvPixel* fillval);
_EXPORT void WarpAffine_NN(IplImage* dst, const IplImageExt* src, const CvMat* matrix, const UCvPixel* fillval);
//_EXPORT void WarpAffine_Bilinear(IplImage* dst, const IplImageExt* src, const CvMat* matrix, const UCvPixel* fillval);

#endif //_UTILOPENCVTRANSFORM_H_