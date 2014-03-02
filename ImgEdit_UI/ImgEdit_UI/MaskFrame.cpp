#include "stdafx.h"
//
//#include "MaskFrame.h"
//
//extern ImgEdit g_ImgEdit;
//
//CMaskFrame::CMaskFrame()
//{
//	m_hInst = NULL;
//	m_SelectMaskPanel = NULL;
//	m_DragMaskPanel   = NULL;
//	m_UnderMaskPanel  = NULL;
//	m_PopupMaskPanel  = NULL;
//	m_pActiveFile = NULL;
//	m_hPanelDragImage = NULL;
//	isPanelDrag = false;
//
//	//scrollbar init
//	SetScrollSizes(MM_TEXT, CSize(0,0));
//}
//
//CMaskFrame::~CMaskFrame()
//{
//}
//
//////////////////////////////////////////////////////
///*!
//	�A�N�e�B�u�ɂȂ����t�@�C�����ς�����Ƃ��ɌĂяo�����C�x���g
//	@param file �A�N�e�B�u�ȃt�@�C���I�u�W�F�N�g
//*/
//void CMaskFrame::OnChangeActiveFile(ImgFile_Ptr pNewFile, ImgFile_Ptr pOldFile)
//{
//	m_pActiveFile = pFile; //�t�@�C���ւ̃|�C���^��ێ�
//	
//	ReSetAllPanel( pFile );
//	AdjastScrollSizes();
//}
//
///////////////////////////////////////////////////////
///*!
//	�t�@�C�����ǉ����ꂽ�Ƃ��ɌĂяo���C�x���g
//	@param[in] pFile �ǉ����ꂽ�t�@�C��
//*/
//void CMaskFrame::OnAddImgFile(ImgFile_Ptr pFile)
//{
//	pFile->AddEventListener( this );
//}
//
///////////////////////////////////////////////////////
///*!
//	�}�X�N���ǉ����ꂽ�Ƃ��ɌĂяo�����C�x���g
//	@param pMask �ǉ����ꂽ�}�X�N
//*/
//void CMaskFrame::OnAddMask(ImgMask* pMask, int index)
//{
//	AddMaskPanel(pMask, index);
//	
//	AdjastPanel();
//	AdjastScrollSizes();
//}
//
///////////////////////////////////////////////////////
///*!
//	�}�X�N���폜�����Ƃ��ɌĂяo�����C�x���g
//	@param[in] pLayer �폜�����}�X�N
//	@param[in] index �}�X�N�C���f�b�N�X
//*/
//void CMaskFrame::OnDeleteMask(ImgMask *pMask, int index)
//{
//	DeleteMaskPanel(index);
//	
//	AdjastPanel();
//	AdjastScrollSizes();
//}
//
//void CMaskFrame::OnDraw(CDC *pDC)
//{
//}
//
//void CMaskFrame::OnInitialUpdate()
//{
//}
//
//BEGIN_MESSAGE_MAP(CMaskFrame, CScrollWnd)
//	ON_WM_CREATE()
//	ON_WM_SIZE()
//	ON_WM_PAINT()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONUP()
//	ON_WM_RBUTTONUP()
//END_MESSAGE_MAP()
//
//int CMaskFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
//{
//	if(0 != CScrollWnd::OnCreate(lpCreateStruct)){
//		return -1;
//	}
//
//	m_width = lpCreateStruct->cx;
//	m_hInst = lpCreateStruct->hInstance;
//
//	//�C�x���g���X�i�[�o�^
//	g_ImgEdit.AddEventListener( this );
//
//	return 0;
//}
//
//void CMaskFrame::OnSize(UINT nType, int cx, int cy)
//{
//	AdjastPanel();
//	AdjastScrollSizes();
//	CScrollWnd::OnSize(nType, cx, cy);
//}
//
//void CMaskFrame::OnPaint()
//{
//	CPaintDC dc(this);
//	CRect rect;
//	GetWindowRect(rect);
//	ScreenToClient(rect);
//
//	rect.InflateRect(1, 1);
//	dc.FillSolidRect(rect, ::GetSysColor(COLOR_WINDOW));
//}
//
//void CMaskFrame::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	m_UnderMaskPanel = NULL;
//
//	m_DragMaskPanel = GetMaskPanelFromPos(point);
//	if(m_DragMaskPanel){
//		isPanelDrag = true;
//		//�C���[�W���X�g�̍쐬
//		m_hPanelDragImage = ImageList_Create(m_width, MASK_PANEL_H, ILC_COLOR4, 1,1);
//		m_DragMaskPanel->CreateDragImage( m_hPanelDragImage );
//		
//		//�C���[�W�̃h���b�O�J�n
//		//���C���[�p�l���͈̔͂��擾���āA�h���b�O�C���[�W�̈ʒu����
//		RECT rc;
//		m_DragMaskPanel->GetClientRect(&rc);
//		POINT pt;
//		pt.x = point.x; pt.y = point.y;
//		ClientToScreen(&pt);
//		ImageList_BeginDrag( m_hPanelDragImage, 0, pt.x - rc.left, pt.y - rc.top);
//		ImageList_DragEnter(NULL, point.x, point.y);
//	}
//
//	CScrollWnd::OnLButtonDown(nFlags, point);
//}
//
//void CMaskFrame::OnMouseMove(UINT nFlags, CPoint point)
//{
//	if(isPanelDrag){
//		POINT pt;
//		GetCursorPos(&pt);
//		ImageList_DragMove(pt.x, pt.y);
//
//		//�}�E�X�̉��ɂ��郌�C���[�p�l�����Ăяo��
//		CMaskPanel* lp = GetMaskPanelFromPos(point);
//		if(lp){
//			lp->OnDragMouse(&pt);
//			if(lp != m_UnderMaskPanel){	
//				m_UnderMaskPanel = lp;
//			}
//		}
//		AllPanelRePaint();
//	}
//
//	CScrollWnd::OnMouseMove(nFlags, point);
//}
//
//void CMaskFrame::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	if(isPanelDrag){
//		m_UnderMaskPanel = NULL;
//
//		isPanelDrag = false;
//		ImageList_EndDrag();
//		ImageList_DragLeave( GetSafeHwnd() );
//		ImageList_Destroy(m_hPanelDragImage);
//
//		//�}�E�X�̍��W���牺�ɂ���E�B���h�E�̃n���h���𓾂�
//		POINT pt;
//		GetCursorPos(&pt);
//		CWnd* wnd = WindowFromPoint(pt);
//
//		//�h���b�v����E�B���h�E���擾���A���ꂪ�����̃t�@�C���̃E�B���h�E�o�Ȃ����
//		//���b�Z�[�W�𑗂�A�h���b�v�������C����ǉ�������B
//		if(wnd && wnd != this){
//			//wnd->SendMessage(WM_USER, UM_DROPLAYER, (LPARAM)m_DragMaskPanel->GetMask());
//			//
//			////�\���摜�̍Đ���
//			//UPDATE_DATA data;
//			//data.isAll = true;
//			//data.update_flag = UPDATE_FLAG::UPDATE_MASK_IMAGE;
//			//m_pActiveFile->PushUpdateData(&data);
//		}
//
//	}
//
//	//�J�[�\�����h���b�O�J�n���̃p�l���̏�ɂ���Ȃ炻�̃p�l����I����Ԃ�
//	CMaskPanel *mp = GetMaskPanelFromPos(point);
//	if(mp && mp == m_DragMaskPanel){
//		//�I�����C���[��ύX����
//		if(m_SelectMaskPanel){
//			m_SelectMaskPanel->NewtralPanel();
//		}
//		m_SelectMaskPanel = mp;
//		m_SelectMaskPanel->SelectPanel();
//	}
//
//	CScrollWnd::OnLButtonUp(nFlags, point);
//}
//
//void CMaskFrame::OnRButtonUp(UINT nFlags, CPoint point)
//{
//	m_PopupMaskPanel = GetMaskPanelFromPos(point);
//	if(m_PopupMaskPanel){
//		POINT spt;
//		spt.x = point.x;
//		spt.y = point.y;
//		ClientToScreen(&spt);
//		m_PopupMaskPanel->PopupMenu( GetSafeHwnd(), &spt);
//	}
//}
//
/////////////////////////////////////////////////////
///*!
//	�R�}���h����
//*/
//BOOL CMaskFrame::OnCommand(WPARAM wp, LPARAM lp)
//{
//	//switch(LOWORD(wp)){
//	//	case LW_CM_COPY_LAYER:
//	//		{
//	//			int index = m_pActiveFile->GetLayerSize();
//	//			AddCopyLayerHandle* handle = (AddCopyLayerHandle*)ImgFileHandleMG::CreateImgFileHandle( m_pActiveFile, IFH_ADD_COPY_LAYER );
//	//			handle->SetAddToIndex( index );
//	//			m_pActiveFile->DoImgFileHandle( handle );
//
//	//			m_pActiveFile->SetSelectLayer( index );
//	//		}
//	//		return TRUE;
//	//	case LW_CM_DELETE_LAYER:
//	//		{
//	//			DeleteLayerPanel( m_PopupLayerPanel );
//	//			m_PopupLayerPanel = NULL;
//	//		}
//	//		return TRUE;
//	//}
//
//	return CScrollWnd::OnCommand(wp, lp);
//}
//
////////////////////////////////////////
///*!
//	�X�N���[���o�[�T�C�Y����
//*/
//void CMaskFrame::AdjastScrollSizes()
//{
//	RECT rc;
//	this->GetClientRect(&rc);
//	size_t size = m_MaskPanel_Vec.size();
//
//	CClientDC dc(NULL);
//	OnPrepareDC(&dc);
//	CSize sizeDoc;
//	sizeDoc.cx = rc.right - rc.left;
//	sizeDoc.cy = MASK_PANEL_H * size;
//	dc.LPtoDP(&sizeDoc);
//	SetScrollSizes(MM_TEXT, sizeDoc);
//}
//
//////////////////////////////////////////////////////
///*!
//*/
//VOID CMaskFrame::ReSetAllPanel(ImgFile_Ptr pFile)
//{
//	//���C���[�p�l���S�폜
//	ClearMaskPanel();
//
//	if(pFile){
//		RECT rc;
//		this->GetClientRect(&rc);
//
//		ImgMask *select_mask = pFile->GetSelectMask();
//		int lSize = pFile->GetMaskSize();
//		for(int i=0; i<lSize; i++){
//			//�}�X�N�I�u�W�F�N�g���擾
//			ImgMask *mask = pFile->GetMaskAtIndex(i);
//			//���C���[�p�l���̍쐬
//			RECT tmp_rc;
//			tmp_rc.top = rc.bottom - MASK_PANEL_H * (i+1);
//			tmp_rc.left = 0;
//			tmp_rc.bottom = tmp_rc.top + MASK_PANEL_H;
//			tmp_rc.right = tmp_rc.left + rc.right - rc.left;
//			CMaskPanel *mp = new CMaskPanel();
//			mp->CreateEx(0,0,"",
//				WS_CHILD | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE,
//				tmp_rc,
//				this,
//				i);
//			mp->SetMask(mask);
//
//			m_MaskPanel_Vec.push_back(mp);
//			
//			if(mask == select_mask){//�I�����C���[�Ȃ�p�l����I����Ԃ�
//				mp->SelectPanel();
//				m_SelectMaskPanel = mp;
//			}
//		}
//
//		InvalidateRect(NULL, FALSE);
//	}
//}
//
//////////////////////////////////////////////////////
///*!
//	index�̈ʒu��pMask�̃}�X�N�p�l����ǉ�����B
//	@param[in] pMask
//	@param[in] index
//*/
//VOID CMaskFrame::AddMaskPanel(ImgMask *pMask, int index)
//{
//	assert(0 <= index && index <= m_MaskPanel_Vec.size() );
//
//	CMaskPanel *mp = new CMaskPanel();
//	mp->CreateEx(0, 0, "",
//		WS_CHILD | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE,
//		CRect(0,0,0,0),
//		this,
//		index);
//	mp->SetMask( pMask );
//
//	MaskPanel_Vec::iterator itr;
//	if(index < m_MaskPanel_Vec.size()/2){
//		itr = m_MaskPanel_Vec.begin();
//		for(int i=0; i<index; i++){
//			itr++;
//		}
//	}
//	else{
//		itr = m_MaskPanel_Vec.end();
//		for(int i=m_MaskPanel_Vec.size(); i > index; i--){
//			itr--;
//		}
//	}
//
//	m_MaskPanel_Vec.insert(itr, mp);
//}
//
//////////////////////////////////////////////////////
///*!
//	index�̈ʒu�̃}�X�N�p�l�����폜����
//	@param[in] index
//*/
//VOID CMaskFrame::DeleteMaskPanel(int index)
//{
//	assert(0 <= index && index <= m_MaskPanel_Vec.size() );
//
//	int size = m_MaskPanel_Vec.size();
//	MaskPanel_Vec::iterator itr;
//	if(index < m_MaskPanel_Vec.size()/2){
//		itr = m_MaskPanel_Vec.begin();
//		for(int i=0; i<index; i++){
//			itr++;
//		}
//	}
//	else{
//		itr = m_MaskPanel_Vec.end();
//		for(int i=m_MaskPanel_Vec.size(); i > index; i--){
//			itr--;
//		}
//	}
//
//	CMaskPanel* mp = (*itr);
//	m_MaskPanel_Vec.erase(itr);
//
//	m_SelectMaskPanel = (mp == m_SelectMaskPanel) ? NULL : m_SelectMaskPanel;
//	m_DragMaskPanel   = (mp == m_DragMaskPanel)   ? NULL : m_DragMaskPanel;
//	m_UnderMaskPanel  = (mp == m_UnderMaskPanel)  ? NULL : m_UnderMaskPanel;
//	m_PopupMaskPanel  = (mp == m_PopupMaskPanel)  ? NULL : m_PopupMaskPanel;
//
//	delete mp;
//}
//
//////////////////////////////////////////////////////
///*!
//*/
//VOID CMaskFrame::AdjastPanel()
//{
//	if(!m_pActiveFile) return;
//
//	RECT rc;
//	this->GetClientRect(&rc);
//	
//	//�X�N���[���ʒu���擾
//	CPoint cpt = GetScrollPosition();
//
//	int mSize = m_pActiveFile->GetMaskSize();
//	ImgMask *select_mask = m_pActiveFile->GetSelectMask();
//	
//	MaskPanel_Vec::iterator itr = m_MaskPanel_Vec.begin();
//
//	int i;
//	for(i=0; itr != m_MaskPanel_Vec.end(); itr++, i++){
//		(*itr)->SetWindowPos(
//			NULL,
//			0,
//			(rc.bottom-rc.top > mSize*MASK_PANEL_H) ? rc.bottom - MASK_PANEL_H * (i+1) : mSize*MASK_PANEL_H - MASK_PANEL_H*(i+1) - cpt.y,
//			rc.right - rc.left,
//			MASK_PANEL_H,
//			SWP_NOZORDER);
//
//		if((*itr)->GetMask() == select_mask){//�I���}�X�N�Ȃ�p�l����I����Ԃ�
//			(*itr)->SelectPanel();
//			m_SelectMaskPanel = (*itr);
//		}else{
//			(*itr)->NewtralPanel();
//		}
//	}
//
//	InvalidateRect(NULL, FALSE);
//}
//
/////////////////////////////////////////////////////
///*!
//	���ׂẴ��C���[�p�l�����ĕ`�悷��B
//*/
//void CMaskFrame::AllPanelRePaint()
//{
//	this->Invalidate(FALSE);
//}
//
//////////////////////////////////////////////////////
///*!
//	�}�E�X�̉��ɂ���p�l���̃|�C���^��Ԃ��B
//	@param[in] point �N���C�A���g�}�E�X���W
//	@return �}�X�N�p�l���̃|�C���^
//*/
//CMaskPanel* CMaskFrame::GetMaskPanelFromPos(CPoint point)
//{
//	HWND hwnd;
//	RECT rc;
//	POINT lt, rb;
//	MaskPanel_Vec::iterator itr = m_MaskPanel_Vec.begin();
//	for(; itr != m_MaskPanel_Vec.end(); itr++){
//		//�p�l���̈ʒu�����߂�
//		(*itr)->GetWindowRect(&rc);
//		lt.x = rc.left; lt.y = rc.top;
//		rb.x = rc.right; rb.y = rc.bottom;
//		this->ScreenToClient(&lt);
//		this->ScreenToClient(&rb);
//		//����
//		if((lt.x <= point.x && point.x <= rb.x) &&
//			(lt.y <= point.y && point.y <= rb.y))
//		{
//			return (*itr);
//		}
//	}
//	return NULL;
//}
//
//////////////////////////////////////////////////////
///*!
//	�}�X�N�p�l�����폜����B
//	@param[in,out] lp �폜����}�X�N�p�l��
//*/
//VOID CMaskFrame::DeleteMaskPanel(CMaskPanel *mp)
//{
//	if(mp){
//		mp->DeleteMask();
//		if(mp == m_SelectMaskPanel) m_SelectMaskPanel = NULL;
//
//		LPUPDATE_DATA data = m_pActiveFile->CreateUpdateData(UPDATE_MASK_IMAGE);
//		data->isAll = true;
//		m_pActiveFile->PushUpdateData( data );
//	}
//}
//
//////////////////////////////////////////////////////
///*!
//	�}�X�N�p�l�������ׂč폜�������[�J��
//*/
//VOID CMaskFrame::ClearMaskPanel()
//{
//	MaskPanel_Vec::iterator itr = m_MaskPanel_Vec.begin();
//	for(; itr != m_MaskPanel_Vec.end(); itr++){
//		delete (*itr);
//	}
//	m_MaskPanel_Vec.clear();
//}
