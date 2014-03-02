#ifndef _IINPUTREVISE_H_
#define _IINPUTREVISE_H_

#include "InputData.h"
#include "ImgEdit.h"

#define IIR_MIN_STEP_SIZE 0.1

class IInputRevise
{
public:
	IInputRevise():
	  m_stepSize(1.0){};
	virtual ~IInputRevise(){};

	void SetStepSize(double size){
		m_stepSize = (size > IIR_MIN_STEP_SIZE) ? size : IIR_MIN_STEP_SIZE;
	}

	//////////////////////////////
	/*!
		���͊J�n
		@param[in] lpd ���̓f�[�^
		@param[in] pImgEdit
		@param[out] inputData_Vec �o�͍��W�z��
	*/
	virtual void OnBeginDrag(LPIE_INPUT_DATA lpd, ImgEdit *pImgEdit, InputData_Vec &inputData_Vec) = 0;
	
	//////////////////////////////
	/*!
		����
		@param[in] lpd ���̓f�[�^
		@param[in] pImgEdit
		@param[out] inputData_Vec �o�͍��W�z��
	*/
	virtual void OnMouseDrag(LPIE_INPUT_DATA lpd, ImgEdit *pImgEdit, InputData_Vec &inputData_Vec) = 0;
	
	//////////////////////////////
	/*!
		���͏I��
		@param[in] lpd ���̓f�[�^
		@param[in] pImgEdit
		@param[out] inputData_Vec �o�͍��W�z��
	*/
	virtual void OnEndDrag(LPIE_INPUT_DATA lpd, ImgEdit *pImgEdit, InputData_Vec &inputData_Vec) = 0;

protected:
	void LineRevise(LPIE_INPUT_DATA startPt, LPIE_INPUT_DATA endPt, InputData_Vec &inputData_Vec);

	double m_stepSize;
};

#endif //_IINPUTREVISE_H_