#include "stdafx.h"

#include "ImgFileHandleMG.h"
#include "OutputError.h"
#include "NullHandle.h"
#include "StackHandle.h"
#include "AddCopyLayerHandle.h"
#include "AddNewLayerHandle.h"
#include "AddNewLayerGroupHandle.h"
#include "AddNewMaskHandle.h"
#include "ChangeCanvasSizeHandle.h"
#include "ChangeLayerLineHandle.h"
#include "ChangeLayerOpacityHandle.h"
#include "ChangeLayerSynthFourCCHandle.h"
#include "ChangeResolutionHandle.h"
#include "EditLayerHandle.h"
#include "EditMaskHandle.h"
#include "MergeLayerHandle.h"
#include "MoveLayerHandle.h"
#include "RemoveLayerHandle.h"
#include "SetLayerNameHandle.h"

#include "OutputError.h"

#ifdef _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif


ImgFileHandle* ImgFileHandleMG::CreateImgFileHandle(ImgFile_Ptr pFile, int id)
{
	ImgFileHandle* ret;
	switch( id ){
		case IFH_NULL:
			ret = new NullHandle;
			break;
		case IFH_STACK:
			ret = new StackHandle;
			break;
		case IFH_ADD_COPY_LAYER:
			ret = new AddCopyLayerHandle;
			strcpy(ret->handle_name, "���C���[�R�s�[");
			break;
		case IFH_ADD_NEW_LAYER:
			ret = new AddNewLayerHandle;
			strcpy(ret->handle_name, "���C���[�V�K�쐬");
			break;
		case IFH_ADD_NEW_LAYER_GROUP:
			ret = new AddNewLayerGroupHandle;
			strcpy(ret->handle_name, "���C���[�O���[�v�V�K�쐬");
			break;
		case IFH_ADD_NEW_MASK:
			ret = new AddNewMaskHandle;
			strcpy(ret->handle_name, "�}�X�N�V�K�쐬");
			break;
		case IFH_CHANGE_CANVAS_SIZE:
			ret = new ChangeCanvasSizeHandle;
			strcpy(ret->handle_name, "�L�����o�X�T�C�Y�ύX");
			break;
		case IFH_CHANGE_LAYER_LINE:
			ret = new ChangeLayerLineHandle;
			strcpy(ret->handle_name, "���C���[���ёւ�");
			break;
		case IFH_CHANGE_LAYER_OPACITY:
			ret = new ChangeLayerOpacityHandle;
			break;
		case IFH_CHANGE_LAYER_SYNTH_FOURCC:
			ret = new ChangeLayerSynthFourCCHandle;
			break;
		case IFH_CHANGE_RESOLUTION:
			ret = new ChangeResolutionHandle;
			strcpy(ret->handle_name, "�摜�𑜓x�ύX");
			break;
		case IFH_EDIT_LAYER:
			ret = new EditLayerHandle;
			break;
		case IFH_EDIT_MASK:
			ret = new EditMaskHandle;
			break;
		case IFH_MERGE_LAYER:
			ret = new MergeLayerHandle;
			strcpy(ret->handle_name, "���C���[����");
			break;
		case IFH_MOVE_LAYER:
			ret = new MoveLayerHandle;
			strcpy(ret->handle_name, "���C���[���s�ړ�");
			break;
		case IFH_REMOVE_LAYER:
			ret = new RemoveLayerHandle;
			strcpy(ret->handle_name, "���C���[�폜");
			break;
		case IFH_SET_LAYER_NAME:
			ret = new SetLayerNameHandle;
			strcpy(ret->handle_name, "���C���[���ύX");
			break;
		default:
			{
				assert(0);
				
				char log[256];
				wsprintf(log, "ImgFileHandleMG::CreateImgFileHandle() unknown img file handle id = %d", id);
				OutputError::PushLog(LOG_LEVEL::_WARN, log);
				return NULL;
			}
			break;
	}

	if( !ret->Init( pFile ) ){
		char log[256];
		wsprintf(log, "ImgFileHandleMG::CreateImgFileHandle() faild handle initialize, id = %d", id);
		OutputError::PushLog(LOG_LEVEL::_ERROR, log);
		
		ReleaseImgFileHandle(ret);
		ret = NULL;
		return NULL;
	}

	return ret;
}

void ImgFileHandleMG::ReleaseImgFileHandle(ImgFileHandle* pHandle)
{
	if (pHandle) {
		delete pHandle;
	}
}