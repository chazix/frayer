#ifndef _OUTPUTERROR_H_
#define _OUTPUTERROR_H_

#include "LibIEDllHeader.h"

#include <string>
#include <list>

//#ifdef _DEBUG
//	#define PushLog(log_level, str) PushLog(__FILE__, __LINE__, log_level, str)
//#endif

typedef std::list<std::string> LogList;

enum LOG_LEVEL{
	_INFO,
	_WARN,
	_ERROR,
	_FATAL,
};

#define IE_ERROR_DOESNT_SUPPORT_SSE2			TEXT("SSE2�Ή���CPU�ł͂���܂���")
#define IE_ERROR_CONFIG_FILE_DONT_EXIST			TEXT("�ݒ�t�@�C����������܂���")
#define IE_ERROR_BRUSH_CONFIG_FILE_DONT_EXIST	TEXT("�u���V�ݒ�t�@�C����������܂���")
#define IE_ERROR_LOAD_FILED_BRUSH_CONFIG_FILE	TEXT("�u���V�ݒ�t�@�C���̓ǂݍ��݂Ɏ��s���܂���")
#define IE_ERROR_BRUSH_CONFIG_FILE_CANT_WRITE	TEXT("�u���V�ݒ�t�@�C���̏������݂Ɏ��s���܂���")
#define IE_ERROR_TOOL_CONFIG_FILE_CANT_WRITE	TEXT("�c�[���ݒ�t�@�C���̏������݂Ɏ��s���܂���")
#define IE_ERROR_EFFECT_CONFIG_FILE_CANT_WRITE	TEXT("�G�t�F�N�g�ݒ�t�@�C���̏������݂Ɏ��s���܂���")
#define IE_ERROR_PALETTE_CONFIG_FILE_CANT_WRITE	TEXT("�p���b�g�ݒ�t�@�C���̏������݂Ɏ��s���܂���")
#define IE_ERROR_LAYER_INVISIBLE				TEXT("���C���[���\������Ă��܂���")
#define IE_ERROR_SELECT_LAYER_DONT_EXIST		TEXT("�I�����C���[������܂���")
#define IE_ERROR_CANT_EDIT_LAYER				TEXT("�ҏW�\�ȃ��C���[�ł͂���܂���")
#define IE_ERROR_SELECT_BRUSH_DONT_EXIST		TEXT("�I���u���V������܂���")
#define IE_ERROR_CANT_LOAD_BRUSH_TEXTURE		TEXT("�u���V�e�N�X�`�����ǂݍ��߂܂���ł���")
#define IE_ERROR_INVALID_BRUSH_TEXTURE			TEXT("�s���ȃu���V�e�N�X�`���ł��B�u���V�e�N�X�`����256x256�A512x512�A1024x1024�̂����ꂩ�̃T�C�Y���g�p���Ă�������")


class  _EXPORTCPP OutputError{
public:
	OutputError(){};
	~OutputError(){};

	static void Init(const char* exePath, const char* logDirPath, LOG_LEVEL log_level);
	static void End();

	static void PushLog(LOG_LEVEL log_level, const char* str);
	static void PushLog(LOG_LEVEL log_level, const char* str, const char* v1);
	static void PushLog(LOG_LEVEL log_level, const char* str, int val);
	//static void PushLog(const char* file, int line, LOG_LEVEL log_level, const char* str);
	//static void FlushLog();
	static void Alert(const char* str);
	static void Alert(const char* str, const char* v1);
private:
	static void SetLogLevel(LOG_LEVEL log_level);

	static bool m_IsInited;
	static int m_LogBufSize;
	static LOG_LEVEL m_LogLevel;
	static std::string m_LogDirPath;
	static LogList m_LogList;
};

#endif