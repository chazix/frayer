// E_EdgeSobel.cpp : DLL �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "E_EdgeSobel.h"


static const int SCALE = 1;
static const int SHIFT = 0;
static int SOBEL_APERTURE_SIZE = 3;



#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

// for detect memory leak
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <cstdlib>
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define DISPLAY_NAME "�G�b�W���o(Sobel)"

EFFECT_EXPORT IEffect* CreateIEEffect()
{
	return new E_EdgeSobel;
}

EFFECT_EXPORT void ReleaseIEEffect(IEffect** ppEffect)
{
	if (*ppEffect) {
		delete (*ppEffect);
		(*ppEffect) = NULL;
	}
}

EFFECT_EXPORT IEMODULE_ID GetIEModule_ID()
{
	return IEMODULE_ID::EFFECT;
}

EFFECT_EXPORT void GetEffectDisplayNameJa(char* dst)
{
	strcpy_s(dst, MAX_IE_MODULE_DISPLAY_NAME, DISPLAY_NAME);
}

E_EdgeSobel::E_EdgeSobel()
{
}

E_EdgeSobel::~E_EdgeSobel()
{
}

bool E_EdgeSobel::Init(ImgFile_Ptr pFile)
{
	IImgLayer_weakPtr _sl = pFile->GetSelectLayer();
	if (_sl.expired()) {
		OutputError::Alert(IE_ERROR_SELECT_LAYER_DONT_EXIST);
		return false;
	}

	IImgLayer_Ptr sl = _sl.lock();
	if (sl->GetLayerType() != IE_LAYER_TYPE::NORMAL_LAYER) {
		OutputError::Alert(IE_ERROR_CANT_EDIT_LAYER);
		return false;
	}
	m_pEditLayer = std::dynamic_pointer_cast<ImgLayer>(sl);

	RECT rect, mask_rect;
	m_pEditLayer->GetLayerRect(&rect);

	//�}�X�N�͈͎̔擾
	if (pFile->GetMaskState() == IE_MASK_STATE_ID::INVALID) {
		m_mask = NULL;
	}
	else {
		pFile->GetMaskRect(&mask_rect);
		//�͈͂��v�Z
		AndRect(&rect, &mask_rect, &rect);

		//�}�X�N�؂�o��
		IplImage* tmp_mask = cvCreateImage(
			cvSize(rect.right - rect.left, rect.bottom - rect.top),
			IPL_DEPTH_8U,
			1);

		pFile->CopyFileMask(
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			tmp_mask,
			0, 0);

		m_mask = tmp_mask;
	}

	CvSize sizeOfImage;
	sizeOfImage.width = rect.right - rect.left;
	sizeOfImage.height = rect.bottom - rect.top;

	m_pEditLayerHandle = (EditLayerHandle*)pFile->CreateImgFileHandle( IFH_EDIT_LAYER );
	m_pEditLayerHandle->SetHandleName( DISPLAY_NAME );
	pFile->DoImgFileHandle( m_pEditLayerHandle );

	m_pEditNode = m_pEditLayerHandle->CreateEditNode(&rect);
	m_pEditNode->blt_rect = rect;

	m_editImage = cvCreateImage(sizeOfImage, IPL_DEPTH_8U, 4);
	m_grayImage = cvCreateImage(sizeOfImage, IPL_DEPTH_8U, 1);
	m_sobelXImage16 = cvCreateImage(sizeOfImage, IPL_DEPTH_16S, 1);
	m_sobelYImage16 = cvCreateImage(sizeOfImage, IPL_DEPTH_16S, 1);
	m_sobelImage8  = cvCreateImage(sizeOfImage, IPL_DEPTH_8U, 1);
	m_editedImage = cvCreateImage(sizeOfImage, IPL_DEPTH_8U, 4);

	//layer����L���͈͕����̃f�[�^���擾
	m_pEditLayer->CopyLayerImage(
		m_editImage,
		0,
		0,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top);

	return true;
}

void E_EdgeSobel::End(ImgFile_Ptr pFile)
{
	m_pEditLayerHandle->EndEdit();

	if(m_mask){
		cvReleaseImage(&m_mask);
	}

	cvReleaseImage(&m_editImage);
	cvReleaseImage(&m_grayImage);
	cvReleaseImage(&m_sobelXImage16);
	cvReleaseImage(&m_sobelYImage16);
	cvReleaseImage(&m_sobelImage8);
	cvReleaseImage(&m_editedImage);
}

void E_EdgeSobel::Edit(ImgFile_Ptr pFile)
{
	//�O���[�X�P�[���쐬
	cvCvtColor(m_editImage, m_grayImage, CV_BGRA2GRAY);
	//X������Sobel�I�y���[�^�[��������
	cvSobel(m_grayImage, m_sobelXImage16, 1, 0, SOBEL_APERTURE_SIZE);
	//Y������Sobel�I�y���[�^�[��������
	cvSobel(m_grayImage, m_sobelYImage16, 1, 0, SOBEL_APERTURE_SIZE);
	
	//X������Y�����̌��ʂ����Z
	cvAdd(m_sobelXImage16, m_sobelYImage16, m_sobelXImage16, m_mask);
	//��������16bit���畄���Ȃ�8bit�l��
	cvConvertScaleAbs(m_sobelXImage16, m_sobelImage8, SCALE, SHIFT);
	//�O���[�X�P�[������BGR��
	ucvCvtColor(m_sobelImage8, m_editedImage, CV_GRAY2BGRA);
	
	//blt edited image
	m_pEditNode->edit_img.ImgBlt(
		m_pEditNode->blt_rect.left - m_pEditNode->node_rect.left,
		m_pEditNode->blt_rect.top - m_pEditNode->node_rect.top,
		m_pEditNode->blt_rect.right - m_pEditNode->blt_rect.left,
		m_pEditNode->blt_rect.bottom - m_pEditNode->blt_rect.top,
		m_editedImage,
		0, 0,
		IPLEXT_RASTER_CODE::COPY,
		m_mask,
		0, 0);

	//
	m_pEditLayerHandle->Update( &(m_pEditNode->blt_rect) );
}

void E_EdgeSobel::SetEditDialog(IEffectEditDialog *pEditDialog)
{
}