#ifndef _UTILOPENCV2D_H_
#define _UTILOPENCV2D_H_

#include "LibDllHeader.h"

////////////////////////////////
/*!
	�x�N�g���̑����Z dst = p1 + p2���s��
	@param[out] dst �o�͌���
	@param[in] p1 �x�N�g��1
	@param[in] p2 �x�N�g��2
*/
_EXPORT inline void AddVec2D(CvPoint2D64f *dst, const CvPoint2D64f *p1, const CvPoint2D64f *p2)
{
	dst->x = p1->x + p2->x;
	dst->y = p1->y + p2->y;
}

////////////////////////////////
/*!
	�x�N�g���̈����Z dst = p1 - p2���s��
	@param[out] dst �o�͌���
	@param[in] p1 �x�N�g��1
	@param[in] p2 �x�N�g��2
*/
_EXPORT inline void SubVec2D(CvPoint2D64f *dst, const CvPoint2D64f *p1, const CvPoint2D64f *p2)
{
	dst->x = p1->x - p2->x;
	dst->y = p1->y - p2->y;
}

////////////////////////////////
/*!
	�x�N�g���ƃX�J���̐� dst = t*p���s��
	@param[out] dst �o�͌���
	@param[in] t �X�J��t
	@param[in] p �x�N�g��
*/
_EXPORT inline void ScaleVec2D(CvPoint2D64f *dst, float t, const CvPoint2D64f *p)
{
	dst->x = t * p->x;
	dst->y = t * p->y;
}

////////////////////////////////
/*!
	�x�N�g���̑傫����Ԃ�
	@param[in] vec
	@return |vec|
*/
_EXPORT inline float Vec2DLength(const CvPoint2D64f *vec)
{
	return sqrt(vec->x * vec->x + vec->y * vec->y);
}

////////////////////////////////
/*!
	2�����x�N�g���̊O�ς̑傫����Ԃ��B
	@param[in] p1 �x�N�g��1
	@param[in] p2 �x�N�g��2
*/
_EXPORT inline float CrossVec2DLength(const CvPoint2D64f *p1, const CvPoint2D64f *p2)
{
	return (p1->x * p2->y - p1->y * p2->x);
}

#endif //_UTILOPENCV2D_H_