#include "stdafx.h"

//#include "MaskPanel.h"
//
//#include "MaskSynthesizerMG.h"
//#include "..\resource1.h"
//
//static HWND g_hWnd;
//
//CMaskPanel::CMaskPanel()
//{
//	isSelectMask = false;
//	isInsertOver = false;
//	isInsertUnder = false;
//
//	m_MaskThm = NULL;
//	m_Mask = NULL;
//}
//
//CMaskPanel::~CMaskPanel()
//{
//}
//
//void CMaskPanel::SetMask(ImgMask *pMask)
//{
//	m_Mask = pMask;
//}
//
//////////////////////////////////////////////
///*!
//	�}�X�N�X�V�C�x���g���ɌĂяo�����B
//*/
//void CMaskPanel::OnUpdateMask()
//{
//	UpdateMaskThm();
//	this->Invalidate(FALSE);
//}
//
//////////////////////////////////////////////
///*!
//	�h���b�O���̃}�E�X���p�l���̏�ɗ����Ƃ��ɌĂяo��
//	@param[in] lpt �}�E�X�̃X�N���[�����W
//*/
//void CMaskPanel::OnDragMouse(LPPOINT lpt)
//{
//}
//
///////////////////////////////////////////////////////
///*!
//	�p�l����I������B
//*/
//void CMaskPanel::SelectPanel()
//{
//	isSelectMask = true;
//	ImgFile_Ptr pFile = m_Mask->GetParentFile();
//	if(pFile){
//		pFile->SetSelectMask( m_Mask );
//	}
//
//	this->Invalidate(FALSE);
//}
//
//////////////////////////////////////////////////
///*!
//	�p�l�����I���ɂ���B
//*/
//void CMaskPanel::NewtralPanel()
//{
//	isSelectMask = false;
//
//	this->Invalidate(FALSE);
//}
//
//////////////////////////////////////////////////
///*!
//	�h���b�O���̃C���[�W���쐬����
//	@param[out] hImageList �o�͐�̃C���[�W���X�g
//*/
//void CMaskPanel::CreateDragImage(HIMAGELIST hImageList)
//{
//}
//
////////////////////////////////////////////////
///*!
//	�}�X�N�X�^�C���_�C�A���O���J��
//*/
//void CMaskPanel::OpenMaskStyleDialog(HWND hParent)
//{
//	m_DrawModeCombo_buf = m_Mask->GetMaskSynthIndex();
//	int id = DialogBoxParam(m_hInst, MAKEINTRESOURCE(IDD_MASK_STYLE), hParent, (DLGPROC)MaskStyleDialogProc, (LPARAM)this);
//	if(id = IDOK){
//		//���C���[�̕`�惂�[�h��ύX����
//		const IMaskSynthesizer *ms = MaskSynthesizerMG::GetMaskSynth(m_DrawModeCombo_buf);
//		m_Mask->SetMaskSynth(m_DrawModeCombo_buf, ms);
//
//		ImgFile_Ptr pFile = m_Mask->GetParentFile();
//		if(pFile){
//			//�C���[�W���Đ���
//			LPUPDATE_DATA data = pFile->CreateUpdateData(UPDATE_MASK_IMAGE);
//			data->isAll = true;
//			pFile->PushUpdateData( data );
//		}
//	}
//}
//
////////////////////////////////////////////////
///*!
//	�}�X�N�폜
//*/
//void CMaskPanel::DeleteMask()
//{
//	ImgFile_Ptr pFile = m_Mask->GetParentFile();
//	if(pFile){
//		pFile->DeleteMask( m_Mask );
//
//		//�C���[�W���Đ���
//		LPUPDATE_DATA data = pFile->CreateUpdateData(UPDATE_MASK_IMAGE);
//		data->isAll = true;
//		pFile->PushUpdateData( data );
//	}
//}
//
////////////////////////////////////////////////
///*!
//	�|�b�v�A�b�v���j���[�\��
//*/
//void CMaskPanel::PopupMenu(HWND hParent, const LPPOINT lpt){
//	//���j���[�̍쐬�E�\��
//	//IEMENU hmenu = IEMenuMG::CreateIEPopupMenu();	
//
//	//IEMenuItemInfo ieii;
//	//ieii.SetMask(MIIM_STRING | MIIM_ID | MIIM_STATE );
//	//ieii.SetState( MFS_ENABLED );
//
//	//ieii.SetTypeData(TEXT("���C���[�𕡐�����"));
//	//ieii.SetID( LW_CM_COPY_LAYER );
//	//IEMenuMG::InsertIEMenuItem(hmenu, 1, TRUE, &ieii);
//
//	//ieii.SetTypeData(TEXT("���C���[���폜����"));
//	//ieii.SetID( LW_CM_DELETE_LAYER );
//	//IEMenuMG::InsertIEMenuItem(hmenu, 1, TRUE, &ieii);
//
//	//IEMenuMG::TrackPopupIEMenu(hmenu, TPM_LEFTALIGN|TPM_TOPALIGN, lpt->x, lpt->y, 0, hParent, NULL);
//	//IEMenuMG::DestroyIEMenu(hmenu);
//}
//
////////////////////////////////////////////////
///*!
//	�}�X�N�I�u�W�F�N�g�ւ̃|�C���^��Ԃ��B
//	@return �}�X�N�I�u�W�F�N�g�ւ̃|�C���^
//*/
//ImgMask* CMaskPanel::GetMask()
//{
//	return m_Mask;
//}
//
//void CMaskPanel::UpdateMaskThm()
//{
//	//RECT rc;
//	//this->GetClientRect(&rc);
//	//int panel_width = rc.right - rc.left;
//	//int panel_height = rc.bottom - rc.top;
//
//	//if(m_MaskThm){
//	//	cvReleaseImage(&m_MaskThm);
//	//}
//
//	//m_MaskThm = cvCreateImage(cvSize(panel_height, panel_height), IPL_DEPTH_8U, 3);
//	//m_Mask->CopyThumbnailImage(m_MaskThm);
//
//	//m_MaskThmRect.top = 0;
//	//m_MaskThmRect.left = 100;
//	//m_MaskThmRect.right = 100 + m_MaskThm->width;
//	//m_MaskThmRect.bottom = m_MaskThm->height;
//}
//
//void CMaskPanel::Draw(CDC* pDC)
//{
//}
//
////////////////////////////////////////////////
///*!
//	�}�X�N�X�^�C���_�C�A���O�̃v���V�[�W��
//*/
//LRESULT CALLBACK CMaskPanel::MaskStyleDialogProc(HWND hDlgWnd, UINT msg, WPARAM wp, LPARAM lp)
//{
//	static CMaskPanel* pMaskPane;
//
//	switch(msg){
//		case WM_INITDIALOG:
//			{
//				//������
//				pMaskPane = (CMaskPanel*)lp;
//				pMaskPane->InitMaskStyleDialog(hDlgWnd);
//			}
//			return TRUE;
//		case WM_COMMAND:
//			switch(LOWORD(wp)){
//				case IDOK:
//					EndDialog(hDlgWnd, IDOK);
//					break;
//				case IDCANCEL:
//					EndDialog(hDlgWnd, IDCANCEL);
//					break;
//				case IDC_DRAW_MODE_COMBO:
//					if(HIWORD(wp) == CBN_SELCHANGE){
//						pMaskPane->OnChangeDrawModeCombo(hDlgWnd);
//					}				
//					break;
//				default:
//					return FALSE;
//			}
//		default:
//			return FALSE;
//	}
//
//	return TRUE;
//}
//
//BEGIN_MESSAGE_MAP(CMaskPanel, CWnd)
//	ON_WM_CREATE()
//	ON_WM_SIZE()
//	ON_WM_PAINT()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONDBLCLK()
//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONUP()
//	ON_WM_RBUTTONUP()
//END_MESSAGE_MAP()
//
//int CMaskPanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if(0 != CWnd::OnCreate(lpCreateStruct)){
//		return -1;
//	}
//
//	m_hInst = lpCreateStruct->hInstance;
//
//	return 0;
//}
//
//void CMaskPanel::OnSize(UINT nType, int cx, int cy)
//{
//	CWnd::OnSize(nType, cx, cy);
//}
//
//void CMaskPanel::OnPaint()
//{
//	if(!m_Mask) return;
//
//	RECT rc;
//	this->GetClientRect(&rc);
//	int panel_width = rc.right - rc.left;
//	int panel_height = rc.bottom - rc.top;
//	
//	//�w�i
//	CPaintDC dc(this);
//	if(isSelectMask){
//		dc.FillSolidRect(&rc, RGB(100, 100, 200));
//	}else{
//		dc.FillSolidRect(&rc, RGB(200, 200, 200));
//	}
//
//	//�t�H���g�ݒ�
//	HFONT hFont, hOldFont;
//	LOGFONT FontInfo;
//	memset(&FontInfo, 0, sizeof(FontInfo));
//	strcpy(FontInfo.lfFaceName, MASK_NAME_FONT_NAME);
//	FontInfo.lfWeight = FW_BOLD;
//	FontInfo.lfHeight = MASK_NAME_FONT_SIZE;
//	FontInfo.lfCharSet = SHIFTJIS_CHARSET;
//
//	hFont = ::CreateFontIndirect(&FontInfo);
//	hOldFont = (HFONT)::SelectObject(dc.GetSafeHdc(), hFont);
//
//	//�}�X�N���\��
//	RECT text_rect;
//	text_rect.top = panel_height/2 - MASK_NAME_FONT_SIZE/2;
//	text_rect.left = 40;
//	text_rect.bottom = text_rect.top + MASK_NAME_FONT_SIZE;
//	text_rect.right = 160;
//	
//	char mask_name[MAX_IMG_MASK_NAME];
//	m_Mask->GetName(mask_name);
//	dc.DrawText(mask_name, strlen(mask_name), &text_rect, DT_LEFT);
//
//	//�t�H���g�����ɖ߂�
//	::SelectObject(dc.GetSafeHdc(), hOldFont);
//
//	////���E�s���A�C�R���\��
//	//CBitmap* cOldBmp = NULL;
//	//CDC cMemDC;
//	//cMemDC.CreateCompatibleDC(NULL);
//	//if(m_Mask->isVisible()){
//	//	cOldBmp = cMemDC.SelectObject(&m_cEye);
//	//}else{
//	//	cOldBmp = cMemDC.SelectObject(&m_cNEye);
//	//}
//	//dc.BitBlt(m_EyeRect.left, m_EyeRect.top,
//	//	m_EyeRect.right - m_EyeRect.left, m_EyeRect.bottom - m_EyeRect.top,
//	//	&cMemDC, 0, 0, SRCCOPY);
//	//cMemDC.SelectObject(cOldBmp);
//	//cMemDC.DeleteDC();
//
//	BITMAPINFO bmpi;
//	VOID *pBit;
//	//���C���[�T���l
//	if(m_MaskThm){
//		ConvertIPLImageToDIB(m_MaskThm, &bmpi, &pBit);
//		SetDIBitsToDevice(dc.GetSafeHdc(), 100, 0, m_MaskThm->width, m_MaskThm->height,
//			0, 0, 0, m_MaskThm->height, pBit, &bmpi, DIB_RGB_COLORS);
//	}
//}
//
//void CMaskPanel::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	RECT rc;
//	POINT pt;
//	this->GetClientRect(&rc);
//	pt.x = point.x - rc.left;
//	pt.y = point.y - rc.top;
//
//	////�ډ摜�Ƃ̃N���b�N����
//	//if((m_EyeRect.left <= pt.x && pt.x <= m_EyeRect.right )&&
//	//	(m_EyeRect.top <= pt.y && pt.y <= m_EyeRect.bottom ))
//	//{
//	//	bool bl = m_Layer->isVisible();
//	//	if(bl) bl = false;
//	//	else   bl = true;
//	//	m_Layer->SetVisible( bl );
//
//	//	ImgFile_Ptr pFile = m_Layer->GetParentFile();
//	//	if(pFile){
//	//		//�\���摜�̍Đ���
//	//		UPDATE_DATA data;
//	//		data.isAll = true;
//	//		data.update_flag = UPDATE_FLAG::UPDATE_FILE_IMAGE;
//	//		pFile->PushUpdateData(&data);
//	//	}
//	//}
//
//	pt.x = point.x; pt.y = point.y;
//	this->ClientToScreen(&pt);
//	CWnd* parent = this->GetParent();
//	parent->ScreenToClient(&pt);
//	LPARAM newLp = pt.x | (pt.y << 16);
//	this->GetParent()->SendMessage(WM_LBUTTONDOWN, MK_LBUTTON, newLp);
//
//	return;
//}
//
//void CMaskPanel::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	POINT pt;
//	pt.x = point.x; pt.y = point.y;
//	this->ClientToScreen(&pt);
//	CWnd* parent = this->GetParent();
//	parent->ScreenToClient(&pt);
//	LPARAM newLp = pt.x | (pt.y << 16);
//	this->GetParent()->SendMessage(WM_LBUTTONUP, MK_LBUTTON, newLp);
//}
//
//void CMaskPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	//���C���[�X�^�C���_�C�A���O���J��
//	OpenMaskStyleDialog( GetSafeHwnd() );
//}
//
//void CMaskPanel::OnMouseMove(UINT nFlags, CPoint point)
//{
//	POINT pt;
//	pt.x = point.x; pt.y = point.y;
//	this->ClientToScreen(&pt);
//	CWnd* parent = this->GetParent();
//	parent->ScreenToClient(&pt);
//	LPARAM newLp = pt.x | (pt.y << 16);
//	this->GetParent()->SendMessage(WM_MOUSEMOVE, 0, newLp);
//}
//
//void CMaskPanel::OnRButtonUp(UINT nFlags, CPoint point)
//{
//	POINT pt;
//	pt.x = point.x; pt.y = point.y;
//	this->ClientToScreen(&pt);
//	CWnd* parent = this->GetParent();
//	parent->ScreenToClient(&pt);
//	LPARAM newLp = pt.x | (pt.y << 16);
//	this->GetParent()->SendMessage(WM_RBUTTONUP, 0, newLp);
//}
//
//void CMaskPanel::InitMaskStyleDialog(HWND hDlgWnd)
//{
//	//�`�惂�[�h�̃R���{�{�b�N�X�̃n���h���𓾂�
//	HWND hCombo = ::GetDlgItem(hDlgWnd, IDC_MASK_DRAW_MODE_COMBO);
//	//�R���{�{�b�N�X�ɃA�C�e����ǉ�
//	MaskSynthesizerMG::SetDrawModeComboBox(hCombo);
//	//���݂̃��C���[�̕`�惂�[�h��I����Ԃ�
//	::SendMessage(hCombo, CB_SETCURSEL, m_Mask->GetMaskSynthIndex(), 0);
//}
//
//void CMaskPanel::OnChangeDrawModeCombo(HWND hDlgWnd)
//{
//	//�`�惂�[�h�̃R���{�{�b�N�X�̃n���h���𓾂�
//	HWND hCombo = ::GetDlgItem(hDlgWnd, IDC_MASK_DRAW_MODE_COMBO);
//	//�I������Ă���A�C�e���̃C���f�b�N�X�𓾂�
//	m_DrawModeCombo_buf = ::SendMessage(hCombo, CB_GETCURSEL, 0, 0);
//}
