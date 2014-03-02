#include "stdafx.h"

#include "OpenCV2VC.h"



/////////////////////////////////////////////////////
/*!
	IplImage����DIB�֕ϊ�
	IplImage�̃������̈�����̂܂܎g�p����̂Œ��ӂ���B
	@param[in] image �ϊ�����IplImage
	@param[out] lpbmi bitmap�w�b�_�[ 
	@param[out] lpBit bitmap�f�[�^
*/
_EXPORT BOOL ConvertIPLImageToDIB(const IplImage* image, BITMAPINFO *lpbmi, VOID **lpBit)
{
	if(!image) return FALSE;

	lpbmi->bmiHeader.biSize   = sizeof(BITMAPINFOHEADER);
	lpbmi->bmiHeader.biWidth  = image->width;
	lpbmi->bmiHeader.biHeight = -image->height;
	lpbmi->bmiHeader.biPlanes = 1;
	lpbmi->bmiHeader.biBitCount			= 8*image->nChannels;		//1�s�N�Z��������̃r�b�g��
	lpbmi->bmiHeader.biCompression		= BI_RGB;					//���k�`��
	lpbmi->bmiHeader.biSizeImage		= 0;
	lpbmi->bmiHeader.biXPelsPerMeter	= 1;
	lpbmi->bmiHeader.biYPelsPerMeter	= 1;
	lpbmi->bmiHeader.biClrUsed			= 0;
	lpbmi->bmiHeader.biClrImportant		= 0;

	(*lpBit) = image->imageData;
	return TRUE;
}

//////////////////////////////////////////////////////
/*
	�f�o�C�X�R���e�L�X�g��IplImage����������
	@param[in] hdc �������ݐ�f�o�C�X�R���e�L�X�g
	@param[in] x �������݊J�n�ʒu
	@param[in] y �������݊J�n�ʒu
	@param[in] cx �������ݕ�
	@param[in] cy �������ݍ���
	@param[in] src_img �������މ摜�f�[�^
	@param[in] x1 �摜�f�[�^�������݊J�n�ʒu
	@param[in] y1 �摜�f�[�^�������݊J�n�ʒu
*/
_EXPORT VOID IPLImageToDevice(HDC hdc, int x, int y, int cx, int cy,
				 const IplImage* src_img, int x1, int y1)
{
	BITMAPINFO bmpi;
	LPVOID pBit;

	ConvertIPLImageToDIB(src_img, &bmpi, &pBit);
	::SetDIBitsToDevice(hdc, x, y, cx, cy, x1, y1, 0, src_img->height, pBit, &bmpi, DIB_RGB_COLORS);
}