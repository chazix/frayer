
// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "ImgEdit_UI.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//test nann dayo
#define OPPAI_IPPAI 1
#define OPPAI_BARRAY 3
//
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	//ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_VS_2005, &CMainFrame::OnApplicationLook)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_VS_2005, &CMainFrame::OnUpdateApplicationLook)
	ON_MESSAGE(WM_APP_MOUSEWHEEL, CMainFrame::OnAppMouseWheel)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame �R���X�g���N�V����/�f�X�g���N�V����

CMainFrame::CMainFrame()
{
	// TODO: �����o�������R�[�h�������ɒǉ����Ă��������B
	//theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);

	//
	m_bAutoMenuEnable = FALSE;
}

CMainFrame::~CMainFrame()
{
	m_hMenuDefault = m_hOldMenu;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_SYSKEYDOWN || pMsg->message == WM_SYSKEYUP) {
		if (pMsg->wParam == VK_MENU) {
			return TRUE;
		}
	}
	if (pMsg->message == WM_MOUSEWHEEL) {
		return TRUE;
	}
	return CMDIFrameWndEx::PreTranslateMessage(pMsg);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//�E�B���h�E�^�C�g��
	char wtxt[256];
#ifdef WIN64
	sprintf(wtxt, "Frayer (x64) (build %s)", __DATE__);
#else
	sprintf(wtxt, "Frayer (build %s)", __DATE__);
#endif //WIN64
	this->SetWindowText(wtxt);

	BOOL bNameValid;
	// �Œ�l�Ɋ�Â��ăr�W���A�� �}�l�[�W���� visual �X�^�C����ݒ肵�܂�
	OnApplicationLook(theApp.m_nAppLook);

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // �g�p�\�Ȃ��̑��̎��o�X�^�C��...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // �^�u�̈�̉E���� [����] �{�^����z�u����ɂ́AFALSE �ɐݒ肵�܂�
	mdiTabParams.m_bTabIcons = FALSE;    // MDI �^�u�Ńh�L�������g �A�C�R����L���ɂ���ɂ́ATRUE �ɐݒ肵�܂�
	mdiTabParams.m_bAutoColor = FALSE;    // MDI �^�u�̎����F�ݒ�𖳌��ɂ���ɂ́AFALSE �ɐݒ肵�܂�
	mdiTabParams.m_bDocumentMenu = TRUE; // �^�u�̈�̉E�[�ɂ���h�L�������g ���j���[��L���ɂ��܂�
	EnableMDITabbedGroups(TRUE, mdiTabParams);


	if (!m_wndMenuBar.Create(this)){
		TRACE0("���j���[ �o�[���쐬�ł��܂���ł���\n");
		return -1;      // �쐬�ł��܂���ł����B
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// �A�N�e�B�u�ɂȂ����Ƃ����j���[ �o�[�Ƀt�H�[�J�X���ړ����Ȃ�
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("�c�[�� �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��܂���ł����B
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// ���[�U�[��`�̃c�[�� �o�[�̑���������܂�:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("�X�e�[�^�X �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;      // �쐬�ł��Ȃ��ꍇ
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �c�[�� �o�[����у��j���[ �o�[���h�b�L���O�\�ɂ��Ȃ��ꍇ�́A���� 5 �̍s���폜���܂�
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// Visual Studio 2005 �X�^�C���̃h�b�L���O �E�B���h�E�����L���ɂ��܂�
	CDockingManager::SetDockingMode(DT_SMART);
	// Visual Studio 2005 �X�^�C���̃h�b�L���O �E�B���h�E�̎�����\�������L���ɂ��܂�
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���j���[���ڃC���[�W (�ǂ̕W���c�[�� �o�[�ɂ��Ȃ��C���[�W) ��ǂݍ��݂܂�:
	//CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// �h�b�L���O �E�B���h�E���쐬���܂�
	if (!CreateDockingWindows())
	{
		TRACE0("�h�b�L���O �E�B���h�E���쐬�ł��܂���ł���\n");
		return -1;
	}

	//�c�[���E�B���h�E�ǉ�
	m_wndTool.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndTool);
	//�G�t�F�N�g�c���[�E�B���h�E�ǉ�
	m_wndEffectTree.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndEffectTree);
	//HSV�T�[�N���E�B���h�E�ǉ�
	m_wndHSVCircle.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndHSVCircle);
	//RGB�X���C�_�[�E�B���h�E�ǉ�
	m_wndRGBSlider.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndRGBSlider);
	//�u���V�E�B���h�E�ǉ�
	m_wndBrushSelect.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndBrushSelect);
	//�i�r�Q�[�^�[�E�B���h�E�ǉ�
	m_wndNavigator.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndNavigator);
	//���C���[�E�B���h�E�ǉ�
	m_wndLayer.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndLayer);
	//�q�X�g���E�B���h�E�ǉ�
	m_wndHistory.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndHistory);
	//�c�[���G�f�B�b�g�E�B���h�E�ǉ�
	m_wndToolEdit.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndToolEdit);
	//�p���b�g�E�B���h�E�ǉ�
	m_wndPalette.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndPalette);
	////�}�X�N�E�B���h�E�ǉ�
	//m_wndMask.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndMask);

	// �g���E�B���h�E�Ǘ��_�C�A���O �{�b�N�X��L���ɂ��܂�
	EnableWindowsDialog(ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// �c�[�� �o�[�ƃh�b�L���O �E�B���h�E ���j���[�̔z�u�ύX��L���ɂ��܂�
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// �c�[�� �o�[�̃N�C�b�N (Alt �L�[�������Ȃ���h���b�O) �J�X�^�}�C�Y��L���ɂ��܂�
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// ���[�U�[��`�̃c�[�� �o�[ �C���[�W��ǂݍ��݂܂�
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// ���j���[�̃p�[�\�i���� (�ŋߎg�p���ꂽ�R�}���h) ��L���ɂ��܂�
	// TODO: ���[�U�[�ŗL�̊�{�R�}���h���`���A�e���j���[���N���b�N�����Ƃ��Ɋ�{�R�}���h�� 1 �ȏ�\�������悤�ɂ��܂��B
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_SAVE_AS);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	//lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	//lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	lstBasicCommands.AddTail( IDIE_BRUSHLOAD );
	lstBasicCommands.AddTail( IDIE_TOOLCONFIG );
	lstBasicCommands.AddTail( IDIE_OPEN_FILE_FROM_SCANER );

	for (int id = g_ImgEdit.GetMinCommandID(); id <= g_ImgEdit.GetMaxCommandID(); id++) {
		lstBasicCommands.AddTail( id );
	}

	CMFCToolBar::SetBasicCommands( lstBasicCommands );

	//�X�^�C���w��
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: ���̈ʒu�� CREATESTRUCT cs ���C������ Window �N���X�܂��̓X�^�C����
	//  �C�����Ă��������B

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	//�c�[���E�B���h�E�쐬
	if (!m_wndTool.Create(
		"�c�[��",
		this,
		CRect(0, 0, 42, 300),
		TRUE,
		ID_TOOL_WND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�c�[���E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE;
	}
	//�q�X�g���E�B���h�E�쐬
	if (!m_wndHistory.Create(
		"�q�X�g��",
		this,
		CRect(500, 500, 800, 800),
		TRUE,
		ID_HISTORY_WND,
		WS_CHILD | /*WS_VISIBLE |*/ WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�q�X�g���E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE;
	}
	//�u���V�E�B���h�E�쐬
	if (!m_wndBrushSelect.Create(
		"�u���V",
		this,
		CRect(300, 300, 600, 600),
		TRUE,
		ID_BRUSH_SELECT,
		WS_CHILD | /*WS_VISIBLE |*/ WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�u���V�E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE;
	}
	//�G�t�F�N�g�c���[�E�B���h�E�쐬
	if (!m_wndEffectTree.Create(
		"�G�t�F�N�g",
		this,
		CRect(0, 0, 300, 300),
		TRUE,
		ID_EFFECT_TREE,
		WS_CHILD | /*WS_VISIBLE |*/ WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�G�t�F�N�g�c���[�E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE;
	}
	//HSV�T�[�N���E�B���h�E�쐬
	if (!m_wndHSVCircle.Create(
		"HSV�T�[�N��",
		this,
		CRect(100, 100, 400, 400),
		TRUE,
		ID_HSV_CIRCLE_WND,
		WS_CHILD | /*WS_VISIBLE |*/ WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("HSV�T�[�N���E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE;
	}
	//RGB�X���C�_�[�E�B���h�E�쐬
	if (!m_wndRGBSlider.Create(
		"RGB�X���C�_�[",
		this,
		CRect(0, 0, 300, 300),
		TRUE,
		ID_RGB_SLIDER_WND,
		WS_CHILD | /*WS_VISIBLE |*/ WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("RGB�X���C�_�[�E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE;
	}
	//���C���[�E�B���h�E�쐬
	if (!m_wndLayer.Create(
		"���C���[",
		this,
		CRect(200, 200, 500, 500),
		TRUE,
		ID_LAYER_WND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("���C���[�E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE;
	}

	//�i�r�Q�[�^�[�E�B���h�E�쐬
	if (!m_wndNavigator.Create(
		"�i�r�Q�[�^�[",
		this,
		CRect(400, 400, 700, 700),
		TRUE, ID_NAVIGATOR_WND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�i�r�Q�[�^�[�E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE;
	}

	//�c�[���G�f�B�b�g�E�B���h�E�쐬
	if (!m_wndToolEdit.Create(
		"�c�[���G�f�B�b�g",
		this,
		CRect(600, 600, 900, 670),
		TRUE,
		ID_TOOL_EDIT_WND,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_TOP | CBRS_FLOAT_MULTI))
	{
		TRACE0("�c�[���G�f�B�b�g�E�B���h�E���쐬�ł��܂���ł���\n");
		return FALSE;
	}

	//�p���b�g�E�B���h�E�쐬
	if (!m_wndPalette.Create(
		"�p���b�g",
		this,
		CRect(600, 200, 800, 400),
		TRUE,
		ID_PALETTE_WND,
		WS_CHILD | /*WS_VISIBLE |*/ WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("�p���b�g�E�B���h�E���쐬�o���܂���ł���\n");
		return FALSE;
	}
	////�}�X�N�E�B���h�E�쐬
	//if (!m_wndMask.Create("�}�X�N", this, CRect(700, 600, 900, 900), TRUE, ID_MASK_WND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("�}�X�N�E�B���h�E���쐬�ł��܂���ł���\n");
	//	return FALSE;
	//}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	UpdateMDITabbedBarsIcons();
}

// CMainFrame �f�f

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ���b�Z�[�W �n���h��

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ���j���[���X�L�������܂�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	//case ID_VIEW_APPLOOK_WIN_2000:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
	//	break;

	//case ID_VIEW_APPLOOK_OFF_XP:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
	//	break;

	//case ID_VIEW_APPLOOK_WIN_XP:
	//	CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	//	break;

	//case ID_VIEW_APPLOOK_OFF_2003:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
	//	CDockingManager::SetDockingMode(DT_SMART);
	//	break;

	//case ID_VIEW_APPLOOK_VS_2005:
	//	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
	//	CDockingManager::SetDockingMode(DT_SMART);
	//	break;

	default:
		//switch (theApp.m_nAppLook)
		//{
		//case ID_VIEW_APPLOOK_OFF_2007_BLUE:
		//	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
		//	break;

		//case ID_VIEW_APPLOOK_OFF_2007_BLACK:
		//	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
		//	break;

		//case ID_VIEW_APPLOOK_OFF_2007_SILVER:
		//	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
		//	break;

		//case ID_VIEW_APPLOOK_OFF_2007_AQUA:
		//	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
		//	break;
		//}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(g_ImgEdit.RunCommandAsync(LOWORD(wParam), NULL))
		return TRUE;

	switch (LOWORD(wParam)) {
		case IDIE_BRUSHLOAD:
			{
				TCHAR strFile[MAX_PATH] = "";
				OPENFILENAME ofn = {0};

				ofn.lStructSize = sizeof(OPENFILENAME);
				ofn.hwndOwner = this->GetSafeHwnd();
				ofn.lpstrFilter = "Adobe PhotoShop Brush (.abr)\0*.abr\0";
				ofn.lpstrFile = strFile;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_FILEMUSTEXIST;

				//�_�C�A���O���J��
				::GetOpenFileName(&ofn);
				if(strcmp(strFile, "") != 0){
					g_ImgEdit.LoadBrush(strFile);
				}
			}
			return TRUE;
		case IDIE_TOOLCONFIG:
			{
				HWND hWnd = this->GetSafeHwnd();
				//HINSTANCE hInst = (HINSTANCE)::GetWindowLong(hWnd, GWL_HINSTANCE);
				HINSTANCE hInst = ::AfxGetInstanceHandle();
				IEConfigDialog dlg;
				dlg.ShowDialog(hInst, hWnd, &g_ImgEdit);
			}
			return TRUE;
		case IDIE_OPEN_FILE_FROM_SCANER:
			{
				CImgEdit_UIApp* pApp = (CImgEdit_UIApp*)AfxGetApp();
				pApp->OnFileOpenFromScaner();
			}
			return TRUE;
	}
	return CMDIFrameWndEx::OnCommand(wParam, lParam);
}

LRESULT CMainFrame::OnAppMouseWheel(WPARAM wParam, LPARAM lParam)
{
	//UINT nFlags = GET_KEYSTATE_WPARAM(wParam);
	//short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
	//CPoint pt;
	//pt.x = GET_X_LPARAM(lParam); 
	//pt.y = GET_Y_LPARAM(lParam);
	////m_PaletteFrame.DoMouseWheel(nFlags, zDelta, pt);
	//
	CWnd* pView = this->GetActiveView();
	pView->SendMessage(WM_APP_MOUSEWHEEL, wParam, lParam);
	return TRUE;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ��{�N���X�����ۂ̓�����s���܂��B
	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	//���j���[�̕ύX
	this->InitIEMenu();
	m_wndMenuBar.CreateFromMenu(m_hIEMenu);
	m_hOldMenu = m_hMenuDefault;
	m_hMenuDefault = m_hIEMenu;


	// ���ׂẴ��[�U�[��`�c�[�� �o�[�̃{�^���̃J�X�^�}�C�Y��L���ɂ��܂�
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::InitIEMenu()
{
	m_hIEMenu = IEMenuMG::CreateIEMenu();

	m_hMenuFile      = IEMenuMG::CreateIEMenu();
	m_hMenuEdit      = IEMenuMG::CreateIEMenu();
	m_hMenuImage     = IEMenuMG::CreateIEMenu();
	m_hMenuDisplay   = IEMenuMG::CreateIEMenu();
	m_hMenuWindow    = IEMenuMG::CreateIEMenu();
	m_hMenuImgSelect = IEMenuMG::CreateIEMenu();
	m_hMenuTool      = IEMenuMG::CreateIEMenu();
	m_hMenuHelp      = IEMenuMG::CreateIEMenu();

	IEMenuItemInfo ieii;
	
	//�t�@�C��
	ieii.SetMask(MIIM_STRING | MIIM_ID | MIIM_STATE);
	ieii.SetState(MFS_ENABLED);

	ieii.SetTypeData(TEXT("�V�K�쐬"));
	ieii.SetID( ID_FILE_NEW );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 0, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�t�@�C�����J��"));
	ieii.SetID( ID_FILE_OPEN );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 1, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�㏑���ۑ�"));
	ieii.SetID( ID_FILE_SAVE );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 2, TRUE, &ieii);

	ieii.SetTypeData(TEXT("���O�����ĕۑ�"));
	ieii.SetID( ID_FILE_SAVE_AS );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 3, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�X�L���i����ǂݍ���"));
	ieii.SetID( IDIE_OPEN_FILE_FROM_SCANER );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 4, TRUE, &ieii);

#ifdef _DEBUG
	ieii.SetTypeData(TEXT("�C�x���g�L���v�`�����J�n����"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("StartEventCapture")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 6, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�C�x���g�L���v�`�����I������"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("EndEventCapture")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 7, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�C�x���g�L���v�`�����Đ�����"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("PlayEventCapture")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 8, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�I��"));
	ieii.SetID( ID_APP_EXIT );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 10, TRUE, &ieii);

	ieii.SetMask(MIIM_FTYPE);
	ieii.SetType(MFT_SEPARATOR);
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 5, TRUE, &ieii);
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 9, TRUE, &ieii);
#else
	ieii.SetTypeData(TEXT("�I��"));
	ieii.SetID( ID_APP_EXIT );
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 6, TRUE, &ieii);

	ieii.SetMask(MIIM_FTYPE);
	ieii.SetType(MFT_SEPARATOR);
	IEMenuMG::InsertIEMenuItem(m_hMenuFile, 5, TRUE, &ieii);
#endif //_DEBUG

	//�ҏW
	ieii.SetMask(MIIM_STRING | MIIM_ID | MIIM_STATE);
	ieii.SetState(MFS_ENABLED);

	ieii.SetTypeData(TEXT("��蒼��"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("Redo")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuEdit, 0, TRUE, &ieii);

	ieii.SetTypeData(TEXT("���ɖ߂�"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("Undo")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuEdit, 1, TRUE, &ieii);

	//�摜
	ieii.SetMask(MIIM_STRING | MIIM_ID | MIIM_STATE);
	ieii.SetState(MFS_ENABLED);

	ieii.SetTypeData(TEXT("�g�[���J�[�u"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("ToneCurve")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuImage, 0, TRUE, &ieii);

	ieii.SetTypeData(TEXT("���x���␳"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("LevelCorrection")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuImage, 1, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�J���[�o�����X"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("ColorBalance")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuImage, 2, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�L�����o�X�T�C�Y�ύX"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("ChangeCanvasSize")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuImage, 4, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�摜�𑜓x�ύX"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("ChangeResolution")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuImage, 5, TRUE, &ieii);

	ieii.SetMask(MIIM_FTYPE);
	ieii.SetType(MFT_SEPARATOR);
	IEMenuMG::InsertIEMenuItem(m_hMenuImage, 3, TRUE, &ieii);

	//�摜�I��
	ieii.SetMask(MIIM_STRING | MIIM_ID | MIIM_STATE);
	ieii.SetState(MFS_ENABLED);

	ieii.SetTypeData(TEXT("���ׂĂ�I��"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("ImgSelectAll")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuImgSelect, 0, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�I��������"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("ImgSelectRelease")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuImgSelect, 1, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�I��͈͂𔽓]"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("ImgSelectFlip")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuImgSelect, 2, TRUE, &ieii);

	//�\��
	ieii.SetMask(MIIM_STRING | MIIM_ID | MIIM_STATE);
	ieii.SetState(MFS_ENABLED);

	ieii.SetTypeData(TEXT("���E���]"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("ViewFlipH")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuDisplay, 0, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�㉺���]"));
	ieii.SetID( g_ImgEdit.GetCommandID(TEXT("ViewFlipV")) );
	IEMenuMG::InsertIEMenuItem(m_hMenuDisplay, 1, TRUE, &ieii);

	//�E�B���h�E
	ieii.SetMask(MIIM_STRING | MIIM_ID | MIIM_STATE);
	ieii.SetState(MFS_ENABLED);

	ieii.SetTypeData(TEXT("�E�B���h�E"));
	ieii.SetID( ID_VIEW_TOOLBAR );
	IEMenuMG::InsertIEMenuItem(m_hMenuWindow, 0, TRUE, &ieii);

	//�c�[��
	ieii.SetMask(MIIM_STRING | MIIM_ID | MIIM_STATE);
	ieii.SetState(MFS_ENABLED);

	ieii.SetTypeData(TEXT("�u���V�̓ǂݍ���"));
	ieii.SetID( IDIE_BRUSHLOAD );
	IEMenuMG::InsertIEMenuItem(m_hMenuTool, 0, TRUE, &ieii);

	ieii.SetTypeData(TEXT("�ݒ�"));
	ieii.SetID( IDIE_TOOLCONFIG );
	IEMenuMG::InsertIEMenuItem(m_hMenuTool, 1, TRUE, &ieii);

	//�e���j���[�𓝍�
	ieii.SetMask(MIIM_TYPE | MIIM_SUBMENU);
	ieii.SetType(MFT_STRING);

	ieii.SetSubMenu(m_hMenuFile);
	ieii.SetTypeData(TEXT("�t�@�C��"));
	IEMenuMG::InsertIEMenuItem(m_hIEMenu, 0, TRUE, &ieii);

	ieii.SetSubMenu(m_hMenuEdit);
	ieii.SetTypeData(TEXT("�ҏW"));
	IEMenuMG::InsertIEMenuItem(m_hIEMenu, 1, TRUE, &ieii);

	ieii.SetSubMenu(m_hMenuImage);
	ieii.SetTypeData(TEXT("�摜"));
	IEMenuMG::InsertIEMenuItem(m_hIEMenu, 2, TRUE, &ieii);

	ieii.SetSubMenu(m_hMenuImgSelect);
	ieii.SetTypeData(TEXT("�͈͑I��"));
	IEMenuMG::InsertIEMenuItem(m_hIEMenu, 3, TRUE, &ieii);

	ieii.SetSubMenu(m_hMenuDisplay);
	ieii.SetTypeData(TEXT("�\��"));
	IEMenuMG::InsertIEMenuItem(m_hIEMenu, 4, TRUE, &ieii);

	ieii.SetSubMenu(m_hMenuWindow);
	ieii.SetTypeData(TEXT("�E�B���h�E"));
	IEMenuMG::InsertIEMenuItem(m_hIEMenu, 5, TRUE, &ieii);

	ieii.SetSubMenu(m_hMenuTool);
	ieii.SetTypeData(TEXT("�c�[��"));
	IEMenuMG::InsertIEMenuItem(m_hIEMenu, 6, TRUE, &ieii);

	ieii.SetSubMenu(m_hMenuHelp);
	ieii.SetTypeData(TEXT("�w���v"));
	IEMenuMG::InsertIEMenuItem(m_hIEMenu, 7, TRUE, &ieii); 
}