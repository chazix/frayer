#ifndef _UTILOPENC_H_
#define _UTILOPENC_H_

#include "LibDllHeader.h"
#include "OpenCV2VC.h"
#include "IplImageExt.h"
#include "UtilOpenCVMat.h"
#include "UtilOpenCVFigure.h"
#include "UtilOpenCVFilter.h"
#include "UtilOpenCVLine.h"
#include "UtilOpenCVTransform.h"
#include "UtilOpenCVIO.h"
#include "UtilOpenCVInterpolate.h"
#include "UtilOpenCVEffect.h"
#include "UtilOpenCVColor.h"


///////////////////////////////////////
/*!
	img1��img2���r���ē����s�N�Z���f�[�^�Ȃ�true�A�����łȂ����false��Ԃ�
	@param[in] img1 ��r����摜1
	@param[in] img2 ��r����摜2
	@return img1��img2���r���ē����s�N�Z���f�[�^�Ȃ�true�A�����łȂ����false��Ԃ�
*/
_EXPORT bool isEqualIplImage(const IplImage* img1, const IplImage* img2);

///////////////////////////////////////
/*!
	img1��img2���r���ē����s�N�Z���f�[�^�Ȃ�true�A�����łȂ����false��Ԃ�
	@param[in] img1 ��r����摜1
	@param[in] img2 ��r����摜2
	@return img1��img2���r���ē����s�N�Z���f�[�^�Ȃ�true�A�����łȂ����false��Ԃ�
*/
_EXPORT bool isEqualIplImageExt(const IplImageExt* img1, const IplImageExt* img2);

///////////////////////////////////////
/*!
	�}�X�N�����ׂĂO�Ŗ�������Ă�����true��Ԃ�
	@param[in] mask ���ׂ�}�X�N
	@return �}�X�N�̗v�f�̒l�����ׂĂO�Ȃ�true �����łȂ����false��Ԃ��B
*/
_EXPORT bool isFillZeroMask(const IplImage* mask);

////////////////////////////////////////
/*!
	�}�X�N�����ׂĂO�Ŗ�������Ă��Ȃ�������true��Ԃ�
	@param[in] mask ���ׂ�}�X�N
	@reutn �}�X�N�̗v�f�ɂO�łȂ��l����������true �����łȂ����false��Ԃ��B
*/
_EXPORT bool isNotFillZeroMask(const IplImage* mask);

////////////////////////////////////////
/*!
	img�̉�f���S��pix�Ɠ������Ȃ�true��Ԃ��A�����łȂ����false��Ԃ��B
	@param[in] img ���ׂ�摜
	@param[in] pix ���ׂ�l
	@reutn
*/
_EXPORT bool isFillMask(const IplImage* img, uint8_t pix);

////////////////////////////////////////
/*!
	img�̉�f���S��pix�Ɠ������Ȃ�true��Ԃ��A�����łȂ����false��Ԃ��B
	@param[in] img ���ׂ�摜
	@param[in] pix ���ׂ�l
	@reutn
*/
_EXPORT bool isFillColor(const IplImage* img, const UCvPixel* pix);

////////////////////////////////////////
/*!
	0�łȂ��l�ň͂܂�Ă��镔����setnum�Ŗ��߂�
	@param[in,out] mask ���ׂ�}�X�N
	@param[in] setnum ���߂�l
*/
_EXPORT void FillCloseArea(IplImage* mask, uint8_t setnum);

//////////////////////////////////////////
/*!
*/
_EXPORT double GetMaxAbsDiff(const IplImage* img1, const IplImage* img2);
_EXPORT double GetMaxAbsDiffExt(const IplImageExt* img1, const IplImageExt* img2);

//////////////////////////////////////////
/*!
	src��dst�ɓ]��
	@param[out] dst �o�͐�
	@param[in] posX dst�]���J�n�ʒu
	@param[in] posY dst�]���J�n�ʒu
	@param[in] width �]������摜��
	@param[in] height �]���摜����
	@param[in] src �]�����摜
	@param[in] startX �]�����摜�J�n�ʒu
	@param[in] startY �]�����摜�J�n�ʒu
*/
_EXPORT void BltMaskToMask(IplImage* dst, int posX, int posY, int width, int height, 
				   const IplImage* src, int startX, int startY);

///////////////////////////////////////////////
/*!
	mask�͈̔�lprc��dst_mask�ɃR�s�[����
	@param[in] mask �}�X�N
	@param[in] lprc �͈�
	@param[out] dst_mask �o�͌���
*/
_EXPORT void CutMask(const IplImage* mask, const LPRECT lprc, IplImage* dst_mask);


//////////////////////////////////
/*!
	�}�X�N�̉������͈̔͂�Ԃ�
	@param[in] mask �}�X�N
	@param[out] lprc �͈�
*/
_EXPORT void ucvGetMaskRect(const IplImage* mask, LPRECT lprc);

///////////////////////////////////////////
/*!
	(x,y)��rc�̒��ɓ����Ă��邩�ǂ���
	���E���܂�
	@param[in] x
	@param[in] y
	@param[in] rc
*/
_EXPORT inline bool isInRect(int x, int y, const LPRECT rc)
{
	if((rc->left <= x && x < rc->right) &&
		(rc->top <= y && y < rc->bottom)){
			return true;
	}
	return false;
}

///////////////////////////////////////////
/*!
	rc1��rc2�̒��ɓ����Ă��邩�ǂ���
	���E���܂�
	@param[in] rc1
	@param[in] rc2
*/
_EXPORT inline bool isRectInRect(const LPRECT rc1, const LPRECT rc2)
{
	if(isInRect(rc1->top, rc1->left, rc2) &&
		isInRect(rc1->top, rc1->right, rc2) &&
		isInRect(rc1->bottom, rc1->right, rc2) &&
		isInRect(rc1->bottom, rc1->left, rc2)){
			return true;
	}
	return false;
}

////////////////////////////////////////
/*!
	�͈̘͂_���a
	src1��src2���܂ލŏ��͈̔�dest��Ԃ�
	@param[in] src1
	@param[in] src2
	@param[in] dest
*/
_EXPORT inline void OrRect(const LPRECT src1, const LPRECT src2, LPRECT dest)
{
	dest->top = (src1->top < src2->top) ? src1->top : src2->top;
	dest->left = (src1->left < src2->left) ? src1->left : src2->left;
	
	dest->bottom = (src1->bottom > src2->bottom) ? src1->bottom : src2->bottom;
	dest->right = (src1->right > src2->right) ? src1->right : src2->right;
}

////////////////////////////////////////
/*!
	�͈̘͂_����
	src1��src2�̏d�Ȃ��Ă��镔���̍ő�͈̔�dest��Ԃ�
	�d�Ȃ肪������Δ͈͂�(0,0)(0,0)�ɂ��ĕԂ��B
	@param[in] src1
	@param[in] src2
	@param[in] dest
*/
_EXPORT inline void AndRect(const LPRECT src1, const LPRECT src2, LPRECT dest)
{
	dest->top = (src1->top > src2->top) ? src1->top : src2->top;
	dest->left = (src1->left > src2->left) ? src1->left : src2->left;
	
	dest->bottom = (src1->bottom < src2->bottom) ? src1->bottom : src2->bottom;
	dest->right = (src1->right < src2->right) ? src1->right : src2->right;

	if((dest->top > dest->bottom) || (dest->left > dest->right)){
		dest->top = dest->bottom = dest->left = dest->right = 0;
	}
}

////////////////////////////////////////
/*!
	�͈͂̈����Z
	dest = src1 - src2
	@param[in] src1
	@param[in] src2
	@param[in] dest
*/
_EXPORT inline void SubRect(const LPRECT src1, const LPRECT src2, LPRECT dest)
{
	if(isRectInRect(src1, src2)){
		dest->top = dest->top = 0;
		dest->left = dest->right = 0;
		return;
	}

	(*dest) = (*src1);
	if(isInRect(src1->left, src1->top, src2) && isInRect(src1->right, src1->top, src2)){
		dest->top = src2->bottom;
	}
	if(isInRect(src1->right, src1->top, src2) && isInRect(src1->right, src1->bottom, src2)){
		dest->right = src2->left;
	}
	if(isInRect(src1->right, src1->bottom, src2) && isInRect(src1->left, src1->bottom, src2)){
		dest->bottom = src2->top;
	}
	if(isInRect(src1->left, src1->bottom, src2) && isInRect(src1->left, src1->top, src2)){
		dest->left = src2->right;
	}
}

/////////////////////////////////////////////
_EXPORT void ucvCvtColor(const IplImage* src, IplImage* dst, int code);

#endif // _UTILOPENCV_H_