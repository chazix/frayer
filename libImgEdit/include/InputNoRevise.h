#ifndef _INPUTNOREVISE_H_
#define _INPUTNOREVISE_H_

#include "IInputRevise.h"

class InputNoRevise : public IInputRevise
{
public:
	InputNoRevise();
	~InputNoRevise();

	//////////////////////////////
	/*!
		���͊J�n
		@param[in] lpd ���̓f�[�^
		@param[in] pImgEdit
		@param[out] inputData_Vec �o�͍��W�z��
	*/
	void OnBeginDrag(LPIE_INPUT_DATA lpd, ImgEdit *pImgEdit, InputData_Vec &inputData_Vec);

	//////////////////////////////
	/*!
		����
		@param[in] lpd ���̓f�[�^
		@param[in] pImgEdit
		@param[out] inputData_Vec �o�͍��W�z��
	*/
	void OnMouseDrag(LPIE_INPUT_DATA lpd, ImgEdit *pImgEdit, InputData_Vec &inputData_Vec);

	//////////////////////////////
	/*!
		���͏I��
		@param[in] lpd ���̓f�[�^
		@param[in] pImgEdit
		@param[out] inputData_Vec �o�͍��W�z��
	*/
	void OnEndDrag(LPIE_INPUT_DATA lpd, ImgEdit *pImgEdit, InputData_Vec &inputData_Vec);

private:

	IE_INPUT_DATA beforData;
};

#endif //_INPUTNOREVISE_H_