#include "PolygonSelecter_Base.h"

void PolygonSelecter_Base::OnDraw(HDC hdc)
{
	HPEN hPen;
	HPEN hOldPen;
	hPen = CreatePen(PS_DASH, 0, 0);
	hOldPen = (HPEN)SelectObject(hdc, hPen);

	m_isLineVecLock = true;
	//�ێ����Ă�������̕`��
	CvLine_Vec::iterator itr = m_CvLine_Vec.begin();
	for(; itr != m_CvLine_Vec.end(); itr++){
		POINT pt[2];
		pt[0].x = (*itr).start.x; pt[0].y = (*itr).start.y;
		pt[1].x = (*itr).end.x; pt[1].y = (*itr).end.y;
		Polyline(hdc, pt, 2);
	}
	m_isLineVecLock = false;

	if(!m_isFirst){//���݂̃}�E�X�ʒu�܂ł̐����̕`��
		POINT pt[2];
		pt[0].x = m_beforePt.x; pt[0].y = m_beforePt.y;
		pt[1].x = m_nowPt.x; pt[1].y = m_nowPt.y;
		Polyline(hdc, pt, 2);
	}

	SelectObject(hdc, hOldPen);
	DeleteObject(hPen);
}

//////////////////////////
/*!
	m_CvLine_Vec�Ɋi�[����Ă�������̍ő�͈͂����߂�
	@param[out] lprect
*/
void PolygonSelecter_Base::GetLinesRect(LPRECT lprect)
{
	ImgFile_Ptr f = m_pImgEdit->GetActiveFile();
	if(f){
		CvSize size = f->GetImgSize();

		lprect->bottom = lprect->right = 0;
		lprect->top = size.height;
		lprect->left = size.width;

		CvLine_Vec::iterator itr = m_CvLine_Vec.begin();
		for(; itr != m_CvLine_Vec.end(); itr++){
			//�E�B���h�E�N���C�A���g���W����摜��̍��W�֕ϊ�
			IE_INPUT_DATA spd, epd;
			spd.x = (*itr).start.x;
			spd.y = (*itr).start.y;
			epd.x = (*itr).end.x;
			epd.y = (*itr).end.y;
			f->ConvertPosWndToImg(&spd);
			f->ConvertPosWndToImg(&epd);
			(*itr).start.x = spd.x;
			(*itr).start.y = spd.y;
			(*itr).end.x = epd.x;
			(*itr).end.y = epd.y;

			//�I��̈�����ׂĊ܂ދ�`�𒲂ׂ�
			int top = ((*itr).start.y < (*itr).end.y) ? (*itr).start.y : (*itr).end.y;
			int bottom = ((*itr).start.y > (*itr).end.y) ? (*itr).start.y : (*itr).end.y;
			int left = ((*itr).start.x < (*itr).end.x) ? (*itr).start.x : (*itr).end.x;
			int right = ((*itr).start.x > (*itr).end.x) ? (*itr).start.x : (*itr).end.x;
			
			if(lprect->top > top){
				lprect->top = top;
			}
			if(lprect->left > left){
				lprect->left = left;
			}
			if(lprect->bottom < bottom + 1){
				lprect->bottom = bottom + 1;
			}
			if(lprect->right < right + 1){
				lprect->right = right + 1;
			}
		}
	}
}

//////////////////////////
/*!
	�ێ����Ă����������I��̈�̍쐬
	@param[in] handle_name �X�V����n���h���̖��O
	@param[in] lprect �X�V����͈�
*/
void PolygonSelecter_Base::CreateMask(const char* handle_name, const LPRECT lprect)
{
	ImgFile_Ptr f = m_pImgEdit->GetActiveFile();
	if(f){
		IplImage *poly_mask = cvCreateImage(
			cvSize(lprect->right - lprect->left, lprect->bottom - lprect->top),
			IPL_DEPTH_8U,
			1);
		cvSet(poly_mask, cvScalar(0));

		int point_size = m_CvLine_Vec.size();
		CvPoint* points = new CvPoint[point_size*2];
		CvLine_Vec::iterator itr = m_CvLine_Vec.begin();
		for(int i=0; itr != m_CvLine_Vec.end(); itr++, i++){
			points[i].x = (*itr).end.x - lprect->left;
			points[i].y = (*itr).end.y - lprect->top;

			assert(0 <= points[i].x && points[i].x < poly_mask->width);
			assert(0 <= points[i].y && points[i].y < poly_mask->height);
		}		

		int npts = point_size;
		cvFillPoly(poly_mask, &points, &npts, 1, cvScalar(255));

		delete[] points;
		points = NULL;

		EditMaskHandle* pEditMaskHandle = (EditMaskHandle*)f->CreateImgFileHandle(IFH_EDIT_MASK);
		strcpy_s(pEditMaskHandle->handle_name, MAX_IMG_FILE_HANDLE_NAME, handle_name);
		f->DoImgFileHandle( pEditMaskHandle );

		EditNode* pEditNode = pEditMaskHandle->CreateEditNode(lprect);

		pEditNode->edit_img.ImgBlt(
			lprect->left,
			lprect->top,
			poly_mask->width,
			poly_mask->height,
			poly_mask,
			0,
			0);

		cvReleaseImage(&poly_mask);

		//�I��̈� �\���摜 �Đ���
		pEditMaskHandle->Update(lprect);
		pEditMaskHandle->EndEdit();
	}
}