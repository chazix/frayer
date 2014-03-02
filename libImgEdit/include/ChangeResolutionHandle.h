#ifndef _CHANGE_RESOLUTION_HANDLE_H_
#define _CHANGE_RESOLUTION_HANDLE_H_

#include "ImgFileHandle.h"

class ImgFile;
class ImgLayerGroup;

class _EXPORTCPP ChangeResolutionHandle : public ImgFileHandle
{
public:
	ChangeResolutionHandle();
	~ChangeResolutionHandle();

	inline void SetNewResolution(CvSize size){
		m_new_resolution = size;
	}

	////////////////////////////////////////////
	/*!
		CV_INTER_NN �ŋߗאڕ��
		CV_INTER_LINEAR �o�C���j�A���
		CV_INTER_AREA �s�N�Z���̈�̊֌W��p���ĕ��
		CV_INTER_CUBIC �o�C�L���[�r�b�N���
	*/
	inline void SetInterpolation(int interpolation){
		m_interpolation = interpolation;
	}

	bool Do(ImgFile_weakPtr wpFile);
	void Redo(ImgFile_weakPtr wpFile);
	void Undo(ImgFile_weakPtr wpFile);

private:
	void CreateNewResolutionLayerGroup(ImgLayerGroup_Ptr pSrcGroup, ImgLayerGroup_Ptr pDstGroup);

	bool m_isChangedResolution;

	int m_interpolation;
	CvSize m_old_resolution;
	CvSize m_new_resolution;

	ImgFile_weakPtr m_pEditFile;
	ImgLayerGroup_weakPtr m_pOldRootLayerGroup;
	ImgLayerGroup_Ptr m_pOldRootLayerGroup_lock;
	ImgLayerGroup_weakPtr m_pNewRootLayerGroup;
	ImgLayerGroup_Ptr m_pNewRootLayerGroup_lock;
};

#endif //_CHANGE_RESOLUTION_HANDLE_H_