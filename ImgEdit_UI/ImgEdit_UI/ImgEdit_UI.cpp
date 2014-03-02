
// ImgEdit_UI.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//
#include "stdafx.h"
#include "afxwinappex.h"
#include "ImgEdit_UI.h"
#include "MainFrm.h"
#include "FileDialogExt.h"

#include "ChildFrm.h"
#include "ImgEdit_UIDoc.h"
#include "ImgEdit_UIView.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
	#include "FPSCounter.h"
	CFPSCounter FPS(60);
#endif

extern ImgEdit g_ImgEdit;

bool CImgEdit_UIApp::m_isHookKeyboard = false;
bool CImgEdit_UIApp::m_isHookMouse = false;
HHOOK CImgEdit_UIApp::m_hGetMessageHook = NULL;
//HHOOK CImgEdit_UIApp::m_hCallWndHook = NULL;

// CImgEdit_UIApp

BEGIN_MESSAGE_MAP(CImgEdit_UIApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CImgEdit_UIApp::OnAppAbout)
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
END_MESSAGE_MAP()


// CImgEdit_UIApp �R���X�g���N�V����

CImgEdit_UIApp::CImgEdit_UIApp()
{
	m_bHiColorIcons = TRUE;
}

CImgEdit_UIApp::~CImgEdit_UIApp()
{
}

// �B��� CImgEdit_UIApp �I�u�W�F�N�g�ł��B

CImgEdit_UIApp theApp;


// CImgEdit_UIApp ������
BOOL CImgEdit_UIApp::InitInstance()
{
#ifdef _DEBUG
	//_CrtSetBreakAlloc(76765); //for detect memory leak
	//for heap chek
	//int new_flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	//new_flag |= _CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF;
	//_CrtSetDbgFlag(new_flag);
	//_CrtSetDbgFlag( _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetDbgFlag( _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_CHECK_ALWAYS_DF | _CRTDBG_LEAK_CHECK_DF);
	assert( _CrtCheckMemory() );
#endif

	CWinAppEx::InitInstance();

	//�����J�����g�f�B���N�g�����擾���AImgEdit�ɓn��
	char cdPath[MAX_PATH];
	if(0 == ::GetCurrentDirectory(MAX_PATH, cdPath)){
		//error
		return FALSE;
	}
	g_ImgEdit.SetStartDirectory(cdPath);

	//
	GetModuleFileName(::AfxGetInstanceHandle(), cdPath, MAX_PATH);
	g_ImgEdit.Init(".\\config.ini", cdPath, ".\\log");

	//add external command
	m_pLoadFromTWAIN = new LoadFromTWAIN;
	IECommandMG::AddExtrnCommandToTable("LoadFromTWAIN",  (IIECommand*)m_pLoadFromTWAIN);

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	SetRegistryKey(_T("imgeditui"));
	LoadStdProfileSettings(0);  // �W���� INI �t�@�C���̃I�v�V���������[�h���܂� (MRU ���܂�)

	InitContextMenuManager();

	//InitKeyboardManager();
	//InitTooltipManager();
	//CMFCToolTipInfo ttParams;
	//ttParams.m_bVislManagerTheme = TRUE;
	//theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
	//	RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ImgEdit_UITYPE,
		RUNTIME_CLASS(CImgEdit_UIDoc),
		RUNTIME_CLASS(CChildFrame), // �J�X�^�� MDI �q�t���[��
		RUNTIME_CLASS(CImgEdit_UIView));

	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ���C�� MDI �t���[�� �E�B���h�E���쐬����load���B
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME)){
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	//�t�@�C���̃h���b�v������
	m_pMainWnd->DragAcceptFiles();

	//// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h ���C������͂��܂��B
	//CCommandLineInfo cmdInfo;
	//ParseCommandLine(cmdInfo);


	//// �R�}���h ���C���Ŏw�肳�ꂽ�f�B�X�p�b�` �R�}���h�ł��B�A�v���P�[�V������
	//// /RegServer�A/Register�A/Unregserver �܂��� /Unregister �ŋN�����ꂽ�ꍇ�AFalse ��Ԃ��܂��B
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;

	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	//timeGettime�̐��x��1ms�ɐݒ�
	::timeBeginPeriod(1);

	//init window
	if (m_BrushPopupWnd.CreatePopupWnd(NULL, ID_BRUSH_POPUP_WND)) {
		g_ImgEdit.SetBrushPopupWnd(&m_BrushPopupWnd);
	}

	//start hook event
	m_hGetMessageHook = ::SetWindowsHookEx(
		WH_GETMESSAGE,
		(HOOKPROC)CImgEdit_UIApp::GetMessageHookProc,
		NULL,
		::GetCurrentThreadId());
	this->HookKeyboard();
	this->HookMouse();

	//
	CImgEdit_UIDoc::SetHInstance( this->m_hInstance );

	return TRUE;
}

//�I��������
int CImgEdit_UIApp::ExitInstance()
{
	//unhook message
	this->UnhookMouse();
	this->UnhookKeyBoard();
	if (m_hGetMessageHook) {
		if (::UnhookWindowsHookEx(m_hGetMessageHook) == FALSE) {
		}
	}
	m_hGetMessageHook = NULL;

	//remove external command
	IECommandMG::RemoveExtrnCommandToTable("LoadFromTWAIN");
	if(m_pLoadFromTWAIN){
		delete m_pLoadFromTWAIN;
		m_pLoadFromTWAIN = NULL;
	}

	//
	g_ImgEdit.SetBrushPopupWnd(NULL);

	//
	_ASSERTE(_CrtCheckMemory());
	//g_ImgEdit.End();
	_ASSERTE(_CrtCheckMemory());

	//timer�I��
	::timeEndPeriod(1);

	int ret = CWinAppEx::ExitInstance();
	return ret;
}

// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

BOOL CImgEdit_UIApp::OnIdle(LONG lCount)
{
	g_ImgEdit.OnIdle();
	return CWinAppEx::OnIdle(lCount);
}

// �_�C�A���O�����s���邽�߂̃A�v���P�[�V���� �R�}���h
void CImgEdit_UIApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CImgEdit_UIApp::OnFileNew()
{
	TRY
	{
		CWinApp::OnFileNew();
	}
	CATCH(CException, e)
	{
		::OutputDebugString("CImgEdit_UIApp::OnFileNew exception\n");
	}
	END_CATCH
}

void CImgEdit_UIApp::OnFileOpen()
{
	TRY
	{
		CString cStr;
		if(DoOrgPromptFileName(cStr, AFX_IDS_OPENFILE,
				OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, TRUE, NULL)){
			OpenDocumentFile(cStr);
		}
	}
	CATCH(CException, e)
	{
		::OutputDebugString("CImgEdit_UIView::OnFileOpen exception\n");
	}
	END_CATCH
}

// CImgEdit_UIApp �̃J�X�^�}�C�Y���ꂽ�ǂݍ��݃��\�b�h�ƕۑ����\�b�h

void CImgEdit_UIApp::PreLoadState()
{
	//BOOL bNameValid;
	//CString strName;
	//bNameValid = strName.LoadString(IDS_EDIT_MENU);
	//ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	//bNameValid = strName.LoadString(IDS_EXPLORER);
	//ASSERT(bNameValid);
	//GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CImgEdit_UIApp::LoadCustomState()
{
}

void CImgEdit_UIApp::SaveCustomState()
{
}

void CImgEdit_UIApp::OnFileOpenFromScaner()
{
	OpenDocumentFile("??scaner??");
}

BOOL CImgEdit_UIApp::DoOrgPromptFileName(
	CString& fileName,
	UINT nIDSTitle,
	DWORD lFlags,
	BOOL bOpenFileDialog,
	CDocTemplate* pTemplate)
{
	CFileDialogExt dlgFile(bOpenFileDialog);
    CString title;
    VERIFY(title.LoadString(nIDSTitle));
    dlgFile.m_ofn.Flags |= lFlags;

	std::string stlstr_filters;
	char* cstr_filters = NULL;
	if(bOpenFileDialog){
		cstr_filters = new char[ ImgFileIOMG::GetLoadExtFiltersLength() +1];
		ImgFileIOMG::GetLoadExtFilters(cstr_filters);
	}
	else{
		cstr_filters = new char[ ImgFileIOMG::GetSaveExtFiltersLength() +1];
		ImgFileIOMG::GetSaveExtFilters(cstr_filters);

		//get extension of save file path
		TCHAR drive[_MAX_DRIVE];
		TCHAR dir[_MAX_DIR];
		TCHAR fname[_MAX_FNAME];
		TCHAR ext[_MAX_EXT];
		_splitpath_s(
			fileName,
			drive,
			_MAX_DRIVE,
			dir,
			_MAX_DIR,
			fname,
			_MAX_FNAME,
			ext,
			_MAX_EXT);
		
		int nIndex = ImgFileIOMG::GetNFilterIndex(ext);
		if (-1 != nIndex) {
			dlgFile.m_ofn.nFilterIndex = nIndex;
		}
	}
	CString strFilter = CString(cstr_filters);
	int nCount = strFilter.Replace(_T('|'), _T('\0'));
	dlgFile.m_ofn.nMaxCustFilter = nCount / 2;
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);
	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();

	delete[] cstr_filters;
	cstr_filters = NULL;

	return nResult == IDOK;
}




//// CImgEdit_UIApp ���b�Z�[�W �n���h��
//LRESULT CImgEdit_UIApp::CallWndHookProc(int nCode, WPARAM wParam, LPARAM lParam)
//{
//	if (nCode < 0 || nCode != HC_ACTION)
//		return ::CallNextHookEx(m_hCallWndHook, nCode, wParam, lParam);
//
//	LRESULT ret = 1;
//	CWPSTRUCT* pcws = (CWPSTRUCT*)lParam;
//	if (pcws->message == WM_MOUSEWHEEL) {
//		ret = MouseWheelHookProc(pcws->message, pcws->wParam, pcws->lParam);
//	}
//
//	if ((pcws->message == WM_KEYDOWN) || (pcws->message == WM_KEYUP) ||
//		(pcws->message == WM_SYSKEYDOWN) || (pcws->message == WM_SYSKEYUP))
//	{
//		ret = KeyboardHookProc(pcws->message, pcws->wParam, pcws->lParam);
//	}
//
//	if (ret)
//		return ::CallNextHookEx(m_hCallWndHook, nCode, wParam, lParam);
//	return 0;
//}

LRESULT CImgEdit_UIApp::GetMessageHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (nCode < 0)
		return ::CallNextHookEx(m_hGetMessageHook, nCode, wParam, lParam);

	LRESULT ret = 1;
	MSG* pMsg = (MSG*) lParam;
	assert(pMsg);
	if (pMsg->message == WM_MOUSEWHEEL) {
		ret = MouseWheelHookProc(pMsg->message, pMsg->wParam, pMsg->lParam);
	}

	if ((pMsg->message == WM_KEYDOWN) || (pMsg->message == WM_KEYUP) ||
		(pMsg->message == WM_SYSKEYDOWN) || (pMsg->message == WM_SYSKEYUP) ||
		(pMsg->message == WM_APP_SYSKEYDOWN) || (pMsg->message == WM_APP_SYSKEYUP))
	{
		ret = KeyboardHookProc(pMsg->message, pMsg->wParam, pMsg->lParam);
	}

	return ::CallNextHookEx(m_hGetMessageHook, nCode, wParam, lParam);
}

LRESULT CImgEdit_UIApp::KeyboardHookProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_isHookKeyboard) return 1;

	::OutputDebugString("keyboard hook! ");
	if ((message == WM_KEYUP) || (message == WM_SYSKEYUP)) {
		UINT optChar = 0;
		//check keyup shift
		if ((::GetKeyState(VK_SHIFT) & 0x8000) > 0) {
			::OutputDebugString("shift ");
			optChar |= IE_OPT_KEY_SHIFT;
		}
		//check keyup ctrl
		if ((::GetKeyState(VK_CONTROL) & 0x8000) > 0) {
			::OutputDebugString("ctrl ");
			optChar |= IE_OPT_KEY_CONTROL;
		}
		//check keydup alt
		if ((::GetKeyState(VK_MENU) & 0x8000) > 0) {
			::OutputDebugString("alt ");
			optChar |= IE_OPT_KEY_ALT;
		}
		UINT c = tolower(wParam);
		char log[64];
		wsprintf(log, "%c up\n", c);
		::OutputDebugString(log);

		g_ImgEdit.OnKeyboardUp(c, optChar);
		return 0;
	}
	else if ((message == WM_KEYDOWN) || (message == WM_SYSKEYDOWN)) {
		UINT optChar = 0;
		//check keydown shift
		if (::GetKeyState(VK_SHIFT) < 0) {
			::OutputDebugString("shift ");
			optChar |= IE_OPT_KEY_SHIFT;
		}
		//check keydown ctrl
		if (::GetKeyState(VK_CONTROL) < 0) {
			::OutputDebugString("ctrl ");
			optChar |= IE_OPT_KEY_CONTROL;
		}
		//check keydown alt
		if (::GetKeyState(VK_MENU) < 0) {
			::OutputDebugString("alt ");
			optChar |= IE_OPT_KEY_ALT;
		}
		UINT c = tolower(wParam);
		char log[64];
		wsprintf(log, "%c down\n", c);
		::OutputDebugString(log);

		if (g_ImgEdit.OnKeyboardDown(c, optChar)) {
			::OutputDebugString("hit keydown");
		}
		return 0;
	}
	return 1;
}

LRESULT CImgEdit_UIApp::MouseWheelHookProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_isHookMouse) return 1;

	POINT mouse_pt;
	::GetCursorPos(&mouse_pt);

	//check view client
	CImgEdit_UIApp* pApp = (CImgEdit_UIApp*)AfxGetApp();
	assert(pApp);
	CMainFrame* pMainWnd = dynamic_cast<CMainFrame*>(pApp->m_pMainWnd);
	assert(pMainWnd);
	CMDIChildWnd* pMDIChildWnd = pMainWnd->MDIGetActive();
	if (pMDIChildWnd) {
		CView* pView = pMDIChildWnd->GetActiveView();
		if (pView) {
			RECT viewrc;
			pView->GetWindowRect(&viewrc);
			if (isInRect(mouse_pt.x, mouse_pt.y, &viewrc)) {
				pView->SendMessage(WM_APP_MOUSEWHEEL, wParam, lParam);
				return 0;
			}
		}
	}

	//send message other window
	pApp->m_pMainWnd->ScreenToClient(&mouse_pt);
	CWnd* pWnd = pApp->m_pMainWnd->ChildWindowFromPoint(mouse_pt, CWP_SKIPINVISIBLE | CWP_SKIPDISABLED | CWP_SKIPTRANSPARENT);
	if (pWnd) {
		pWnd->SendMessage(WM_APP_MOUSEWHEEL, wParam, lParam);
		return 0;
	}

	return 1;
}