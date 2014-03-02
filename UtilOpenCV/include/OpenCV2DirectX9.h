#ifndef _OPENCV2DIRECTX9_H_
#define _OPENCV2DIRECTX9_H_

#include <d3dx9.h>
#include <cxcore.h>

///////////////////////////////////////////////////////
/*!
	IplImage����DirectX�e�N�X�`�����쐬����B
	@param[in] pDevice �g�p����f�o�C�X
	@param[in] Width �쐬����e�N�X�`���̕�
	@param[in] Height �쐬����e�N�X�`���̍���
	@param[in] Levels �~�b�v�}�b�v���x��
	@param[out] ppTexture �쐬���ꂽ�e�N�X�`�� ���\�[�X��\��
*/
HRESULT CreateTextureIPLImage(LPDIRECT3DDEVICE9 pDevice, IplImage* iplImage, UINT Width, UINT Height, UINT Levels, LPDIRECT3DTEXTURE9* ppTexture);

#endif //_OPENCV2DIRECTX9_H_