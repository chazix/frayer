#ifndef _RECTIMGSELECTER_H_
#define _RECTIMGSELECTER_H_

#include "IMakeRectImgSelecter.h"


//�l�p�`�͈͑I��
class RectImgSelecter : 
	public IMakeRectImgSelecter
{
public:
	RectImgSelecter();
	~RectImgSelecter();

	//�{�^���摜
	HBITMAP GetButtonImg();

	void OnDraw(HDC hdc);

	void OnMouseLButtonDown(UINT nFlags, const LPIE_INPUT_DATA lpd);
	void OnMouseLDrag(UINT nFlags, const LPIE_INPUT_DATA lpd);
	void OnMouseLButtonUp(UINT nFlags, const LPIE_INPUT_DATA lpd);

	void OnKeyboardDown(UINT nChar, UINT optChar);

private:
	ImgMask* m_pEditMask;
};

#endif