// E_ColorReverse.cpp : DLL �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//
#include "stdafx.h"
#include "E_ColorReverse.h"

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

#define DISPLAY_NAME "�F���]"

EFFECT_EXPORT IEffect* CreateIEEffect()
{
	return new E_ColorReverse;
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

E_ColorReverse::E_ColorReverse()
{
}

E_ColorReverse::~E_ColorReverse()
{
}

bool E_ColorReverse::Init(ImgFile_Ptr pFile)
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

	return true;
}

void E_ColorReverse::End(ImgFile_Ptr pFile)
{
	m_pEditLayerHandle->EndEdit();

	if(m_mask){
		cvReleaseImage(&m_mask);
	}
}

void E_ColorReverse::Edit(ImgFile_Ptr pFile)
{
	m_pEditDialog->SetProgPos(0);

	ColorReverseBltOp bltOp;
	m_pEditNode->edit_img.OperateBlt(
		bltOp,
		m_pEditNode->blt_rect.left,
		m_pEditNode->blt_rect.top,
		m_pEditNode->blt_rect.right - m_pEditNode->blt_rect.left,
		m_pEditNode->blt_rect.bottom - m_pEditNode->blt_rect.top,
		m_pEditLayer->GetLayerImage(),
		m_pEditNode->blt_rect.left,
		m_pEditNode->blt_rect.top);

	//
	m_pEditLayerHandle->Update( &(m_pEditNode->blt_rect) );

	m_pEditDialog->SetProgPos(100);
}

void E_ColorReverse::SetEditDialog(IEffectEditDialog *pEffectEdit)
{
	m_pEditDialog = pEffectEdit;
	m_pEditDialog->SetProgRange(0, 100);
	m_pEditDialog->SetProgPos(0);
}