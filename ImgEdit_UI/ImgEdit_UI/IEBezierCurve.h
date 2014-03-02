#ifndef _IEBEZIERCURVE_H_
#define _IEBEZIERCURVE_H_

#include "StdAfx.h"

using namespace std;

//�x�W�F�Ȑ��N���X
class IEBezierCurve {
public:
	///////////////////////////////////////
	/*!
		3���x�W�F�Ȑ��̍��W���X�g���擾
		@param[in] p0 �n�_
		@param[in] p1 ����_1
		@param[in] p2 ����_2
		@param[in] p3 �I�_
		@param[in] DIVIDE ����x
		@return ���W��vector�z��
	*/
	static vector<POINT> GetBezier3CurverPointList(LPPOINT p0, LPPOINT p1, 
										LPPOINT p2, LPPOINT p3, int DIVIDE=16);

	//////////////////////////////////////
	/*!
		2���x�W�F�Ȑ��̍��W���X�g���擾
		@param[in] p0 �n�_
		@param[in] p1 ����_
		@param[in] p2 �I�_
		@parma[in] DIVIDE ����x
		@return ���W��vector�z��
	*/
	static vector<POINT> GetBezier2CurverPointList(LPPOINT p0, LPPOINT p1,
										LPPOINT p2, int DIVIDE=16);
};

#endif