#ifndef _LASSOSELECTER_H_
#define _LASSOSELECTER_H_

#include "PolygonSelecter_Base.h"


//������I��
class LassoSelecter : public PolygonSelecter_Base
{
public:
	LassoSelecter();
	virtual ~LassoSelecter();

	//���͏��A��ԁA�␳�L��
	virtual bool isRevise(){ return true; }

	//�{�^���摜
	virtual HBITMAP GetButtonImg();

	virtual void OnMouseLButtonDown(UINT nFlags, const LPIE_INPUT_DATA lpd);
	virtual void OnMouseLDrag(UINT nFlags, const LPIE_INPUT_DATA lpd);
	virtual void OnMouseLButtonUp(UINT nFlags, const LPIE_INPUT_DATA lpd);

private:
	///////////////////////////
	/*!
	*/
	virtual void Push_Line(LPIE_INPUT_DATA lpd);

	bool m_isSelected;
};

#endif //_LASSOSELECTER_H_