
// ImgEdit_UIDoc.cpp : CImgEdit_UIDoc �N���X�̎���
//

#include "stdafx.h"
#include "../resource1.h"
#include "ImgEdit_UI.h"
#include "CreateNewFileDialog.h"

#include "ImgEdit_UIDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HINSTANCE CImgEdit_UIDoc::m_hInst;
extern ImgEdit g_ImgEdit;

// CImgEdit_UIDoc

IMPLEMENT_DYNCREATE(CImgEdit_UIDoc, CDocument)

BEGIN_MESSAGE_MAP(CImgEdit_UIDoc, CDocument)
END_MESSAGE_MAP()


// CImgEdit_UIDoc �R���X�g���N�V����/�f�X�g���N�V����

CImgEdit_UIDoc::CImgEdit_UIDoc()
{
	m_hIEMenu        = NULL;
	m_hMenuFile      = NULL;
	m_hMenuEdit      = NULL;
	m_hMenuImage     = NULL;
	m_hMenuDisplay   = NULL;
	m_hMenuImgSelect = NULL;
	m_hMenuTool      = NULL;
	m_hMenuHelp      = NULL;
	m_pImgFile = NULL;
}

CImgEdit_UIDoc::~CImgEdit_UIDoc()
{
	if(m_pImgFile){
		g_ImgEdit.DeleteImgFile(m_pImgFile);
		m_pImgFile = NULL;
	}

	_ASSERTE(_CrtCheckMemory());
}

BOOL CImgEdit_UIDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument()){
		return FALSE;
	}

	CCreateNewFileDialog dialog;
	dialog.SetImgEdit(&g_ImgEdit);
	
	INT_PTR ret = dialog.DoModal();
	if(ret == IDOK){
		m_pImgFile = dialog.GetCreateImgFile();
	}
	else{
		return FALSE;
	}

	if(!m_pImgFile){
		return FALSE;
	}

	m_bModified = TRUE;
	//
	InitIEMenu();

	return TRUE;
}

BOOL CImgEdit_UIDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	TCHAR drive[_MAX_DRIVE];
	TCHAR dir[_MAX_DIR];
	TCHAR fname[_MAX_FNAME];
	TCHAR ext[_MAX_EXT];
	_splitpath_s(
		lpszPathName,
		drive,
		_MAX_DRIVE,
		dir,
		_MAX_DIR,
		fname,
		_MAX_FNAME,
		ext,
		_MAX_EXT);

	if(strcmp(fname, "??scaner??") == 0){
		if (!CDocument::OnNewDocument()){
			return FALSE;
		}

		m_pImgFile = g_ImgEdit.CreateNewFile();
		if(!m_pImgFile){
			return FALSE;
		}

		
		int id = g_ImgEdit.GetCommandID("LoadFromTWAIN");
		g_ImgEdit.RunCommandAsync(id, &m_pImgFile);
	}
	else{
		if(!CDocument::OnOpenDocument(lpszPathName)){
			return FALSE;
		}

		m_pImgFile = g_ImgEdit.LoadImgFile(lpszPathName);
		if(!m_pImgFile){
			return FALSE;
		}
	}

	SetModifiedFlag(TRUE);
	//
	InitIEMenu();

	return TRUE;
}

BOOL CImgEdit_UIDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	if(!CDocument::OnSaveDocument(lpszPathName)){
		return FALSE;
	}

	if(!m_pImgFile){
		return FALSE;
	}

	g_ImgEdit.SaveAsImgFile(lpszPathName, m_pImgFile);
	return TRUE;
}

void CImgEdit_UIDoc::OnCloseDocument()
{
	CDocument::OnCloseDocument();
}

BOOL CImgEdit_UIDoc::DoSave(LPCSTR lpszPathName, BOOL bReplace)
{
	CString newName = lpszPathName;
	if (newName.IsEmpty())
	{
		CDocTemplate* pTemplate = GetDocTemplate();
		ASSERT(pTemplate != NULL);

		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = m_strTitle;
			// check for dubious filename
			int iBad = newName.FindOneOf(_T(":/\\"));
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			CString strExt;
			if (pTemplate->GetDocString(strExt, CDocTemplate::filterExt) &&
			  !strExt.IsEmpty())
			{
				ASSERT(strExt[0] == '.');
				int iStart = 0;
				newName += strExt.Tokenize(_T(";"), iStart);
			}	
		}

		if (!((CImgEdit_UIApp*)AfxGetApp())->DoOrgPromptFileName(newName,
		  bReplace ? AFX_IDS_SAVEFILE : AFX_IDS_SAVEFILECOPY,
		  OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, FALSE, pTemplate))
			return FALSE;       // don't even attempt to save
	}

	CWaitCursor wait;

	if (!OnSaveDocument(newName))
	{
		if (lpszPathName == NULL)
		{
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE(traceAppMsg, 0, "Warning: failed to delete file after failed SaveAs.\n");
				//DELETE_EXCEPTION(e);
				e->Delete();
			}
			END_CATCH_ALL
		}
		return FALSE;
	}

	// reset the title and change the document name
	if (bReplace)
		SetPathName(newName);

	return TRUE;        // success
}

// CImgEdit_UIDoc �V���A����

void CImgEdit_UIDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �i�[����R�[�h�������ɒǉ����Ă��������B
	}
	else
	{
		// TODO: �ǂݍ��ރR�[�h�������ɒǉ����Ă��������B
	}
}

HMENU CImgEdit_UIDoc::GetDefaultMenu()
{
	return m_hIEMenu;
}

void CImgEdit_UIDoc::SetHInstance(HINSTANCE hInst)
{
	CImgEdit_UIDoc::m_hInst = hInst;
}


// CImgEdit_UIDoc �f�f

#ifdef _DEBUG
void CImgEdit_UIDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImgEdit_UIDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CImgEdit_UIDoc::SetEmptyPath()
{
	m_strPathName.Empty();
	SetTitle("untitled");
}

ImgFile_Ptr CImgEdit_UIDoc::GetImgFile() const
{
	return m_pImgFile;
}

void CImgEdit_UIDoc::InitIEMenu()
{
	m_hIEMenu = IEMenuMG::CreateIEMenu();

	m_hMenuFile      = IEMenuMG::CreateIEMenu();
	m_hMenuEdit      = IEMenuMG::CreateIEMenu();
	m_hMenuImage     = IEMenuMG::CreateIEMenu();
	m_hMenuDisplay   = IEMenuMG::CreateIEMenu();
	m_hMenuWindow = IEMenuMG::CreateIEMenu();
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
	ieii.SetID( g_ImgEdit.GetCommandID("LoadFromTWAIN") );
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