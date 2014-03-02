// LassoSelecter.cpp : DLL �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "resource.h"
#include "LassoSelecter.h"

// for detect memory leak
#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <cstdlib>
	#include <crtdbg.h>
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

static HMODULE g_hLibrary;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	g_hLibrary = hModule;
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#define DISPLAY_NAME_JA "������I��"
#define DISPLAY_NAME DISPLAY_NAME_JA

IETOOL_EXPORT IEMODULE_ID GetIEModule_ID()
{
	return IEMODULE_ID::TOOL;
}

IETOOL_EXPORT IIETool* CreateIETool()
{
	return new LassoSelecter;
}

IETOOL_EXPORT void ReleaseIETool(IIETool** ppTool)
{
	if (*ppTool) {
		delete (*ppTool);
		(*ppTool) = NULL;
	}
}


LassoSelecter::LassoSelecter()
{
	m_isSelected = false;
	strcpy_s(m_name, MAX_IE_MODULE_NAME, "LassoSelecter");
	strcpy_s(m_disp_name, MAX_IE_MODULE_DISPLAY_NAME, DISPLAY_NAME);
}

LassoSelecter::~LassoSelecter()
{
	if (m_buttonImg) {
		::DeleteObject(m_buttonImg);
		m_buttonImg = NULL;
	}
}

HBITMAP LassoSelecter::GetButtonImg()
{
	if (m_buttonImg) {
		::DeleteObject(m_buttonImg);
		m_buttonImg = NULL;
	}

	m_buttonImg = ::LoadBitmap((HINSTANCE)g_hLibrary, MAKEINTRESOURCE(IDB_BUTTON_BMP));
	return m_buttonImg;
}

void LassoSelecter::OnMouseLButtonDown(UINT nFlags, const LPIE_INPUT_DATA lpd)
{
	m_beforePt.x = m_nowPt.x = lpd->x;
	m_beforePt.y = m_nowPt.y = lpd->y;

	m_isSelected = false;
}

void LassoSelecter::OnMouseLDrag(UINT nFlags, const LPIE_INPUT_DATA lpd)
{
	if(m_isSelected == false){
		m_nowPt.x = lpd->x;
		m_nowPt.y = lpd->y;
		Push_Line(lpd);
	}
}

void LassoSelecter::OnMouseLButtonUp(UINT nFlags, const LPIE_INPUT_DATA lpd)
{
	if(m_isSelected == false){
		//�i�[�����̏���
		m_CvLine_Vec.clear();
	}
}

void LassoSelecter::Push_Line(LPIE_INPUT_DATA lpd){
	//���݂̓_��������𐶐�
	CvLine2D64f line;
	CvPoint2D64f pt;

	pt.x = lpd->x;
	pt.y = lpd->y;
	line.start = m_beforePt;
	line.end = m_beforePt = pt;

	//��������A����������I��̈�𐶐����ďI������
	for(int i=0; i < m_CvLine_Vec.size(); i++){
		if(IsIntersect(&(m_CvLine_Vec[i]), &line)){
			//�X�V�͈͂����߂�
			RECT rect;
			GetLinesRect(&rect);

			//�������Ă����炻�̌�_�����߂�
			CvPoint2D64f p;
			IntersectPoint(&p, &(m_CvLine_Vec[i]), &line);
			//�I��̈悪��_���܂ݕ����`�ɂȂ�悤�ɂ���
			m_CvLine_Vec[i].start = p;
			line.end = p;
			//�Ō�̐����̒ǉ�
			//�X�s�����b�N
			while(m_isLineVecLock);
			m_CvLine_Vec.push_back(line);
			
			//�����}�`�ɂ������Ȃ�����������
			
			for(int j=0; j < i; j++){
				CvLine_Vec::iterator itr = m_CvLine_Vec.begin();
				m_CvLine_Vec.erase(itr);
			}

			//�I��̈�̍쐬
			CreateMask(DISPLAY_NAME, &rect);

			//�X�s�����b�N
			while(m_isLineVecLock);
			m_CvLine_Vec.clear();
			m_isSelected = true;
			return;
		}
	}

	//�X�s�����b�N
	while(m_isLineVecLock);
	m_CvLine_Vec.push_back(line);
}
