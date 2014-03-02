#ifndef _POLYGONSELECTER_H_
#define _POLYGONSELECTER_H_

#include "PolygonSelecter_Base.h"


//���p�`�I��
class PolygonSelecter : public PolygonSelecter_Base
{
public:
	PolygonSelecter();
	virtual ~PolygonSelecter();

	//�{�^���摜
	virtual HBITMAP GetButtonImg();

	virtual void OnMouseLButtonDown(UINT nFlags, const LPIE_INPUT_DATA lpd);
	virtual void OnMouseMove(UINT nFlags,  const LPIE_INPUT_DATA lpd);
	virtual void OnMouseLButtonUp(UINT nFlags,  const LPIE_INPUT_DATA lpd);

	virtual void OnMouseRButtonUp(UINT nFlags, const LPPOINT lpt);
};

#endif //_POLYGONSELECTER_H_