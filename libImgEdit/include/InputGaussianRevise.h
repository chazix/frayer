#ifndef _INPUTGAUSSIANREVISE_H_
#define _INPUTGAUSSIANREVISE_H_

#include "IInputRevise.h"

class InputGaussianRevise : public IInputRevise
{
public:
	InputGaussianRevise(int N, double sigma);
	~InputGaussianRevise();

	//////////////////////////////
	/*!
		���͊J�n
		@param[in] lpd ���̓f�[�^
		@param[out] inputData_Vec �o�͍��W�z��
	*/
	void OnBeginDrag(LPIE_INPUT_DATA lpd, ImgEdit *pImgEdit, InputData_Vec &inputData_Vec);

	//////////////////////////////
	/*!
		����
		@param[in] lpd ���̓f�[�^
		@param[out] inputData_Vec �o�͍��W�z��
	*/
	void OnMouseDrag(LPIE_INPUT_DATA lpd, ImgEdit *pImgEdit, InputData_Vec &inputData_Vec);

	//////////////////////////////
	/*!
		���͏I��
		@param[in] lpd ���̓f�[�^
		@param[out] inputData_Vec �o�͍��W�z��
	*/
	void OnEndDrag(LPIE_INPUT_DATA lpd, ImgEdit *pImgEdit, InputData_Vec &inputData_Vec);

private:

	void InitInputData(LPIE_INPUT_DATA lpd);
	void AddInputData(LPIE_INPUT_DATA lpd);
	void GetAverageData(LPIE_INPUT_DATA lpd){
		*lpd = m_avrgData;
	}
	void GetBeforeData(LPIE_INPUT_DATA lpd){
		*lpd = m_beforeData;
	}

	int m_N;
	double m_sigma;
	unsigned int m_bufferIndex;
	unsigned int m_indexMask;
	IE_INPUT_DATA m_avrgData;
	IE_INPUT_DATA m_beforeData;
	IE_INPUT_DATA* m_inputData; //!< buffer
};

#endif //_INPUTGAUSSIANREVISE_H_