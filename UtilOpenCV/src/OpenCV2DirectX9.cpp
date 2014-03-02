#include "OpenCV2DirectX9.h"

HRESULT CreateTextureIPLImage(LPDIRECT3DDEVICE9 pDevice, IplImage* iplImage, UINT Width, UINT Height, UINT Levels, LPDIRECT3DTEXTURE9* ppTexture)
{	
	HRESULT hr;			
	
	//�e�N�X�`���쐬
	hr = pDevice->CreateTexture(Width, Height, Levels, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, ppTexture, NULL);
	if(FAILED(hr)){
		return hr;
	}

	//���b�N
	D3DLOCKED_RECT pLockedRect;
	hr = (*ppTexture)->LockRect(0, &pLockedRect, NULL, 0);
	if(FAILED(hr)){
		return hr;
	}

	//�]��
	switch(iplImage->depth){
		case IPL_DEPTH_8U:
		{
			if(iplImage->nChannels <= 2){//�`�����l������2�ȉ��Ȃ�G���[��Ԃ�
				return E_FAIL;
			}
			
			int byte = iplImage->nChannels*8;
			unsigned long data; //32bit
			unsigned long r;
			unsigned long g;
			unsigned long b;
			unsigned long a;
			unsigned long *to = (unsigned long*)pLockedRect.pBits;
			for(UINT y=0; y<Height; y++){
				for(UINT x=0; x<Width; x++){
					data = 0;

					b = ((unsigned char*)(iplImage->imageData + iplImage->widthStep * y))[x * 3];		//B
					data += b;
					g = ((unsigned char*)(iplImage->imageData + iplImage->widthStep * y))[x * 3 + 1];	//G
					g <<= 8;
					data += g;
					r = ((unsigned char*)(iplImage->imageData + iplImage->widthStep * y))[x * 3 + 2];	//R
					r <<= 16;
					data += r;

					if(iplImage->nChannels == 4){
						a = ((unsigned char*)(iplImage->imageData + iplImage->widthStep * y))[x * 3 + 3];	//A
						a <<= 24;
						data += a;
					}else{
						data += 0xFF000000;
					}
					*to++ = data;
				}
			}
		}
		break;
		default:
			return E_FAIL;
			break;
	}

	//�A�����b�N
	hr = (*ppTexture)->UnlockRect(0);
	if(FAILED(hr)){
		return hr;
	}

	return S_OK;
}
