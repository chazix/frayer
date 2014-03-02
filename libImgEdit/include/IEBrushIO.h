#ifndef _IEBRUSHIO_H_
#define _IEBRUSHIO_H_

#include "IEImgBrush.h"
#include "IEEMBrush.h"
#include "IEFileIOCommon.h"

typedef struct _AbrInfo AbrInfo;

struct _AbrInfo
{
	short version;
	short subversion;
	short count;
};

typedef std::vector<IEBrush_Ptr> IEBrush_Vec;

class _EXPORTCPP IEBrushIO
{
public:
	////////////////////////
	/*!
		IEBrush��Ǝ��`���ŕۑ�
		@param[in] pBrush �ۑ�����u���V
		@param[in] path �ۑ���̃p�X
	*/
	static void SaveIEBrush(const IEImgBrush_Ptr pBrush, const char* path);

	////////////////////////
	/*!
		�Ǝ��`���u���V�t�@�C����ǂݍ���
		@param[in] path �Ǎ���̃p�X
		@return �ǂݍ��݁A���������u���V�I�u�W�F�N�g�ւ̃p�X
	*/
	static IEBrush_Ptr LoadIEBrush(const char* path);

	////////////////////////
	/*!
		photoshop�`���̃u���V�t�@�C���̓ǂݍ���
		@return �ǂݍ��񂾃u���V��, �ǂݍ��ݎ��s����0
	*/
	static int StartReadAbrBrush(const char* path);

	//////////////////////////
	/*!
		IEBrushIO���ێ����Ă���u���V��Ԃ��B
		@return �u���V�I�u�W�F�N�g�̃|�C���^
	*/
	static IEBrush_Ptr GetBrush();

	//////////////////////////
	/*!
		�ǂݍ��ݏ����̏I���B
	*/
	static void EndReadAbrBrush();
private:

	/////////////////////////
	/*!
		�u���V�t�@�C���̃w�b�_�[������ǂݍ���
	*/
	static bool ReadAbrInfo(FILE* abr, AbrInfo* abr_hdr);

	//////////////////////////
	/*!
		ver6�̏ꍇ�̃u���V�T���v�����̌��o
	*/
	static int FindSampleCountV6(FILE* abr, AbrInfo * abr_hdr);

	//////////////////////////
	/*!
		8BIM�^�O����name�Ɠ��������ǂ���
	*/
	static bool ReachAbr8BIMSection(FILE *abr, const char* name);

	/////////////////////////
	/*!
		�u���V�̃o�[�W�������ǂݍ��݉\�Ȃ��̂��H
	*/
	static bool isSupportedAbrHdr(const AbrInfo *abr_hdr);

	/////////////////////////
	/*!
		�u���V�̓ǂݍ���
	*/
	static bool AbrBrushLoad(FILE *abr, const AbrInfo *abr_hdr, const char* filename, IEBrush_Ptr pBrush, int id);

	//////////////////////////
	/*!
		ver1 ver2 �̃u���V�̓ǂݍ���
	*/
	static bool AbrBrushLoad_v12(FILE *abr, const AbrInfo *abr_hdr, const char *filename, IEBrush_Ptr pBrush, int id);

	////////////////////////////
	/*!
		ver6�@�̃u���V�̓ǂݍ���
	*/
	static bool AbrBrushLoad_v6(FILE *abr, const AbrInfo *abr_hdr, const char *filename, IEBrush_Ptr pBrush, int id);

	static void AbrBrushNormalLoad(FILE *abr, int width, int height, short depth, IEBrush_Ptr pBrush);
	static void AbrBrushDecodeLoad(FILE *abr, int widht, int height, short depth, IEBrush_Ptr pBrush);

	static char* abr_v1_brush_name(const char* filename, int id);
	static char* abr_read_ucs2_text(FILE *abr);


	static bool m_isReading;
	static IEBrush_Vec m_ieBrush_Vec;
};

#endif