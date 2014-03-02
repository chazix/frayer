#ifndef _IMGEDIT_H_
#define _IMGEDIT_H_

#include "LibIEDllHeader.h"

#include "IEBrush.h"
#include "IEBrushIO.h"
#include "ILayerSynthesizer.h"
#include "LayerSynthesizerMG.h"
#include "ImgFileIOMG.h"
#include "InputReviseMG.h"
#include "IEBrushMG.h"
#include "IEPaletteMG.h"
#include "IIECommand.h"
#include "IIEBrushPopupWnd.h"
#include "IECommandMG.h"
#include "EffectMG.h"
#include "LayerSynthesizerMG.h"
#include "IEToolMG.h"
#include "ImgFileIOMG.h"
#include "IEKeyMap.h"
#include "IEParamRegister.h"
#include "IEEventCapture.h"
#include "ImgEditConfig.h"

#include <list>


typedef std::list<ImgEditEventListener* > ImgEditEventListener_List;
typedef std::vector<ImgFile_Ptr> ImgFile_Vec;

enum IE_RESOURCE_ID{
	IE_COMMAND=0x01,
	IE_TOOL=0x02,
};

enum IE_CURSOR_ID{
	NORMAL_CURSOR=1,
	BRUSH_CURSOR=2,
};

enum {
	IMG_EDIT_MAX_FILE_WIDTH = 10000,
	IMG_EDIT_MAX_FILE_HEIGHT = 10000,
};

class _EXPORTCPP ImgEdit
{
public:
	ImgEdit();
	~ImgEdit();

	//////////////////////////////////
	/*!
		����������
		@param[in] logFilePath
	*/
	bool Init(const char* configini_path, const char* exe_path, const char* log_dir);

	/////////////////////////////////
	/*!
		�I������
	*/
	void End();

	/////////////////////////////////
	/*!
		�C�x���g���X�i�[��o�^����B
		@param[in] pListener �o�^���郊�X�i�[
	*/
	void AddEventListener(ImgEditEventListener*  pListener);

	//////////////////////////////////
	/*!
		�C�x���g���X�i�[���폜����
		@param[in] pListener �폜���郊�X�i�[
	*/
	void DeleteEventListener(ImgEditEventListener*  pListener);

	/////////////////////////////////
	/*!
		�����f�B���N�g�����w�肷��B
		@param[in] path �����f�B���N�g���p�X
	*/
	void SetStartDirectory(const char* path);

	///////////////////////////////
	/*!
		�ݒ�N���X�ւ̃|�C���^��Ԃ��B
		@return
	*/
	ImgEditConfig* GetIEConfig();

	///////////////////////////////
	/*!
		�O�i�F�Ɣw�i�F����������
	*/
	void ExchangeColorFGForBG();

	///////////////////////////////
	/*!
		�O�i�F��ݒ肷��
		@param[in] color �ݒ肷��F
	*/
	void SetSelectFGColor(IEColor color);

	///////////////////////////////
	/*!
		���ݑI�𒆂̑O�i�F��Ԃ�
		@return �I�𒆂̐F
	*/
	IEColor GetSelectFGColor() const {
		return m_SelectFGColor;
	}

	///////////////////////////////
	/*!
		���ݑI�𒆂̑O�i�F��Ԃ�
		@return �I�𒆂̐F
	*/
	IEColor GetSelectBGColor() const {
		return m_SelectBGColor;
	}

	////////////////////////////////
	/*!
		�ҏW���̃t�@�C����؂�ւ���B
		@param[in] pFile �؂�ւ���̕ҏW�t�@�C��
	*/
	void SetActiveFile(ImgFile_Ptr pFile);

	////////////////////////////////
	/*!
		�ҏW���̃t�@�C���̃|�C���^��Ԃ�
		@return �ҏW���̃t�@�C���̃|�C���^
	*/
	ImgFile_Ptr GetActiveFile() const {
		return m_pNowActiveFile;
	}

	////////////////////////////////
	/*!
		�t�@�C���I�u�W�F�N�g��ǉ�����B
		@param[in] pFile �ǉ�����t�@�C��
	*/
	void AddImgFile(ImgFile_Ptr pFile);

	//////////////////////////////
	/*!
		�t�@�C���I�u�W�F�N�g�̍폜�A�������J��
		@param[in] pFile �t�@�C���̃|�C���^
	*/
	void DeleteImgFile(ImgFile_Ptr pFile);

	///////////////////////////////
	/*!
	*/
	void AddIEBrushMGEventListener(IEBrushMGEventListener*  pListener) {
		m_IEBrushMG.AddEventListener( pListener );
	}

	///////////////////////////////
	/*
	*/
	void DeleteIEBrushMGEventListener(IEBrushMGEventListener*  pListener) {
		m_IEBrushMG.DeleteEventListener( pListener );
	}

	void SetBrushGroupComboBox(HWND hCombo) const {
		m_IEBrushMG.SetBrushGroupComboBox( hCombo );
	}

	void SetSelectBrushGroup(int nIndex) {
		m_IEBrushMG.SetSelectBrushGroup( nIndex );
	}

	int GetSelectBrushGroupIndex() {
		return m_IEBrushMG.GetSelectBrushGroupIndex();
	}

	////////////////////////////////
	/*!
		�u���V��I������
		@param[in] pBrush �I������u���V�̃|�C���^
	*/
	void SetSelectBrush(IEBrush_Ptr pBrush) {
		m_IEBrushMG.SetSelectBrush( pBrush );
	}

	////////////////////////////////
	/*!
		�u���V��I������
		@param[in] index �I������u���V�̃C���f�b�N�X
	*/
	void SetSelectBrush(int index) {
		m_IEBrushMG.SetSelectBrush(index);
	}

	////////////////////////////////
	/*!
		�I�𒆂̃u���V��Ԃ�
		@return �I�𒆂̃u���V
	*/
	IEBrush_Ptr GetSelectBrush() const {
		return m_IEBrushMG.GetSelectBrush();
	};

	int GetSelectBrushIndex() {
		return m_IEBrushMG.GetSelectBrushIndex();
	}

	////////////////////////////////
	/*!
		�ێ����Ă���u���V����Ԃ�
		@return �ێ����Ă���u���V��
	*/
	int GetBrushSize() const {
		return m_IEBrushMG.GetBrushSize();
	};

	////////////////////////////////
	/*!
		�C���f�b�N�X���w�肵�ău���V��Ԃ�
		@param[in] index �u���V�C���f�b�N�X
		@return �w�肳�ꂽ�u���V
	*/
	IEBrush_Ptr GetBrushAtIndex(int index) const {
		return m_IEBrushMG.GetBrushAtIndex( index );
	}

	////////////////////////////////
	/*!
		�u���V�t�@�C����ǂݍ���
		@param[in] filename �t�@�C���p�X
	*/
	void LoadBrush(const char* filename) {
		return m_IEBrushMG.LoadBrush( filename );
	}

	//////////////////////////////
	/*!
		�u���V��ǉ�����B
		@param[in] pBruhs �ǉ�����u���V
	*/
	void AddBrush(IEBrush_Ptr pBrush) {
		m_IEBrushMG.AddBrush( pBrush );
	}

	//////////////////////////////
	/*!
		�u���V���폜����B
		@param[in] pBrush�@�폜����u���V
	*/
	void DeleteBrush(IEBrush_Ptr pBrush) {
		m_IEBrushMG.DeleteBrush( pBrush );
	}

	//////////////////////////////
	/*!
		�u���V�e�N�X�`���̖��O����C���f�b�N�X�𓾂�
		@param[in] texture_name
	*/
	int GetBrushTextureIndex(const char* texture_name) const {
		return m_IEBrushMG.GetBrushTextureIndex(texture_name);
	}

	//////////////////////////////
	/*!
		�u���V�e�N�X�`���̃C���f�b�N�X����p�X�𓾂�
		@param[in] texture_path
	*/
	void GetBrushTexturePath(char* dst_path, int max_path_size, int index) const {
		m_IEBrushMG.GetBrushTexturePath(dst_path, max_path_size, index);
	}

	//////////////////////////////
	/*!
		�R���{�{�b�N�X�Ƀu���V�e�N�X�`������ǉ�����
		@param[in] hCombo
	*/
	void SetBrushTextureComboBox(HWND hCombo) const {
		m_IEBrushMG.SetBrushTextureComboBox(hCombo);
	}


	//////////////////////////////
	/*!
		�g�p����u���V�|�b�v�A�b�v�E�B���h�E���Z�b�g����B
		@param[in] pWnd �u���V�|�b�v�A�b�v�E�B���h�E�C���^�[�t�F�[�X
	*/
	void SetBrushPopupWnd(IIEBrushPopupWnd* pWnd);

	//////////////////////////////
	/*!
		�u���V�|�b�v�A�b�v�E�B���h�E��Ԃ�
		@return �u���V�|�b�v�A�b�v�C���^�[�t�F�[�X��Ԃ�
	*/
	IIEBrushPopupWnd* GetBrushPopupWnd();

	///////////////////////////////
	/*!
	*/
	void AddIEPaletteMGEventListener(IEPaletteMGEventListener*  pListener) {
		m_IEPaletteMG.AddEventListener( pListener );
	}

	///////////////////////////////
	/*
	*/
	void DeleteIEPaletteMGEventListener(IEPaletteMGEventListener*  pListener) {
		m_IEPaletteMG.DeleteEventListener( pListener );
	}
	///////////////////////////////
	/*!
		���ׂẴp���b�g�����R���{�{�b�N�X�ɒǉ�����
		@param[in] hCombo
	*/
	void SetPaletteComboBox(HWND hCombo) {
		m_IEPaletteMG.SetPaletteComboBox( hCombo );
	}

	///////////////////////////////
	/*!
		index���w�肵�ăp���b�g���w�肷��
		@param[in] index
	*/
	void SetSelectPalette(int index) {
		m_IEPaletteMG.SetSelectPalette( index );
	}

	////////////////////////////////
	/*!
		�I������Ă���p���b�g���ێ����Ă���F����Ԃ�
		@return �ێ����Ă���F��
	*/
	int GetSelectPaletteSize() const {
		return m_IEPaletteMG.GetSelectPaletteSize();
	};

	//////////////////////////////
	/*!
		�F����ǉ�����B
		@param[in] color �ǉ�����F
	*/
	void AddColorToPalette(const char* name, IEColor color) {
		m_IEPaletteMG.AddColor(name, color);
	}

	//////////////////////////////
	/*!
		�F���폜����B
		@param[in] index�@�폜����F�̃C���f�b�N�X
	*/
	void DeleteColorFromPalette(int index) {
		m_IEPaletteMG.DeleteColor( index );
	}

	//////////////////////////////
	/*!
		�V�K�Ƀt�@�C�����쐬
		@return �쐬�����t�@�C���̃|�C���^(���s�����NULL��Ԃ�)
	*/
	ImgFile_Ptr CreateNewFile();

	//////////////////////////////
	/*!
		�T�C�Y���w�肵�ĐV�K�Ƀt�@�C�����쐬
		@param[in] width �쐬����t�@�C���̕�
		@param[in] height �쐬����t�@�C���̍���
		@param[in] fillcolor �h��Ԃ��F
		@return �쐬�����t�@�C���̃|�C���^(���s�����NULL��Ԃ�)
	*/
	ImgFile_Ptr CreateNewFile(int width, int height, const UCvPixel* fillcolor);

	//////////////////////////////
	/*!
		�摜�t�@�C����ǂݍ��ށB
		@param[in] filePath �摜�t�@�C���̃p�X
		@return �쐬�����t�@�C���̃|�C���^(���s�����NULL��Ԃ�)
	*/
	ImgFile_Ptr LoadImgFile(const char* filePath);

	//////////////////////////////
	/*!
		���O�����ĕۑ�
		@param[in] filePath �ۑ�����摜�̃t�@�C���p�X
		@param[in] pFile �ۑ�����t�@�C��
	*/
	void SaveAsImgFile(const char *filePath, ImgFile_Ptr pFile);

	//////////////////////////////
	/*!
		�㏑���ۑ�
		@param[in] pFile �ۑ�����t�@�C��
	*/
	void SaveImgFile(ImgFile_Ptr pFile);

	//////////////////////////////
	/*!
		�C�x���g�L���v�`�����J�n����
		@param[in] capture_file_path �C�x���g�L���v�`���̕ۑ���t�@�C���p�X
	*/
	void StartEventCapture(const char *capture_file_path); 

	//////////////////////////////
	/*!
		�C�x���g�L���v�`�����I������
	*/
	void EndEventCapture();

	void PlayEventCapture(const char *capture_file_path);
	void PlayEventCaptureAsync(const char *capture_file_path);

	//////////////////////////////
	/*!
		���ݕҏW���̃t�@�C���̃T�C�Y(��x����)��Ԃ��B
		@return rc �t�@�C���̃T�C�Y
	*/
	CvSize GetNowActiveFileSize();

	///////////////////////////////
	/*!
		�f�t�H���g�̃��C���[�V���Z�T�C�U�[��Ԃ��B
		@return default ILayerSynthesizer
	*/
	//const ILayerSynthesizer* GetDefaultLayerSynthesizer(){
	//	return LayerSynthesizerMG::GetDefaultLayerSynth();
	//}

	//////////////////////////////
	/*!
		�␳���@�I���R���{�{�b�N�X���Z�b�g����
	*/
	void SetInputReviseComboBox(HWND hCombo);

	////////////////////////////////
	/*!
		�␳���@�̖��O���w�肵�ĕ␳���@��ύX
	*/
	void SelectInputRevise(const char* name);

	void GetInputReviseName(char* dst_name);

	///////////////////////////////
	/*!
	*/
	void AddIEToolMGEventListener(IEToolMGEventListener*  pListener){
		m_IEToolMG.AddEventListener( pListener );
	}

	///////////////////////////////
	/*
	*/
	void DeleteIEToolMGEventListener(IEToolMGEventListener*  pListener){
		m_IEToolMG.DeleteEventListener( pListener );
	}

	////////////////////////////////
	/*!
		�c�[�����w�肷��
		@param[in] id �w�肷��c�[���̃|�C���^
	*/
	void SetSelectTool(IIETool* pTool) {
		return m_IEToolMG.SetSelectTool( pTool );
	}

	////////////////////////////////
	/*!
		�I������Ă���c�[����Ԃ�
		@return �I������Ă���c�[���ւ̃|�C���^
	*/
	IIETool* GetSelectTool() {
		return m_IEToolMG.GetSelectTool();
	}

	/////////////////////////////////
	/*!
		�ێ����Ă���c�[���O���[�v�̐���Ԃ�
	*/
	int GetSizeOfToolGroup() {
		return m_IEToolMG.GetSizeOfToolGroup();
	}

	IEToolGroup* GetToolGroupAtIndex(int index) {
		return m_IEToolMG.GetToolGroupAtIndex( index );
	}

	bool GetToolName(const char *disp_name, char* dst) {
		return m_IEToolMG.GetToolName(disp_name, dst);
	}

	/////////////////////////////////
	/*!
		�c�[�������w�肵�āA�c�[���ւ̃|�C���^�𓾂�
	*/
	IIETool* GetIEToolFromName(const char* tool_name) {
		return m_IEToolMG.GetIEToolFromName(tool_name);
	}

	/////////////////////////////////
	/*!
		�c�[���̕\�������擾����
	*/
	bool GetToolDisplayName(const char *tool_name, char* dst) {
		return m_IEToolMG.GetDisplayName(tool_name, dst);
	}

	////////////////////////////////
	/*!
		�c�[���G�f�B�b�g�E�B���h�E�n���h�����Z�b�g
		@param[in,out] pHandle
	*/
	void SetToolEditWndHandle(IToolEditWndHandle* pHandle){
		m_IEToolMG.SetToolEditWndHandle( pHandle );
	}

	void SetEffectTree(HWND hTree) {
		m_EffectMG.SetEditTree(hTree);
	}
	IEffect* CreateEffect(const char* effect_display_name) {
		return m_EffectMG.CreateEffect(effect_display_name);
	}

	void ReleaseEffect(IEffect* pEffect) {
		m_EffectMG.ReleaseEffect(pEffect);
	}

	//////////////////////////////////
	///*!
	//	�}�E�X�J�[�\����I�𒆂̃u���V�̌`��֕ς���
	//*/
	//VOID ChangeCusorToSelectBrush();

	////////////////////////////////
	/*!
		cm_id�̃R�}���h�𓯊����Ď��s����
		@param[in] cm_id ���s����R�}���h��ID
		@return �R�}���h�����s������ture �����łȂ����false
	*/
	bool RunCommand(int cm_id, void* pvoid);
	bool RunCommand(const char* cm_name, void* pvoid);

	////////////////////////////////
	/*!
		cm_id�̃R�}���h��񓯊��Ɏ��s����
		@param[in] cm_id ���s����R�}���h��ID
		@return �R�}���h�����s������ture �����łȂ����false
	*/
	bool RunCommandAsync(int cm_id, void* pvoid);
	bool RunCommandAsync(const char* cm_name, void* pvoid);

	////////////////////////////////
	/*!
		���Ocm_name�̃R�}���h��ID�𓾂�B
		@param[in] cm_name ID�𓾂�R�}���h�̖��O
		@reutrn �R�}���h��ID
	*/
	int GetCommandID(const char* cm_name);

	////////////////////////////////
	/*!
		�ŏ��̃R�}���hID��Ԃ��B
		@return �ŏ��̃R�}���hID
	*/
	int GetMinCommandID();

	////////////////////////////////
	/*!
		�ő�̃R�}���h��ID��Ԃ��B
		�R�}���hID�͘A���ł���A
		GetMinCommanID()+�R�}���h���̒l���Ԃ�
		@return �ő�̃R�}���hID
	*/
	int GetMaxCommandID();


	IIECommand* GetRunningCommand();

	////////////////////////////////
	/*!
		�G�t�F�N�g��񓯊��Ɏ��s����
		@param[in] cm_id ���s����R�}���h��ID
		@return �R�}���h�����s������ture �����łȂ����false
	*/
	bool RunEffectAsync(const char* effect_name);

	///////////////////////////////////
	///*!
	//	EffectEditDialog��Ԃ��B
	//*/
	//IEffectEditDialog* GetEffectEditDialog();

	/////////////////////////////////////
	/*!
		�L�[�}�b�v����I�u�W�F�N�g�ւ̃|�C���^��Ԃ��B
	*/
	IEKeyMap* GetKeyMap();

	/////////////////////////////////////
	/*!
		�p�����[�^�ǂݏ������s��
	*/
	bool ReadFromParamRegister(const IIEModule* pModule, const char* name, char* val);
	void WriteToParamRegister(const IIEModule* pModule, const char* name, const char* val);

	/////////////////////////////////////
	/*!
		���ׂẴt�@�C���̉摜�X�V
		@param[in] data �摜�X�V�f�[�^�t���O
	*/
	void PushUpdateDataToAllFile(const LPUPDATE_DATA data);

	////////////////////////////////
	/*!
		�A�C�h�����Ăяo��
	*/
	VOID OnIdle();

	////////////////////////////////
	/*!
		���}�E�X�{�^���������ꂽ�Ƃ��Ăяo��
		@param[in] lpd �}�E�X�̓��͏��
	*/
	VOID OnMouseLButtonDown(UINT nFlags, LPIE_INPUT_DATA lpd);

	////////////////////////////////
	/*!
		�E�}�E�X�{�^����������Ƃ��Ăяo��
		@param[in] lpd �}�E�X�̓��͏��
	*/
	VOID OnMouseRButtonDown(UINT nFlags, LPPOINT lps);
	
	////////////////////////////////
	/*!
		�}�E�X�ړ���
		@param[in] lpd �}�E�X�̓��͏��
	*/
	VOID OnMouseMove(UINT nFlags, LPIE_INPUT_DATA lpd);
	
	////////////////////////////////
	/*!
		���}�E�X�{�^�����������Ƃ��Ăяo��
		@param[in] lpd �}�E�X�̓��͏��
	*/
	VOID OnMouseLButtonUp(UINT nFlags, LPIE_INPUT_DATA lpd);
	
	////////////////////////////////
	/*!
		�E�}�E�X�{�^�����������Ƃ��Ăяo���B
		@param[in] lps �}�E�X�̈ʒu
	*/
	VOID OnMouseRButtonUp(UINT nFlags, LPPOINT lps);
	
	////////////////////////////////
	/*!
		�L�[�{�[�h�̃L�[�������ꂽ�Ƃ��ɌĂяo��
		@param[in] nChar �����ꂽ�L�[�̉��z�R�[�h
		@param[in] optChar �����ɉ�����Ă���Ctrl,Shift�L�[�Ȃǂ̏��
		@return if hit a shortcut or tool  will return true, o/w return false
	*/
	bool OnKeyboardDown(UINT nChar, UINT optChar);

	////////////////////////////////
	/*!
		�L�[�{�[�h�̃L�[�������ꂽ�Ƃ��ɌĂяo��
		@param[in] nChar �����ꂽ�L�[�̉��z�R�[�h
		@param[in] optChar �����ɉ�����Ă���Ctrl,Shift�L�[�Ȃǂ̏��
	*/
	VOID OnKeyboardUp(UINT nChar, UINT optChar);

	/////////////////////////////////
	/*!
		�`�揈���Ăяo��
		@param[in] hdc
	*/
	VOID OnDraw(HDC hdc);

	////////////////////////////////
	/*!
		�I��̈�̏�Ƀ}�E�X�����邩�ǂ����H
		@param[in] pFile �t�@�C��
		@param[in] lpt �}�E�X�̓��͏��
		@return �}�E�X���I��̈�̏�Ȃ�true �����łȂ��Ȃ�false
	*/
	bool IsOnSelectImage(ImgFile_Ptr pFile, LPPOINT lpt);
	/////////////////////////////////
	/*!
		�G�f�B�b�g�E�B���h�E�̔w�i�F��Ԃ��B
		@return �w�i�F
	*/
	IEColor GetEditWndBGColor() const {
		return m_IEConfig.GetEditWndBGColor();
	}
	/////////////////////////////////
	/*!
		�G�f�B�b�g�E�B���h�E�̔w�i�F��ݒ肷��B
		@param[in] color �ݒ肷��F
	*/
	void SetEditWndBGColor(IEColor color);

	unsigned int GetNumOfHistory() const {
		return m_IEConfig.GetNumOfHistory();
	}

	/////////////////////////////////
	/*!
		�q�X�g������ݒ肷��
		@param[in] num
	*/
	void SetNumOfHistory(unsigned int num);

	///////////////////////////////////
	/*!
		�g�p����J�[�\�����w�肷��B
		�J�[�\���̕ύX�A�`���GUI�Ŏ�������
		@param[in] coursor_id �g�p����J�[�\��ID
	*/
	void SetIECursor(IE_CURSOR_ID cursor_id){ m_IECursorID = cursor_id; };
	IE_CURSOR_ID GetIECursor() const { return m_IECursorID; };

private:

	///////////////////////////////////
	inline void ResourceLock(unsigned int id){
#ifdef _WIN32
		switch( id ){
			case IE_RESOURCE_ID::IE_COMMAND:
				::EnterCriticalSection( &m_cs_ie_command );
				break;
			case IE_RESOURCE_ID::IE_TOOL:
				::EnterCriticalSection( &m_cs_ie_tool );
				break;
			default:
				assert( 0 );
				break;
		}
#else
		while((m_LockResourceID & id) == id); //is_lock_resource
		m_LockResourceID |= id;
#endif //_WIN32
	}
	inline void ResourceUnlock(unsigned int id){
#ifdef _WIN32
		switch( id ){
			case IE_RESOURCE_ID::IE_COMMAND:
				::LeaveCriticalSection( &m_cs_ie_command );
				break;
			case IE_RESOURCE_ID::IE_TOOL:
				::LeaveCriticalSection( &m_cs_ie_tool );
				break;
			default:
				assert( 0 );
				break;
		}
#else
		m_LockResourceID &= (~id);
#endif //_WIN32
	}

	//////////////////////////////////
	/*!
		�R�}���h�I������
	*/
	void EndCommand();

	std::string m_StartDirPath;
	std::string m_ConfigINIFilePath;

	ImgEditEventListener_List m_ImgEditEventListener_List;
	ImgFile_Vec m_ImgFile_Vec;  //�ێ����Ă���t�@�C���z��

	IIEBrushPopupWnd* m_pIIEBrushPopupWnd;

	InputReviseMG m_InputReviseMG;
	EffectMG m_EffectMG;
	IEToolMG m_IEToolMG;
	IEBrushMG m_IEBrushMG;
	IEPaletteMG m_IEPaletteMG;

	IEKeyMap m_IEKeyMap;
	IEParamRegister m_IEParamRegister;
	IEEventCapture m_IEEventCapture;
	
	IIECommand* m_pIIECommand;
	void* m_pIECommandArg;

	ImgFile_Ptr m_pNowActiveFile;

	IEColor m_SelectFGColor;
	IEColor m_SelectBGColor;

	ImgEditConfig m_IEConfig;

	bool m_isInit;
	bool m_isLDrag;
	bool m_isKeyDown;
	DWORD m_KeyDownTime;
	IE_CURSOR_ID m_IECursorID;

#ifdef _WIN32
	CRITICAL_SECTION m_cs_ie_command;
	CRITICAL_SECTION m_cs_ie_tool;
#else
	unsigned int m_LockResourceID;
#endif //_WIN32
};

#endif //_IMGEDIT_H_