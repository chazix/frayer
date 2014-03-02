// E_GaussianBlur.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//
#include "E_GaussianBlur.h"

#include <libImgEdit.h>

#define DEFAULT_BS 3

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
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

#define DISPLAY_NAME "ガウシアンフィルター"

EFFECT_EXPORT IEffect* CreateIEEffect()
{
	return new E_GaussianBlur;
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

E_GaussianBlur::E_GaussianBlur()
{
	m_bs = DEFAULT_BS;
}

E_GaussianBlur::~E_GaussianBlur()
{
}

bool E_GaussianBlur::Init(ImgFile_Ptr pFile)
{
	m_bs = DEFAULT_BS;

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

	//マスクの範囲取得
	if (pFile->GetMaskState() == IE_MASK_STATE_ID::INVALID) {
		m_mask = NULL;
	}
	else {
		pFile->GetMaskRect(&mask_rect);
		//範囲を計算
		AndRect(&rect, &mask_rect, &rect);

		//マスク切り出し
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

	m_pEditLayerHandle = (EditLayerHandle*) pFile->CreateImgFileHandle(IFH_EDIT_LAYER);
	m_pEditLayerHandle->SetHandleName( DISPLAY_NAME );
	pFile->DoImgFileHandle( m_pEditLayerHandle );

	m_pEditNode = m_pEditLayerHandle->CreateEditNode(&rect);
	m_pEditNode->blt_rect = rect;

	m_editImg = cvCreateImage(sizeOfImage, IPL_DEPTH_8U, 4);
	m_editedImg = cvCreateImage(sizeOfImage, IPL_DEPTH_8U, 4);

	//layerから有効範囲部分のデータを取得
	m_pEditLayer->CopyLayerImage(
		m_editImg,
		0,
		0,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top);

	return true;
}

void E_GaussianBlur::End(ImgFile_Ptr pFile)
{
	m_pEditLayerHandle->EndEdit();

	cvReleaseImage(&m_editImg);
	cvReleaseImage(&m_editedImg);

	if(m_mask){
		cvReleaseImage(&m_mask);
	}
}

void E_GaussianBlur::Edit(ImgFile_Ptr pFile)
{
	m_pEditDialog->SetProgPos(0);

	//ガウシアンフィルター
	//cvSmooth(m_editImg, m_editedImg, CV_GAUSSIAN, 2*m_bs-1, 0, 0, 0);
	cvSmooth(m_editImg, m_editedImg, CV_GAUSSIAN, 2*m_bs-1, 2*m_bs-1, 0, 0);
	m_pEditDialog->SetProgPos(25);

	//blt edited image
	m_pEditNode->edit_img.ImgBlt(
		m_pEditNode->blt_rect.left - m_pEditNode->node_rect.left,
		m_pEditNode->blt_rect.top - m_pEditNode->node_rect.top,
		m_pEditNode->blt_rect.right - m_pEditNode->blt_rect.left,
		m_pEditNode->blt_rect.bottom - m_pEditNode->blt_rect.top,
		m_editedImg,
		0, 0,
		IPLEXT_RASTER_CODE::COPY,
		m_mask,
		0, 0);

	m_pEditDialog->SetProgPos(75);

	//
	m_pEditLayerHandle->Update(&(m_pEditNode->blt_rect));
	m_pEditDialog->SetProgPos(100);
}

void E_GaussianBlur::SetEditDialog(IEffectEditDialog *pEffectEdit)
{
	m_pEditDialog = pEffectEdit;
	m_pEditDialog->SetProgRange(0, 100);
	m_pEditDialog->SetProgPos(0);

	pEffectEdit->AddTrackbar("適用半径", 1, 150, DEFAULT_BS, &m_bs);
}