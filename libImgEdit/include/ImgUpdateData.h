#ifndef _IMGUPDATEDATA_H_
#define _IMGUPDATEDATA_H_

#include <windows.h>
#include <list>

//
enum UPDATE_FLAG{
	NO_UPDATE = 0,
	UPDATE_LAYER_IMAGE = 1,			//���C���[�摜�̍X�V
	UPDATE_MASK_IMAGE = 3,			//
	UPDATE_DISPLAY_IMAGE = 4,		//�\����ʂ̍X�V
};

//�C���[�W�Đ����f�[�^
typedef struct _UPDATE_DATA
{
	RECT rect;
	UPDATE_FLAG update_flag;
	bool isAll;
} UPDATE_DATA;

typedef UPDATE_DATA* LPUPDATE_DATA;
typedef std::list<LPUPDATE_DATA>  LPUpdateData_List;


#endif //_IMGUPDATEDATA_H_