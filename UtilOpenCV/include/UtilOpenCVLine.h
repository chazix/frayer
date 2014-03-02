#ifndef _UTILOPENCVLINE_H_
#define _UTILOPENCVLINE_H_

#include "LibDllHeader.h"
#include "stdafx.h"

typedef struct _CvLine2D32f {
	CvPoint2D32f start;
	CvPoint2D32f end;
} CvLine2D32f;

typedef struct _CvLine2D64f {
	CvPoint2D64f start;
	CvPoint2D64f end;
} CvLine2D64f;

typedef struct _AfPoint2D64f{
	CvPoint2D64f src;
	CvPoint2D64f dst;
}AfPoint2D64f;

typedef std::vector<CvLine2D32f> CvLine2D32f_Vec;
typedef std::vector<CvLine2D64f> CvLine2D64f_Vec;

//////////////////////////
/*!
	����line1�Ɛ���line2�̌�_p�����߂�B
	@param[out] p ��_
	@param[in] line1 ����1
	@param[in] line2 ����2
*/
_EXPORT void IntersectPoint(CvPoint2D64f *p, const CvLine2D64f *line1, const CvLine2D64f *line2);

//////////////////////////
/*!
	����line1�Ɛ���line2�̌�������
	�������Ă���ꍇ��true��Ԃ��B�����łȂ��ꍇ��false��Ԃ�
	@param[in] line1 ����1
	@param[in] line2 ����2
	@return �������Ă���Ȃ�true���A�����łȂ��Ȃ�false��Ԃ��B
*/
_EXPORT bool IsIntersect(const CvLine2D64f *line1, const CvLine2D64f *line2);

//////////////////////////
/*!
	����line��x=c�̌������������B
	�������Ă���ꍇ��true��Ԃ��B�����łȂ��ꍇ��false��Ԃ��B
	@param[in] line ����
	@param[in] c 
	@return �������Ă���Ȃ�true���A�����łȂ��Ȃ�false��Ԃ��B
*/
_EXPORT bool IsIntersectX(const CvLine2D64f *line, double c);

//////////////////////////
/*!
	����line��y=c�̌������������B
	�������Ă���ꍇ��true��Ԃ��B�����łȂ��ꍇ��false��Ԃ��B
	@param[in] line ����
	@param[in] c 
	@return �������Ă���Ȃ�true���A�����łȂ��Ȃ�false��Ԃ��B
*/
_EXPORT bool IsIntersectY(const CvLine2D64f *line, double c);

//////////////////////////
/*!
	�X�L�������C���ƕ�(v1,v2)�̌�_�����߂�
*/
_EXPORT bool AfIntersect(const AfPoint2D64f *v1, const AfPoint2D64f *v2,
			   int y, double* x, double* sx, double* sy);

#endif //_UTILOPENCVLINE_H_