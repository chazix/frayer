#ifndef _UTILOPENCVEFFECT_H_
#define _UTILOPENCVEFFECT_H_

#include "LibDllHeader.h"
#include "UtilOpenCVPixel.h"

///////////////////////////////////////////////////////
/*!
	���ˏ�u���[
	@param[in] src ���͉摜
	@param[out] dst �o�͉摜
	@param[in] ef �u���[�� 0~1�Ŏw��
*/
_EXPORT void RadiallyBlur(const IplImage* src, IplImage* dst, double ef);

#endif //_UTILOPENCVEFFECT_H_