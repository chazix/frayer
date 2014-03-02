#include "stdafx.h"

#include "ImgEdit.h"

#include "IInputRevise.h"
#include "ImgLayer.h"
#include "ImgFile.h"
#include "ImgEditEventListener.h"
#include "ImgFileHandleMG.h"
#include "OutputError.h"

#include "NullHandle.h"
#include "AddNewLayerHandle.h"
#include "AddNewMaskHandle.h"
#include "EditLayerHandle.h"

#define IE_COMMAND_PLUGIN_DIR_PATH TEXT(".\\data\\command_dll")
#define IE_FILEIO_PLUGIN_DIR_PATH TEXT(".\\data\\img_file_io_dll")

#define IEBRUSH_BRUSH_DIR_FIND_FORMAT TEXT("*.ieb")
#define IEBRUSH_BRUSH_PATH_FORMAT     TEXT("%d.ieb")

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


static bool IsSupportSSE2()
{
	int CPUInfo[4];
	int InfoType = 1;
	__cpuid(CPUInfo, InfoType);

	return (CPUInfo[3] & 0x04000000);
}

ImgEdit::ImgEdit()
{
	m_pIIECommand  = NULL;
	m_pIECommandArg = NULL;
	m_pIIEBrushPopupWnd = NULL;
	m_SelectFGColor.value = 0xff000000;
	m_SelectBGColor.value = 0xffffffff; 
	m_pNowActiveFile = NULL;
	m_isInit    = false;
	m_isLDrag   = false;
	m_isKeyDown = false;
	m_IECursorID = IE_CURSOR_ID::NORMAL_CURSOR;

#ifdef _WIN32
	::InitializeCriticalSection( &m_cs_ie_command );
	::InitializeCriticalSection( &m_cs_ie_tool );
#else
	m_LockResourceID = 0;
#endif //_WIN32
}

ImgEdit::~ImgEdit()
{
	assert( _CrtCheckMemory() );

	if(m_isInit == true){
		this->End();
	}

#ifdef _WIN32
	::DeleteCriticalSection( &m_cs_ie_command );
	::DeleteCriticalSection( &m_cs_ie_tool );
#endif //_WIN32

	assert( _CrtCheckMemory() );
}

bool ImgEdit::Init(const char* configini_path, const char* exe_path, const char* log_dir)
{
	assert( m_isInit == false );

	m_ConfigINIFilePath = configini_path;

	//
	OutputError::Init(exe_path, log_dir, LOG_LEVEL::_INFO);
	OutputError::PushLog(LOG_LEVEL::_INFO, "start ImgEdit::Init()");

	if (!IsSupportSSE2()) {
		OutputError::PushLog(LOG_LEVEL::_ERROR, "this cpu doesn't support SSE2");
		OutputError::Alert(IE_ERROR_DOESNT_SUPPORT_SSE2);
		return false;
	}

	//
	babel::init_babel();

	m_isInit = true;
	
	//�ݒ�ǂݍ���
	if (!m_ConfigINIFilePath.empty()) {
		m_IEConfig.LoadConfigFile(m_ConfigINIFilePath.c_str());
		m_InputReviseMG.SelectInputRevise( m_IEConfig.GetInputRevise() );
	} else {
		OutputError::PushLog(LOG_LEVEL::_WARN, "not load config ini file" );
	}

	//MG������
	LayerSynthesizerMG::Init();
	IECommandMG::Init( IE_COMMAND_PLUGIN_DIR_PATH );
	ImgFileIOMG::Init( IE_FILEIO_PLUGIN_DIR_PATH );
	m_IEKeyMap.SetIEKeyMapDat( m_IEConfig.GetKeymapDat() );
	m_IEKeyMap.Init();
	m_IEParamRegister.SetIEParamRegisterDat( m_IEConfig.GetPrmRegDat() );
	m_IEParamRegister.Init();
	m_EffectMG.Init( m_IEConfig.GetEffectXml() );
	m_IEToolMG.Init( m_IEConfig.GetToolXml() );
	m_IEToolMG.SetImgEdit( this );
	m_IEBrushMG.Init( m_IEConfig.GetBrushXml() );
	m_IEPaletteMG.Init( m_IEConfig.GetPaletteXml() );

	//load select tool
	IIETool* pTool = NULL;
	char buf[MAX_IE_MODULE_NAME];
	if (m_IEParamRegister.ReadParam("imgedit", "selected_tool", buf)) {
		pTool = m_IEToolMG.GetIEToolFromName(buf);
	}
	this->SetSelectTool(pTool);

	//load select color
	if (m_IEParamRegister.ReadParam("imgedit", "selected_bg_color", buf)) {
		IEColor color;
		color.value = atoi(buf);
		SetSelectFGColor(color);
		ExchangeColorFGForBG();
	}
	if (m_IEParamRegister.ReadParam("imgedit", "selected_fg_color", buf)) {
		IEColor color;
		color.value = atoi(buf);
		SetSelectFGColor(color);
	}

	OutputError::PushLog(LOG_LEVEL::_INFO, "end ImgEdit::Init()");
	return true;
}

/////////////////////////////////
/*!
	�I������
*/
void ImgEdit::End()
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "start ImgEdit::End()");

	assert( m_isInit == true );

	m_isInit = false;

	int cnt=0;
	char fname[_MAX_PATH];
	char path[_MAX_PATH];
	char buf[256];

	//save select color
	IEColor color = GetSelectFGColor();
	wsprintf(buf, "%d", color.value);
	m_IEParamRegister.WriteParam("imgedit", "selected_fg_color", buf);

	color = GetSelectBGColor();
	wsprintf(buf, "%d", color.value);
	m_IEParamRegister.WriteParam("imgedit", "selected_bg_color", buf);

	//save select tool
	IIETool* pTool = this->GetSelectTool();
	if (pTool) {
		m_IEParamRegister.WriteParam("imgedit", "selected_tool", pTool->GetName());
		this->SetSelectTool(NULL);
	}

	//�R�}���h���c���Ă�����J��
	EndCommand();

	if (!m_StartDirPath.empty()) {
		::SetCurrentDirectory(m_StartDirPath.c_str());
	}

	//�ݒ菑���o��
	if (!m_ConfigINIFilePath.empty()) {
		m_IEConfig.SaveConfigFile(m_ConfigINIFilePath.c_str());
	}

	//�ێ����Ă���t�@�C���̉��
	m_ImgFile_Vec.clear();
	//while (!m_ImgFile_Vec.empty()) {
	//	//delete top file
	//	DeleteImgFile(&(m_ImgFile_Vec[0]));
	//}

	//MG�I��
	m_IEPaletteMG.End();
	m_IEBrushMG.End();
	m_IEToolMG.End();
	m_EffectMG.End();
	m_IEParamRegister.End();
	m_IEKeyMap.End();
	ImgFileIOMG::End();
	IECommandMG::End();
	LayerSynthesizerMG::End();

	//
	OutputError::PushLog( LOG_LEVEL::_INFO, "end ImgEdit::End()");
	OutputError::End();
}

/////////////////////////////////
/*!
	�C�x���g���X�i�[��o�^����B
	@param[in] pListener �o�^���郊�X�i�[
*/
void ImgEdit::AddEventListener(ImgEditEventListener*  pListener)
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "start ImgEdit::AddEventListener()");

	m_ImgEditEventListener_List.push_back(pListener);

	OutputError::PushLog(LOG_LEVEL::_INFO, "end ImgEdit::AddEventListener()");
}

/////////////////////////////
/*!
	�C�x���g���X�i�[���폜����B
	@param[in] pListener �폜���郊�X�i�[
*/
void ImgEdit::DeleteEventListener(ImgEditEventListener*  pListener)
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "end ImgEdit::DeleteEventListener()");

	ImgEditEventListener_List::iterator itr = m_ImgEditEventListener_List.begin();
	for(; itr != m_ImgEditEventListener_List.end(); itr++){
		if((*itr) == pListener){
			m_ImgEditEventListener_List.erase(itr);
			return;
		}
	}

	OutputError::PushLog(LOG_LEVEL::_INFO, "end ImgEdit::DeleteEventListener()");
}

/////////////////////////////////
/*!
	�����f�B���N�g�����w�肷��B
	@param[in] path �����f�B���N�g���p�X
*/
void ImgEdit::SetStartDirectory(const char* path)
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "start ImgEdit::SetStartDirectory()");

	m_StartDirPath = path;

	IECommandMG::SetStartDir(path);
	ImgFileIOMG::SetStartDir(path);

	m_IEToolMG.SetStartDir(path);
	m_EffectMG.SetStartDir(path);
	m_IEBrushMG.SetStartDir(path);
	m_IEKeyMap.SetStartDir(path);
	m_IEParamRegister.SetStartDir(path);

	OutputError::PushLog(LOG_LEVEL::_INFO, "end ImgEdit::SetStartDirectory()");
}

///////////////////////////////
/*!
	�ݒ�N���X�ւ̃|�C���^��Ԃ��B
	@return
*/
ImgEditConfig* ImgEdit::GetIEConfig()
{
	return &m_IEConfig;
}

///////////////////////////////
/*!
	�O�i�F�Ɣw�i�F����������
*/
void ImgEdit::ExchangeColorFGForBG()
{
	IEColor tmp = m_SelectBGColor;
	m_SelectBGColor = m_SelectFGColor;
	m_SelectFGColor = tmp;

	ImgEditEventListener_List::iterator itr = m_ImgEditEventListener_List.begin();
	for(; itr != m_ImgEditEventListener_List.end(); itr++){
		if((*itr)->IsLockImgEditEvent() == false && (*itr)->IsCalledImgEdit() == false){
			//�����ɃC�x���g�Ăяo���������Ȃ��悤�Ƀt���O�𗧂Ă�
			(*itr)->SetCalledImgEdit(true);
			//
			(*itr)->OnChangeSelectColor(tmp);
		}

		(*itr)->SetCalledImgEdit(false);
	}
}

///////////////////////////////
/*!
	�I�𒆂̐F��ݒ肷��
	@param[in] color �ݒ肷��F
*/
void ImgEdit::SetSelectFGColor(IEColor color)
{
	//�I��F�̐؂�ւ�
	m_SelectFGColor = color;

	ImgEditEventListener_List::iterator itr = m_ImgEditEventListener_List.begin();
	for(; itr != m_ImgEditEventListener_List.end(); itr++){
		if((*itr)->IsLockImgEditEvent() == false && (*itr)->IsCalledImgEdit() == false){
			//�����ɃC�x���g�Ăяo���������Ȃ��悤�Ƀt���O�𗧂Ă�
			(*itr)->SetCalledImgEdit(true);
			//
			(*itr)->OnChangeSelectColor(color);
		}

		(*itr)->SetCalledImgEdit(false);
	}
}

////////////////////////////////
/*!
	�ҏW���̃t�@�C����؂�ւ���B
	@param[in] pFile �؂�ւ���̕ҏW�t�@�C��
*/
void ImgEdit::SetActiveFile(ImgFile_Ptr pFile)
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "ImgEdit::SetActiveFile()");

	if(m_pNowActiveFile == pFile) return;

	//�ҏW�t�@�C���؂�ւ�
	ImgFile_Ptr pOldFile = m_pNowActiveFile;
	m_pNowActiveFile = pFile;

	ImgEditEventListener_List::iterator itr = m_ImgEditEventListener_List.begin();
	for(; itr != m_ImgEditEventListener_List.end(); itr++){
		if((*itr)->IsLockImgEditEvent() == false && (*itr)->IsCalledImgEdit() == false){
			//�����ɃC�x���g�Ăяo���������Ȃ��悤�Ƀt���O�𗧂Ă�
			(*itr)->SetCalledImgEdit(true);
			//
			(*itr)->OnChangeActiveFile(m_pNowActiveFile, pOldFile);
		}
		(*itr)->SetCalledImgEdit(false);
	}
}

////////////////////////////////
/*!
	�t�@�C���I�u�W�F�N�g��ǉ�����B
	@param[in] pFile �ǉ�����t�@�C��
*/
void ImgEdit::AddImgFile(ImgFile_Ptr pFile)
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "ImgEdit::AddImgFile()");

	ImgEditEventListener_List::iterator itr = m_ImgEditEventListener_List.begin();
	for(; itr != m_ImgEditEventListener_List.end(); itr++){
		if((*itr)->IsLockImgEditEvent() == false && (*itr)->IsCalledImgEdit() == false){
			//�����ɃC�x���g�Ăяo���������Ȃ��悤�Ƀt���O�𗧂Ă�
			(*itr)->SetCalledImgEdit(true);
			//
			(*itr)->OnAddImgFile(pFile);
		}

		(*itr)->SetCalledImgEdit(false);
	}

	//push to vector
	m_ImgFile_Vec.push_back(pFile);
}

//////////////////////////////
/*!
	�t�@�C���I�u�W�F�N�g�̍폜�A�������J��
	@param[in] pFile �t�@�C���̃|�C���^
*/
void ImgEdit::DeleteImgFile(ImgFile_Ptr pFile)
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "ImgEdit::DeleteImgFile()");

	if(GetActiveFile() == pFile){
		SetActiveFile( NULL );
	}

	//�t�@�C���z�񂩂�폜
	ImgFile_Vec::iterator itr = m_ImgFile_Vec.begin();
	for(; itr != m_ImgFile_Vec.end(); itr++){
		if(pFile == (*itr)){
			pFile->Close();
			m_ImgFile_Vec.erase(itr);
			break;
		}
	}
}

//////////////////////////////
/*!
	�g�p����u���V�|�b�v�A�b�v�E�B���h�E���Z�b�g����B
	@param[in] pWnd �u���V�|�b�v�A�b�v�E�B���h�E�C���^�[�t�F�[�X
*/
void ImgEdit::SetBrushPopupWnd(IIEBrushPopupWnd* pWnd)
{
	m_pIIEBrushPopupWnd = pWnd;
}

//////////////////////////////
/*!
	�u���V�|�b�v�A�b�v�E�B���h�E��Ԃ�
	@return �u���V�|�b�v�A�b�v�C���^�[�t�F�[�X��Ԃ�
*/
IIEBrushPopupWnd* ImgEdit::GetBrushPopupWnd()
{
	return m_pIIEBrushPopupWnd;
}

//////////////////////////////
/*!
	�V�K�Ƀt�@�C�����쐬
	@return �쐬�����t�@�C���̃|�C���^(���s�����NULL��Ԃ�)
*/
ImgFile_Ptr ImgEdit::CreateNewFile()
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "ImgEdit::CreateNewFile()");

	ImgFile_Ptr pFile = ImgFile_Ptr(new ImgFile);
	pFile->SetBGColor(m_IEConfig.GetEditWndBGColor());

	//�t�@�C���I�u�W�F�N�g��ǉ�
	AddImgFile( pFile );
	//�ҏW���̃t�@�C���؂�ւ�
	SetActiveFile( pFile );

	return pFile;
}

//////////////////////////////
/*!
	�V�K�Ƀt�@�C�����쐬
	@param[in] width �쐬����t�@�C���̕�
	@param[in] height �쐬����t�@�C���̍���
	@param[in] fillcolor �h��Ԃ��F
	@return �쐬�����t�@�C���̃|�C���^(���s�����NULL��Ԃ�)
*/
ImgFile_Ptr ImgEdit::CreateNewFile(
	int width, int height, const UCvPixel* fillcolor)
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "ImgEdit::CreateNewFile()");

	//���̓T�C�Y����t�@�C�����쐬
	ImgFile_Ptr pFile = ImgFile_Ptr(new ImgFile);
	pFile->Init(cvSize(width, height));
	pFile->SetBGColor(m_IEConfig.GetEditWndBGColor());

	int index = pFile->GetLayerSize();
	AddNewLayerHandle* handle = (AddNewLayerHandle*)pFile->CreateImgFileHandle(IFH_ADD_NEW_LAYER);
	handle->Do( pFile );
	ImgLayer_Ptr new_layer = handle->GetNewLayer().lock();
	pFile->ReleaseImgFileHandle(handle);
	handle = NULL;

	//
	new_layer->RectFillColor(
		0, 0,
		width,
		height,
		fillcolor);

	//�t�@�C���I�u�W�F�N�g��ǉ�
	AddImgFile( pFile );
	//�ҏW���̃t�@�C���؂�ւ�
	SetActiveFile( pFile );

	//update
	LPUPDATE_DATA pData = new_layer->CreateUpdateData();
	pData->isAll = true;
	new_layer->PushUpdateData( pData );

	//
	NullHandle* null_handle = (NullHandle*)pFile->CreateImgFileHandle(IFH_NULL);
	strcpy_s(null_handle->handle_name, MAX_IMG_FILE_HANDLE_NAME, "�V�K�t�@�C���쐬");
	pFile->DoImgFileHandle( null_handle );

	return pFile;
}

//////////////////////////////
/*!
	�摜�t�@�C����ǂݍ��ށB
	@param[in] filePath �摜�t�@�C���̃p�X
	@return �쐬�����t�@�C���̃|�C���^(���s�����NULL��Ԃ�)
*/
ImgFile_Ptr ImgEdit::LoadImgFile(const char *filePath)
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "ImgEdit::LoadImgFile()");

	//try to load image from module
	ImgFile_Ptr pFile = ImgFileIOMG::LoadImgFile(filePath);

	if(pFile){
		pFile->SetBGColor( m_IEConfig.GetEditWndBGColor() );
		pFile->SetNumOfHistory( m_IEConfig.GetNumOfHistory() );
	}
	else{//���߂Ȃ�W���̓ǂݍ���
		//�摜��ǂݍ���
		IplImage* img = cvLoadImage(filePath, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
		if(img == NULL){//�ǂݍ��ݎ��s
			return NULL;
		}

		IplImage* img8;
		//8bit�����Ȃ��f�[�^�łȂ����8bit�����Ȃ��f�[�^�ɕϊ�
		if(img->depth != IPL_DEPTH_8U){
			img8 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
			cvConvertScaleAbs( img, img8, 1, 0);
			cvReleaseImage( &img );
		}
		else{
			img8 = img;
		}

		IplImage* img8_4ch;
		if(img8->nChannels != 4){
			img8_4ch = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 4);
			ucvCvtColor(img8, img8_4ch, CV_BGR2BGRA);
			cvReleaseImage( &img8 );
		}
		else{
			img8_4ch = img8;
		}

		//create file
		pFile = ImgFile_Ptr(new ImgFile());
		pFile->Init(cvSize(img8_4ch->width, img8_4ch->height));
		pFile->SetBGColor( m_IEConfig.GetEditWndBGColor() );
		pFile->SetNumOfHistory( m_IEConfig.GetNumOfHistory() );

		//add new layer
		AddNewLayerHandle* handle = (AddNewLayerHandle*)pFile->CreateImgFileHandle(IFH_ADD_NEW_LAYER);
		handle->Do( pFile );
		ImgLayer_Ptr new_layer = handle->GetNewLayer().lock();
		pFile->ReleaseImgFileHandle(handle);
		handle = NULL;

		pFile->SetSelectLayer( new_layer );

		//
		new_layer->ImgBltToLayer(
			0, 0,
			img8_4ch->width, img8_4ch->height,
			img8_4ch,
			0, 0);
		new_layer->RectFillAlpha(
			0, 0,
			img8_4ch->width, img8_4ch->height,
			255);

		//update
		LPUPDATE_DATA pData = new_layer->CreateUpdateData();
		pData->isAll = true;
		new_layer->PushUpdateData( pData );

		//
		NullHandle* null_handle = (NullHandle*)pFile->CreateImgFileHandle(IFH_NULL);
		strcpy_s(null_handle->handle_name, MAX_IMG_FILE_HANDLE_NAME, "�t�@�C���ǂݍ���");
		pFile->DoImgFileHandle( null_handle );

		//
		cvReleaseImage( &img8_4ch );
	}

	//set file path
	pFile->SetPath(filePath);
	
	//add file to vector
	AddImgFile( pFile );
	//change active file
	SetActiveFile( pFile );

	return pFile;
}

//////////////////////////////
/*
	���O�����ĕۑ�
	@param[in] filePath �ۑ�����摜�̃t�@�C���p�X
	@param[in] pFile �ۑ�����t�@�C��
*/
void ImgEdit::SaveAsImgFile(const char *filePath, ImgFile_Ptr pFile)
{
	OutputError::PushLog(LOG_LEVEL::_INFO, "ImgEdit::SaveAsImgFile()");

	if(!ImgFileIOMG::SaveAsImgFile(filePath, pFile)){
		CvSize size = pFile->GetImgSize();
		IplImage *img4ch = cvCreateImage(size, IPL_DEPTH_8U, 4);
		IplImage *img = cvCreateImage(size, IPL_DEPTH_8U, 3);

		//�t�@�C���摜���R�s�[����3ch�ɕϊ����ĕۑ�
		pFile->CopyFileImage(img4ch);

		int x,y;
		UCvPixel pixel;
		for(y=0; y<img4ch->height; y++){
			for(x=0; x<img4ch->width; x++){
				GetPixelFromBGRA(img4ch, x, y, &pixel);
				SetPixelToBGR(img, x, y, &pixel);
			}
		}

		cvSaveImage(filePath, img);
		//�t�@�C���p�X���X�V
		pFile->SetPath(filePath);

		cvReleaseImage(&img);
	}
}

//////////////////////////////
/*
	�㏑���ۑ�
	@param[in] pFile �ۑ�����t�@�C��
*/
void ImgEdit::SaveImgFile(ImgFile_Ptr pFile)
{
	SaveAsImgFile(pFile->GetPath().c_str(), pFile);
}

void ImgEdit::StartEventCapture(const char *capture_file_path)
{
	m_IEEventCapture.StartCapture(capture_file_path);
}

void ImgEdit::EndEventCapture()
{
	m_IEEventCapture.EndCapture();
}

void ImgEdit::PlayEventCapture(const char* capture_file_path)
{
	m_IEEventCapture.PlayCapture(capture_file_path, this);
}

void ImgEdit::PlayEventCaptureAsync(const char* capture_file_path)
{
	m_IEEventCapture.PlayCaptureAsync(capture_file_path, this);
}

//////////////////////////////
/*
	���ݕҏW���̃t�@�C���̃T�C�Y(��x����)��Ԃ��B
	@return rc �t�@�C���̃T�C�Y
*/
CvSize ImgEdit::GetNowActiveFileSize()
{
	return GetActiveFile()->GetImgSize();
}

//////////////////////////////
/*
	�␳���@�I���R���{�{�b�N�X���Z�b�g����
*/
void ImgEdit::SetInputReviseComboBox(HWND hCombo)
{
	m_InputReviseMG.SetInputReviseComboBox(hCombo);
}

////////////////////////////////
/*!
	�␳���@�̖��O���w�肵�ĕ␳���@��ύX
*/
void ImgEdit::SelectInputRevise(const char* name)
{
	m_IEConfig.SetInputRevise(name);
	m_InputReviseMG.SelectInputRevise(name);
}

void ImgEdit::GetInputReviseName(char* dst_name)
{
	m_InputReviseMG.GetInputReviseName(dst_name);
}

//////////////////////////////////
///*!
//	�}�E�X�J�[�\����I�𒆂̃u���V�̌`��֕ς���
//*/
//VOID ImgEdit::ChangeCusorToSelectBrush()
//{
//	//ImgFile_Vec::iterator itr = m_ImgFile_Vec.begin();
//	//for(; itr != m_ImgFile_Vec.end(); itr++){
//	//	IECoursorMG::SetBrushCoursor((*itr)->GetHWND(), GetSelectBrush(), 1.0);
//	//}
//}

////////////////////////////////
/*!
*/
bool ImgEdit::RunCommand(int cm_id, void* pvoid)
{
	//�O�̃R�}���h���I��
	EndCommand();

	//id����R�}���h���Z�b�g
	m_pIIECommand = IECommandMG::CreateIECommand(cm_id);
	if(m_pIIECommand){
		if( m_pIIECommand->Init(this) ){
			m_pIECommandArg = pvoid;
			while( m_pIIECommand->Run( this, m_pIECommandArg ) );
			EndCommand();
			return true;
		}
		else{
			//faild command init
			m_pIIECommand = NULL;
		}
	}

	return false;;
}

bool ImgEdit::RunCommand(const char* cm_name, void* pvoid)
{
	//�O�̃R�}���h���I��
	EndCommand();

	//id����R�}���h���Z�b�g
	m_pIIECommand = IECommandMG::CreateIECommand(cm_name);
	if( m_pIIECommand ){
		if( m_pIIECommand->Init(this) ){
			m_pIECommandArg = pvoid;
			while( m_pIIECommand->Run( this, m_pIECommandArg ) );
			EndCommand();
			return true;
		}
		else{
			//faild command init
			m_pIIECommand = NULL;
		}
	}

	return false;;
}

bool ImgEdit::RunCommandAsync(int cm_id, void* pvoid)
{
	//�O�̃R�}���h���I��
	EndCommand();

	//id����R�}���h���Z�b�g
	m_pIIECommand = IECommandMG::CreateIECommand(cm_id);
	if(m_pIIECommand){
#ifdef _OUTPUT_LOG_
		char cm_name[MAX_IE_MODULE_NAME];
		IECommandMG::GetCommandName(cm_id, cm_name); 
		char log[MAX_IE_MODULE_NAME+256];
		wsprintf(log, "ImgEdit::RunCommandAsync() cm_name=%s", cm_name);
		OutputError::PushLog( LOG_LEVEL::_INFO, log );
#endif

		if( m_pIIECommand->Init(this) ){
			m_pIECommandArg = pvoid;
			return true;
		}
		else{
			//faild command init
			m_pIIECommand = NULL;
		}
	}

	return false;;
}

bool ImgEdit::RunCommandAsync(const char* cm_name, void* pvoid)
{
	//�O�̃R�}���h���I��
	EndCommand();

	//id����R�}���h���Z�b�g
	m_pIIECommand = IECommandMG::CreateIECommand(cm_name);
	if( m_pIIECommand ){
#ifdef _OUTPUT_LOG_
		char log[MAX_IE_MODULE_NAME+256];
		wsprintf(log, "ImgEdit::RunCommandAsync() cm_name=%s", cm_name);
		OutputError::PushLog( LOG_LEVEL::_INFO, log );
#endif

		if( m_pIIECommand->Init(this) ){
			m_pIECommandArg = pvoid;
			return true;
		}
		else{
			//faild command init
			m_pIIECommand = NULL;
		}
	}

	return false;;
}

int ImgEdit::GetCommandID(const char* cm_name)
{
	return IECommandMG::GetCommandID(cm_name);
}

int ImgEdit::GetMinCommandID()
{
	return IECommandMG::GetMinID();
}

int ImgEdit::GetMaxCommandID()
{
	return IECommandMG::GetMaxID();
}

IIECommand* ImgEdit::GetRunningCommand()
{
	return m_pIIECommand;
}

IEKeyMap* ImgEdit::GetKeyMap()
{
	return (&m_IEKeyMap);
}

bool ImgEdit::ReadFromParamRegister(
	const IIEModule *pModule,
	const char *name,
	char *val)
{
	return m_IEParamRegister.ReadParam(pModule->GetName(), name, val);
}

void ImgEdit::WriteToParamRegister(
	const IIEModule *pModule,
	const char *name,
	const char *val)
{
	return m_IEParamRegister.WriteParam(pModule->GetName(), name, val);
}

/////////////////////////////////////
/*!
	���ׂẴt�@�C���̉摜�X�V
	@param[in] data �摜�X�V�f�[�^�t���O
*/
void ImgEdit::PushUpdateDataToAllFile(const LPUPDATE_DATA data)
{
	LPUPDATE_DATA lud;
	ImgFile_Vec::iterator itr = m_ImgFile_Vec.begin();
	for(; itr != m_ImgFile_Vec.end(); itr++){
		lud = (*itr)->CreateUpdateData();
		(*lud) = (*data);
		(*itr)->PushUpdateData(lud);
	}
}

////////////////////////////////
/*!
	�A�C�h�����Ăяo��
*/
VOID ImgEdit::OnIdle()
{
	if(m_pIIECommand){
		BOOL ret;
		//���b�N�҂��������ĕ`�揈�����Ƃ܂�
		//ResourceLock( IE_RESOURCE_ID::IE_COMMAND );
		ret = m_pIIECommand->Run(this, m_pIECommandArg);
		//ResourceUnlock( IE_RESOURCE_ID::IE_COMMAND );
			
		if(ret == FALSE){
			EndCommand();
		}
	}
}


////////////////////////////////
/*!
	���}�E�X�{�^���������ꂽ�Ƃ��Ăяo��
	@param[in] pFile �}�E�X�h���b�O���s����t�@�C��
	@param[in] lpd �}�E�X�̓��͏��
*/
VOID ImgEdit::OnMouseLButtonDown(UINT nFlags, LPIE_INPUT_DATA lpd)
{
	m_IEEventCapture.OnMouseLButtonDown(nFlags, lpd);

	IIEGuiFrame* pGuiFrame = NULL;
	IIETool* pTool = this->GetSelectTool();
	if (m_pIIECommand) {
		pGuiFrame = m_pIIECommand;
	} else if (pTool) {
		pGuiFrame = pTool;
	}

	if(!pGuiFrame) return;
	ImgFile_Ptr pFile = GetActiveFile();
	if(!pFile) return;

	InputData_Vec inputData_Vec;
	m_isLDrag = true;

	switch(pGuiFrame->GetInputDataFlag()){
		case IE_INPUT_DATA_FLAG::IMAGE_POS:
			pFile->ConvertPosWndToImg(lpd);
			break;
		case IE_INPUT_DATA_FLAG::WINDOW_POS:
			break;
		default:
			OutputError::Alert("IIETool::GetInputDataFlag() return unknown flag");
			return;
	}

	if(pGuiFrame->isRevise()){//�␳�L��
		InputData_Vec inputData_Vec;
		IInputRevise* iir = m_InputReviseMG.GetInputRevise();
		if(iir){
			iir->SetStepSize( pGuiFrame->GetReviseStepSize(lpd) );
			iir->OnBeginDrag(lpd, this, inputData_Vec);	

			InputData_Vec::iterator itr = inputData_Vec.begin();
			if(itr != inputData_Vec.end()){
				pGuiFrame->OnMouseLButtonDown(nFlags, &(*itr));
				itr++;
			}
			for(; itr != inputData_Vec.end(); itr++){
				pGuiFrame->OnMouseLDrag(nFlags, &(*itr));
			}
		}
		else{
			OutputError::Alert("ImgEdit::OnMouseLButtonDown iir is NULL pointer");
			return;
		}	
	}
	else{//�␳�Ȃ�
		pGuiFrame->OnMouseLButtonDown(nFlags, lpd);
	}
}

////////////////////////////////
/*!
	�E�}�E�X�{�^����������Ƃ��Ăяo��
	@param[in] lpd �}�E�X�̓��͏��
*/
VOID ImgEdit::OnMouseRButtonDown(UINT nFlags, LPPOINT lps)
{
	m_IEEventCapture.OnMouseRButtonDown(nFlags, lps);

	IIEGuiFrame* pGuiFrame = NULL;
	IIETool* pTool = this->GetSelectTool();
	if (m_pIIECommand) {
		pGuiFrame = m_pIIECommand;
	} else if (pTool) {
		pGuiFrame = pTool;
	}

	if (!pGuiFrame) return;

	pGuiFrame->OnMouseRButtonDown(nFlags, lps);
}

////////////////////////////////
/*! 
	�}�E�X�ړ���
	@param[in] lpd �}�E�X�̓��͏��
*/
VOID ImgEdit::OnMouseMove(UINT nFlags, LPIE_INPUT_DATA lpd)
{
	if (m_isLDrag) {
		m_IEEventCapture.OnMouseLDrag(nFlags, lpd);
	}

	IIEGuiFrame* pGuiFrame = NULL;
	IIETool* pTool = this->GetSelectTool();
	if (m_pIIECommand) {
		pGuiFrame = m_pIIECommand;
	} else if (pTool) {
		pGuiFrame = pTool;
	}

	if (!pGuiFrame) return;

	ImgFile_Ptr pFile = GetActiveFile();
	if (pFile == NULL) return;

	switch (pGuiFrame->GetInputDataFlag()) {
		case IE_INPUT_DATA_FLAG::IMAGE_POS:
			pFile->ConvertPosWndToImg(lpd);
			break;
		case IE_INPUT_DATA_FLAG::WINDOW_POS:
			break;
		default:
			OutputError::Alert("IIETool::GetInputDataFlag() return unknown flag");
			return;
	}

	//
	pGuiFrame->OnMouseMove(nFlags, lpd);

	if (m_isLDrag) {

		if(pGuiFrame->isRevise()){//�␳�L��
			InputData_Vec inputData_Vec;
			IInputRevise* iir = m_InputReviseMG.GetInputRevise();
			if(iir){
				iir->SetStepSize( pGuiFrame->GetReviseStepSize(lpd) );
				iir->OnMouseDrag(lpd, this, inputData_Vec);	

				InputData_Vec::iterator itr = inputData_Vec.begin();
				for(; itr != inputData_Vec.end(); itr++){
					pGuiFrame->OnMouseLDrag(nFlags, &(*itr));
				}
			}
			else{
				OutputError::Alert("ImgEditExt::OnMouseLButtonDown iir is NULL pointer");
				return;
			}	
		}
		else{//�␳�Ȃ�
			pGuiFrame->OnMouseLDrag(nFlags, lpd);
		}
	}
}

////////////////////////////////
/*!
	���}�E�X�{�^�����������Ƃ��Ăяo��
	@param[in] pFile �}�E�X�N���b�N���s����t�@�C��
*/
VOID ImgEdit::OnMouseLButtonUp(UINT nFlags, LPIE_INPUT_DATA lpd)
{
	m_IEEventCapture.OnMouseLButtonUp(nFlags, lpd);

	IIEGuiFrame* pGuiFrame = NULL;
	IIETool* pTool = this->GetSelectTool();
	if (m_pIIECommand) {
		pGuiFrame = m_pIIECommand;
	} else if (pTool) {
		pGuiFrame = pTool;
	}

	if( !pTool ) return;

	ImgFile_Ptr pFile = GetActiveFile();
	if(!pFile) return;

	if(m_isLDrag){
		InputData_Vec inputData_Vec;
		m_isLDrag = false;

		switch(pGuiFrame->GetInputDataFlag()){
			case IE_INPUT_DATA_FLAG::IMAGE_POS:
				pFile->ConvertPosWndToImg(lpd);
				break;
			case IE_INPUT_DATA_FLAG::WINDOW_POS:
				break;
			default:
				OutputError::Alert("IIETool::GetInputDataFlag() return unknown flag");
				return;
		}

		if(pGuiFrame->isRevise()){//�␳�L��
			InputData_Vec inputData_Vec;
			IInputRevise* iir = m_InputReviseMG.GetInputRevise();
			if(iir){
				iir->SetStepSize( pGuiFrame->GetReviseStepSize(lpd) );
				iir->OnEndDrag(lpd, this, inputData_Vec);

				int size = inputData_Vec.size();
				if(size <= 0){
					pGuiFrame->OnMouseLButtonUp(nFlags, lpd);
				}
				else if(size == 1){
					pGuiFrame->OnMouseLButtonUp(nFlags, &(inputData_Vec[0]));
				}
				else{	
					for(int i=0; i < size-2; i++){
						pGuiFrame->OnMouseLDrag(nFlags, &(inputData_Vec[i]));
					}
					pGuiFrame->OnMouseLButtonUp(nFlags, &(inputData_Vec[size-1]));
				}
			}
			else{
				OutputError::Alert("ImgEditExt::OnMouseLButtonDown iir is NULL pointer");
				return;
			}	
		}
		else{//�␳�Ȃ�
			pGuiFrame->OnMouseLButtonUp(nFlags, lpd);
		}
	}
}

////////////////////////////////
/*!
	�E�}�E�X�{�^�����������Ƃ��Ăяo���B
	@param[in] lps �}�E�X�̈ʒu
*/
VOID ImgEdit::OnMouseRButtonUp(UINT nFlags, LPPOINT lps)
{
	m_IEEventCapture.OnMouseRButtonUp(nFlags, lps);

	IIEGuiFrame* pGuiFrame = NULL;
	IIETool* pTool = this->GetSelectTool();
	if (m_pIIECommand) {
		pGuiFrame = m_pIIECommand;
	} else if (pTool) {
		pGuiFrame = pTool;
	}

	if (!pGuiFrame) return;

	pGuiFrame->OnMouseRButtonUp(nFlags, lps);
}

////////////////////////////////
/*!
	�L�[�{�[�h�̃L�[�������ꂽ�Ƃ��ɌĂяo��
	@param[in] nChar �����ꂽ�L�[�̉��z�R�[�h
	@param[in] optChar �����ɉ�����Ă���Ctrl,Shift�L�[�Ȃǂ̏��
	@return if hit a shortcut or tool  will return true, o/w return false
*/
bool ImgEdit::OnKeyboardDown(UINT nChar, UINT optChar)
{
	m_IEEventCapture.OnKeyboardDown(nChar, optChar);

	uint32_t key_code = nChar;
	key_code |= (optChar & IE_OPT_KEY_CONTROL) ? IE_EXT_KEY_CONTROL : 0;
	key_code |= (optChar & IE_OPT_KEY_SHIFT) ? IE_EXT_KEY_SHIFT : 0;
	key_code |= (optChar & IE_OPT_KEY_ALT) ? IE_EXT_KEY_ALT : 0;

	bool is_hitkey = false;
	char cmd[MAX_IE_MODULE_NAME];
  	if(m_IEKeyMap.GetShortCutCmd(key_code, cmd)){
		RunCommandAsync(cmd, NULL);
		is_hitkey = true;
	}
	else{
		//�L�[�������̎��Ԏ擾
		if(m_isKeyDown == false){
			char tool_name[MAX_IE_MODULE_NAME];
			if(m_IEKeyMap.GetToolShiftName(key_code, tool_name)){
				//change to corresponding tool
				IIETool* pTool = m_IEToolMG.GetIEToolFromName(tool_name);
				SetSelectTool( pTool );

				m_KeyDownTime = ::timeGetTime();
				m_isKeyDown = true;
				is_hitkey = true;
			}
		}
	}

	//�R�}���h�A�c�[���փL�[��n��
	IIEGuiFrame* pGuiFrame = NULL;
	IIETool* pTool = this->GetSelectTool();
	if (m_pIIECommand) {
		pGuiFrame = m_pIIECommand;
	} else if (pTool) {
		pGuiFrame = pTool;
	}

	if (pGuiFrame) {
		pGuiFrame->OnKeyboardDown(nChar, optChar);
	}

	return is_hitkey;
}

////////////////////////////////
/*!
	�L�[�{�[�h�̃L�[�������ꂽ�Ƃ��ɌĂяo��
	@param[in] nChar �����ꂽ�L�[�̉��z�R�[�h
	@param[in] optChar �����ɉ�����Ă���Ctrl,Shift�L�[�Ȃǂ̏��
*/
VOID ImgEdit::OnKeyboardUp(UINT nChar, UINT optChar)
{
	m_IEEventCapture.OnKeyboardUp(nChar, optChar);

	if(m_isKeyDown == true){
		DWORD nowTime = ::timeGetTime();
		if((nowTime - m_KeyDownTime) > m_IEConfig.GetToolShiftTime()){
			//��߂�ꂽ���Ԉُ�L�[�������Ă����ꍇ�̓c�[���V�t�g�Ƃ��Ĕ���
			//��O�̃c�[���ɖ߂��B
			m_IEToolMG.ReturnPrevSelectTool();
		}
		m_isKeyDown = false;
	}

	//�c�[���փL�[��n��
	IIEGuiFrame* pGuiFrame = NULL;
	IIETool* pTool = this->GetSelectTool();
	if (m_pIIECommand) {
		pGuiFrame = m_pIIECommand;
	} else if (pTool) {
		pGuiFrame = pTool;
	}

	if (pGuiFrame) {
		pGuiFrame->OnKeyboardDown(nChar, optChar);
	}
}

/////////////////////////////////
/*!
	�`�揈���Ăяo��
	@param[in] hdc
*/
VOID ImgEdit::OnDraw(HDC hdc)
{
	//
	ResourceLock( IE_RESOURCE_ID::IE_COMMAND );
	IIECommand* pCmd = GetRunningCommand();
	if( pCmd ){
		pCmd->OnDraw( hdc );
	}
	ResourceUnlock( IE_RESOURCE_ID::IE_COMMAND );

	ResourceLock( IE_RESOURCE_ID::IE_TOOL );
	IIETool* pTool = GetSelectTool();
	if( pTool ){
		pTool->OnDraw( hdc );
	}
	ResourceUnlock( IE_RESOURCE_ID::IE_TOOL );
}

////////////////////////////////
/*!
	�I��̈�̏�Ƀ}�E�X�����邩�ǂ����H
	@param[in] pFile �t�@�C��
	@param[in] lpt �}�E�X�̓��͏��
	@return �}�E�X���I��̈�̏�Ȃ�true �����łȂ��Ȃ�false
*/
bool ImgEdit::IsOnSelectImage(ImgFile_Ptr pFile, LPPOINT lpt)
{
	if(pFile){
		return pFile->IsOnSelectImage(lpt);
	}

	return false;
}

/////////////////////////////////
/*!
	�G�f�B�b�g�E�B���h�E�̔w�i�F��ݒ肷��B
	@param[in] color �ݒ肷��F
*/
void ImgEdit::SetEditWndBGColor(IEColor color)
{
	if(color.value == m_IEConfig.GetEditWndBGColor().value) return;

	m_IEConfig.SetEditWndBGColor( color );
	//change all files bg color
	ImgFile_Vec::iterator itr = m_ImgFile_Vec.begin();
	for(; itr != m_ImgFile_Vec.end(); itr++){
		(*itr)->SetBGColor( color );
	}
}

/////////////////////////////////
/*!
	�q�X�g������ݒ肷��
	@param[in] num
*/
void ImgEdit::SetNumOfHistory(unsigned int num)
{
	if(num == m_IEConfig.GetNumOfHistory()) return;
	
	m_IEConfig.SetNumOfHistory( num );
	//change all files num of history
	ImgFile_Vec::iterator itr = m_ImgFile_Vec.begin();
	for(; itr != m_ImgFile_Vec.end(); itr++){
		(*itr)->SetNumOfHistory( num );
	}
}

void ImgEdit::EndCommand(){
	if (m_pIIECommand) {
		OutputError::PushLog( LOG_LEVEL::_INFO, "ImgEdit::EndCommand()");

		ResourceLock( IE_RESOURCE_ID::IE_COMMAND );

		m_pIIECommand->End(this);
		IECommandMG::ReleaseIECommand(&m_pIIECommand);
		m_pIIECommand = NULL;
		m_pIECommandArg = NULL;

		ResourceUnlock( IE_RESOURCE_ID::IE_COMMAND );
	}
}
