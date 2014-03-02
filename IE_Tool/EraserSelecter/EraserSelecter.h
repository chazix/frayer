#ifndef _ERASERSELECTER_H_
#define _ERASERSELECTER_H_

#include "IImgSelecter.h"
#include <libImgEdit.h>

class EraserSelecter : public IImgSelecter {
public:
	EraserSelecter();
	~EraserSelecter();

	//�}�E�X���W�␳�L��
	bool isRevise(){
		return true;
	}

	//��ԓ_�̊Ԋu
	double GetReviseStepSize(const LPIE_INPUT_DATA lpd){
		if(m_pImgEdit){
			IEBrush_Ptr pBrush = m_pImgEdit->GetSelectBrush();
			if(pBrush){
				return pBrush->GetStepSize(lpd);
			}
		}

		return 1.0;
	}

	//�{�^���摜
	HBITMAP GetButtonImg();

	void OnSelect();
	void OnNeutral();

	void OnSetToolEdtiWnd(IToolEditWndHandle* pHandl);

	void OnMouseLButtonDown(UINT nFlags, const LPIE_INPUT_DATA lpd);
	void OnMouseLDrag(UINT nFlags, const LPIE_INPUT_DATA lpd);
	void OnMouseLButtonUp(UINT nFlags, const LPIE_INPUT_DATA lpd);

private:

	void Select(LPIE_INPUT_DATA lpd);

	int m_brush_alpha;
	IplImage* m_brush_alpha_map;
	IplImageExt* m_brush_alpha_map_ext;

	ImgMask_Ptr m_pEditMask;

	EditNode* m_editNode;
	EditMaskHandle* m_pEditMaskHandle;
};

class EraserSelecterCopyOp
{
public:
	inline void operator()(void* dst, uint8_t pix) const
	{
		uint8_t* pdst = (uint8_t*) dst;

		if (pix == 0) return;

		if ((*pdst) == 0) {
			(*pdst) = pix;
		} else if ((*pdst) == 255) {
			//
		} else {
			uint8_t ra = ~pix;
			uint8_t alpha = 255 - (ra*(255 - (*pdst))>>8);
			(*pdst) = alpha;
		}
	}
};


#endif