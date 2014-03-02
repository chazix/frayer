#include "stdafx.h"

#include "EffectTreeWnd.h"
#include "EffectMG.h"
#include "IE_EffectEditDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern ImgEdit g_ImgEdit;

CEffectTreeWnd::CEffectTreeWnd()
{
	m_isDragging = false;
}

CEffectTreeWnd::~CEffectTreeWnd()
{
	_ASSERTE(_CrtCheckMemory());
}

BEGIN_MESSAGE_MAP(CEffectTreeWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

int CEffectTreeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (0 != CDockablePane::OnCreate(lpCreateStruct))
		return -1;

	m_hInst = lpCreateStruct->hInstance;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	//�c���[�r���[�̍쐬
	m_hTree = CreateWindowEx(0, WC_TREEVIEW, "",
		WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_HASLINES |
		TVS_HASBUTTONS | TVS_SHOWSELALWAYS | TVS_TRACKSELECT | TVS_LINESATROOT,
		0,
		0,
		0,
		0,
		m_hWnd, (HMENU) 101, lpCreateStruct->hInstance, NULL);

	//�h���b�O�C���[�W���X�g�̍쐬
	//m_hImLt = ImageList_Create(16, 16, ILC_COLOR4 | ILC_MASK, 0, 0);
	//if (m_hImLt == NULL)
	//	OutputError::Alert("�C���[�W���X�g�쐬���s");

	::DragAcceptFiles(m_hTree,TRUE);

	//�G�t�F�N�g�c�[���[�A�C�e���̒ǉ�
	g_ImgEdit.SetEffectTree( m_hTree );

	return 0;
}

void CEffectTreeWnd::OnSize(UINT nType, int cx, int cy)
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	//m_effectTree.SetWindowPos(NULL, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
	::SetWindowPos(m_hTree, NULL, rectClient.left + 1, rectClient.top + 1, rectClient.Width() - 2, rectClient.Height() - 2, SWP_NOACTIVATE | SWP_NOZORDER);
	CDockablePane::OnSize(nType, cx, cy);
}

void CEffectTreeWnd::OnPaint()
{
	CDockablePane::OnPaint();

	CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

	RECT rect;
	::GetWindowRect(m_hTree, &rect);
	CRect rectTree(&rect);
	ScreenToClient(rectTree);

	rectTree.InflateRect(1, 1);
	dc.FillSolidRect(rectTree, ::GetSysColor(COLOR_WINDOW));
}

BOOL CEffectTreeWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR){
		if(pNMHDR->code == TVN_BEGINDRAG){
			//OnBegindrag((LPNMTREEVIEW)lParam);
			return TRUE;
		}
		else if(pNMHDR->code == NM_DBLCLK){
			HTREEITEM hItem = TreeView_GetSelection(m_hTree);
			char pszBuf[256];
			TVITEM ti;
			ti.mask = TVIF_TEXT;
			ti.hItem = hItem;
			ti.pszText = pszBuf;
			ti.cchTextMax = 256;

			if(!TreeView_GetItem(m_hTree, &ti)){
				OutputError::PushLog( LOG_LEVEL::_ERROR, "CEffectTreeWnd::OnNotify() �c���[�̕����񂪒������܂��B");
				return TRUE;
			}
			//�ǉ�����G�t�F�N�g�𐶐�
			IEffect *ef = g_ImgEdit.CreateEffect(pszBuf);
			if(!ef){
				OutputError::PushLog( LOG_LEVEL::_WARN, "EffectMG::CreateEffect() return NULL");
				return TRUE;
			}
			
			ImgFile_Ptr pFile = g_ImgEdit.GetActiveFile();
			if(!pFile) return TRUE;

			//�}�X�N�̕`����@��_���\���ɕύX
			int old_mask_draw_id = pFile->GetMaskDrawType();
			pFile->SetMaskDrawType(MASK_DOT_LINE_DRAW);

			////�G�t�F�N�g��������
			//g_ImgEdit.AddEffect( pFile, ef );
			//�G�t�F�N�g�_�C�A���O��\��������
			IE_EffectEditDialog *pDialog = new IE_EffectEditDialog();

			if(ef->Init( pFile )){
				int ret = pDialog->CreateDialogBox(m_hInst,
					this->GetParent()->GetSafeHwnd(), pFile, ef);
				ef->End( pFile );
				g_ImgEdit.ReleaseEffect(ef);
				ef = NULL;
			
				if(ret == IDOK){
				}
				else if(ret == IDCANCEL){
					//�G�t�F�N�g�̏���
					pFile->Undo(1);
				}
			}
			delete pDialog;
			pDialog = NULL;

			//�}�X�N�̕\�������ɖ߂�
			pFile->SetMaskDrawType(old_mask_draw_id);

			return TRUE;
		}
	}

	return CDockablePane::OnNotify(wParam, lParam, pResult);;
}

void CEffectTreeWnd::OnBegindrag(LPNMTREEVIEW lpnmtv)
{
	m_hDragItem = lpnmtv->itemNew.hItem;
	LPSTR pstr = lpnmtv->itemNew.pszText;
	//�h���b�O�C���[�W�쐬
	//m_hImLt = TreeView_CreateDragImage(m_hTree, lpnmtv->itemNew.hItem);
	////TreeView_GetItemRect(m_hTree, lpnmtv->itemNew.hItem, &rcItem, TRUE); 
	//::ImageList_BeginDrag(m_hImLt, 0, 0, 0);
	//�J�[�\�����L���v�`���[
	CWnd *wnd = this->SetCapture();
	//::SetCapture( this->GetSafeHwnd() );
	//�J�[�\��������
	//::ShowCursor(FALSE);

	POINT pt;
	pt.x = lpnmtv->ptDrag.x;
	pt.y = lpnmtv->ptDrag.y;	
	
	ClientToScreen(&(lpnmtv->ptDrag));
	//ImageList_DragEnter(NULL, lpnmtv->ptDrag.x, lpnmtv->ptDrag.y);

	m_isDragging = true;
	return;
}

void CEffectTreeWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDockablePane::OnLButtonDown(nFlags, point);
}

void CEffectTreeWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_isDragging){
		TVHITTESTINFO tvht;
		POINT pt;

		//�}�E�X�̃X�N���[�����W�擾
		GetCursorPos(&pt);

		ZeroMemory(&tvht, sizeof(TVHITTESTINFO));
		tvht.flags = TVHT_ONITEM;
		tvht.pt.x = pt.x;
		tvht.pt.y = pt.y;
		ScreenToClient(&(tvht.pt));
		
		ImageList_DragMove(pt.x, pt.y);
	}

	CDockablePane::OnMouseMove(nFlags, point);
}

void CEffectTreeWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_isDragging){
		//ImageList_EndDrag();
		//ImageList_DragLeave(m_hTree);
		//::ShowCursor(TRUE);
		::ReleaseCapture();
		m_isDragging = FALSE;

		//�}�E�X�̍��W���牺�ɂ���E�B���h�E�̃n���h���𓾂�
		POINT pt;
		GetCursorPos(&pt);
		//HWND hSendWnd = ::WindowFromPoint(pt);
		CWnd* pwnd = WindowFromPoint(pt);
		//�h���b�v����E�B���h�E���擾���Ă���΃h���b�O���Ă���A�C�e���̃e�L�X�g�𑗂�B
		//if(hSendWnd){
		if(pwnd){
			char pszBuf[256];
			TVITEM ti;
			ti.mask = TVIF_TEXT;
			ti.hItem = m_hDragItem;
			ti.pszText = pszBuf;
			ti.cchTextMax = 256;

			if(TreeView_GetItem(m_hTree, &ti)){
				//SendMessage(hSendWnd, WM_USER, UM_DROPITEM, (LPARAM)pszBuf);
				pwnd->SendMessage(WM_USER, UM_DROPITEM, (LPARAM)pszBuf);
			}
			else{
				OutputError::PushLog( LOG_LEVEL::_ERROR, "EffectTreeWindow::OnEndDrag() �c���[�̕����񂪒������܂��B");
			}
		}
	}
	
	CDockablePane::OnLButtonUp(nFlags, point);
}