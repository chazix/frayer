#ifndef _SPOITTOOL_H_
#define _SPOITTOOL_H_

#include "IIETool.h"
#include <libImgEdit.h>


//�X�|�C�g�c�[��
//�摜����F�𓾂�
class SpoitTool : public IIETool
{
public:
	SpoitTool();
	virtual ~SpoitTool();

	//�}�E�X���W���͏�̓E�B���h�E�N���C�A���g���W
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

	void SpoitColor(const LPIE_INPUT_DATA lpd);
};

#endif