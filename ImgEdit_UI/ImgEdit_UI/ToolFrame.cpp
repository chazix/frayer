#include "stdafx.h"
#include "ToolFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern ImgEdit g_ImgEdit;

CToolFrame::CToolFrame()
{
}

CToolFrame::~CToolFrame()
{
	CToolPopupPanel_Vec::iterator itr = m_ToolPopupPanel_Vec.begin();
	for(; itr != m_ToolPopupPanel_Vec.end(); itr++){
		//(*itr)->DestroyWindow();
		delete (*itr);
	}
	m_ToolPopupPanel_Vec.clear();
}

void CToolFrame::OnChangeSelectTool(IIETool* pNewTool, IIETool* pOldTool)
{
	//newtral
	CToolPopupPanel_Vec::iterator itr = m_ToolPopupPanel_Vec.begin();
	for(; itr != m_ToolPopupPanel_Vec.end(); itr++){
		(*itr)->AllButtonNewtral();
	}

	//set select button
	SelectToolButton( pNewTool );
}

BEGIN_MESSAGE_MAP(CToolFrame, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	//ON_WM_LBUTTONDOWN()
	//ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONUP()
	//ON_WM_TIMER()
	ON_NOTIFY(TTN_GETDISPINFO, 0, OnTipsDispInfo)
END_MESSAGE_MAP()

int CToolFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(0 != CWnd::OnCreate(lpCreateStruct)){
		return -1;
	}

	int group_size = g_ImgEdit.GetSizeOfToolGroup();
	if(group_size < 0){ return -1; }

	RECT rc;
	rc.left = rc.top = 0;
	rc.right = rc.bottom = 0;
	for(int i=0; i<group_size; i++){
		IEToolGroup* pTGroup = g_ImgEdit.GetToolGroupAtIndex( i );

		CToolPopupPanel* pPanel = new CToolPopupPanel();
		pPanel->CreateEx(
			0, 0, "",
			WS_CHILD | WS_CLIPCHILDREN | WS_BORDER | WS_VISIBLE,
			rc,
			this,
			i);

		pPanel->SetIEToolGroup( pTGroup );

		m_ToolPopupPanel_Vec.push_back( pPanel );
	}

	//init select tool
	OnChangeSelectTool(g_ImgEdit.GetSelectTool(), NULL);

	//
	g_ImgEdit.AddIEToolMGEventListener( this );
	return 0;
}

void CToolFrame::OnDestroy()
{
	g_ImgEdit.DeleteIEToolMGEventListener( this );
	CWnd::OnDestroy();
}

void CToolFrame::OnSize(UINT nType, int cx, int cy)
{
	int y = 0;
	int height;
	CToolPopupPanel_Vec::iterator itr = m_ToolPopupPanel_Vec.begin();
	for(; itr != m_ToolPopupPanel_Vec.end(); itr++){
		height = (*itr)->GetHeight( cx );
		(*itr)->SetWindowPos(
			NULL,
			0, y,
			cx, height,
			SWP_NOZORDER);
		y += height;
		(*itr)->Invalidate(FALSE);
	}
	CWnd::OnSize(nType, cx, cy);
}

void CToolFrame::OnPaint()
{
	TRY
	{
		CPaintDC dc(this);

		CRect rectWnd;
		GetWindowRect(rectWnd);
		ScreenToClient(rectWnd);

		rectWnd.InflateRect(1, 1);
		dc.FillSolidRect(rectWnd, ::GetSysColor(COLOR_WINDOW));
		
		CWnd::OnPaint();
	}
	CATCH(CException, e)
	{
		::OutputDebugString("CToolFrame::OnPaint() exception\n");
	}
	END_CATCH
}

void CToolFrame::OnTipsDispInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTTDISPINFO lpttd;
	CToolPopupPanel_Vec::iterator itr = m_ToolPopupPanel_Vec.begin();
	for(; itr != m_ToolPopupPanel_Vec.end(); itr++){
		if((*itr)->m_tool_tip_txt != ""){
			lpttd = (LPNMTTDISPINFO)pNMHDR;
			lpttd->lpszText = (LPSTR)(LPCSTR)(*itr)->m_tool_tip_txt;
		}
	}

	*pResult = 0;
}

//void CToolFrame::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	TRY
//	{
//		CToolPopupPanel* p = GetToolPanelFromPos(point);
//		if(p){
//			ToolButton* b = p->GetTopButton();
//			if(b){
//				//�����ꂽ�{�^����ID��ImgEdit�ɑ���
//				int id = b->GetButtonID();
//				SetIEToolID(id);
//
//				//�I���p�l���ƃ{�^�����i�[
//				m_SelectToolPopupPanel = p;
//				m_SelectToolButton = b;
//			
//				m_isMouseLDown = true;
//				m_MouseLDownTime = ::timeGetTime();
//
//				return;
//			}
//		}
//
//		CWnd::OnLButtonDown(nFlags, point);
//	}
//	CATCH(CException, e)
//	{
//		::OutputDebugString("CToolFrame::OnLButtonDown() exception\n");
//	}
//	END_CATCH
//}

//void CToolFrame::OnMouseMove(UINT nFlags, CPoint point)
//{
//	TRY
//	{
//		if(m_isPanelPopup){//�|�b�v�A�b�v���o�Ă���Ȃ�}�E�X�̏����|�b�v�A�b�v�ɑ���
//			POINT pt;
//			pt.x = point.x;
//			pt.y = point.y;
//			this->ClientToScreen(&pt);
//			m_SelectToolPopupPanel->OnMouseMove(&pt);
//			return;
//		}	
//		else{
//			//�J�[�\���̉��̃p�l�������o���B
//			CToolPopupPanel* under_panel = GetToolPanelFromPos(point);
//			if(under_panel){
//				//�J�[�\���̉��̃{�^�����}�E�X�I����Ԃɂ���B
//				ToolButton* b = under_panel->GetTopButton();
//				if(b != m_SelectToolButton){
//					b->OnMouseOn();
//					//�ĕ`��
//					RECT rc;
//					b->GetButtonPos(&rc);
//					this->InvalidateRect(&rc, FALSE);
//				}
//			}
//
//			if(m_isMouseLDown){
//				//�}�E�X�_�E�����̃p�l���łȂ���΁A�|�b�v�A�b�v�̎��Ԍv�Z�͖����ɂ���
//				if(m_SelectToolPopupPanel != under_panel){
//					m_SelectToolPopupPanel = NULL;
//					m_MouseLDownTime = 0;
//				}
//			}
//		}
//
//		CWnd::OnMouseMove(nFlags, point);
//	}
//	CATCH(CException, e)
//	{
//		::OutputDebugString("CToolFrame::OnMouseMove() exception\n");
//	}
//	END_CATCH
//}

//void CToolFrame::OnLButtonUp(UINT nFlags, CPoint point)
//{
//	TRY
//	{
//		if(m_isPanelPopup){
//			//�|�b�v�A�b�v�̃{�^���̑I���𔻒肷��
//			bool bl = false;
//		
//			POINT pt;
//			pt.x = point.x;
//			pt.y = point.y;
//			this->ClientToScreen(&pt);
//
//			if(m_SelectToolPopupPanel){
//				bl = m_SelectToolPopupPanel->OnMouseLButtonUp(&pt);
//			}
//
//			if(bl){
//				ToolButton*b = m_SelectToolPopupPanel->GetTopButton();
//
//				//�����ꂽ�{�^����ID��ImgEdit�ɑ���
//				int id = b->GetButtonID();
//				SetIEToolID(id);
//				//�I���{�^�����i�[
//				m_SelectToolButton = b;
//			}
//
//		}
//
//		m_isMouseLDown = false;
//		if(m_SelectToolPopupPanel){
//			m_SelectToolPopupPanel->HidePopup();
//			//�}�E�X�L���v�`���[�����[�X
//			ReleaseCapture();
//		}
//		m_isPanelPopup = false;
//
//		CWnd::OnLButtonUp(nFlags, point);
//	}
//	CATCH(CException, e)
//	{
//		::OutputDebugString("CToolFrame::OnLButtonUp() exception\n");
//	}
//	END_CATCH
//}

//void CToolFrame::OnTimer(UINT_PTR nIDEvent)
//{
//	TRY
//	{
//		POINT pt;
//		::GetCursorPos(&pt);
//		this->ScreenToClient(&pt);
//		CToolPopupPanel* under_panel = GetToolPanelFromPos(CPoint(pt));
//		ToolButton* under_btn = (under_panel != NULL) ? under_panel->GetTopButton() : NULL;
//
//		//�}�E�X�I����Ԃ����ɖ߂��B
//		ToolPopupPanel_Vec::iterator itr = m_ToolPopupPanel_Vec.begin();
//		for(; itr != m_ToolPopupPanel_Vec.end(); itr++){
//			ToolButton* tb = (*itr)->GetTopButton();
//			if(under_btn != tb){
//				if(tb && tb->GetButtonState() == BUTTONSTATE::MOUSEON){
//					tb->OnNewtral();
//					//�ĕ`��
//					RECT rc;
//					tb->GetButtonPos(&rc);
//					this->InvalidateRect(&rc, FALSE);
//				}
//			}
//		}
//
//		if(m_isMouseLDown){
//			//�}�E�X��������Ă��鎞�Ԃ�����׈��ȏ�Ȃ�|�b�v�A�b�v��\��
//			int nowTime = timeGetTime();
//			if(nowTime - m_MouseLDownTime >= POPUP_TIME){
//				if(m_isPanelPopup == false && m_SelectToolPopupPanel != NULL){
//					POINT pt;
//					::GetCursorPos(&pt);
//					m_SelectToolPopupPanel->AllButtonNewtral();
//					m_SelectToolPopupPanel->ShowPopup(&pt);
//					m_isPanelPopup = true;
//
//					//�}�E�X�L���v�`���[set
//					SetCapture();
//
//				}
//			}
//		}
//	}
//	CATCH(CException, e)
//	{
//		::OutputDebugString("CToolFrame::OnTimer() exception\n");
//	}
//	END_CATCH
//}

////////////////////////////
/*!
	�ʒu����{�^���O���[�v�ւ̃|�C���^�𓾂�
*/
CToolPopupPanel* CToolFrame::GetToolPanelFromPos(CPoint point)
{
	if(m_ToolPopupPanel_Vec.empty()) return NULL;

	int n = point.y / TOOL_BUTTON_H;
	if(0 <= n && n <= m_ToolPopupPanel_Vec.size() - 1){
		return m_ToolPopupPanel_Vec[n];
	}

	return NULL;
}

bool CToolFrame::SelectToolButton(const IIETool* pTool)
{
	if(m_ToolPopupPanel_Vec.empty()) return false;

	int i,j;
	int id;
	CToolButton* btn;
	CToolPopupPanel_Vec::iterator itr = m_ToolPopupPanel_Vec.begin();
	for(; itr != m_ToolPopupPanel_Vec.end(); itr++){
		(*itr)->SelectToolButton( pTool );
	}

	return true;
}