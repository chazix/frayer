#ifndef _POLYGONSELECTER_BASE_H_
#define _POLYGONSELECTER_BASE_H_


#include "IImgSelecter.h"

typedef std::vector<CvLine2D64f> CvLine_Vec;

class PolygonSelecter_Base : public IImgSelecter
{
public:
	PolygonSelecter_Base(){};
	virtual ~PolygonSelecter_Base(){};

	//�}�E�X���W�̓E�B���h�E�N���C�A���g���W�ƌn
	virtual IE_INPUT_DATA_FLAG GetInputDataFlag(){
		return IE_INPUT_DATA_FLAG::WINDOW_POS;
	}

	virtual void OnDraw(HDC hdc);

protected:

	//////////////////////////
	/*!
		m_CvLine_Vec�Ɋi�[����Ă�������̍ő�͈͂����߂�
		@param[out] lprect
	*/
	void GetLinesRect(LPRECT lprect);


	//////////////////////////
	/*!
		�ێ����Ă����������I��̈�̍쐬
		@param[in] handle_name �X�V����n���h���̖��O
		@param[in] lprect �X�V����͈�
	*/
	void CreateMask(const char* handle_name, const LPRECT lprect);

	CvLine_Vec m_CvLine_Vec;	
	bool m_isFirst;
	bool m_isLineVecLock;
	CvPoint2D64f m_beforePt;
	CvPoint2D64f m_nowPt;

private:
	ImgMask* m_pEditMask;
};

#endif