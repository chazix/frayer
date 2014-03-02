#ifndef _VIEWROT_H_
#define _VIEWROT_H_


#include "IIETool.h"
#include <libImgEdit.h>


//��ʉ�]�c�[��
class ViewRot : public IIETool
{
public:
	ViewRot();
	virtual ~ViewRot();

	//���͏����E�B���h�E�N���C�A���g���W�ɂ���
	virtual IE_INPUT_DATA_FLAG GetInputDataFlag(){ return IE_INPUT_DATA_FLAG::WINDOW_POS; }
	
	//�{�^���摜
	virtual HBITMAP GetButtonImg();

	virtual void OnDraw(HDC hdc){};

	virtual void OnMouseLButtonDown(UINT nFlags, const LPIE_INPUT_DATA lpd);
	virtual void OnMouseLDrag(UINT nFlags, const LPIE_INPUT_DATA lpd);
	virtual void OnMouseLButtonUp(UINT nFlags, const LPIE_INPUT_DATA lpd);
	virtual void OnMouseRButtonDown(UINT nFlags, const LPPOINT lpt);
	virtual void OnMouseRButtonUp(UINT nFlags, const LPPOINT lpt);
private:
	double GetRot(LPPOINT start_lpt, LPIE_INPUT_DATA end_lpd);

	bool m_isLDrag;
	POINT startPt;
	CvPoint2D64f center;
	double start_rot;
};

#endif //_VIEWROT_H_